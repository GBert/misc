-- declare our protocol
local maerklin_proto = Proto("maerklin","Maerklin Protocol")

local maerklin_pdu_len = 13
local makeLine

local function dissect_common(buffer, pinfo, tree, offset)


    local rbit, command, dlc

    local subtree = tree:add(maerklin_proto,buffer(offset,maerklin_pdu_len),"Maerklin Protocol Data")

    subtree:add(buffer(offset,1)  ,"Prio:    " .. buffer(offset,1):uint())

    command = bit.band ( buffer(offset+1,1):uint(), 0xfe)
    rbit = bit.band ( buffer(offset+1,1):uint(), 0x01)
    dlc = buffer(offset+4,1):uint()

    if command == 0x00 then
        subtree:add(buffer(offset+1,1),"Command: System " .. string.format('0x%02x',buffer(offset+1,1):uint()))
    elseif command == 0x02 then
        subtree:add(buffer(offset+1,1),"Command: Loc Discovery " .. string.format('0x%02x', buffer(offset+1,1):uint()))
    elseif command == 0x04 then
        subtree:add(buffer(offset+1,1),"Command: MFX Bind " .. string.format('0x%02x',buffer(offset+1,1):uint()))
    elseif command == 0x36 then
        subtree:add(buffer(offset+1,1),"Command: Bootloader (CAN) " .. string.format('0x%02x',buffer(offset+1,1):uint()))
    else
        subtree:add(buffer(offset+1,1),"Command: unknown " .. string.format('0x%02x',buffer(offset+1,1):uint()))
    end
    if rbit == 1 then
        subtree:add(buffer(offset+0,1),"Responsebit:     set")
    else
        subtree:add(buffer(offset+0,1),"Responsebit: not set")
    end
    subtree:add(buffer(offset+2,2),"Hash:    " .. string.format('0x%04x',buffer(offset+2,2):uint()))
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

    local output
    if command == 0 then
	output = "Maerklin CAN (" .. string.format("System  0x%02x",buffer(offset,2):uint()) ..")"
    elseif command == 0x02 then
	output = "Maerklin CAN (" .. string.format("Loco Discovery  0x%02x",buffer(offset,2):uint()) ..")"
    elseif command == 0x04 then
	output = "Maerklin CAN (" .. string.format("MFX Bind    0x%02x",buffer(offset,2):uint()) ..")"
    elseif command == 0x06 then
	output = "Maerklin CAN (" .. string.format("MFX Verify  0x%02x",buffer(offset,2):uint()) ..")"
    elseif command == 0x08 then
	output = "Maerklin CAN (" .. string.format("Loco Velocity  0x%02x",buffer(offset,2):uint()) ..")"
    elseif command == 0x0A then
	output = "Maerklin CAN (" .. string.format("Loco Direction 0x%02x",buffer(offset,2):uint()) ..")"
    elseif command == 0x0C then
	output = "Maerklin CAN (" .. string.format("Loco Function  0x%02x",buffer(offset,2):uint()) ..")"
    elseif command == 0x22 then
	output = "Maerklin CAN (" .. string.format("S88 Event   0x%02x",buffer(offset,2):uint()) ..")"
    elseif command == 0x30 then
	output = "Maerklin CAN (" .. string.format("Software Version, Ping 0x%02x",buffer(offset,2):uint()) ..")"
    elseif command == 0x36 then
	output = "Maerklin CAN (" .. string.format("Bootloader CAN   0x%02x",buffer(offset,2):uint()) ..")"
    elseif command == 0x38 then
	output = "Maerklin CAN (" .. string.format("Bootloader Track 0x%02x",buffer(offset,2):uint()) ..")"
    elseif command == 0x3A then
	output = "Maerklin CAN (" .. string.format("Status Config  0x%02x",buffer(offset,2):uint()) ..")"
    elseif command == 0x40 then
	output = "Maerklin CAN (" .. string.format("Config Data Request 0x%02x",buffer(offset,2):uint()) ..")"
    elseif command == 0x42 then
	output = "Maerklin CAN (" .. string.format("Config Data Stream 0x%02x",buffer(offset,2):uint()) ..")"
    elseif command == 0x60 then
	output = "Maerklin CAN (" .. string.format("Start Automatic 0x%02x",buffer(offset,2):uint()) ..")"
    else
	output = "Maerklin CAN (" .. string.format("Unknown 0x%02x",buffer(offset,2):uint()) ..")"
    end
    -- return number of bytes consumed so that more maerklin PDUs can be discovered
    return maerklin_pdu_len, output
end

-- create a function to dissect it
function maerklin_proto.dissector(buffer, pinfo, tree)   
    -- pinfo.cols.protocol = "MAERKLIN"

    local pktlen = buffer:len()

    local consumed, output = dissect_common(buffer, pinfo, tree, 0)
    local remaining = pktlen - consumed
    pinfo.cols.info:set(output)

    while remaining >= maerklin_pdu_len do
        consumed, output = dissect_common(buffer, pinfo, tree, pktlen - remaining)
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

