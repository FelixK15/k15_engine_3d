#ifndef _K15_ResourceCompiler_Arguments_h_
#define _K15_ResourceCompiler_Arguments_h_

#include "K15_ResourceCompiler_Prerequisites.h"

namespace K15_ResourceCompiler
{
	class Arguments
	{		
		K15_NON_COPYABLE(Arguments);
		K15_NO_DEFAULT_CONSTRUCTOR(Arguments);
		K15_NON_MOVEABLE(Arguments);

	public:
		Arguments(int p_ArgumentCount, char** p_Arguments);
	
		bool hasCommand(const String& p_Key);
		StringSet getArgumentsForCommand(const String& p_Key);

	private:
		void _fillArgumentMap(int p_ArgumentCount, char** p_Arguments);

	private:
		HashMap(String, StringSet) m_Arguments;
	};
}

#endif //_K15_ResourceCompiler_Arguments_h_