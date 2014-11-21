#include "K15_Reflect_Arguments.h"

namespace K15_Reflect
{
	namespace internal
	{
		std::vector<Argument> retrieveArguments(int p_ArgumentCount, char** p_ArgumentArray)
		{
			std::vector<Argument> args;
			for(int i = 0;i < p_ArgumentCount; ++i)
			{
				Argument arg;
				arg.name = p_ArgumentArray[i];

				if(i + 1 != p_ArgumentCount)
				{
					if(!strstr(p_ArgumentArray[i + 1], "-"))
					{
						arg.value = p_ArgumentArray[++i];
					}	
				}
				
				args.push_back(arg);
			}

			return args;
		}
	}

	/*********************************************************************************/
	Arguments::Arguments(int p_ArgumentCount, char** p_ArgumentArray)
		: m_Arguments()
	{
		m_Arguments = internal::retrieveArguments(p_ArgumentCount, p_ArgumentArray);	
	}
	/*********************************************************************************/
	Arguments::~Arguments()
	{
		m_Arguments.clear();
	}
	/*********************************************************************************/
	uint32 const Arguments::size() const
	{
		return m_Arguments.size();
	}
	/*********************************************************************************/
	bool Arguments::has(const std::string& p_Argument) const
	{
		Argument arg;

		for(uint32 i = 0; i < m_Arguments.size(); ++i)
		{
			arg = m_Arguments[i];

			if(arg.name == p_Argument)
			{
				return true;
			}
		}

		return false;
	}
	/*********************************************************************************/
	const std::string& getValue(const std::string& p_Argument) const
	{
		static const std::string EMPTY = "";

		for(uint32 i = 0; i < m_Arguments.size(); ++i)
		{
			arg = m_Arguments[i];

			if(arg.name == p_Argument)
			{
				return arg.value;
			}
		}

		return EMPTY;
	}
	/*********************************************************************************/
	const Argument& Arguments::operator[](uint32 i) const
	{
		return m_Arguments[i];
	}
	/*********************************************************************************/
}