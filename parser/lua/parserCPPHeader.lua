require(".\\parserHelper")
require(".\\parserCPPClass")
require(".\\parserCPPEnums")

--global enum table
enums = {}

function parseCPPHeader(file)
    local content = cleanCPPSnippet(file:read("*all"))
    local currentClass = nil
    
    enums = parseCPPEnums(content)
    
    repeat
        currentClass = string.match(content,"class.+{.+};")
        if currentClass ~= nil then
            parseCPPClass(currentClass)
            content = content:sub(content:find("class.+{.+};") + currentClass:len())
        end
    until currentClass == nil
    
end