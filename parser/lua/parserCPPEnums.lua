require(".\\parserDebug")

function parseCPPEnums(content)
    local foundEnums = {}
	local enumTable = nil
	local currentEnum = nil
	local currentValue = 1
	
	for line in content:gmatch("[^\r\n]+") do
	    if line ~= nil or line ~= "" then
            if currentEnum ~= nil then
                --check if this is the end of the enum block
                if string.find(line,"}") ~= nil then
                    currentEnum = nil
                    currentValue = 1
                elseif string.find(line,"{") == nil then --skip the start of the enum block
                    line = string.gsub(line,"%s*","") -- remove white spaces
                    line = string.gsub(line,"%=*","") --remove equal symbol
                    line = string.gsub(line,",","") --remove comma
                    line = string.gsub(line,"//%a+","") --remove comments
                    if string.find(line,"%d") ~= nil then
                        currentValue = string.gsub(line,"%D","")
                        currentValue = tonumber(currentValue)
                        
                        if currentValue == 0 then
                            currentValue = 1 -- Lua's indices start at 1, no 0
                        end
                        
                        line = string.gsub(line,"%d*","")
                    else
                        currentValue = currentValue + 1
                    end
                    
                    enumTable = foundEnums[currentEnum]
                    enumTable[currentValue] = line
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