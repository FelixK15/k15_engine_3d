require(".\\parserDebug")

function parseCPPAttributes(class)
    local attributes = {}
    local exposed = false
    local serializable = false
    local attributeSignature = nil
    local keyword_filer = {"serializable","expose"} --remove those keywords from the attribute signature
    
    -- parse class definition line by line
    for line in class:gmatch("[^\r\n]+") do
        line = string.match(line,"%a+.*"); --trim
        if line ~= nil then
            local attribute = {}
            -- get attribute signature (e.g. "serializable int m_Cost;")
            attributeSignature = line
            for key, value in pairs(keyword_filer) do
                attributeSignature = string.gsub(attributeSignature,value,""); --filter all keywords
                if string.find(line,value) ~= nil then
                    attribute[value] = true
                else
                    attribute[value] = false
                end
            end
            -- get attribute signature without keywords ("serializable int m_Cost;" -> "int m_Cost;")
            attributeSignature = string.match(attributeSignature,".+%s+m_?%a+;")
            
            if attributeSignature ~= nil then
                attribute.signature = string.match(attributeSignature,"%a+.*")
                table.insert(attributes,attribute)
            end
        end
    end
    
    return attributes
end
