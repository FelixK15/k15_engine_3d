require(".\\parserCPPHeader")

function writeCPPEnums(fileName)
    local file = io.open(".\\EnumString.h","r+")
    if file == nil then
        file = io.open(".\\EnumString.h","w")
    end
    local code = ""
    local newFile = false
    local fileHeader = "#ifndef _K15_CORE_ENUMSTRINGS_H_\n#define _K15_CORE_ENUMSTRINGS_H_\n\n"
    for k, v in pairs(enums) do
        code = code .. "const char* " .. k .. "_Str[] = {\n"
        for value, name in pairs(enums[k]) do
            code = code .. "\"" .. name .. "\""
            if value ~= table.getn(enums[k]) then
                code = code .. ",\n"
            end
        end
        code = code .. "\n};\n"
    end
    
    code = "/*START_" .. fileName .. "*/\n" .. code .. "/*END_" .. fileName .. "*/\n"
    
    -- search if code for this enum has been written before
    local fileContent = file:read("*all")
    
    if fileContent == nil or fileContent == "" then
        fileContent = fileHeader
        newFile = true
    end
    
    if fileContent:find(fileName) ~= nil then
       -- written before! 
       -- delete old code
       fileContent = fileContent:gsub("/*START_" .. fileName .. "*/.+/*END_" .. fileName .. "*/",code)
    else
       fileContent = fileContent:gsub(fileHeader,fileHeader .. code)
    end
    
    if newFile == true then
        fileContent = fileContent .. "\n#endif //_K15_CORE_ENUMSTRINGS_H_"
    end
    
    file:close()
    file = io.open("EnumString.h","w+")
    file:write(fileContent)
    file:close()
end