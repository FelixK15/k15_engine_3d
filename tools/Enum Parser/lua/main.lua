require("getopt")
require("parseEnum")
require("enums")


function printHelp()
    local helpMsg = "";

    helpMsg = [[
EnumParser command line tool
Syntax:
enum_tool.exe [-h] [-i input] [-o output] [-l] [-s] [-r]

Options:

   -t         Displays the help message.

   -i input   Specify either a single C/C++ file (.h, .inl, .c, .cpp)
              or a .txt file with a list of directories, which contains
              C/C++ files. The tool will parse the file(s) for enum blocks.

   -o output  Specify a file, where the string arrays will get written to.

   -l         If specified, the tool will ignore the last value inside
              an enum block.

   -s         The string arrays that will get written are normally of
              type const char*. If you want to use std::string instead,
              use this option.

   -r         If the input is a list of directories written to a .txt file,
              use this option to search the directories recursively.]]

    print(helpMsg)
end
function main(arguments)
	table.remove(arguments,1);

    for opt, arg in os.getopt(arguments, "hi:o:l:sr") do
    	if opt == "h" then -- help
            printHelp()
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