require("getopt")
require("parseEnum")
require("enums")

function main(arguments)
	table.remove(arguments,1);

    for opt, arg in os.getopt(arguments, "hi:o:l:sr") do
    	if opt == "h" then -- help

    	elseif opt == "i" then -- input file
    		flagInputFile = true
    		inputFile = arg
        elseif opt == "o" then -- output file
            flagOutputFile = true
            outputFile = arg
        elseif opt == "l" then -- ignore last value
            flagIgnoreLastItem = true
        elseif opt == "s" then -- use stl strings
            flagUseSTLStrings = true
        elseif opt == "r" then -- also scan subdirectories
            flagRecursive = true
    	end
    end
    
    if flagInputFile == false then
        print("No input file has been specified")
        do return end
    end
    
    if flagOutputFile == false then
        print("No output file has been specified")
        do return end
    end
    
    if inputFile:find(".txt") ~= nil then
        print("Scanning all files listed in the text file \"" .. inputFile .. "\".");
        parseInputListFile(inputFile)
    else
        parseForEnums(inputFile)
    end 
    
    writeEnums(outputFile)
end