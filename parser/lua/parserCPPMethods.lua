require(".\\parserDebug")

function parseCPPMethods(class)
    local methods = {}
    local keyword_filter = {"INLINE","OVERRIDE"} --remove those keywords from the method signature
    local methodSignature = nil
    
    -- parse the class definition line by line
    for line in class:gmatch("[^\r\n]+") do
        for key, value in pairs(keyword_filter) do --filter all keywords
            line = string.gsub(line,value,"")
        end
        
        -- try to get method signature (e.g. "void doStuff(int i);")
        methodSignature = string.match(line,".*%a+%s*%a+%(.*%).*;") --filter class
        
        -- if method signature could be retrieved, save it in the 'methods' table
        if methodSignature ~= nil then
            table.insert(methods,string.match(methodSignature,"%a+.*")) --trim method signature and add to table
        end
    end

    return methods
end