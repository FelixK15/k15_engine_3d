#ifndef _TD_Level_h_
#define _TD_Level_h_

#include "TD_Prerequisities.h"
#include "K15_AllocatedObject.h"
#include "K15_Object.h"


namespace TowerDefense
{
	class Level : public Object, public AllocatedObject<BaseAllocatedObject::AC_GENERAL>
	{
	public:
		typedef DynamicArray(GameObject*) GameObjectArray;

	public:
		Level(const String& p_Name);
		~Level();

		void draw(RenderProcess* m_RenderProcess);
	
	private:
		void _loadLevel(const String& p_Name);
	
	private:
		GameObjectArray m_GameObjects;
	};
}

#endif //_TD_Level_h_