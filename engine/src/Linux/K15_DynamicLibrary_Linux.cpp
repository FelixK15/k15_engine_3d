#include "K15_PrecompiledHeader.h"

#include "Linux\K15_DynamicLibrary_Linux.h"


namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	DynamicLibrary_Linux::DynamicLibrary_Linux()
	{

	}
	/*********************************************************************************/
	DynamicLibrary_Linux::DynamicLibrary_Linux(const String& p_FileName)
		: DynamicLibraryBase(p_FileName),
		m_Module(0)
	{

	}
	/*********************************************************************************/
	DynamicLibrary_Linux::~DynamicLibrary_Linux()
	{

	}
	/*********************************************************************************/
	bool DynamicLibrary_Linux::load()
	{
		m_Module = dlopen(m_FileName.c_str(),RTLD_NOW | RTLD_LOCAL);

		if(!m_Module)
		{
			return false;
		}

		return true;
	}
	/*********************************************************************************/
	bool DynamicLibrary_Linux::unload()
	{
		dlclose(m_Module);
	}
	/*********************************************************************************/
	void* DynamicLibrary_Linux::getSymbolInternal( const String& p_SymbolName )
	{
		return dlsym(m_Module,p_SymbolName.c_str());
	}
	/*********************************************************************************/

}}//end of K15_Engine::Core namespace