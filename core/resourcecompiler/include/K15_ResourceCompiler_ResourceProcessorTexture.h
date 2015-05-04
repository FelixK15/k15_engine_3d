#ifndef _K15_ResourceCompiler_ResourceProcessorTexture_h_
#define _K15_ResourceCompiler_ResourceProcessorTexture_h_

#include "K15_ResourceCompiler_Prerequisites.h"
#include "K15_ResourceCompiler_ResourceProcessorBase.h"

namespace K15_ResourceCompiler
{
	class CompilerEnvironment;

	class ResourceprocessorTexture : public ResourceProcessorBase
	{
		K15_NON_MOVEABLE(ResourceprocessorTexture);
		K15_NON_COPYABLE(ResourceprocessorTexture);
		K15_NO_DEFAULT_CONSTRUCTOR(ResourceprocessorTexture);

	public:
		ResourceprocessorTexture(const CompilerEnvironment& p_CompilerEnvironment);

		virtual ProcessorEnvironment getProcessorEnvironment() const override;
		virtual bool canProcess(const String& p_FileExtension) const override;
		virtual ResourceProcessResult process(const ResourceData& p_ResourceData) override;
		virtual ResourceData gatherData(const String& p_File) const override;
	};
}

#endif //_K15_ResourceCompiler_ResourceProcessorTexture_h_