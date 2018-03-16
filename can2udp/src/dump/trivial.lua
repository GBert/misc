-- declare our protocol
local trivial_proto = Proto("trivial","Trivial Protocol")

local trivial_pdu_len = 4
local makeLine

local function dissect_common(buffer, pinfo, tree, offset)

    local subtree = tree:add(trivial_proto,buffer(offset,trivial_pdu_len),"Trivial Protocol Data")

    subtree:add(buffer(offset,2),"The first two bytes: " .. buffer(offset,2):uint())
    subtree = subtree:add(buffer(2,2),"The next two bytes")
    subtree:add(buffer(offset+2,1),"The 3rd byte: " .. buffer(offset+2,1):uint())
    subtree:add(buffer(offset+3,1),"The 4th byte: " .. buffer(offset+3,1):uint())

    local output = "Trivial Info (" .. buffer(offset,2):uint() ..")"

    -- return number of bytes consumed so that more trivial PDUs can be discovered
    return trivial_pdu_len, output
end

-- create a function to dissect it
function trivial_proto.dissector(buffer, pinfo, tree)   
    pinfo.cols.protocol = "TRIVIAL"

    local pktlen = buffer:len()

    local consumed, output = dissect_common(buffer, pinfo, tree, 0)
    local remaining = pktlen - consumed
    pinfo.cols.info:set(output)

    while remaining >= trivial_pdu_len do
        consumed, output = dissect_common(buffer, pinfo, tree, pktlen - remaining)
        pinfo.cols.info:append(makeLine(pinfo,output))
        remaining = remaining - consumed
    end

    if remaining > 0 then
        pinfo.desegment_offset = pktlen - remaining
        pinfo.desegment_len = trivial_pdu_len - remaining
    end

    return pktlen - remaining
end

-- load the tcp.port table
local tcp_table = DissectorTable.get("tcp.port")
-- register our protocol to handle tcp port 15731
tcp_table:add(15731,trivial_proto)

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
        "TRIVIAL",
        tostring(pinfo.len),
        output,
    }

    return table.concat(line," ")
end
