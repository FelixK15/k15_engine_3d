#include "K15_ResourceCompiler_SearchUtil.h"
#include "K15_ResourceCompiler_ResourceParameter.h"

namespace K15_ResourceCompiler
{
	/*********************************************************************************/
	bool SearchUtil::findTextureParameterNormalMap(const ResourceParameter& p_ResourceParameter)
	{
		return p_ResourceParameter.name == PARAMETER_TYPE_NORMAL_TEXTURE;
	}
	/*********************************************************************************/

} // end of K15_ResourceCompiler namespace
