require(".\\parserDebug")

function parseCPPEnums(content)
    local foundEnums = {}
    local currentEnum = nil
	local enumTable = nil
	local value = 0
	
	for enum in content:gmatch("enum%s+[%a%p%d]+%s*{.-}") do
	    local enumName = string.match(enum,"enum%s*([%a%p%d]+)")
	    currentEnum = {}
	    
	    local enumValues = string.gsub(string.match(enum,"{(.-)}"),",","")
	    for enumKey in enumValues:gmatch("%s*([%a%p%d]+%s*=?%s*%d?)") do
	        enumKey = enumKey:gsub("[\n%s*]","")
            enumValue = enumKey:match(".+=+(%d+)")
            enumKey = enumKey:gsub("=%d*","")
            if enumValue ~= nil then
                value = tonumber(enumValue) + 1
            else
                value = value + 1
            end
            
            currentEnum[value] = enumKey
	    end
	    
	    foundEnums[enumName] = currentEnum
	end
	
	return foundEnums
end