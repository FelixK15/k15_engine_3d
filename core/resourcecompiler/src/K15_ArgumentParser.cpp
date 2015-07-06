/*********************************************************************************/
enum K15_ArgumentParserState
{
	K15_PARSER_STATE_NONE = 0,
	K15_PARSER_STATE_INPUT,
	K15_PARSER_STATE_OUTPUT
};
/*********************************************************************************/
struct K15_ArgumentParser
{
	K15_ArgumentParserState state;
	char* inputPath;
	char* outputPath;
	bool8 recursive;
	bool8 replace;
	bool8 daemon;
};
/*********************************************************************************/



/*********************************************************************************/
void K15_ParseArguments(K15_ArgumentParser* p_ArgumentParser, int p_ArgumentCount, char** p_Arguments)
{
	for (int argumentIndex = 0;
		argumentIndex < p_ArgumentCount;
		++argumentIndex)
	{
		char* argument = p_Arguments[argumentIndex];
		K15_ArgumentParserState parserState = p_ArgumentParser->state;

		if (parserState == K15_PARSER_STATE_NONE)
		{
			if (strcmp(argument, "-i") == 0)
			{
				parserState = K15_PARSER_STATE_INPUT;
			}

			if (strcmp(argument, "-o") == 0)
			{
				parserState = K15_PARSER_STATE_OUTPUT;
			}

			if (strcmp(argument, "-r") == 0)
			{
				p_ArgumentParser->recursive = K15_TRUE;
				continue;
			}

			if (strcmp(argument, "-d") == 0)
			{
				p_ArgumentParser->daemon = K15_TRUE;
				K15_PrintDaemonInfo();
				continue;
			}

			if (strcmp(argument, "-u") == 0)
			{
				p_ArgumentParser->replace = K15_TRUE;
				continue;
			}

			if (strcmp(argument, "-h") == 0)
			{
				K15_PrintHelp();
				exit(0);
			}
		}
		else if (parserState == K15_PARSER_STATE_INPUT || parserState == K15_PARSER_STATE_OUTPUT)
		{
			char* convertedArgument = K15_ConvertToDirectoryPath(argument);

			if (parserState == K15_PARSER_STATE_INPUT)
			{
				if (p_ArgumentParser->inputPath)
				{
					K15_LOG_WARNING_MESSAGE("Input path already set. Overwriting old input path '%s' with new input path '%s'", p_ArgumentParser->inputPath, convertedArgument);
					free(p_ArgumentParser->inputPath);
				}

				p_ArgumentParser->inputPath = convertedArgument;
			}
			else
			{
				if (p_ArgumentParser->outputPath)
				{
					K15_LOG_WARNING_MESSAGE("Output path already set. Overwriting old output path '%s' with new output path '%s'", p_ArgumentParser->outputPath, convertedArgument);
					free(p_ArgumentParser->outputPath);
				}

				p_ArgumentParser->outputPath = convertedArgument;
			}

			parserState = K15_PARSER_STATE_NONE;
		}

		p_ArgumentParser->state = parserState;
	}
}
/*********************************************************************************/
bool8 K15_CheckArguments(K15_ArgumentParser* p_ArgumentParser)
{
	bool8 valid = K15_TRUE;

	if (!p_ArgumentParser->inputPath)
	{
		K15_LOG_ERROR_MESSAGE("Not input path specified. Use option '-i' to specify an input path.");
		valid = K15_FALSE;
	}
	else if(K15_DirectoryExists(p_ArgumentParser->inputPath) == K15_FALSE)
	{
		K15_LOG_ERROR_MESSAGE("Input path '%s' is not a valid directory or the directory does not exist.", p_ArgumentParser->inputPath);
		valid = K15_FALSE;
	}

	if (!p_ArgumentParser->outputPath)
	{
		K15_LOG_ERROR_MESSAGE("No output path specified. Use option '-o' to specify an output path.");
		valid = K15_FALSE;
	}
	else if(K15_DirectoryExists(p_ArgumentParser->outputPath) == K15_FALSE)
	{
		K15_LOG_ERROR_MESSAGE("Output path '%s' is not a valid directory or the directory does not exist.", p_ArgumentParser->outputPath);
		valid = K15_FALSE;
	}

	if (p_ArgumentParser->state != K15_PARSER_STATE_NONE)
	{
		K15_LOG_ERROR_MESSAGE("Arguments ended unexpectatly. No argument given for %s.", p_ArgumentParser->state == K15_PARSER_STATE_INPUT ? "Input path" : "Output path");
		valid = K15_FALSE;
	}

	if (valid == K15_FALSE)
	{
		K15_LOG_ERROR_MESSAGE("Detected missing or wrong arguments. You can use option '-h' to display the help message.");
	}

	return valid;
}
/*********************************************************************************/