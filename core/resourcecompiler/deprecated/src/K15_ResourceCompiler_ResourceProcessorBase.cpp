#include "K15_ResourceCompiler_ResourceProcessorBase.h"

namespace K15_ResourceCompiler
{
	/*********************************************************************************/
	ResourceProcessorBase::ResourceProcessorBase(const CompilerEnvironment& p_CompilerEnvironment)
		: m_CompilerEnvironment(p_CompilerEnvironment),
		m_Error()
	{

	}
	/*********************************************************************************/
	ResourceProcessorBase::~ResourceProcessorBase()
	{

	}
	/*********************************************************************************/
	void ResourceProcessorBase::setError(const String& p_String)
	{
		m_Error = p_String;
	}
	/*********************************************************************************/
	const CompilerEnvironment& ResourceProcessorBase::getCompilerEnvironment() const
	{
		return m_CompilerEnvironment;
	}
	/*********************************************************************************/
	const String& ResourceProcessorBase::getError() const
	{
		return m_Error;
	}
	/*********************************************************************************/
}