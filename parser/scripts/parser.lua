local debugOutput = false

function printDebug(text)
    if debugOutput == true then
        print(text)
    end
end

function parse(filePath,debugOut)
    debugOutput = debugOut
    local file = io.open(filePath,"r")
    
    if file == nil then 
        print("Could not open header file \"" .. filePath .. "\".")
    else
        print("Parsing " .. filePath .."\n")
        
            for line in file:lines() do
                printDebug("Parsing line:\"" .. line .. "\"\n")
            end
    end
end