require("enums")
require("writeEnums")
require("lfs")

local enumPattern = "enum%s+[%a%d_]+%s*%{[%d%a_=,%s]+%}"
local enumNamePattern = "enum%s+([%a%d_]+)%s*%{[%d%a_=,%s]+%}"
local enumBlockPattern = "enum%s+[%a%d_]+%s*%{([%d%a_=,%s]+)%}"
local enumValuePattern = "([%a%d_=]+),?"

function parseEnumBlock(enumBlock)
    local enumName = ""
    local enumValues = nil
    local counter = 1
    local enumValue = nil
    
    -- get the name of the enum and the block containing the values for the enum
    enumName = enumBlock:match(enumNamePattern)
    enumValueBlock = enumBlock:match(enumBlockPattern)
    enumValueBlock = enumValueBlock:gsub("%s","")
    
    -- check if a table for this enum already exists.
    if enums[enumName] ~= nil then
        enumValues = enums[enumName]
    else
        enumValues = {}
    end
    
    -- iterate through all values of the enum
    for value in enumValueBlock:gmatch(enumValuePattern) do
        local enumValuePos = value:find("=")
        
        --check if the value had a numerical value
        if enumValuePos ~= nil then
            enumValue = value:match("=(%d)")
            enumValue = tonumber(enumValue) + 1
        else
            enumValue = nil
        end
        
        -- if the value had no numerical value then set the numerical value to the value of the counter
        if enumValue == nil then
            enumValue = counter
        end
        
        -- insert value without its numerical value to the table and increase counter
        enumValues[enumValue] = value:gsub("=%d+","")
        counter = counter + 1
    end
    
    enums[enumName] = enumValues
end

function parseForEnums(filePath)
	local file = assert(io.open(filePath,"r"))
	local fileContent = file:read("*a")
	file:close();
	-- remove comments, defines, etc.
    fileContent = fileContent:gsub("%/%/[^\n]*","")
    fileContent = fileContent:gsub("/%*.-%*/","")
    fileContent = fileContent:gsub("%s*#%s*define[^\n]*","")
    fileContent = fileContent:gsub("[\n\t]","")

    print("Scanning C/C++ file \"" .. filePath .. "\"...\n");

    -- parse each enum block in the current file
    for enumBlock in fileContent:gmatch(enumPattern) do
    	parseEnumBlock(enumBlock)
    end 
end

function readDirectory(path,recursive)
    path = path:gsub("%*","")
    for file in lfs.dir(path) do
        if file ~= "." and file ~= ".." then
            file = path .. file
            
            local attr = lfs.attributes(file)
            if attr.mode == "directory" then
                if recursive then
                    readDirectory(file .. "\\",recursive)
                end
            else
                if file:match(".+%.cpp") or file:match(".+%.h") then
                    parseForEnums(file)
                end
            end
        end
    end
end

function parseInputListFile(filePath)
    local file = assert(io.open(filePath,"r"))   
    
    for line in file:lines() do
        readDirectory(line,flagRecursive)
    end
    
    file:close()
end