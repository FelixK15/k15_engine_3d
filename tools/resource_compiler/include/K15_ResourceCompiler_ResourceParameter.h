#ifndef _K15_ResourceCompiler_ResourceParameter_h_
#define _K15_ResourceCompiler_ResourceParameter_h_

#include "K15_ResourceCompiler_Prerequisites.h"

namespace K15_ResourceCompiler
{
	struct ResourceParameter;

	/*********************************************************************************/
	extern const char* PARAMETER_TYPE_DIFFUSE_TEXTURE;
	extern const char* PARAMETER_TYPE_NORMAL_TEXTURE;
	extern const char* PARAMETER_TYPE_SPECULAR_TEXTURE;
	/*********************************************************************************/
	typedef std::vector<ResourceParameter>  ResourceParameters;
	/*********************************************************************************/
	struct ResourceParameter
	{
		String name;
		String value;
	}; // ResourceParameter struct declaration
} // end of K15_ResourceCompiler namespace

#endif //_K15_ResourceCompiler_ResourceParameter_h_
