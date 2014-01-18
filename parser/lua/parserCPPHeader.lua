require(".\\parserHelper")
require(".\\parserCPPClass")
require(".\\parserCPPEnums")

--global enum table
enums = {}

function parseCPPHeader(file)
    local content = file:read("*all")
    local currentClass = nil
    
    enums = parseCPPEnums(content)
    
    repeat
        currentClass = string.match(content,"class.+{.+};")
        if currentClass ~= nil then
            parseCPPClass(cleanCPPSnippet(currentClass))
            content = content:sub(content:find("class.+{.+};") + currentClass:len())
        end
    until currentClass == nil
    
end