#include "K15_ResourceCompiler_Arguments.h"
#include "K15_ResourceCompiler_CompilerEnvironment.h"
#include "K15_ResourceCompiler_Compiler.h"
#include "K15_ResourceCompiler_Logging.h"
#include "K15_ResourceCompiler_Help.h"
#include "K15_ResourceCompiler_StringUtil.h"

using namespace K15_ResourceCompiler;

int main(int argc, char** argv)
{
	Arguments arguments(argc + 1, argv);
	CompilerEnvironment environment;

	String workingDir = argv[0];
	
	std::replace(workingDir.begin(), workingDir.end(), '\\', '/');

	//get actual working dir
	workingDir = workingDir.substr(0, workingDir.find_last_of('/'));

	environment.setWorkingDir(workingDir);

	if(arguments.hasCommand("-help"))
	{
		Help::printHelp();
	}

	if(arguments.hasCommand("-o"))
	{
		String firstArgument = (*arguments.getArgumentsForCommand("-o").begin());
		environment.setOutputPath(StringUtil::toPath(firstArgument));
	}

	if(arguments.hasCommand("-i"))
	{
		String firstArgument = (*arguments.getArgumentsForCommand("-i").begin());
		environment.setInputPath(firstArgument);
	}

	if(arguments.hasCommand("-u"))
	{
		environment.setFileCollisionBehavior(FEB_REPLACE_EXISTING_FILE);
	}
	else
	{
		environment.setFileCollisionBehavior(FEB_DONT_REPLACE_EXISTING_FILE);
	}

	Compiler resourceCompiler(environment);

	resourceCompiler.addDefaultProcessors();

	if(!resourceCompiler.compile())
	{
		LOG_ERROR(resourceCompiler.getError().c_str());
	}
	else
	{
		LOG_NORMAL("Resourcecompiler finished");
	}

	return resourceCompiler.getReturnValue();
}