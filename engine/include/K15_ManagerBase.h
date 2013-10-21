#ifndef _K15Engine_Core_ManagerBase_h_
#define _K15Engine_Core_ManagerBase_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace Core 

	template<class OBJECTTYPE>
	class K15_API_EXPORT ManagerBase
	{
	public:
		typedef K15_List(OBJECTTYPE) ObjectList;

	public:
		virtual ~ManagerBase();

		OBJECTTYPE createObject();

	protected:
		ManagerBase();

	protected:
		ObjectList m_ObjectList;
	};

}} //end of K15_Engine::System namespace

#endif //_K15Engine_Core_ManagerBase_h_