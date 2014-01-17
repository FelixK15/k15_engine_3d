require("scripts\\parserDebug")
require("scripts\\typeInfo")

function parse(filePath,debugOut)
	debugSettings.output = debugOut
	local file = io.open(filePath,"r")

	if file == nil then 
        print("Could not open header file \"" .. filePath .. "\".")
    else
		print("Parsing " .. filePath .."\n")
        
		parseEnums(file)
		parseTokens(file)
	end
	
end

function parseEnums(file)
	local foundEnums = {}
	local enumTable = nil
	local currentEnum = nil
	local currentValue = 0
	
	for line in file:lines() do
	    if line ~= "" then
            if currentEnum ~= nil then
                --check if this is the end of the enum block
                if string.find(line,"}") ~= nil then
                    currentEnum = nil
                    currentValue = 0
                elseif string.find(line,"{") == nil then --skip the start of the enum block
                    line = string.gsub(line,"%s*","") -- remove white spaces
                    line = string.gsub(line,"%=*","") --remove equal symbol
                    line = string.gsub(line,",","") --remove comma
                    line = string.gsub(line,"//%a+","") --remove comments
                    if string.find(line,"%d") ~= nil then
                        currentValue = string.gsub(line,"%D","")
                        currentValue = tonumber(currentValue)
                        line = string.gsub(line,"%d*","")
                    else
                        currentValue = currentValue + 1
                    end
                    
                    enumTable = foundEnums[currentEnum]
                    enumTable[line] = currentValue
                end
            else
                if string.find(line,"enum") ~= nil then
                    -- found line with "enum" in it. Trying to extract enum name.
                    line = string.gsub(line,"enum","") -- remove 'enum' keyword
                    line = string.gsub(line,"%a+%s*","%1")
                    currentEnum = string.gsub(line,"%s*","") -- remove white spaces
                    foundEnums[currentEnum] = {} --new table for the new enum
                end
            end
	    end
	end
	
	return foundEnums
end

function parseTokens(file)
    --[[local interestingLines = {}
    local tokens = {"serializable"="","expose"=""}
    local counter = 0
        
    for line in file:lines() do
        local token = nil
        for key, value in pairs(tokens) do
            token = value
            if string.find(line,token) ~= nil then
                interestingLines[counter] = line
                counter = counter + 1;
                printDebug("found interesting line \"" .. line .. "\".\n")
            end
        end
    end]]--
end