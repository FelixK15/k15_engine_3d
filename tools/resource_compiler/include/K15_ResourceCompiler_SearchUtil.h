#ifndef _K15_ResourceCompiler_SearchUtil_h_
#define _K15_ResourceCompiler_SearchUtil_h_

#include "K15_ResourceCompiler_Prerequisites.h"

namespace K15_ResourceCompiler
{
	struct ResourceParameter;

	class SearchUtil
	{
	public:
		static bool findTextureParameterNormalMap(const ResourceParameter& p_ResourceParameter);
	};
} // end of K15_ResourceCompiler namespace

#endif //_K15_ResourceCompiler_SearchUtil_h_
