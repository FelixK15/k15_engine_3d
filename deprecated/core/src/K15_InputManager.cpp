/**
 * @file K15_InputManager.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/05
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "K15_PrecompiledHeader.h"

#include "K15_InputManager.h"
#include "K15_Mouse.h"
#include "K15_Keyboard.h"


#include "K15_IniFileParser.h"

#include "K15_RenderWindow.h"
#include "K15_IOUtil.h"

#include "OIS.h"

#ifdef K15_OS_WINDOWS
	#pragma comment(lib, "dinput8")

	#ifndef _WIN32_WINNT
		#pragma comment(lib, "dxguid")
	#endif // _WIN32_WINNT

	#ifdef OIS_WIN32_XINPUT_SUPPORT
		#pragma comment(lib, "XINPUT_1_3")
	#endif //OIS_WIN32_XINPUT_SUPPORT
#endif //K15_OS_WINDOWS

#define K15_SAFE_DELETE_OIS_OBJECT(o) if(o) m_InputManager->destroyInputObject(o); o = 0;

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	Enum resolveToMouseButton(OIS::MouseButtonID p_ID)
	{
		switch (p_ID)
		{
		case OIS::MB_Left:
			return InputDevices::Mouse::BTN_LEFT;
		case OIS::MB_Right:
			return InputDevices::Mouse::BTN_RIGHT;
		case OIS::MB_Middle:
			return InputDevices::Mouse::BTN_MIDDLE;
		case OIS::MB_Button3:
			return InputDevices::Mouse::BTN_SPECIAL1;
		case OIS::MB_Button4:
			return InputDevices::Mouse::BTN_SPECIAL2;
		}

		return InputDevices::Mouse::BTN_NONE;
	}
	/*********************************************************************************/
	Enum resolveToKey(OIS::KeyCode p_ID)
	{
		switch(p_ID)
		{
		case OIS::KC_0:
			return InputDevices::Keyboard::KEY_0;
		case OIS::KC_1:
			return InputDevices::Keyboard::KEY_1;
		case OIS::KC_2:
			return InputDevices::Keyboard::KEY_2;
		case OIS::KC_3:
			return InputDevices::Keyboard::KEY_3;
		case OIS::KC_4:
			return InputDevices::Keyboard::KEY_4;
		case OIS::KC_5:
			return InputDevices::Keyboard::KEY_5;
		case OIS::KC_6:
			return InputDevices::Keyboard::KEY_6;
		case OIS::KC_7:
			return InputDevices::Keyboard::KEY_7;
		case OIS::KC_8:
			return InputDevices::Keyboard::KEY_8;
		case OIS::KC_9:
			return InputDevices::Keyboard::KEY_9;
		// F Keys
		case OIS::KC_F1:
			return InputDevices::Keyboard::KEY_F1;
		case OIS::KC_F2:
			return InputDevices::Keyboard::KEY_F2;
		case OIS::KC_F3:
			return InputDevices::Keyboard::KEY_F3;
		case OIS::KC_F4:
			return InputDevices::Keyboard::KEY_F4;
		case OIS::KC_F5:
			return InputDevices::Keyboard::KEY_F5;
		case OIS::KC_F6:
			return InputDevices::Keyboard::KEY_F6;
		case OIS::KC_F7:
			return InputDevices::Keyboard::KEY_F7;
		case OIS::KC_F8:
			return InputDevices::Keyboard::KEY_F8;
		case OIS::KC_F9:
			return InputDevices::Keyboard::KEY_F9;
		case OIS::KC_F10:
			return InputDevices::Keyboard::KEY_F10;
		case OIS::KC_F11:
			return InputDevices::Keyboard::KEY_F11;
		case OIS::KC_F12:
			return InputDevices::Keyboard::KEY_F12;
		// Letters
		case OIS::KC_A:
			return InputDevices::Keyboard::KEY_A;
		case OIS::KC_B:
			return InputDevices::Keyboard::KEY_B;
		case OIS::KC_C:
			return InputDevices::Keyboard::KEY_C;
		case OIS::KC_D:
			return InputDevices::Keyboard::KEY_D;
		case OIS::KC_E:
			return InputDevices::Keyboard::KEY_E;
		case OIS::KC_F:
			return InputDevices::Keyboard::KEY_F;
		case OIS::KC_G:
			return InputDevices::Keyboard::KEY_G;
		case OIS::KC_H:
			return InputDevices::Keyboard::KEY_H;
		case OIS::KC_I:
			return InputDevices::Keyboard::KEY_I;
		case OIS::KC_J:
			return InputDevices::Keyboard::KEY_J;
		case OIS::KC_K:
			return InputDevices::Keyboard::KEY_K;
		case OIS::KC_L:
			return InputDevices::Keyboard::KEY_L;
		case OIS::KC_M:
			return InputDevices::Keyboard::KEY_M;
		case OIS::KC_N:
			return InputDevices::Keyboard::KEY_N;
		case OIS::KC_O:
			return InputDevices::Keyboard::KEY_O;
		case OIS::KC_P:
			return InputDevices::Keyboard::KEY_P;
		case OIS::KC_Q:
			return InputDevices::Keyboard::KEY_Q;
		case OIS::KC_R:
			return InputDevices::Keyboard::KEY_R;
		case OIS::KC_S:
			return InputDevices::Keyboard::KEY_S;
		case OIS::KC_T:
			return InputDevices::Keyboard::KEY_T;
		case OIS::KC_U:
			return InputDevices::Keyboard::KEY_U;
		case OIS::KC_V:
			return InputDevices::Keyboard::KEY_V;
		case OIS::KC_W:
			return InputDevices::Keyboard::KEY_W;
		case OIS::KC_X:
			return InputDevices::Keyboard::KEY_X;
		case OIS::KC_Y:
			return InputDevices::Keyboard::KEY_Y;
		case OIS::KC_Z:
			return InputDevices::Keyboard::KEY_Z;
		// Special keys
		case OIS::KC_TAB:
			return InputDevices::Keyboard::KEY_TABULATOR;
		case OIS::KC_ESCAPE:
			return InputDevices::Keyboard::KEY_ESCAPE;
		case OIS::KC_RETURN:
			return InputDevices::Keyboard::KEY_RETURN;
		case OIS::KC_LSHIFT:
			return InputDevices::Keyboard::KEY_L_SHIFT;
		case OIS::KC_RSHIFT:
			return InputDevices::Keyboard::KEY_R_SHIFT;
		case OIS::KC_LCONTROL:
		case OIS::KC_RCONTROL:
			return InputDevices::Keyboard::KEY_CTRL;
		case OIS::KC_RIGHT:
			return InputDevices::Keyboard::KEY_RIGHT;
		case OIS::KC_LEFT:
			return InputDevices::Keyboard::KEY_LEFT;
		case OIS::KC_UP:
			return InputDevices::Keyboard::KEY_UP;
		case OIS::KC_DOWN:
			return InputDevices::Keyboard::KEY_DOWN;
		}

		return InputDevices::Keyboard::KEY_NONE;
	}
	/*********************************************************************************/
	void getMouseEventArgs(MouseEventArguments* p_Args, const OIS::MouseEvent& p_Event)
	{
		p_Args->xPx = p_Event.state.X.abs;
		p_Args->yPx = p_Event.state.Y.abs;

		p_Args->xNDC = (float)p_Args->xPx / (float)RenderWindow::getWidth();
		p_Args->yNDC = (float)p_Args->yPx / (float)RenderWindow::getHeight();

		if(p_Args->xPx > RenderWindow::getWidth()) p_Args->xPx = RenderWindow::getWidth();
		if(p_Args->yPx > RenderWindow::getHeight()) p_Args->yPx = RenderWindow::getHeight();

		p_Args->xNDC -= 0.5f; p_Args->yNDC -= 0.5f;
		p_Args->xNDC *= 2.f; p_Args->yNDC *= 2.f;

		if(p_Args->xNDC > 1.f) p_Args->xNDC = 1.f;
		if(p_Args->xNDC < -1.f) p_Args->xNDC = -1.f;

		if(p_Args->yNDC > 1.f) p_Args->yNDC = 1.f;
		if(p_Args->yNDC < -1.f) p_Args->yNDC = -1.f;
	}
	/*********************************************************************************/
	void getKeyboardEventArgs(KeyboardEventArguments* p_Args, const OIS::KeyEvent& p_Event)
	{
		p_Args->key = resolveToKey(p_Event.key);
		p_Args->text = p_Event.text;
	}
	/*********************************************************************************/



	/*********************************************************************************/
	class OIS_InputHandler : public OIS::JoyStickListener,
							#ifdef K15_PLATFORM_MOBILE
							 public OIS::MultiTouchListener
							#else
							 public OIS::KeyListener,
							 public OIS::MouseListener
							#endif //K15_PLATFORM_MOBILE
							 
	{
	public:
		/*********************************************************************************/
		OIS_InputHandler()
			: m_Initialized(false)
		{
			
		}
		/*********************************************************************************/
		~OIS_InputHandler()
		{
			
		}
		/*********************************************************************************/

		/*********************************************************************************/
		void capture_input()
		{
			#ifdef K15_PLATFORM_MOBILE
				m_Touch->capture();
			#else
				m_Keyboard->capture();
				m_Mouse->capture();

				if(m_JoyStick)
				{
					m_JoyStick->capture();
				}
			#endif //K15_PLATFORM_MOBILE
		}
		/*********************************************************************************/
		bool init()
		{
			OIS::ParamList params;

			#ifdef K15_OS_WINDOWS
				size_t windowHnd = (size_t)RenderWindowImpl::getHandleWindow();
				std::ostringstream windowHndStr;
				windowHndStr << windowHnd;
				params.insert(std::make_pair("WINDOW", windowHndStr.str()));

				#ifdef K15_DEBUG
					params.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
					params.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
					params.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
					params.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
				#endif // K15_DEBUG

			#elif defined K15_OS_LINUX
				std::ostringstream windowHndStr;
				windowHndStr << RenderWindowImpl::ms_Window;
				params.insert(std::make_pair("WINDOW", windowHndStr.str()));

				#ifdef K15_DEBUG
					params.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
					params.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
					params.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
					params.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
				#endif //K15_DEBUG
			#endif
			
			if(!(m_InputManager = OIS::InputManager::createInputSystem(params)))
			{
				K15_LOG_ERROR_MESSAGE("Could not initialize OIS InputManager.");
				return false;
			}

			K15_LOG_SUCCESS_MESSAGE("Successfully initialized OIS InputManager (Version: %s)",
				m_InputManager->getVersionName().c_str());

			#ifndef K15_PLATFORM_MOBILE
				m_Mouse = static_cast<OIS::Mouse*>(m_InputManager->createInputObject(OIS::OISMouse, true));
				m_Keyboard = static_cast<OIS::Keyboard*>(m_InputManager->createInputObject(OIS::OISKeyboard, true));	
				m_Mouse->setEventCallback(this);
				m_Keyboard->setEventCallback(this);
			#else
				m_Touch = static_cast<OIS::MultiTouch*>(m_InputManager->createInputObject(OIS::OISMultiTouch, true));
				m_Touch->setEventCallback(this);
			#endif // K15_PLATFORM_MOBILE

			if(m_InputManager->getNumberOfDevices(OIS::OISJoyStick) > 0)
			{
				m_JoyStick = static_cast<OIS::JoyStick*>(m_InputManager->createInputObject(OIS::OISJoyStick, true));
				m_JoyStick->setEventCallback(this);
			}

			m_Initialized = true;

			//set window region
			const OIS::MouseState& ms = m_Mouse->getMouseState();
			ms.width = RenderWindow::getWidth();
			ms.height = RenderWindow::getHeight();
			
			return true;
		}
		/*********************************************************************************/
		void shutdown()
		{
			if(m_Initialized)
			{
				K15_SAFE_DELETE_OIS_OBJECT(m_Mouse);
				K15_SAFE_DELETE_OIS_OBJECT(m_Keyboard);
				K15_SAFE_DELETE_OIS_OBJECT(m_JoyStick);

				#ifdef K15_PLATFORM_MOBILE
					K15_SAFE_DELETE_OIS_OBJECT(m_Touch);
				#endif //K15_PLATFORM_MOBILE

				OIS::InputManager::destroyInputSystem(m_InputManager);
				m_InputManager = 0;
				m_Initialized = false;
			}
		}
		/*********************************************************************************/
		bool isInitialized()
		{
			return m_Initialized;
		}
		/*********************************************************************************/
		virtual bool buttonPressed(const OIS::JoyStickEvent &arg, int button)
		{
			return true;
		}
		/*********************************************************************************/
		virtual bool buttonReleased(const OIS::JoyStickEvent &arg, int button)
		{
			return true;
		}
		/*********************************************************************************/
		virtual bool axisMoved(const OIS::JoyStickEvent &arg, int axis)
		{
			return true;
		}
		/*********************************************************************************/

		#ifdef K15_PLATFORM_MOBILE
		/*********************************************************************************/
		virtual bool touchMoved(const MultiTouchEvent &arg) 
		{

		}
		/*********************************************************************************/
		virtual bool touchPressed(const MultiTouchEvent &arg) 
		{

		}
		/*********************************************************************************/
		virtual bool touchReleased(const MultiTouchEvent &arg) 
		{

		}
		/*********************************************************************************/
		virtual bool touchCancelled(const MultiTouchEvent &arg) 
		{

		}
		#else
		/*********************************************************************************/
		virtual bool keyPressed(const OIS::KeyEvent &arg)
		{
			KeyboardEventArguments args;
			args.pressed = true;
			getKeyboardEventArgs(&args, arg);
			triggerEvent(&args, InputDevices::Keyboard::EventKeyPress);

			TriggeredInput input;
			input.Device = _ON(keyboard);
			input.Key = args.key;
			input.Value = 1.f;

			m_TriggeredInput.push_back(input);

			return true;
		}
		/*********************************************************************************/
		virtual bool keyReleased(const OIS::KeyEvent &arg)
		{
			KeyboardEventArguments args;
			args.pressed = false;
			getKeyboardEventArgs(&args, arg);
			triggerEvent(&args, InputDevices::Keyboard::EventKeyPress);

			for(DynamicArray(TriggeredInput)::iterator iter = m_TriggeredInput.begin();
				iter != m_TriggeredInput.end(); ++iter)
			{
				const TriggeredInput& input = (*iter);
				if(input.Device == _ON(keyboard) && input.Key == args.key)
				{
					m_TriggeredInput.erase(iter);
					break;
				}
			}

			return true;
		}
		/*********************************************************************************/
		virtual bool mouseMoved(const OIS::MouseEvent &arg)
		{
			MouseEventArguments args;
			args.pressed = false;
			args.button = InputDevices::Mouse::BTN_NONE;
			getMouseEventArgs(&args, arg);
			triggerEvent(&args, InputDevices::Mouse::EventMouseMoved);

			TriggeredInput input;
			input.Device = _ON(mouse);
			
			float screenMid_x = (float)RenderWindow::getWidth() * 0.5f;
			float screenMid_y = (float)RenderWindow::getHeight() * 0.5f;

			float posX = screenMid_x + (float)arg.state.X.rel;
			float PosY = screenMid_y + (float)arg.state.Y.rel;

			float deltaNDC_x = posX / RenderWindow::getWidth();
			float deltaNDC_y = PosY / RenderWindow::getHeight();

			deltaNDC_x -= 0.5f; deltaNDC_y -= 0.5f;
			deltaNDC_x *= 2.f;  deltaNDC_y *= 2.f;

			if(fabs(deltaNDC_x) > K15_EPSILON)
			{
				input.Key = InputDevices::Mouse::MA_HORIZONTAL;
				input.Value = deltaNDC_x;
				m_TriggeredInput.push_back(input);
			}

			if(fabs(deltaNDC_y) > K15_EPSILON)
			{
				input.Key = InputDevices::Mouse::MA_VERTICAL;
				input.Value = deltaNDC_y;
				m_TriggeredInput.push_back(input);
			}

			if(InputDevices::Mouse::isMouseLocked())
			{
				InputDevices::Mouse::setMousePos(RenderWindow::getWidth() / 2, RenderWindow::getHeight() / 2);
			}

			return true;
		}
		/*********************************************************************************/
		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
		{
			MouseEventArguments args;
			args.pressed = true;
			args.button = resolveToMouseButton(id);
			getMouseEventArgs(&args, arg);
			triggerEvent(&args, InputDevices::Mouse::EventMousePressed);

			TriggeredInput input;
			input.Device = _ON(mouse);
			input.Key = args.button;
			input.Value = 1.f;

			m_TriggeredInput.push_back(input);

			return true;
		}
		/*********************************************************************************/
		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
		{
			MouseEventArguments args;
			args.pressed = false;
			args.button = resolveToMouseButton(id);
			getMouseEventArgs(&args, arg);
			triggerEvent(&args, InputDevices::Mouse::EventMousePressed);

			for(DynamicArray(TriggeredInput)::iterator iter = m_TriggeredInput.begin();
				iter != m_TriggeredInput.end(); ++iter)
			{
				const TriggeredInput& input = (*iter);
				if(input.Device == _ON(mouse) && input.Key == args.button)
				{
					m_TriggeredInput.erase(iter);
					break;
				}
			}

			return true;
		}
		/*********************************************************************************/
		void triggerEvent(MouseEventArguments* p_Args, const EventName& p_EventName)
		{
			GameEvent* gameEvent = K15_NEW GameEvent(p_EventName, (void*)p_Args, sizeof(MouseEventArguments));
			g_EventManager->triggerEvent(gameEvent);
		}
		/*********************************************************************************/
		void triggerEvent(KeyboardEventArguments* p_Args, const EventName& p_EventName)
		{
			GameEvent* gameEvent = K15_NEW GameEvent(p_EventName, (void*)p_Args, sizeof(KeyboardEventArguments));
			g_EventManager->triggerEvent(gameEvent);
		}
		/*********************************************************************************/
		bool onWindowResize(GameEvent* p_Event)
		{
			if(m_Mouse)
			{
				const OIS::MouseState& ms = m_Mouse->getMouseState();
				ResizeEventArguments* args = static_cast<ResizeEventArguments*>(p_Event->getArgument());
				ms.width = args->newWidth;
				ms.height = args->newHeight;

				return true;
			}

			return false;
		}
		/*********************************************************************************/
		#endif //K15_PLATFORM_MOBILE

		DynamicArray(TriggeredInput) m_TriggeredInput;

	private:
		OIS::InputManager* m_InputManager;
		OIS::JoyStick* m_JoyStick;

		#ifdef K15_PLATFORM_MOBILE
			OIS::MultiTouch m_Touch;
		#else
			OIS::Keyboard* m_Keyboard;
			OIS::Mouse* m_Mouse;
		#endif //K15_PLATFORM_MOBILE

		

		bool m_Initialized;
	};
	/*********************************************************************************/



	/*********************************************************************************/
	InputEvent::InputEvent(Enum p_InputSource, float p_Value)
		: m_InputSource(p_InputSource),
		m_Value(p_Value)
	{

	}
	/*********************************************************************************/
	InputEvent::~InputEvent()
	{

	}
	/*********************************************************************************/



	/*********************************************************************************/
	InputHandler::InputHandler()
		: m_Object(0),
		m_Handler(0)
	{

	}
	/*********************************************************************************/
	InputHandler::InputHandler(const InputHandler& p_InputHandler)
		: m_Object(p_InputHandler.m_Object),
		m_Handler(p_InputHandler.m_Handler)
	{

	}
	/*********************************************************************************/
	InputHandler::~InputHandler()
	{

	}
	/*********************************************************************************/

	OIS_InputHandler DelegateHandler = OIS_InputHandler();

	/*********************************************************************************/
	InputManager::InputManager()
		:  m_InputMapping()
	{
		g_EventManager->addHandler(RenderWindow::EventInitialized, K15_EventHandler(InputManager, onRenderWindowInitialized, this));
		g_EventManager->addHandler(RenderWindow::EventResolutionChanged, K15_EventHandler(OIS_InputHandler, onWindowResize, &DelegateHandler));
	}
	/*********************************************************************************/
	InputManager::~InputManager()
	{
		g_EventManager->removeListener(RenderWindow::EventInitialized, K15_EventHandler(InputManager, onRenderWindowInitialized, this));
		g_EventManager->removeListener(RenderWindow::EventResolutionChanged, K15_EventHandler(OIS_InputHandler, onWindowResize, &DelegateHandler));

		m_InputMapping.clear();
	}
	/*********************************************************************************/
	bool InputManager::init()
	{
		if(!DelegateHandler.isInitialized())
		{
			return DelegateHandler.init();
		}
		
		return false;
	}
	/*********************************************************************************/
	void InputManager::shutdown()
	{
		if(DelegateHandler.isInitialized())
		{
			DelegateHandler.shutdown();
		}
	}
	/*********************************************************************************/
	void InputManager::captureInput()
	{
		if(DelegateHandler.isInitialized())
		{
			DelegateHandler.capture_input();

			const TriggeredInputArray& inputArray = DelegateHandler.m_TriggeredInput;

			for(TriggeredInputArray::const_iterator iter = inputArray.begin(); 
				iter != inputArray.end(); ++iter)
			{
				const TriggeredInput& input = (*iter);
				
				for(uint32 j = 0; j < m_InputMapping.size(); ++j)
				{
					const InputMapping& mapping = m_InputMapping[j];
					if(mapping.Device == input.Device && mapping.Key == input.Key && mapping.Handler.isValid())
					{
						InputEvent inputEvent(0, input.Value);
						mapping.Handler(&inputEvent);

						//delete mouse move inputs immediately
						if(mapping.Device == _ON(mouse) && (mapping.Key == InputDevices::Mouse::MA_HORIZONTAL ||
															mapping.Key == InputDevices::Mouse::MA_VERTICAL))
						{
							iter = DelegateHandler.m_TriggeredInput.erase(iter);

							if(iter == inputArray.end())
							{
								return;
							}
						}
						break;
					}
				}
			}
		}
	}
	/*********************************************************************************/
	void InputManager::addInputBinding(const ObjectName& p_ActionName, const InputHandler& p_Handler)
	{
		for(uint32 i = 0; i < m_InputMapping.size(); ++i)
		{
			InputMapping& mapping = m_InputMapping.at(i);

			if(mapping.Action == p_ActionName)
			{
				mapping.Handler = p_Handler;
				return;
			}
		}

		K15_LOG_WARNING("No action found with the name \"%s\".", p_ActionName.c_str());
	}
	/*********************************************************************************/
	void InputManager::removeInputBinding(const ObjectName& p_ActionName)
	{
		for(uint32 i = 0; i < m_InputMapping.size(); ++i)
		{
			InputMapping& mapping = m_InputMapping.at(i);

			if(mapping.Action == p_ActionName)
			{
				mapping.Handler = InputHandler();
				return;
			}
		}

		K15_LOG_WARNING("No action found with the name \"%s\".", p_ActionName.c_str());
	}
	/*********************************************************************************/
	void InputManager::parseInputConfig()
	{
		K15_LOG_NORMAL_MESSAGE("Trying to open input file \"%s\".", Application::InputFileName.c_str());

		ObjectName actionName;
		String bindingComplete;
		String binding;
		String device;
		String deviceInput;
		String inputFileName = g_Application->getGameRootDir() + Application::InputFileName;
		
		if(!IOUtil::fileExists(inputFileName))
		{
			inputFileName = g_Application->getHomeDir() + Application::InputFileName;
		}

		IniFileParser inputFile(inputFileName);

		if(inputFile.isValid())
		{
			IniFileGroup group;
			if(inputFile.getGroupEntries("",&group))
			{
				for(IniFileGroup::IniEntryList::iterator iter = group.entries.begin();iter != group.entries.end();++iter)
				{
					actionName = (*iter).key;
					bindingComplete = (*iter).value;
					String::size_type pos = 0;
					do 
					{
						pos = bindingComplete.find_first_of(',');
						binding = bindingComplete.substr(0,pos == String::npos ? bindingComplete.size() : pos);
						if(pos != String::npos)
						{
							bindingComplete = bindingComplete.substr(pos+1);
						}
						else
						{
							bindingComplete.clear();
						}

						if(!binding.empty())
						{
							pos = binding.find_first_of('.');
							if(pos != String::npos)
							{
								device = StringUtil::toLowerString(binding.substr(0,pos));
								deviceInput = binding.substr(pos+1);

								if(device == "keyboard")
								{
									InputDevices::Keyboard::InputStringToEnumMap::iterator input_iter = 
										InputDevices::Keyboard::InputStringToEnum.find(ObjectName(deviceInput));

									if(input_iter != InputDevices::Keyboard::InputStringToEnum.end())
									{
										InputMapping mapping;
										mapping.Action = actionName;
										mapping.Device = device;
										mapping.Key = input_iter->second;

										m_InputMapping.push_back(mapping);
									}
									else
									{
										K15_LOG_ERROR_MESSAGE("Could not find input \"%s\".",binding.c_str());
									}

								}
								else if(device == "gamepad")
								{

								}
								else if(device == "mouse")
								{
									InputDevices::Mouse::InputStringToEnumMap::iterator input_iter = 
										InputDevices::Mouse::InputStringToEnum.find(ObjectName(deviceInput));

									if(input_iter != InputDevices::Mouse::InputStringToEnum.end())
									{
										InputMapping mapping;
										mapping.Action = actionName;
										mapping.Device = device;
										mapping.Key = input_iter->second;

										m_InputMapping.push_back(mapping);
									}
									else
									{
										K15_LOG_ERROR_MESSAGE("Could not find input \"%s\".",binding.c_str());
									}
								}
								else
								{
									K15_LOG_ERROR_MESSAGE("Invalid input device \"%s\"",device.c_str());
								}
							}
							else
							{
								K15_LOG_ERROR_MESSAGE("invalid binding \"%s\"",bindingComplete.c_str());
							}
						}
					} while(!bindingComplete.empty());
				}
			}
		}
		else
		{
			K15_LOG_ERROR_MESSAGE("Could not open input file.");
		}

	}
	/*********************************************************************************/
	bool InputManager::onRenderWindowInitialized(GameEvent* p_Args)
	{
		return init();
	}
	/*********************************************************************************/
}}