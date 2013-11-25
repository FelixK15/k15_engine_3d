#include "K15_PrecompiledHeader.h"

#include "K15_Keyboard.h"

namespace K15_Engine { namespace Core { namespace InputDevices {
	/*********************************************************************************/
	Keyboard::InputStringToEnumMap Keyboard::InputStringToEnum = Keyboard::createStringToEnumMap();
	/*********************************************************************************/

	/*********************************************************************************/
	Keyboard::InputTrigger::InputTrigger(Enum p_Key)
		: m_Key(p_Key)
	{

	}
	/*********************************************************************************/
	bool Keyboard::InputTrigger::isActive()
	{
		return Keyboard::isPressed(m_Key);
	}
	/*********************************************************************************/
	
	/*********************************************************************************/
	Keyboard::Listener::Listener()
	{
		g_EventManager->addListener(_EN(onKeyPress),this);
		g_EventManager->addListener(_EN(onKeyRelease),this);
	}
	/*********************************************************************************/
	Keyboard::Listener::~Listener()
	{
		g_EventManager->removeListener(_EN(onKeyPress),this);
		g_EventManager->removeListener(_EN(onKeyRelease),this);
	}
	/*********************************************************************************/
	void Keyboard::Listener::handleEvent(GameEvent* p_Event)
	{
		if(p_Event->getName() == _EN(onKeyPress))
		{
			onKeyPress((Enum)p_Event->getArgument());
		}
		else if(p_Event->getName() == _EN(onKeyRelease))
		{
			onKeyRelease((Enum)p_Event->getArgument());
		}
	}
	/*********************************************************************************/

	/*********************************************************************************/
	const Keyboard::InputStringToEnumMap& Keyboard::createStringToEnumMap()
	{
		static InputStringToEnumMap map;

		map.insert(Pair(ObjectName,Enum)(_N(1),KEY_1));
		map.insert(Pair(ObjectName,Enum)(_N(2),KEY_2));
		map.insert(Pair(ObjectName,Enum)(_N(3),KEY_3));
		map.insert(Pair(ObjectName,Enum)(_N(4),KEY_4));
		map.insert(Pair(ObjectName,Enum)(_N(5),KEY_5));
		map.insert(Pair(ObjectName,Enum)(_N(6),KEY_6));
		map.insert(Pair(ObjectName,Enum)(_N(7),KEY_7));
		map.insert(Pair(ObjectName,Enum)(_N(8),KEY_8));
		map.insert(Pair(ObjectName,Enum)(_N(9),KEY_9));
		map.insert(Pair(ObjectName,Enum)(_N(0),KEY_0));
		map.insert(Pair(ObjectName,Enum)(_N(Backspace),KEY_BACKSPACE));
		map.insert(Pair(ObjectName,Enum)(_N(LeftShift),KEY_L_SHIFT));
		map.insert(Pair(ObjectName,Enum)(_N(RightShift),KEY_R_SHIFT));
		map.insert(Pair(ObjectName,Enum)(_N(Capslock),KEY_CAPSLOCK));
		map.insert(Pair(ObjectName,Enum)(_N(Tabulator),KEY_TABULATOR));
		map.insert(Pair(ObjectName,Enum)(_N(Space),KEY_SPACE));
		map.insert(Pair(ObjectName,Enum)(_N(LeftAlt),KEY_L_ALT));
		map.insert(Pair(ObjectName,Enum)(_N(RightAlt),KEY_R_ALT));
		map.insert(Pair(ObjectName,Enum)(_N(Ctrl),KEY_CTRL));
		map.insert(Pair(ObjectName,Enum)(_N(Return),KEY_RETURN));
		map.insert(Pair(ObjectName,Enum)(_N(Escape),KEY_ESCAPE));
		map.insert(Pair(ObjectName,Enum)(_N(A),KEY_A));
		map.insert(Pair(ObjectName,Enum)(_N(B),KEY_B));
		map.insert(Pair(ObjectName,Enum)(_N(C),KEY_C));
		map.insert(Pair(ObjectName,Enum)(_N(D),KEY_D));
		map.insert(Pair(ObjectName,Enum)(_N(E),KEY_E));
		map.insert(Pair(ObjectName,Enum)(_N(F),KEY_F));
		map.insert(Pair(ObjectName,Enum)(_N(G),KEY_G));
		map.insert(Pair(ObjectName,Enum)(_N(H),KEY_H));
		map.insert(Pair(ObjectName,Enum)(_N(I),KEY_I));
		map.insert(Pair(ObjectName,Enum)(_N(J),KEY_J));
		map.insert(Pair(ObjectName,Enum)(_N(K),KEY_K));
		map.insert(Pair(ObjectName,Enum)(_N(L),KEY_L));
		map.insert(Pair(ObjectName,Enum)(_N(M),KEY_M));
		map.insert(Pair(ObjectName,Enum)(_N(N),KEY_N));
		map.insert(Pair(ObjectName,Enum)(_N(O),KEY_O));
		map.insert(Pair(ObjectName,Enum)(_N(P),KEY_P));
		map.insert(Pair(ObjectName,Enum)(_N(Q),KEY_Q));
		map.insert(Pair(ObjectName,Enum)(_N(R),KEY_R));
		map.insert(Pair(ObjectName,Enum)(_N(S),KEY_S));
		map.insert(Pair(ObjectName,Enum)(_N(T),KEY_T));
		map.insert(Pair(ObjectName,Enum)(_N(U),KEY_U));
		map.insert(Pair(ObjectName,Enum)(_N(V),KEY_V));
		map.insert(Pair(ObjectName,Enum)(_N(W),KEY_W));
		map.insert(Pair(ObjectName,Enum)(_N(X),KEY_X));
		map.insert(Pair(ObjectName,Enum)(_N(Y),KEY_Y));
		map.insert(Pair(ObjectName,Enum)(_N(Z),KEY_Z));
		map.insert(Pair(ObjectName,Enum)(_N(F1),KEY_F1));
		map.insert(Pair(ObjectName,Enum)(_N(F2),KEY_F2));
		map.insert(Pair(ObjectName,Enum)(_N(F3),KEY_F3));
		map.insert(Pair(ObjectName,Enum)(_N(F4),KEY_F4));	
		map.insert(Pair(ObjectName,Enum)(_N(F5),KEY_F5));
		map.insert(Pair(ObjectName,Enum)(_N(F6),KEY_F6));
		map.insert(Pair(ObjectName,Enum)(_N(F7),KEY_F7));
		map.insert(Pair(ObjectName,Enum)(_N(F8),KEY_F8));
		map.insert(Pair(ObjectName,Enum)(_N(F9),KEY_F9));
		map.insert(Pair(ObjectName,Enum)(_N(F10),KEY_F10));
		map.insert(Pair(ObjectName,Enum)(_N(F11),KEY_F11));
		map.insert(Pair(ObjectName,Enum)(_N(F12),KEY_F12));
		map.insert(Pair(ObjectName,Enum)(_N(Up),KEY_UP));
		map.insert(Pair(ObjectName,Enum)(_N(Down),KEY_DOWN));
		map.insert(Pair(ObjectName,Enum)(_N(Left),KEY_LEFT));
		map.insert(Pair(ObjectName,Enum)(_N(Right),KEY_RIGHT));

		return map;
	}
	/*********************************************************************************/
}}}//end of K15_Engine::Core::InputDevices namespace