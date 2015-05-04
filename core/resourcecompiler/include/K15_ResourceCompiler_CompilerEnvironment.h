#ifndef _K15_ResourceCompiler_CompilerEnvironment_h_
#define _K15_ResourceCompiler_CompilerEnvironment_h_

#include "K15_ResourceCompiler_Prerequisites.h"

namespace K15_ResourceCompiler
{
	enum FileCollisionBehavior
	{
		FEB_REPLACE_EXISTING_FILE,
		FEB_DONT_REPLACE_EXISTING_FILE
	};

	class CompilerEnvironment
	{
		K15_NON_MOVEABLE(CompilerEnvironment);

	public:
		CompilerEnvironment();
		~CompilerEnvironment();

		void setWorkingDir(const String& p_WorkingDirectory);
		void setInputPath(const String& p_InputPath);
		void setOutputPath(const String& p_OutputPath);
		void setFileCollisionBehavior(FileCollisionBehavior p_FileCollisionBehavior);

		const String& getWorkingDir() const;
		const String& getInputPath() const;
		const String& getOutputPath() const;
		FileCollisionBehavior getFileCollisionBehavior() const;

	private:
		String m_WorkingDirectory;
		String m_InputPath;
		String m_OutputPath;
		FileCollisionBehavior m_FileCollisionBehavior;
	};
}

#endif //_K15_ResourceCompiler_CompilerEnvironment_h_