#include "K15_Reflect_Prerequisites.h"
#include "K15_Reflect_Arguments.h"

using namespace K15_Reflect;

int main(int argc, char** argv)
{
	Arguments arguments(argc, argv);

	if(arguments.count() <= 1)
	{
		printf("No arguments...\n");
		return 1;
	}

	std::string inputFile;
	std::string outputFile;

	if(arguments.has("-i"))
	{
		inputFile = arguments.getValue("-i");
	}
	else
	{
		printf("No inputfile assigned...\n");
		return 2;
	}

	if(arguments.has("-o"))
	{
		outputFile = arguments.getValue("-o");
	}
	else
	{
		printf("No outputfile assigned...\n");
		return 3;
	}

	HeaderScanner scanner(inputFile);
	ScannerResult result = scanner.scan();


	return 0;
}