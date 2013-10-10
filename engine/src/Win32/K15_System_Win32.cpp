/**
 * @file K15_System_Win32.cpp
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/11
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
 *
 * @section DESCRIPTION
 * This is the Microsoft Windows (c) implementation of the System class.
 * 
 */

#include "K15_System.h"

#include "windows.h"
#include "windowsx.h"

#include "K15_LogManager.h"

#include "K15_ResourceManager.h"
#include "K15_EventManager.h"
#include "K15_EventTypes.h"
#include "K15_Game.h"

using namespace K15_EngineV2;

/**
* Anonymous namespace to make the following variables only accessible in this file.
*/
namespace{
	HANDLE _hHeapHandle = 0;
	HINSTANCE _hInstance = 0;
	HWND _hWHandle = 0;
	MSG _mMsg;

	bool _bHighResolutionTimerSupported = false;
	uint32 _iMemoryFlags = 0;
	double _dFreq = 0;
	double _dEngineStart = 0;

	Game *_pGame = 0;
}

/**
 * WinProc function for Windows Message Processing.
 */
LRESULT CALLBACK WindowProc(HWND hwnd,uint32 msg,WPARAM wParam,LPARAM lParam)
{
	if(msg == WM_CLOSE){
		PostQuitMessage(0);
		return 0;
	}else if(msg == WM_KEYDOWN){
//		_pGame->OnKeyDown(wParam);
	}else if(msg == WM_KEYUP){
		//_pGame->OnKeyUp(wParam);
	}else if(msg == WM_MOUSEMOVE){
		//_pGame->OnMouseMove(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	}

	//If the message could net get processed, we'll let DefWindowProc take care of things.
	return DefWindowProc(hwnd,msg,wParam,lParam);
}

/**
 * Function that initializes the window class using the WNDCLASS struct.
 * Will be called internally during InitializeWindow().
 */
void InitializeWindowClass(uint32 iFlags)
{
	WNDCLASS windowClass;
	ZeroMemory(&windowClass,sizeof(WNDCLASS));

	windowClass.hInstance = _hInstance;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hCursor = LoadCursor(_hInstance,IDC_ARROW);
	windowClass.hIcon = LoadIcon(_hInstance,IDI_SHIELD);
	windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	windowClass.lpszClassName = "K15GameWindow";
	windowClass.lpszMenuName = 0;

	if(HAS_FLAG(System::WF_FRAME,iFlags)){
		windowClass.style = WS_OVERLAPPED;
	}

	RegisterClass(&windowClass);
}

/**
 * CreateAndShowWindow is the core of the InitializeWindow Function.
 * It checks if some of WindowFlags have been set and creates the window afterwards.
 * If the window has been created successfully the window will be made visible.
 * Will be called internally during InitializeWindow() and after InitializeWindowClass().
 */
bool CreateAndShowWindow(const String &sTitle,uint32 iFlags,uint32 iPosX,uint32 iPosY,uint32 iWidth,uint32 iHeight)
{
	uint32 iStyle = 0;

	if(HAS_FLAG(System::WF_FRAME,iFlags)){
		iStyle = WS_OVERLAPPEDWINDOW;
	}

	if(HAS_FLAG(System::WF_AUTOALIGNMENT,iFlags)){
		iWidth = GetSystemMetrics(SM_CXSCREEN);
		iHeight = GetSystemMetrics(SM_CYSCREEN);
	}

	if(HAS_FLAG(System::WF_FULLSCREEN,iFlags)){
		DEVMODE dmScreenSettings;
		ZeroMemory(&dmScreenSettings,sizeof(DEVMODE));

		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmSize = sizeof(DEVMODE);
		dmScreenSettings.dmPelsWidth = iWidth;
		dmScreenSettings.dmPelsHeight = iHeight;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSHEIGHT | DM_PELSWIDTH;

		ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN);

		iPosX = iPosY = 0;
	}

	_hWHandle = CreateWindow("K15GameWindow",sTitle.C_Str(),iStyle,iPosX,iPosY,iWidth,iHeight,0,0,_hInstance,0);

	if(_hWHandle){
		::ShowWindow(_hWHandle,5);
		SetForegroundWindow(_hWHandle);
		SetFocus(_hWHandle);

		return true;
	}else{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////

System::System()
{
	_hInstance = 0;
	_hWHandle = 0;
	m_pGame = 0;
	_iMemoryFlags = 0;

	m_bFullscreen = false;
	m_bWindowInitialized = false;
}

System::~System()
{
	//Nothing here.
}

bool System::Initialize(uint32 iFlags)
{
	//We'll create our log first.
	m_pEngineLog =  g_pLogManager->CreateLog("K15_Log",true,true,true);

	_LogNormal("Starting K15 System...");

	//We need to obtain a Handle to the Process' Heap in order to 
	//allocate memory via the HeapAlloc function.
	_hHeapHandle = GetProcessHeap();

	//We also need the HINSTANCE handle of the current module instance in order to
	//create a window class and do some other stuff.
	_hInstance = GetModuleHandle(0);

	//The amount of system memory gets queried.
	size_t iTotalMemory, iProcessMemory;
	_QueryMemory(&iTotalMemory,&iProcessMemory);

	_bHighResolutionTimerSupported = (QueryPerformanceFrequency((LARGE_INTEGER*)&_dFreq) == TRUE);

	if(_bHighResolutionTimerSupported){
		QueryPerformanceCounter((LARGE_INTEGER*)&_dEngineStart);
		_dFreq /= 1000.0;
	}else{
		_dEngineStart = (double)GetTickCount64();
	}
	
	return _hHeapHandle != 0 && _hInstance != 0;
}

bool System::InitializeWindow(const String &sTitle,uint32 iFlags,uint32 iPosX,uint32 iPosY,uint32 iWidth,uint32 iHeight)
{
	_LogNormal("Initializing window...");

	//Before we can create a new window, we must create and register a so called WindowClass (WNDCLASS).
	InitializeWindowClass(iFlags);

	//After the WindowClass has been created and registered, we try to create a new window.
	if(!CreateAndShowWindow(sTitle,iFlags,iPosX,iPosY,iWidth,iHeight)){
		_LogNormal("Error - Could not create Window");
		MessageBox(_hWHandle,"Could not create Window","Error",0);
		return false;
	}

	m_bWindowInitialized = true;

	return true;
}

void System::Shutdown()
{
	_LogNormal("Shutting down system...");

	//The window will get closed and destroyed during system shutdown.
	//CloseWindow(_hWHandle);
	//DestroyWindow(_hWHandle);

	//The normal desktop resolution gets set.
	if(m_bFullscreen){
		ChangeDisplaySettings(0,0);
	}

	Memory::CheckForMemoryLeak();
	//Object::PrintObjectsInUse();

	_LogNormal("Shutting down game...");

	if(m_pGame){
		m_pGame->Shutdown();
		K15_DELETE m_pGame;
		m_pGame = 0;
	}

	UnregisterClass("K15GameWindow",_hInstance);
	
	_LogNormal("Shutting down log...");
	g_pLogManager->Shutdown();
	Memory::CleanUp();
}

double System::TimeSinceStart()
{
	double dTicks;

	//If high resolution timers are supported use them.
	//If they're not supported, use the GetTickCount64() to
	//get the time.
	if(_bHighResolutionTimerSupported){
		QueryPerformanceCounter((LARGE_INTEGER*)&dTicks);
		dTicks = (dTicks - _dEngineStart) / _dFreq;
	}else{
		dTicks = (double)GetTickCount64();
	}
	
	return dTicks;
}

void System::Run()
{
	// @todo read fps from file.
	const double dMaxFrameDuration = 1/60;

	double dDifference = 0;
	double dStart = 0;
	double dDeltaTime = dMaxFrameDuration;
	
	GameTime gtTime;
	gtTime.m_dDeltaTime = dDeltaTime;
	gtTime.m_dMulitplicator = 1.0;

	while(true)/*Game Running*/{
		dStart = TimeSinceStart();
		if(PeekMessage(&_mMsg,_hWHandle,0,0,PM_REMOVE)){
			TranslateMessage(&_mMsg);
			DispatchMessage(&_mMsg);

			if(_mMsg.message == WM_QUIT){
				break;
			}
		}else{
			m_pGame->Update(gtTime);
		}

		// @todo move fps code to own function.
		dDeltaTime = TimeSinceStart() - dStart;
		if(dDeltaTime > dMaxFrameDuration){
			dDeltaTime = dMaxFrameDuration;
		}else{
			dDifference = (dMaxFrameDuration - dDeltaTime) * 1000;
			Sleep(dDifference);
		}

		gtTime.m_dDeltaTime = dDeltaTime;
	}
}

void System::_QueryMemory(size_t *pSystemMemory,size_t *pProcessMemory)
{
	MEMORYSTATUSEX msMemoryStatus;
	ZeroMemory(&msMemoryStatus,sizeof(MEMORYSTATUSEX));
	msMemoryStatus.dwLength = sizeof(msMemoryStatus);

	GlobalMemoryStatusEx(&msMemoryStatus);

	if(pSystemMemory){
		*pSystemMemory = (size_t)msMemoryStatus.ullTotalPhys;
	}

	if(pProcessMemory){
		*pProcessMemory = (size_t)msMemoryStatus.ullAvailPhys;
	}
}

String System::GetHomeDirectory() const
{
	CHAR sDirectory[_MAX_PATH];
	GetEnvironmentVariable("userprofile",sDirectory,_MAX_PATH);

	return String(sDirectory);
}

String System::GetApplicationDirectory() const
{
	CHAR sDirectory[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH,sDirectory);

	return String(sDirectory);
}

void System::Sleep( double dTime )
{
	::Sleep((DWORD)dTime);
}

void System::ShowMessageBox( const String& sMessage,const String& sTitle) const
{
	MessageBox(_hWHandle,sMessage.C_Str(),sTitle.C_Str(),0);
}

void *System::QuerySystemObject( SystemObjectDescription eObjectDescription ) const
{
	if(eObjectDescription == SOD_WINDOWS_FORM_HANDLE){
		return _hWHandle;
	}

	return 0;
}

void System::SetGame(Game *pGame)
{
	if(m_pGame){
		m_pGame->Shutdown();
		K15_DELETE m_pGame;
	}

	m_pGame = pGame;
	m_pGame->Initialize();
	_pGame = m_pGame;
//	g_pResourceManager->SetResourceFile(m_pGame->GetResourceFile());
}

String System::GetSystemError() const
{
	DWORD iErrorCode = GetLastError();
	LPTSTR pMessageBuffer = 0;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,iErrorCode,0,(LPTSTR)&pMessageBuffer,1024,0);

	String sErrorMessage(pMessageBuffer);
	LocalFree(pMessageBuffer);

	return sErrorMessage;
}
