#include "K15_ArgumentParser.h"
#include "K15_Logging.h"

#include <cstring>
#include <memory>

/*********************************************************************************/
K15_ArgumentParserState K15_ParseArgument(K15_ArgumentParserContext* p_Context, const char* p_Argument)
{
	K15_ArgumentParserState outputState = PARSER_STATE_NO_STATE;
	K15_ArgumentParserState inputState = p_Context->state;

	switch(inputState)
	{
	case PARSER_STATE_NO_STATE:
		{
			if (strcmp("-i", p_Argument) == 0)
			{
				//template file input
				outputState = PARSER_STATE_INPUT;
				break;
			}
			else if (strcmp("-o", p_Argument) == 0)
			{
				//code output
				outputState = PARSER_STATE_OUTPUT;
				break;
			}
			else if (strcmp("-t", p_Argument) == 0)
			{
				//template define
				outputState = PARSER_STATE_TEMPLATE_DEFINE;
				break;
			}
			else if (strcmp("-h", p_Argument) == 0)
			{
				p_Context->flags |= PARSER_NO_CHECK;
				//show help
				break;
			}
			else if (strcmp("-v", p_Argument) == 0)
			{
				//verbose parser
				p_Context->flags |= PARSER_VERBOSE;
				break;
			}
			else
			{
				K15_LOG_ERROR_MESSAGE("Undefined argument '%s' (Ignoring argument).", p_Argument);
				break;
			}
		}

	case PARSER_STATE_OUTPUT:
		{
			const char* outputFile = p_Context->outputFile;

			if (outputFile)
			{
				//already has output
				K15_LOG_WARNING_MESSAGE("Too many output files defined. Previous output file was '%s'. Overwriting old output with '%s'.", outputFile, p_Argument);
			}

			K15_LOG_VERBOSE(p_Context, "Found output file '%s'.", p_Argument);

			p_Context->outputFile = p_Argument;
			break;
		}

	case PARSER_STATE_INPUT:
		{
			const char* inputFile = p_Context->inputFile;

			if (inputFile)
			{
				//already has input
				K15_LOG_WARNING_MESSAGE("Too many input files defined. Previous input file was '%s'. Overwriting old input with '%s'.", inputFile, p_Argument);
			}

			K15_LOG_VERBOSE(p_Context, "Found input file '%s'.", p_Argument);

			p_Context->inputFile = p_Argument;
			break;
		}

	case PARSER_STATE_TEMPLATE_DEFINE:
		{
			size_t defineLength = strlen(p_Argument);

			char* templateDefine = (char*)malloc(defineLength + 1); //+1 for 0 terminator
			char* templateName = 0;
			char* templateValue = 0;

			memcpy(templateDefine, p_Argument, defineLength);
			templateDefine[defineLength] = 0;

			int numTemplateDefines = p_Context->numTemplateDefines;
			int defineIndex = 0;
			unsigned char equalFound = 0;

			if (numTemplateDefines == K15_MAX_TEMPLATE_INPUT)
			{
				unsigned int numTemplateDefineMax = K15_MAX_TEMPLATE_INPUT;
				K15_LOG_ERROR_MESSAGE("Too many template defines. Could not process '%s' (Only %d template defines are allowed).", p_Argument, numTemplateDefineMax);
				break;
			}

			K15_LOG_VERBOSE(p_Context, "Found template argument '%s'.", p_Argument);

			while(templateDefine[defineIndex] != 0)
			{
				if (templateDefine[defineIndex++] == '=')
				{
					if (defineIndex == 0)
					{
						//invalid. define name is not given
						K15_LOG_ERROR_MESSAGE("No template name specified for template argument '%s' (Ignoring template argument).", templateDefine);
						break;
					}
					else if(defineIndex == defineLength)
					{
						//invalid. define value is not given
						K15_LOG_ERROR_MESSAGE("No template value specified for template argument '%s' (Ignoring template argument).", templateDefine);
						break;
					}

					equalFound = 1;
					templateName = templateDefine;
					templateName[(defineIndex - 1)] = 0; //set equal sign as 0 terminator to separate name and value

					templateValue = (templateDefine + defineIndex);

					p_Context->templateDefines[numTemplateDefines].name = templateName;
					p_Context->templateDefines[numTemplateDefines].define = templateValue;
					++p_Context->numTemplateDefines;

					break;
				}
			}

			if (equalFound == 0)
			{
				K15_LOG_ERROR_MESSAGE("Could not process template argument '%s'. Syntax is 'NAME=VALUE'. (Ignoring template argument).", p_Argument);
			}

			break;
		}
	}

	return outputState;
}
/*********************************************************************************/
unsigned int K15_CheckArguments(K15_ArgumentParserContext* p_Context)
{
	unsigned int returnValue = 0;

	if ((p_Context->flags & PARSER_NO_CHECK) == 0)
	{
		if (!p_Context->inputFile)
		{
			K15_LOG_ERROR_MESSAGE("No input file defined.");
			returnValue = 1;
		}

		if (!p_Context->outputFile)
		{
			K15_LOG_ERROR_MESSAGE("No output file defined.");
			returnValue = 1;
		}

		if (p_Context->numTemplateDefines == 0)
		{
			K15_LOG_ERROR_MESSAGE("No template arguments defined.");
			returnValue = 1;
		}
	}

	return returnValue;
}
/*********************************************************************************/