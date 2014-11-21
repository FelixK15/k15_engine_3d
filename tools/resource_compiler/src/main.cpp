#include "K15_ResourceCompiler_Arguments.h"
#include "K15_ResourceCompiler_Help.h"

using namespace K15_ResourceCompiler;

int main(int argc, char** argv)
{
	Arguments arguments(argc + 1, argv);

	if(arguments.hasCommand("-help"))
	{
		Help::printHelp();
	}


	_getch();
	return 0;
}