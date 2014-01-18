require(".\\parserDebug")
require(".\\parserCPPHeader")
require(".\\writerCPPEnums")

function parse(filePath,debugOut)
    debugSettings.output = debugOut
    local file = io.open(filePath,"r")
    local fileName = filePath:gsub("[[%d%a%p%s]*\\+","")
    if file == nil then
        print("Could not open header file \"" .. filePath .. "\".")
    else
        print("Parsing " .. filePath .. " ...")
        parseCPPHeader(file)
        writeCPPEnums(fileName)
    end
end