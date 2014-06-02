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
	const Keyboard::InputStringToEnumMap& Keyboard::createStringToEnumMap()
	{
		static InputStringToEnumMap map;

		map.insert(Pair(ObjectName,Enum)(_ON(1),KEY_1));
		map.insert(Pair(ObjectName,Enum)(_ON(2),KEY_2));
		map.insert(Pair(ObjectName,Enum)(_ON(3),KEY_3));
		map.insert(Pair(ObjectName,Enum)(_ON(4),KEY_4));
		map.insert(Pair(ObjectName,Enum)(_ON(5),KEY_5));
		map.insert(Pair(ObjectName,Enum)(_ON(6),KEY_6));
		map.insert(Pair(ObjectName,Enum)(_ON(7),KEY_7));
		map.insert(Pair(ObjectName,Enum)(_ON(8),KEY_8));
		map.insert(Pair(ObjectName,Enum)(_ON(9),KEY_9));
		map.insert(Pair(ObjectName,Enum)(_ON(0),KEY_0));
		map.insert(Pair(ObjectName,Enum)(_ON(Numpad_1),KEY_NUMPAD_1));
		map.insert(Pair(ObjectName,Enum)(_ON(Numpad_2),KEY_NUMPAD_2));
		map.insert(Pair(ObjectName,Enum)(_ON(Numpad_3),KEY_NUMPAD_3));
		map.insert(Pair(ObjectName,Enum)(_ON(Numpad_4),KEY_NUMPAD_4));
		map.insert(Pair(ObjectName,Enum)(_ON(Numpad_5),KEY_NUMPAD_5));
		map.insert(Pair(ObjectName,Enum)(_ON(Numpad_6),KEY_NUMPAD_6));
		map.insert(Pair(ObjectName,Enum)(_ON(Numpad_7),KEY_NUMPAD_7));
		map.insert(Pair(ObjectName,Enum)(_ON(Numpad_8),KEY_NUMPAD_8));
		map.insert(Pair(ObjectName,Enum)(_ON(Numpad_9),KEY_NUMPAD_9));
		map.insert(Pair(ObjectName,Enum)(_ON(Numpad_0),KEY_NUMPAD_0));
		map.insert(Pair(ObjectName,Enum)(_ON(Backspace),KEY_BACKSPACE));
		map.insert(Pair(ObjectName,Enum)(_ON(LeftShift),KEY_L_SHIFT));
		map.insert(Pair(ObjectName,Enum)(_ON(RightShift),KEY_R_SHIFT));
		map.insert(Pair(ObjectName,Enum)(_ON(Capslock),KEY_CAPSLOCK));
		map.insert(Pair(ObjectName,Enum)(_ON(Tabulator),KEY_TABULATOR));
		map.insert(Pair(ObjectName,Enum)(_ON(Space),KEY_SPACE));
		map.insert(Pair(ObjectName,Enum)(_ON(LeftAlt),KEY_L_ALT));
		map.insert(Pair(ObjectName,Enum)(_ON(RightAlt),KEY_R_ALT));
		map.insert(Pair(ObjectName,Enum)(_ON(Ctrl),KEY_CTRL));
		map.insert(Pair(ObjectName,Enum)(_ON(Return),KEY_RETURN));
		map.insert(Pair(ObjectName,Enum)(_ON(Escape),KEY_ESCAPE));
		map.insert(Pair(ObjectName,Enum)(_ON(A),KEY_A));
		map.insert(Pair(ObjectName,Enum)(_ON(B),KEY_B));
		map.insert(Pair(ObjectName,Enum)(_ON(C),KEY_C));
		map.insert(Pair(ObjectName,Enum)(_ON(D),KEY_D));
		map.insert(Pair(ObjectName,Enum)(_ON(E),KEY_E));
		map.insert(Pair(ObjectName,Enum)(_ON(F),KEY_F));
		map.insert(Pair(ObjectName,Enum)(_ON(G),KEY_G));
		map.insert(Pair(ObjectName,Enum)(_ON(H),KEY_H));
		map.insert(Pair(ObjectName,Enum)(_ON(I),KEY_I));
		map.insert(Pair(ObjectName,Enum)(_ON(J),KEY_J));
		map.insert(Pair(ObjectName,Enum)(_ON(K),KEY_K));
		map.insert(Pair(ObjectName,Enum)(_ON(L),KEY_L));
		map.insert(Pair(ObjectName,Enum)(_ON(M),KEY_M));
		map.insert(Pair(ObjectName,Enum)(_ON(N),KEY_N));
		map.insert(Pair(ObjectName,Enum)(_ON(O),KEY_O));
		map.insert(Pair(ObjectName,Enum)(_ON(P),KEY_P));
		map.insert(Pair(ObjectName,Enum)(_ON(Q),KEY_Q));
		map.insert(Pair(ObjectName,Enum)(_ON(R),KEY_R));
		map.insert(Pair(ObjectName,Enum)(_ON(S),KEY_S));
		map.insert(Pair(ObjectName,Enum)(_ON(T),KEY_T));
		map.insert(Pair(ObjectName,Enum)(_ON(U),KEY_U));
		map.insert(Pair(ObjectName,Enum)(_ON(V),KEY_V));
		map.insert(Pair(ObjectName,Enum)(_ON(W),KEY_W));
		map.insert(Pair(ObjectName,Enum)(_ON(X),KEY_X));
		map.insert(Pair(ObjectName,Enum)(_ON(Y),KEY_Y));
		map.insert(Pair(ObjectName,Enum)(_ON(Z),KEY_Z));
		map.insert(Pair(ObjectName,Enum)(_ON(F1),KEY_F1));
		map.insert(Pair(ObjectName,Enum)(_ON(F2),KEY_F2));
		map.insert(Pair(ObjectName,Enum)(_ON(F3),KEY_F3));
		map.insert(Pair(ObjectName,Enum)(_ON(F4),KEY_F4));	
		map.insert(Pair(ObjectName,Enum)(_ON(F5),KEY_F5));
		map.insert(Pair(ObjectName,Enum)(_ON(F6),KEY_F6));
		map.insert(Pair(ObjectName,Enum)(_ON(F7),KEY_F7));
		map.insert(Pair(ObjectName,Enum)(_ON(F8),KEY_F8));
		map.insert(Pair(ObjectName,Enum)(_ON(F9),KEY_F9));
		map.insert(Pair(ObjectName,Enum)(_ON(F10),KEY_F10));
		map.insert(Pair(ObjectName,Enum)(_ON(F11),KEY_F11));
		map.insert(Pair(ObjectName,Enum)(_ON(F12),KEY_F12));
		map.insert(Pair(ObjectName,Enum)(_ON(Up),KEY_UP));
		map.insert(Pair(ObjectName,Enum)(_ON(Down),KEY_DOWN));
		map.insert(Pair(ObjectName,Enum)(_ON(Left),KEY_LEFT));
		map.insert(Pair(ObjectName,Enum)(_ON(Right),KEY_RIGHT));

		return map;
	}
	/*********************************************************************************/
}}}//end of K15_Engine::Core::InputDevices namespace