#ifndef _K15_ResourceCompiler_ResourceProcessorMesh_h_
#define _K15_ResourceCompiler_ResourceProcessorMesh_h_

#include "K15_ResourceCompiler_Prerequisites.h"
#include "K15_ResourceCompiler_ResourceProcessorBase.h"

namespace K15_ResourceCompiler
{
	class ResourceProcessorMesh : public ResourceProcessorBase
	{
		K15_NON_COPYABLE(ResourceProcessorMesh);
		K15_NON_MOVEABLE(ResourceProcessorMesh);
		K15_NO_DEFAULT_CONSTRUCTOR(ResourceProcessorMesh);

	public:
		ResourceProcessorMesh(const CompilerEnvironment& p_CompilerEnvironment);
		virtual ~ResourceProcessorMesh();

		virtual ProcessorEnvironment getProcessorEnvironment() const override;

		virtual bool canProcess(const String& p_FileExtension) const override;
		virtual ResourceProcessResult process(const ResourceData& p_File) override;
		ResourceData gatherData(const String& p_File) const override;
	};
}

#endif //_K15_ResourceCompiler_ResourceProcessorMesh_h_