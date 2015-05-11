#ifndef DIDFT_OPTIONAL
#define DIDFT_OPTIONAL      0x80000000
#endif

#define K15_WIN32_DIRECT_INPUT_DEVICE_BUFFER_SIZE 128

#define K15_WIN32_DIRECT_INPUT_DPAD_NORTH 0
#define K15_WIN32_DIRECT_INPUT_DPAD_NORTH_EAST 4500
#define K15_WIN32_DIRECT_INPUT_DPAD_EAST 9000
#define K15_WIN32_DIRECT_INPUT_DPAD_SOUTH_EAST 13500
#define K15_WIN32_DIRECT_INPUT_DPAD_SOUTH 18000
#define K15_WIN32_DIRECT_INPUT_DPAD_SOUTH_WEST 22500
#define K15_WIN32_DIRECT_INPUT_DPAD_WEST 27000
#define K15_WIN32_DIRECT_INPUT_DPAD_NORTH_WEST 31500

/*********************************************************************************/
//http://www.libsdl.org/tmp/SDL/src/joystick/windows/SDL_dinputjoystick.c
static DIOBJECTDATAFORMAT dfDIJoystick2[] = {
	{ &GUID_XAxis, DIJOFS_X, DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_YAxis, DIJOFS_Y, DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_ZAxis, DIJOFS_Z, DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_RxAxis, DIJOFS_RX, DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_RyAxis, DIJOFS_RY, DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_RzAxis, DIJOFS_RZ, DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_Slider, DIJOFS_SLIDER(0), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_Slider, DIJOFS_SLIDER(1), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_POV, DIJOFS_POV(0), DIDFT_OPTIONAL | DIDFT_POV | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_POV, DIJOFS_POV(1), DIDFT_OPTIONAL | DIDFT_POV | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_POV, DIJOFS_POV(2), DIDFT_OPTIONAL | DIDFT_POV | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_POV, DIJOFS_POV(3), DIDFT_OPTIONAL | DIDFT_POV | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(0), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(1), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(2), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(3), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(4), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(5), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(6), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(7), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(8), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(9), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(10), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(11), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(12), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(13), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(14), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(15), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(16), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(17), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(18), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(19), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(20), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(21), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(22), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(23), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(24), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(25), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(26), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(27), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(28), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(29), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(30), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(31), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(32), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(33), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(34), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(35), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(36), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(37), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(38), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(39), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(40), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(41), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(42), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(43), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(44), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(45), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(46), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(47), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(48), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(49), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(50), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(51), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(52), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(53), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(54), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(55), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(56), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(57), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(58), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(59), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(60), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(61), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(62), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(63), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(64), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(65), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(66), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(67), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(68), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(69), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(70), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(71), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(72), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(73), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(74), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(75), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(76), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(77), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(78), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(79), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(80), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(81), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(82), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(83), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(84), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(85), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(86), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(87), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(88), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(89), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(90), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(91), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(92), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(93), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(94), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(95), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(96), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(97), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(98), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(99), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(100), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(101), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(102), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(103), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(104), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(105), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(106), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(107), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(108), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(109), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(110), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(111), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(112), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(113), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(114), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(115), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(116), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(117), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(118), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(119), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(120), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(121), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(122), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(123), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(124), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(125), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(126), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ NULL, DIJOFS_BUTTON(127), DIDFT_OPTIONAL | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_XAxis, FIELD_OFFSET(DIJOYSTATE2, lVX), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_YAxis, FIELD_OFFSET(DIJOYSTATE2, lVY), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_ZAxis, FIELD_OFFSET(DIJOYSTATE2, lVZ), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_RxAxis, FIELD_OFFSET(DIJOYSTATE2, lVRx), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_RyAxis, FIELD_OFFSET(DIJOYSTATE2, lVRy), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_RzAxis, FIELD_OFFSET(DIJOYSTATE2, lVRz), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_Slider, FIELD_OFFSET(DIJOYSTATE2, rglVSlider[0]), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_Slider, FIELD_OFFSET(DIJOYSTATE2, rglVSlider[1]), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_XAxis, FIELD_OFFSET(DIJOYSTATE2, lAX), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_YAxis, FIELD_OFFSET(DIJOYSTATE2, lAY), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_ZAxis, FIELD_OFFSET(DIJOYSTATE2, lAZ), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_RxAxis, FIELD_OFFSET(DIJOYSTATE2, lARx), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_RyAxis, FIELD_OFFSET(DIJOYSTATE2, lARy), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_RzAxis, FIELD_OFFSET(DIJOYSTATE2, lARz), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_Slider, FIELD_OFFSET(DIJOYSTATE2, rglASlider[0]), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_Slider, FIELD_OFFSET(DIJOYSTATE2, rglASlider[1]), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_XAxis, FIELD_OFFSET(DIJOYSTATE2, lFX), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_YAxis, FIELD_OFFSET(DIJOYSTATE2, lFY), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_ZAxis, FIELD_OFFSET(DIJOYSTATE2, lFZ), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_RxAxis, FIELD_OFFSET(DIJOYSTATE2, lFRx), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_RyAxis, FIELD_OFFSET(DIJOYSTATE2, lFRy), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_RzAxis, FIELD_OFFSET(DIJOYSTATE2, lFRz), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_Slider, FIELD_OFFSET(DIJOYSTATE2, rglFSlider[0]), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
	{ &GUID_Slider, FIELD_OFFSET(DIJOYSTATE2, rglFSlider[1]), DIDFT_OPTIONAL | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
};
/*********************************************************************************/
//http://www.libsdl.org/tmp/SDL/src/joystick/windows/SDL_dinputjoystick.c
const DIDATAFORMAT c_dfDIJoystick2 = {
	sizeof(DIDATAFORMAT),
	sizeof(DIOBJECTDATAFORMAT),
	DIDF_ABSAXIS,
	sizeof(DIJOYSTATE2),
	K15_ARRAY_COUNT(dfDIJoystick2),
	dfDIJoystick2
};
/*********************************************************************************/


/*********************************************************************************/
struct K15_InternalInputEnumerationResult
{
	DIDEVICEINSTANCEA supportedDevices[K15_MAX_CONTROLLER];
	DIDEVICEINSTANCEA unsupportedDevices[K15_MAX_CONTROLLER];

	uint32 numSupportedDevices;
	uint32 numUnsupportedDevices;
// 
// 	GUID xInputFilter[K15_MAX_CONTROLLER];
// 	uint32 xInputFilterCounter;
};
/*********************************************************************************/


//straight from MSDN (https://msdn.microsoft.com/en-us/library/windows/desktop/ee417014(v=vs.85).aspx)
/*********************************************************************************/
intern BOOL K15_InternalIsXInputDevice(const GUID* p_DirectInputGUID/*, K15_InternalInputEnumerationResult* p_EnumerationResult*/)
{
	IWbemLocator*           pIWbemLocator  = NULL;
	IEnumWbemClassObject*   pEnumDevices   = NULL;
	IWbemClassObject*       pDevices[20]   = {0};
	IWbemServices*          pIWbemServices = NULL;
	BSTR                    bstrNamespace  = NULL;
	BSTR                    bstrDeviceID   = NULL;
	BSTR                    bstrClassName  = NULL;
	DWORD                   uReturned      = 0;
	bool                    bIsXinputDevice= false;
	UINT                    iDevice        = 0;
	VARIANT                 var;
	HRESULT                 hr;

	// CoInit if needed
	hr = CoInitialize(NULL);
	bool bCleanupCOM = SUCCEEDED(hr);

	// Create WMI
	hr = CoCreateInstance( __uuidof(WbemLocator),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IWbemLocator),
		(LPVOID*) &pIWbemLocator);
	if( FAILED(hr) || pIWbemLocator == NULL )
		goto LCleanup;

	bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" );if( bstrNamespace == NULL ) goto LCleanup;        
	bstrClassName = SysAllocString( L"Win32_PNPEntity" );   if( bstrClassName == NULL ) goto LCleanup;        
	bstrDeviceID  = SysAllocString( L"DeviceID" );          if( bstrDeviceID == NULL )  goto LCleanup;        

	// Connect to WMI 
	hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L, 
		0L, NULL, NULL, &pIWbemServices );
	if( FAILED(hr) || pIWbemServices == NULL )
		goto LCleanup;

	// Switch security level to IMPERSONATE. 
	CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, 
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE );                    

	hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices ); 
	if( FAILED(hr) || pEnumDevices == NULL )
		goto LCleanup;

	// Loop over all devices
	for( ;; )
	{
		// Get 20 at a time
		hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
		if( FAILED(hr) )
			goto LCleanup;
		if( uReturned == 0 )
			break;

		for( iDevice=0; iDevice<uReturned; iDevice++ )
		{
			// For each device, get its device ID
			hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );
			if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL )
			{
				// Check if the device ID contains "IG_".  If it does, then it's an XInput device
				// This information can not be found from DirectInput 
				if( wcsstr( var.bstrVal, L"IG_" ) )
				{
					// If it does, then get the VID/PID from var.bstrVal
					DWORD dwPid = 0, dwVid = 0;
					WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
					if( strVid && swscanf( strVid, L"VID_%4X", &dwVid ) != 1 )
						dwVid = 0;
					WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
					if( strPid && swscanf( strPid, L"PID_%4X", &dwPid ) != 1 )
						dwPid = 0;

					// Compare the VID/PID to the DInput device
					DWORD dwVidPid = MAKELONG( dwVid, dwPid );
					if( dwVidPid == p_DirectInputGUID->Data1 )
					{
						bIsXinputDevice = true;
						goto LCleanup;
					}
				}
			}   
			WIN32_COM_SAFE_RELEASE( pDevices[iDevice] );
		}
	}

LCleanup:
	if(bstrNamespace)
		SysFreeString(bstrNamespace);
	if(bstrDeviceID)
		SysFreeString(bstrDeviceID);
	if(bstrClassName)
		SysFreeString(bstrClassName);
	for( iDevice=0; iDevice<20; iDevice++ )
		WIN32_COM_SAFE_RELEASE( pDevices[iDevice] );
	WIN32_COM_SAFE_RELEASE( pEnumDevices );
	WIN32_COM_SAFE_RELEASE( pIWbemLocator );
	WIN32_COM_SAFE_RELEASE( pIWbemServices );

	if( bCleanupCOM )
		CoUninitialize();

// 	if (bIsXinputDevice)
// 	{
// 		//add to filter if xinput guid
// 		uint32 filterIndex = p_EnumerationResult->xInputFilterCounter;
// 		p_EnumerationResult->xInputFilter[filterIndex++] = *p_DirectInputGUID;
// 		p_EnumerationResult->xInputFilterCounter = filterIndex;
// 	}

	return bIsXinputDevice;
}
/*********************************************************************************/
// intern inline BOOL K15_InternalCheckXInputGUIDFilter(GUID* p_ProductGUID, K15_InternalInputEnumerationResult* p_EnumerationResult)
// {
// 	uint32 productGUIDFilterCount = p_EnumerationResult->xInputFilterCounter;
// 
// 	for (uint32 productGUIDFilterIndex = 0;
// 		productGUIDFilterIndex < productGUIDFilterCount;
// 		++productGUIDFilterIndex)
// 	{
// 		GUID currentFilterGUID = p_EnumerationResult->xInputFilter[productGUIDFilterIndex];
// 
// 		if (currentFilterGUID == *p_ProductGUID)
// 		{
// 			return TRUE;
// 		}
// 	}
// 
// 	return FALSE;
// }
/*********************************************************************************/
intern inline BOOL K15_Win32EnumDirectInputDeviceCallback(LPCDIDEVICEINSTANCEA p_DeviceInstance, LPVOID p_Ref)
{
	K15_InternalInputEnumerationResult* enumerationResult = (K15_InternalInputEnumerationResult*)p_Ref;
	LPDIRECTINPUTDEVICE controllerDevice = 0;
	GUID productGUID = p_DeviceInstance->guidProduct;
	DWORD deviceTypeBitmask = p_DeviceInstance->dwDevType;
	BYTE deviceType = (BYTE)(deviceTypeBitmask);
	BYTE deviceSubType = (BYTE)(deviceTypeBitmask >> 8);

	//let xinput handle xinput devices
	if (K15_InternalIsXInputDevice(&productGUID) == TRUE)
	{
		return DIENUM_CONTINUE;
	}

	//only handle gamepads
	if (deviceType == DI8DEVTYPE_GAMEPAD
		&& deviceSubType == DI8DEVTYPEGAMEPAD_STANDARD)
	{
		int supportedDevices = enumerationResult->numSupportedDevices;

		if (supportedDevices == K15_MAX_CONTROLLER)
		{
			return DIENUM_CONTINUE;
		}

		enumerationResult->supportedDevices[supportedDevices++] = *p_DeviceInstance;
		enumerationResult->numSupportedDevices = supportedDevices;
	}
	else
	{
		int unsupportedDevices = enumerationResult->numUnsupportedDevices;

		if (unsupportedDevices == K15_MAX_CONTROLLER)
		{
			return DIENUM_CONTINUE;
		}

		enumerationResult->unsupportedDevices[unsupportedDevices++] = *p_DeviceInstance;
		enumerationResult->numUnsupportedDevices = unsupportedDevices;

	}

	return DIENUM_CONTINUE;
}
/*********************************************************************************/
intern BOOL K15_InternalEnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCEA p_DeviceObjectInstance, LPVOID p_Parameter)
{
	DWORD objectType = p_DeviceObjectInstance->dwType;
	DWORD objectFlags = DIDOI_ASPECTACCEL | DIDOI_ASPECTVELOCITY | DIDOI_ASPECTPOSITION;
	GUID objectGUID = p_DeviceObjectInstance->guidType;

	LPDIRECTINPUTDEVICE deviceInstace = (LPDIRECTINPUTDEVICE)p_Parameter;

	//Data for min/max range of the axis values
	DIPROPRANGE dibRange = {};
	dibRange.diph.dwSize       = sizeof(DIPROPRANGE);
	dibRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dibRange.diph.dwHow        = DIPH_BYID;
	dibRange.diph.dwObj        = objectType;
	dibRange.lMin              = 0;
	dibRange.lMax              = SHRT_MAX;

	if ((objectType & DIDFT_AXIS) > 0)
	{
		HRESULT result = deviceInstace->SetProperty(DIPROP_RANGE, &dibRange.diph);

		result = result;
	}

	return DIENUM_CONTINUE;
}
/*********************************************************************************/
intern void K15_Win32InitializeDirectInputDevices(K15_Win32Context* p_Win32Context, uint32 p_NumXinputDevices, HWND p_HWND)
{
 	K15_InternalInputEnumerationResult enumerationResult = {};
// 	memcpy(enumerationResult.xInputFilter, p_Win32Context->DirectInput.xInputGUIDFilter, sizeof(p_Win32Context->DirectInput.xInputGUIDFilter));
// 	enumerationResult.xInputFilterCounter = p_Win32Context->DirectInput.xInputGUIDFilterCounter;

	HRESULT result;
	
	result = p_Win32Context->DirectInput.directInputHandle->EnumDevices(DI8DEVCLASS_GAMECTRL, K15_Win32EnumDirectInputDeviceCallback, (LPVOID)&enumerationResult, DIEDFL_ALLDEVICES);

	DWORD xInputDeviceCount = p_NumXinputDevices;

	if (SUCCEEDED(result))
	{
		int32 numSupportedDevices = min(enumerationResult.numSupportedDevices, K15_MAX_CONTROLLER);

		//save supported devices
		for (int32 supportedDeviceIndex = 0;
			supportedDeviceIndex < numSupportedDevices;
			++supportedDeviceIndex)
		{
			if ((supportedDeviceIndex + xInputDeviceCount) == K15_MAX_CONTROLLER)
			{
				int maxController = K15_MAX_CONTROLLER;
				K15_LOG_ERROR_MESSAGE("Too many controller. Only a number of max %d is supported.", maxController);
				break;
			}

			uint32 controllerIndex = xInputDeviceCount + supportedDeviceIndex;

			//Check if the current device has been scanned before and is maybe a XInput device
			K15_Win32Controller* controller = &p_Win32Context->controller[controllerIndex];
			
			if (controller->APIType == K15_WIN32_APITYPE_XINPUT)
			{
				continue;
			}

			LPCDIDEVICEINSTANCEA deviceInformationInstance = &enumerationResult.supportedDevices[supportedDeviceIndex];
			LPDIRECTINPUTDEVICE deviceInstance = 0;
			HRESULT result = 0;

			//Data for internal buffer that holds input between frames
			DIPROPDWORD dipBufferSize = {};
			dipBufferSize.diph.dwSize = sizeof(DIPROPDWORD);
			dipBufferSize.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			dipBufferSize.diph.dwHow = DIPH_DEVICE;
			dipBufferSize.dwData = K15_WIN32_DIRECT_INPUT_DEVICE_BUFFER_SIZE;


			result = p_Win32Context->DirectInput.directInputHandle->CreateDevice(deviceInformationInstance->guidProduct, &deviceInstance, 0);

			if (FAILED(result))
			{

			}

			deviceInstance->EnumObjects(K15_InternalEnumDeviceObjectsCallback, (LPVOID)deviceInstance, DIDFT_AXIS);


			result = deviceInstance->SetDataFormat(&c_dfDIJoystick2);

			if (FAILED(result))
			{

			}

			result = deviceInstance->SetCooperativeLevel(p_HWND, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

			if (FAILED(result))
			{

			}

			result = deviceInstance->SetProperty(DIPROP_BUFFERSIZE, &dipBufferSize.diph);

			if (FAILED(result))
			{

			}

			result = deviceInstance->Acquire();

			if (FAILED(result))
			{

			}


			K15_LOG_DEBUG_MESSAGE("Found supported controller \"%s\".", deviceInformationInstance->tszProductName);

			p_Win32Context->controller[xInputDeviceCount].APIType = K15_WIN32_APITYPE_DIRECTINPUT;
			p_Win32Context->controller[xInputDeviceCount].controllerState = K15_WIN32_CONTROLLER_STATE_CONNECTED;
			p_Win32Context->controller[xInputDeviceCount].directInputDevice = deviceInstance;

			if (controllerIndex == K15_MAX_CONTROLLER)
			{
				break;
			}
		}

		//list unsupported devices
		for (uint32 unsupportedDeviceIndex = 0;
			unsupportedDeviceIndex < enumerationResult.numUnsupportedDevices;
			++unsupportedDeviceIndex)
		{
			LPCDIDEVICEINSTANCEA deviceInstance = &enumerationResult.unsupportedDevices[unsupportedDeviceIndex];

			K15_LOG_WARNING_MESSAGE("Controller \"%s\" unsupported.", deviceInstance->tszProductName);
		}
	}

// 	memcpy(p_Win32Context->DirectInput.xInputGUIDFilter, enumerationResult.xInputFilter, sizeof(p_Win32Context->DirectInput.xInputGUIDFilter));
// 
// 	p_Win32Context->DirectInput.xInputGUIDFilterCounter = enumerationResult.xInputFilterCounter;
}
/*********************************************************************************/
intern inline BOOL K15_Win32CheckDirectInputConnectivity(K15_Win32Context* p_Win32Context, K15_Win32Controller* p_Controller)
{

	if (p_Controller->APIType == K15_WIN32_APITYPE_DIRECTINPUT)
	{
		LPDIRECTINPUTDEVICE currentDevice = p_Controller->directInputDevice;
		DIDEVICEOBJECTDATA dummyData = {};
		DWORD dummySize = 0;
		HRESULT result = currentDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &dummyData, &dummySize ,DIGDD_PEEK);
		BOOL connected = result == DI_OK ? TRUE : FALSE;

		return connected;
	}

	return FALSE;
}
/*********************************************************************************/
intern inline void K15_Win32DirectInputThumpStickEvent(LPDIDEVICEOBJECTDATA p_InputData)
{
	float value = (float)p_InputData->dwData / (float)SHRT_MAX; // [0, 1]
	value = value * 2.f - 1.f;


	K15_SystemEvent win32Event = {};
	win32Event.eventFlags = K15_INPUT_EVENT_FLAG;
	win32Event.params.thumbValue = value;

	switch(p_InputData->dwOfs)
	{
		case DIJOFS_X:
		case DIJOFS_RX:
		{
			win32Event.event = K15_LEFT_THUMB_X;
			break;
		}

		case DIJOFS_Y:
		case DIJOFS_RY:
		{
			win32Event.event = K15_LEFT_THUMB_Y;
			break;
		}

		case DIJOFS_Z:
		{
			if (p_InputData->dwData < 0)
			{
				win32Event.event = K15_LEFT_SHOULDER_TRIGGER;
			}
			else
			{
				win32Event.event = K15_RIGHT_SHOULDER_TRIGGER;
			}
			break;
		}
	}

	K15_AddSystemEventToQueue(&win32Event);
};
/*********************************************************************************/
intern inline void K15_Win32DirectInputPOVEvent(LPDIDEVICEOBJECTDATA p_InputData)
{
	DWORD data = p_InputData->dwData;
	uint8 pressed = ((1 << 7) & data) > 0;

	//DPAD UP
	if (p_InputData->dwData == K15_WIN32_DIRECT_INPUT_DPAD_NORTH ||
		p_InputData->dwData == K15_WIN32_DIRECT_INPUT_DPAD_NORTH_EAST ||
		p_InputData->dwData == K15_WIN32_DIRECT_INPUT_DPAD_NORTH_WEST)
	{
		K15_SystemEvent dpadUpEvent = {};
		dpadUpEvent.params.controllerButton = K15_CONTROLLER_DPAD_UP;
		dpadUpEvent.eventFlags = K15_INPUT_EVENT_FLAG;
		dpadUpEvent.event = pressed ?  K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;

		K15_AddSystemEventToQueue(&dpadUpEvent);
	}

	//DPAD LEFT
	if (p_InputData->dwData == K15_WIN32_DIRECT_INPUT_DPAD_EAST ||
		p_InputData->dwData == K15_WIN32_DIRECT_INPUT_DPAD_SOUTH_EAST ||
		p_InputData->dwData == K15_WIN32_DIRECT_INPUT_DPAD_NORTH_EAST)
	{
		K15_SystemEvent dpadLeftEvent = {};
		dpadLeftEvent.params.controllerButton = K15_CONTROLLER_DPAD_LEFT;
		dpadLeftEvent.eventFlags = K15_INPUT_EVENT_FLAG;
		dpadLeftEvent.event = pressed ?  K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;

		K15_AddSystemEventToQueue(&dpadLeftEvent);
	}

	//DPAD DOWN
	if (p_InputData->dwData == K15_WIN32_DIRECT_INPUT_DPAD_SOUTH_EAST ||
		p_InputData->dwData == K15_WIN32_DIRECT_INPUT_DPAD_SOUTH_EAST ||
		p_InputData->dwData == K15_WIN32_DIRECT_INPUT_DPAD_SOUTH_WEST)
	{
		K15_SystemEvent dpadDownEvent = {};
		dpadDownEvent.params.controllerButton = K15_CONTROLLER_DPAD_DOWN;
		dpadDownEvent.eventFlags = K15_INPUT_EVENT_FLAG;
		dpadDownEvent.event = pressed ?  K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;

		K15_AddSystemEventToQueue(&dpadDownEvent);
	}

	//DPAD RIGHT
	if (p_InputData->dwData == K15_WIN32_DIRECT_INPUT_DPAD_WEST ||
		p_InputData->dwData == K15_WIN32_DIRECT_INPUT_DPAD_SOUTH_WEST ||
		p_InputData->dwData == K15_WIN32_DIRECT_INPUT_DPAD_NORTH_WEST)
	{
		K15_SystemEvent dpadRightEvent = {};
		dpadRightEvent.params.controllerButton = K15_CONTROLLER_DPAD_RIGHT;
		dpadRightEvent.eventFlags = K15_INPUT_EVENT_FLAG;
		dpadRightEvent.event = pressed ?  K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;

		K15_AddSystemEventToQueue(&dpadRightEvent);
	}
}
/*********************************************************************************/
intern inline void K15_Win32DirectInputButtonEvent(LPDIDEVICEOBJECTDATA p_InputData)
{
	DWORD data = p_InputData->dwData;
	uint8 pressed = ((1 << 7) & data) > 0;

	K15_SystemEvent win32Event = {};

	win32Event.event = pressed ?  K15_CONTROLLER_BUTTON_PRESSED : K15_CONTROLLER_BUTTON_RELEASED;
	win32Event.eventFlags = K15_INPUT_EVENT_FLAG;

	switch(p_InputData->dwOfs)
	{
		case DIJOFS_BUTTON(0):
		{
			win32Event.params.controllerButton = K15_CONTROLLER_FACE_LEFT_BUTTON;
			break;
		}

		case DIJOFS_BUTTON(1):
		{
			win32Event.params.controllerButton = K15_CONTROLLER_FACE_DOWN_BUTTON;
			break;
		}

		case DIJOFS_BUTTON(2):
		{
			win32Event.params.controllerButton = K15_CONTROLLER_FACE_RIGHT_BUTTON;
			break;
		}

		case DIJOFS_BUTTON(3):
		{
			win32Event.params.controllerButton = K15_CONTROLLER_FACE_UP_BUTTON;
			break;
		}

		case DIJOFS_BUTTON(4):
		{
			win32Event.params.controllerButton = K15_CONTROLLER_LEFT_SHOULDER_BUTTON;
			break;
		}

		case DIJOFS_BUTTON(5):
		{
			win32Event.params.controllerButton = K15_CONTROLLER_RIGHT_SHOULDER_BUTTON;
			break;
		}

		case DIJOFS_BUTTON(6):
		{
			win32Event.params.controllerButton = K15_CONTROLLER_START;
			break;
		}

		case DIJOFS_BUTTON(7):
		{
			win32Event.params.controllerButton = K15_CONTROLLER_SELECT;
			break;
		}

		case DIJOFS_BUTTON(8):
		{
			win32Event.params.controllerButton = K15_CONTROLLER_LEFT_THUMB_BUTTON;
			break;
		}

		case DIJOFS_BUTTON(9):
		{
			win32Event.params.controllerButton = K15_CONTROLLER_RIGHT_THUMB_BUTTON;
			break;
		}
	}

	K15_AddSystemEventToQueue(&win32Event);
}
/*********************************************************************************/
intern inline void K15_Win32PumpDirectInputControllerEvents(K15_Win32Context* p_Win32Context, K15_Win32Controller* p_Controller)
{
	DWORD inputBufferSize = K15_WIN32_DIRECT_INPUT_DEVICE_BUFFER_SIZE;
	LPDIRECTINPUTDEVICE currentDevice = p_Controller->directInputDevice;
	DIDEVICEOBJECTDATA inputBuffer[K15_WIN32_DIRECT_INPUT_DEVICE_BUFFER_SIZE] = {};

	HRESULT result = currentDevice->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), inputBuffer, &inputBufferSize, 0);

	if (result == DI_OK)
	{
		for (DWORD inputIndex = 0;
			inputIndex < inputBufferSize;
			++inputIndex)
		{
			switch (inputBuffer[inputIndex].dwOfs)
			{
				case DIJOFS_X:
				case DIJOFS_Y:
				case DIJOFS_Z:
				case DIJOFS_RX:
				case DIJOFS_RY:
				case DIJOFS_RZ:
				{
					K15_Win32DirectInputThumpStickEvent(&inputBuffer[inputIndex]);
					break;
				}

				case DIJOFS_POV(0):
				case DIJOFS_POV(1):
				case DIJOFS_POV(2):
				case DIJOFS_POV(3):
				{
					K15_Win32DirectInputPOVEvent(&inputBuffer[inputIndex]);
					break;
				}

				default:
				{
					//Buttons - 128 is max (see dfDIJoystick2 struct above)
					if (inputBuffer[inputIndex].dwOfs >= DIJOFS_BUTTON(0) && inputBuffer[inputIndex].dwOfs < DIJOFS_BUTTON(128))
					{
						K15_Win32DirectInputButtonEvent(&inputBuffer[inputIndex]);
					}
					break;
				}
			}
		}
	}
}
/*********************************************************************************/