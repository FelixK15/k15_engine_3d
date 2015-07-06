#include "K15_ResourceCompiler_CompilerEnvironment.h"

namespace K15_ResourceCompiler
{
	/*********************************************************************************/
	CompilerEnvironment::CompilerEnvironment()
		: m_WorkingDirectory(),
		m_InputPath(),
		m_OutputPath()
	{

	}
	/*********************************************************************************/
	CompilerEnvironment::~CompilerEnvironment()
	{

	}
	/*********************************************************************************/
	void CompilerEnvironment::setWorkingDir(const String& p_WorkingDirectory)
	{
		m_WorkingDirectory = p_WorkingDirectory;
	}
	/*********************************************************************************/
	void CompilerEnvironment::setInputPath(const String& p_InputPath)
	{
		m_InputPath = p_InputPath;
	}
	/*********************************************************************************/
	void CompilerEnvironment::setOutputPath(const String& p_OutputPath)
	{
		m_OutputPath = p_OutputPath;
	}
	/*********************************************************************************/
	void CompilerEnvironment::setFileCollisionBehavior(FileCollisionBehavior p_FileCollisionBehavior)
	{
		m_FileCollisionBehavior = p_FileCollisionBehavior;
	}
	/*********************************************************************************/
	const String& CompilerEnvironment::getWorkingDir() const
	{
		return m_WorkingDirectory;
	}
	/*********************************************************************************/
	const String& CompilerEnvironment::getInputPath() const
	{
		return m_InputPath;
	}
	/*********************************************************************************/
	const String& CompilerEnvironment::getOutputPath() const
	{
		return m_OutputPath;
	}
	/*********************************************************************************/
	FileCollisionBehavior CompilerEnvironment::getFileCollisionBehavior() const
	{
		return m_FileCollisionBehavior;
	}
	/*********************************************************************************/
}