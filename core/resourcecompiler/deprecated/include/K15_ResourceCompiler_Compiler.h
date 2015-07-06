#ifndef _K15_ResourceCompiler_Compiler_h_
#define _K15_ResourceCompiler_Compiler_h_

#include "K15_ResourceCompiler_Prerequisites.h"
#include "K15_ResourceCompiler_CompilerEnvironment.h"

namespace K15_ResourceCompiler
{
	class ResourceProcessorBase;
	struct ResourceData;

	class Compiler 
	{
		K15_NON_COPYABLE(Compiler);
		K15_NON_MOVEABLE(Compiler);
		K15_NO_DEFAULT_CONSTRUCTOR(Compiler);

	public:
		Compiler(const CompilerEnvironment p_Environment);
		~Compiler();

		bool hasProcessor(const String& p_Name) const;
		void addProcessor(ResourceProcessorBase* p_ResourceProcessor);
		void addDefaultProcessors();

		bool compile();
		bool compileResourceData(const ResourceData& p_File);

		const String& getError() const;
		int getReturnValue() const;

	private:
		std::vector<ResourceProcessorBase*> m_ResourceProcessors;
		CompilerEnvironment m_Environment;
		String m_Error;
		int m_ReturnValue;
	};
}

#endif //_K15_ResourceCompiler_Compiler_h_