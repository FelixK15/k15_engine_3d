#include "K15_PrecompiledHeader.h"

#include "K15_Keyboard.h"

namespace K15_Engine { namespace Core { namespace InputDevices {
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
}}}//end of K15_Engine::Core::InputDevices namespace