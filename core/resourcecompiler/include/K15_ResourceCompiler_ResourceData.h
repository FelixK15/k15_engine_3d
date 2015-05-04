#ifndef _K15_ResourceCompiler_ResourceData_h_
#define _K15_ResourceCompiler_ResourceData_h_

#include "K15_ResourceCompiler_Prerequisites.h"
#include "K15_ResourceCompiler_ResourceParameter.h"

#include "enums/K15_ResourceCompiler_ResourceType.h"

namespace K15_ResourceCompiler
{
	struct ResourceParameter;
	struct ResourceData;

	/*********************************************************************************/
	typedef std::vector<ResourceData>		ResourceDependencies;
	typedef std::vector<ResourceData>		ResourceDataVector;
	/*********************************************************************************/

	struct ResourceData
	{
		ResourceType type;
		String filePath;
		ResourceDependencies dependencies;
		ResourceParameters parameters;
	}; // ResourceData struct declaration
} // end of K15_ResourceCompiler namespace

#endif //_K15_ResourceCompiler_ResourceData_h_