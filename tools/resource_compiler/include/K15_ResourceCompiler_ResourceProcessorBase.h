#ifndef _K15_ResourceCompiler_ResourceProcessorBase_h_
#define _K15_ResourceCompiler_ResourceProcessorBase_h_

#include "K15_ResourceCompiler_Prerequisites.h"
#include "K15_ResourceCompiler_ResourceProcessorEnvironment.h"
#include "K15_ResourceCompiler_CompilerEnvironment.h"

namespace K15_ResourceCompiler
{
	class ResourceProcessorBase;
	class CompilerEnvironment;
	struct ResourceData;

	enum ResourceProcessState
	{
		RPS_SUCCESS,
		RPS_RUNNING,
		RPS_FAILURE
	}; // ResourceProcessState enum declaration

	struct ResourceProcessResult
	{
		StringSet resourceDependencies;
		ResourceProcessState processState;
	}; // ResourceProcessResult struct declaration

	/*********************************************************************************/
	typedef std::vector<ResourceProcessorBase*> ResourceProcessorVector;
	/*********************************************************************************/

	class ResourceProcessorBase
	{
		K15_NON_COPYABLE(ResourceProcessorBase);
		K15_NON_MOVEABLE(ResourceProcessorBase);
		K15_NO_DEFAULT_CONSTRUCTOR(ResourceProcessorBase);
	
	public:
		ResourceProcessorBase(const CompilerEnvironment& p_CompilerEnvironment);
		virtual ~ResourceProcessorBase();

		virtual ProcessorEnvironment getProcessorEnvironment() const = 0;
		virtual bool canProcess(const String& p_FileExtension) const = 0;
		virtual ResourceProcessResult process(const ResourceData& p_File) = 0;
		virtual ResourceData gatherData(const String& p_File) const = 0;

		const String& getError() const;
		const CompilerEnvironment& getCompilerEnvironment() const;

	protected:
		void setError(const String& p_String);

	private:
		CompilerEnvironment m_CompilerEnvironment;
		String m_Error;
	}; // K15_ResourceProcessorBase class declaration
} // end of K15_ResourceCompiler namespace

#endif //_K15_ResourceCompiler_ResourceProcessorBase_h_