require("enums")
require("LuaHash")

local hashValue = 0

function writeEnum(file, enumName, enumTable)
    local stringType = "const char*"
    
    if flagUseSTLStrings == true then
        stringType = "const std::string"
    end
    
    file:write("/*" .. enumName .." Strings */\n");
    file:write(stringType .. " " .. enumName .. "Str[] = {\n");
    
    for i, k in ipairs(enumTable) do
        file:write("\t\"" .. k .. "\"\n");
    end
    
    file:write("}; //Strings for " .. enumName .. " enumeration.\n\n")
end

function writeHeader(file)
    file:write("/***************************************************/\n");
    file:write("/*Automatically generated file by the k15 enum tool*/\n");
    file:write("/*Generated Hash Value for this file:\""..hashValue.."\"****/\n");
    file:write("/***************************************************/\n");
    file:write("#ifndef K15_ENUM_STRINGS_H_\n");
    file:write("#define K15_ENUM_STRINGS_H_\n");
end

function writeFooter(file)
    file:write("#endif //K15_ENUM_STRINGS_H_");
end

function hashEqual(file)
    if file == nil then
        return false
    end
    
    local fileHash = 0
    
    for line in file:lines() do
        fileHash = line:match("\"(%d+)\"")
        
        if fileHash ~= nil then
            return tonumber(fileHash) == hashValue
        end
    end
    
    return false
end

function generateHash()
    local enumString = ""
    
    for i, k in pairs(enums) do
        enumString = enumString .. i

        for j, v in ipairs(k) do
            enumString = enumString .. v
        end   
    end
    
    hashValue = LuaHash.hash(enumString)
end

function writeEnums(fileName)
    generateHash()

    local file = io.open(fileName,"r")
    -- read the files hash value before writing anything
    
    if hashEqual(file) == false then
        if file ~= nil then
            file:close()
        end
        
        file = assert(io.open(fileName,"w"))
        
        writeHeader(file)
        
        for i, k in pairs(enums) do
            writeEnum(file, i, k)
        end
        
        writeFooter(file)
        
        print("=============================\n");
        print("        Rewrote File!        \n");
        print("=============================\n");
    else
        print("=============================\n");
        print("     Did not rewrite File!   \n");
        print("=============================\n");
    end
    
    file:close();
end