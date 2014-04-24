#include "K15_PrecompiledHeader.h"

#include "Android\K15_ApplicationLayerOS_Android.h"
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
	#if defined K15_OS_ANDROID
		String path = "lib" + m_FileName;
	#else
		String path = g_Application->getGameRootDir() + m_FileName;
	#endif //K15_OS_ANDROID
		_LogNormal("Trying to load \"%s\".",path.c_str());
		m_Module = dlopen(path.c_str(),RTLD_NOW);

		if(!m_Module)
		{
			g_Application->getOSLayer().setError(dlerror());
			return false;
		}

		m_Loaded = true;

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