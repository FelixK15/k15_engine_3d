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

		void draw(RenderQueue* p_RenderQueue);
		
		const GameObjectArray& getWalls() const;

		const Vector3& getStartPosition() const;
		const Vector3& getEndPosition() const;

		GameObject* getEnd() const;
	private:
		void _loadLevel(const String& p_Name);
	
	private:
		GameObjectArray m_GameObjects;
		GameObjectArray m_Walls;
		GameObjectArray m_Floor;
		GameObject* m_Start;
		GameObject* m_End;
	};
}

#endif //_TD_Level_h_