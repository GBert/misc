-- declare our protocol
local maerklin_proto = Proto("maerklin","Maerklin Protocol")

local maerklin_pdu_len = 13
local makeLine

local function hex_dump(buf,len)
      for i=1,math.ceil(len/maerklin_pdu_len) * maerklin_pdu_len do
         if (i-1) % maerklin_pdu_len == 0 then io.write(string.format('%08X  ', i-1)) end
         io.write( i > len and '   ' or string.format('%02X ', buf:byte(i)) )
         if i % maerklin_pdu_len == 0 then io.write( buf:sub(i-16+1, i):gsub('%c','.'), '\n' ) end
      end
end

local function dissect_common(buffer, pinfo, tree, offset)


    local command

    local subtree = tree:add(maerklin_proto,buffer(offset,maerklin_pdu_len),"Maerklin Protocol Data")

    subtree:add(buffer(offset,1)  ,"Prio:    " .. buffer(offset,1):uint())

    command = bit.band ( buffer(offset+1,1):uint(), 0xfe)

    if command == 0 then
        subtree:add(buffer(offset+1,1),"Command: System " .. buffer(offset+1,1):uint())
    elseif command == 2 then
        subtree:add(buffer(offset+1,1),"Command: Loc Discovery " .. buffer(offset+1,1):uint())
    elseif command == 4 then
        subtree:add(buffer(offset+1,1),"Command: MFX Bind " .. buffer(offset+1,1):uint())
    elseif command == 54 then
        subtree:add(buffer(offset+1,1),"Command: Bootloader (CAN) " .. buffer(offset+1,1):uint())
    else
        subtree:add(buffer(offset+1,1),"Command: unknown " .. buffer(offset+1,1):uint())
    end
    subtree:add(buffer(offset+2,2),"Hash:    " .. buffer(offset+2,2):uint())
    subtree:add(buffer(offset+4,1),"DLC:     " .. buffer(offset+4,1):uint())
    subtree = subtree:add(buffer(5,8),"Data")
    subtree:add(buffer(offset+5,1),"Data 0: " .. buffer(offset+5,1):uint())
    subtree:add(buffer(offset+6,1),"Data 1: " .. buffer(offset+6,1):uint())
    subtree:add(buffer(offset+7,1),"Data 2: " .. buffer(offset+7,1):uint())
    subtree:add(buffer(offset+8,1),"Data 3: " .. buffer(offset+8,1):uint())
    subtree:add(buffer(offset+9,1),"Data 4: " .. buffer(offset+9,1):uint())
    subtree:add(buffer(offset+10,1),"Data 5: " .. buffer(offset+0,1):uint())
    subtree:add(buffer(offset+11,1),"Data 6: " .. buffer(offset+11,1):uint())
    subtree:add(buffer(offset+12,1),"Data 7: " .. buffer(offset+12,1):uint())

    local output = "Maerklin CAN Info (" .. buffer(offset,2):uint() ..")"

    -- return number of bytes consumed so that more maerklin PDUs can be discovered
    return maerklin_pdu_len, output
end

-- create a function to dissect it
function maerklin_proto.dissector(buffer, pinfo, tree)   
    pinfo.cols.protocol = "MAERKLIN"

    local pktlen = buffer:len()
--  hex_dump(buffer,pktlen)

    local consumed, output = dissect_common(buffer, pinfo, tree, 0)
    local remaining = pktlen - consumed
    pinfo.cols.info:set(output)

    while remaining >= maerklin_pdu_len do
        consumed, output = dissect_common(buffer, pinfo, tree, pktlen - remaining)
        pinfo.cols.info:append(makeLine(pinfo,output))
        remaining = remaining - consumed
    end

    if remaining > 0 then
        pinfo.desegment_offset = pktlen - remaining
        pinfo.desegment_len = maerklin_pdu_len - remaining
    end

    return pktlen - remaining
end

-- load the tcp.port table
local udp_table = DissectorTable.get("udp.port")
local tcp_table = DissectorTable.get("tcp.port")
-- register our protocol to handle tcp port 15731
tcp_table:add(15731,maerklin_proto)
udp_table:add(15730,maerklin_proto)
udp_table:add(15731,maerklin_proto)

-- helper function to enable multi-line packet output
makeLine = function (pinfo,output)
    local rel_time = string.format("%.6f\t", pinfo.rel_ts)

    local line = {
        "\n ",
        tostring(pinfo.number),
        rel_time,
        tostring(pinfo.src),
        tostring(pinfo.src_port),
        tostring(pinfo.dst),
        tostring(pinfo.dst_port),
        "MAERKLIN",
        tostring(pinfo.len),
        output,
    }

    return table.concat(line," ")
end

