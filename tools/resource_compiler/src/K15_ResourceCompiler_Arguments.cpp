#include "K15_ResourceCompiler_Arguments.h"
#include "K15_ResourceCompiler_Logging.h"

namespace K15_ResourceCompiler
{
	/*********************************************************************************/
	Arguments::Arguments(int p_ArgumentCount, char** p_Arguments)
		: m_Arguments()
	{
		_fillArgumentMap(p_ArgumentCount, p_Arguments);
	}
	/*********************************************************************************/
	bool Arguments::hasCommand( const String& p_Key )
	{
		HashMap(String, StringSet)::const_iterator iter = 
			m_Arguments.find(p_Key);

		return iter != m_Arguments.end();
	}
	/*********************************************************************************/
	StringSet Arguments::getArgumentsForCommand( const String& p_Key )
	{
		StringSet arguments;

		HashMap(String, StringSet)::const_iterator iter = 
			m_Arguments.find(p_Key);

		if(iter != m_Arguments.end())
		{
			arguments = iter->second;
		}

		return arguments;
	}
	/*********************************************************************************/
	void Arguments::_fillArgumentMap(int p_ArgumentCount, char** p_Arguments)
	{
		String currentCommand;
		String previousArgument;
		String currentArgument;

		for(int i = 1; i < p_ArgumentCount; ++i)
		{
			if(p_Arguments[i])
			{
				currentArgument = p_Arguments[i];

				if(currentArgument[0] != '-')
				{
					if(previousArgument.empty())
					{
						LOG_ERROR("Invalid argument \"%s\".", currentArgument.c_str());
					}
					else
					{
						StringSet& commandArguments = m_Arguments[currentCommand];
						commandArguments.insert(currentArgument);
					}
				}
				else
				{
					currentCommand = currentArgument;
					m_Arguments.insert(HashPair(String, StringSet)(currentCommand, StringSet()));
				}

				previousArgument = currentArgument;
			}
		}
	}
	/*********************************************************************************/
} // end of namespace K15_ResourceCompiler