#include "K15_PrecompiledHeader.h"

#include "K15_Keyboard.h"

namespace K15_Engine { namespace Core { namespace InputDevices {
	/*********************************************************************************/
	Keyboard::InputStringToEnumMap Keyboard::InputStringToEnum;
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
	void Keyboard::initialize()
	{
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(1),KEY_1));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(2),KEY_2));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(3),KEY_3));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(4),KEY_4));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(5),KEY_5));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(6),KEY_6));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(7),KEY_7));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(8),KEY_8));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(9),KEY_9));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(0),KEY_0));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(Backspace),KEY_BACKSPACE));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(LeftShift),KEY_L_SHIFT));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(RightShift),KEY_R_SHIFT));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(Capslock),KEY_CAPSLOCK));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(Tabulator),KEY_TABULATOR));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(Space),KEY_SPACE));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(LeftAlt),KEY_L_ALT));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(RightAlt),KEY_R_ALT));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(Ctrl),KEY_CTRL));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(Return),KEY_RETURN));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(Escape),KEY_ESCAPE));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(A),KEY_A));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(B),KEY_B));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(C),KEY_C));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(D),KEY_D));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(E),KEY_E));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(F),KEY_F));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(G),KEY_G));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(H),KEY_H));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(I),KEY_I));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(J),KEY_J));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(K),KEY_K));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(L),KEY_L));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(M),KEY_M));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(N),KEY_N));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(O),KEY_O));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(P),KEY_P));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(Q),KEY_Q));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(R),KEY_R));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(S),KEY_S));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(T),KEY_T));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(U),KEY_U));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(V),KEY_V));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(W),KEY_W));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(X),KEY_X));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(Y),KEY_Y));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(Z),KEY_Z));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(F1),KEY_F1));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(F2),KEY_F2));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(F3),KEY_F3));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(F4),KEY_F4));	
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(F5),KEY_F5));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(F6),KEY_F6));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(F7),KEY_F7));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(F8),KEY_F8));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(F9),KEY_F9));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(F10),KEY_F10));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(F11),KEY_F11));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(F12),KEY_F12));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(Up),KEY_UP));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(Down),KEY_DOWN));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(Left),KEY_LEFT));
		InputStringToEnum.insert(Pair(ObjectName,Enum)(_N(Right),KEY_RIGHT));
	}
	/*********************************************************************************/
}}}//end of K15_Engine::Core::InputDevices namespace