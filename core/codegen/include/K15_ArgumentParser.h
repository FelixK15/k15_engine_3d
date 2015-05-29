#ifndef _K15_ArgumentParser_h_
#define _K15_ArgumentParser_h_

#define K15_MAX_TEMPLATE_INPUT 256

#define K15_LOG_VERBOSE(c, m, ...) {if((c->flags & PARSER_VERBOSE) > 0){K15_LOG_NORMAL_MESSAGE(m, ##__VA_ARGS__);}}

/*********************************************************************************/
enum K15_ArgumentParserState
{
	PARSER_STATE_NO_STATE = 0,
	PARSER_STATE_TEMPLATE_DEFINE,
	PARSER_STATE_INPUT,
	PARSER_STATE_OUTPUT
};
/*********************************************************************************/
enum K15_ArgumentParserFlags
{
	PARSER_VERBOSE = 0x01,
	PARSER_NO_CHECK = 0x02
};
/*********************************************************************************/
struct K15_TemplateDefine
{
	const char* name;
	const char* define;
};
/*********************************************************************************/
struct K15_ArgumentParserContext
{
	K15_TemplateDefine templateDefines[K15_MAX_TEMPLATE_INPUT];
	K15_ArgumentParserState state;
	unsigned int flags;
	unsigned int numTemplateDefines;
	const char* workingDir;
	const char* outputFile;
	const char* inputFile;
};
/*********************************************************************************/

K15_ArgumentParserState K15_ParseArgument(K15_ArgumentParserContext* p_Context, const char* p_Argument);
unsigned int K15_CheckArguments(K15_ArgumentParserContext* p_Context);

#endif //_K15_ArgumentParser_h_