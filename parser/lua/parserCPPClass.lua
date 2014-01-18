require(".\\parserCPPMethods")
require(".\\parserCPPAttributes")
require(".\\parserDebug")

-- global table in which class information will get saved
classes = {}

function parseCPPClass(class)
    -- get class name (e.g. 'GameObject')
    local className = string.gsub(string.match(class,"class%s*%a*"),"class%s*","")
    -- get class signature (e.g. 'class GameObject : public Object, public SerializableObject<GameObject>
    local classSignature = class:match("[^\r\n]+")
    -- check if class has been parsed before
    if classes[className] ~= nil then
        print("Class \"" .. className .. "\" already parsed.")
        return
    end
    
    -- create new table for class and fill it with attributes and methods
    classes[className] = {} 
    classes[className].methods = parseCPPMethods(class)
    classes[className].attributes = parseCPPAttributes(class)
    classes[className].baseClasses = {}
    
    -- get the base classes of this class and also save them in the table
    local baseClasses = classSignature:match(":+%s*(.+),?")
        if baseClasses ~= nil then
            for baseClass in baseClasses:gmatch("[private?public?protected?]+%s+[%d%a%p]+") do
            table.insert(classes[className].baseClasses,baseClass)
            baseClasses = baseClasses:sub(baseClass:len())
        end
    end
end