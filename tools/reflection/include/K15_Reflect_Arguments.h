#ifndef _K15_Reflect_Arguments_h_
#define _K15_Reflect_Arguments_h_

#include "K15_Reflect_Prerequisites.h"

namespace K15_Reflect
{
	struct Argument
	{
		std::string name;
		std::string value;
	};

	class Arguments
	{
	public:
		Arguments(int p_ArgCount, char** p_ArgArray);
		~Arguments();

		uint32 const size() const;
		bool has(const std::string& p_Argument) const;

		const std::string getValue(const std::string& p_Argument) const;
		const Argument& operator[](uint32 i) const;

	private:
		Argument _retrieveArgument(const char* p_ArgumentString);

	private:
		std::vector<Argument> m_Argumets;
	};
}

#endif //_K15_Reflect_Arguments_h_