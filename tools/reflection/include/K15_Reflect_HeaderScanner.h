#ifndef _K15_Reflect_HeaderScanner_h_
#define _K15_Reflect_HeaderScanner_h_

#include "K15_Reflect_Prerequisites.h"

#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/CompilerInstance.h"

namespace K15_Reflect
{
	class HeaderScanner
	{
	public:
		HeaderScanner(const std::string& p_FilePath. Arguments& p_Arguments);
		~HeaderScanner();

	private:
		std::string m_FilePath;

		llvm::OwningPtr<clang::CompilerInvocation> m_CompilerInvocation;
		clang::CompilerInstance m_CompilerInstance;
	};
}

#endif //_K15_Reflect_HeaderScanner_h_