package k15games.engine.launcher;

import android.app.Activity;
import android.app.AlertDialog;
import android.view.KeyEvent;
import android.view.View;
import android.view.MotionEvent;
import android.view.InputDevice;
import android.content.Context;
import android.content.DialogInterface;
import android.util.Log;
import android.os.Bundle;

import java.util.ArrayDeque;
import java.util.Deque;
import java.util.ArrayList;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.lang.InterruptedException;

/*********************************************************************************/
final class SystemEvent
{
	public int eventType = EventType.NoEvent;
	public byte[] param = null;
	public int inputFlag = 0;
	
	/*********************************************************************************/
	public static final int getByteSize()
	{
		int sizeInBytes = 12; // eventType(4 bytes) + inputFlag(4 bytes) + param(4 bytes);

		return sizeInBytes;
	}
	/*********************************************************************************/
	static class EventType
	{
		public static final int NoEvent = 0;
		//Controller Input
		public static final int ControllerButtonPressed = 308;
		public static final int ControllerButtonReleased = 309;

		public static final int ControllerLeftThumbX = 326;
		public static final int ControllerLeftThumbY = 327;
		public static final int ControllerRightThumbX = 328;
		public static final int ControllerRightThumbY = 329;

		public static final int ControllerLeftShoulderTrigger = 324;
		public static final int ControllerRightShoulderTrigger = 325;
		
		//Touch Input
		public static final int TouchMotion = 340;
		public static final int TouchInput = 341;
		
		//Window
		public static final int WindowFocusLost = 202;
		public static final int WindowFocusGained = 201;
		
		//System
		public static final int ApplicationQuit = 100;
	};
	/*********************************************************************************/
	static class ButtonIdentifier
	{
		public static final int ControllerFaceDownButton = 320;
		public static final int ControllerFaceLeftButton = 321;
		public static final int ControllerFaceUpButton = 322;
		public static final int ControllerFaceRightButton = 323;
		
		public static final int ControllerStart = 314;
		public static final int ControllerBack = 315;

		public static final int ControllerLeftShoulderButton = 316;
		public static final int ControllerRightShoulderButton = 317;
		
		public static final int ControllerLeftThumbButton = 318;
		public static final int ControllerRightThumbButton = 319;
		
		public static final int ControllerDPadLeft = 310;
		public static final int ControllerDPadRight = 311;
		public static final int ControllerDPadUp = 312;
		public static final int ControllerDPadDown = 313;
	};
	/*********************************************************************************/
	static class InputFlags
	{
		public static final int WindowEventFlag = 0x001;
		public static final int SystemEventFlag = 0x002;
		public static final int InputEventFlag = 0x004;
	};
	/*********************************************************************************/
}
/*********************************************************************************/



/*********************************************************************************/
class LauncherThread extends Thread
{
	private final float NANOSECONDS_TO_MILLISECONDS = 1 / 1000000;
	private boolean m_Running = true;
	private LauncherContentView m_ContentView = null;
	private LauncherMainActivity m_Activity = null;
	
	/*********************************************************************************/
	public LauncherThread(LauncherContentView p_ContentView, LauncherMainActivity p_Activity)
	{
		m_ContentView = p_ContentView;
		m_Activity = p_Activity;
	}
	/*********************************************************************************/
	@Override
	public void run() 
	{
		boolean engineRunning = true;
		ArrayDeque<SystemEvent> systemEvents = null;
		ByteBuffer systemEventByteBuffer = null;

		K15_InitEngine();

		while(engineRunning)
		{
			SystemEventQueue eventQueue = m_ContentView.GetSystemEventQueue();
			eventQueue.FlipBuffers();
			systemEvents = eventQueue.GetFrontBuffer();

			int systemEventByteBufferSize = systemEvents.size() * SystemEvent.getByteSize(); //+4 byte for extra integer parameter indicating the offset
			systemEventByteBuffer = ByteBuffer.allocate(systemEventByteBufferSize);
			systemEventByteBuffer.order(ByteOrder.LITTLE_ENDIAN);
			
			for (SystemEvent currentSystemEvent : systemEvents)
			{
				systemEventByteBuffer.putInt(currentSystemEvent.eventType);
				systemEventByteBuffer.putInt(currentSystemEvent.inputFlag);
				systemEventByteBuffer.put(currentSystemEvent.param);
			}

			engineRunning = K15_TickEngine(systemEventByteBuffer.array());			
		}

		K15_ShutdownEngine();

		m_Activity.exitLauncher();
	}
	/*********************************************************************************/
	
	public native boolean K15_TickEngine(byte[] p_SystemEventByteBuffer);
	public native void K15_InitEngine();
	public native void K15_ShutdownEngine();
}
/*********************************************************************************/

class SystemEventQueue
{
	private final int SYSTEM_EVENT_CAPACITY = 64;
	private Object m_InputQueueLock = new Object();
	private int m_ActiveInputDeque = 0;
	private ArrayList<ArrayDeque<SystemEvent>> m_InputDeques = new ArrayList<ArrayDeque<SystemEvent>>(2);
	
	/*********************************************************************************/
	public SystemEventQueue()
	{
		//Intantiate double bufferd input deques
		m_InputDeques.add(new ArrayDeque<SystemEvent>(SYSTEM_EVENT_CAPACITY));
		m_InputDeques.add(new ArrayDeque<SystemEvent>(SYSTEM_EVENT_CAPACITY));
	}
	/*********************************************************************************/
	public ArrayDeque<SystemEvent> GetFrontBuffer()
	{
		ArrayDeque<SystemEvent> frontBuffer = null;

		synchronized(m_InputQueueLock)
		{
			int inputDequeIndex = m_ActiveInputDeque == 0 ? 1 : 0;

			frontBuffer = m_InputDeques.get(inputDequeIndex); 
		}
		return frontBuffer;
	}
	/*********************************************************************************/
	public void FlipBuffers()
	{
		synchronized(m_InputQueueLock)
		{
			m_ActiveInputDeque = m_ActiveInputDeque == 0 ? 1 : 0;
			m_InputDeques.get(m_ActiveInputDeque).clear();
		}
	}
	/*********************************************************************************/
	public void AddSystemEvent(int p_EventType, int p_Flags, float p_FloatParam)
	{
		SystemEvent systemEvent = new SystemEvent();
		ByteBuffer paramByteBuffer = ByteBuffer.allocate(4);

		paramByteBuffer.order(ByteOrder.LITTLE_ENDIAN);
		paramByteBuffer.putFloat(p_FloatParam);

		systemEvent.eventType = p_EventType;
		systemEvent.inputFlag = p_Flags;
		systemEvent.param = paramByteBuffer.array();

		synchronized(m_InputQueueLock)
		{
			m_InputDeques.get(m_ActiveInputDeque).add(systemEvent);
		}
	}
	/*********************************************************************************/
	public void AddSystemEvent(int p_EventType, int p_Flags, short p_ShortParam1, short p_ShortParam2)
	{
		SystemEvent systemEvent = new SystemEvent();
		ByteBuffer paramByteBuffer = ByteBuffer.allocate(4);

		paramByteBuffer.order(ByteOrder.LITTLE_ENDIAN);
		paramByteBuffer.putShort(p_ShortParam1);
		paramByteBuffer.putShort(p_ShortParam2);

		systemEvent.eventType = p_EventType;
		systemEvent.inputFlag = p_Flags;
		systemEvent.param = paramByteBuffer.array();

		synchronized(m_InputQueueLock)
		{
			m_InputDeques.get(m_ActiveInputDeque).add(systemEvent);
		}
	}
	/*********************************************************************************/
	public void AddSystemEvent(int p_EventType, int p_Flags, int p_IntParam)
	{
		SystemEvent systemEvent = new SystemEvent();
		ByteBuffer paramByteBuffer = ByteBuffer.allocate(4);

		paramByteBuffer.order(ByteOrder.LITTLE_ENDIAN);
		paramByteBuffer.putInt(p_IntParam);

		systemEvent.eventType = p_EventType;
		systemEvent.inputFlag = p_Flags;
		systemEvent.param = paramByteBuffer.array();

		synchronized(m_InputQueueLock)
		{
			m_InputDeques.get(m_ActiveInputDeque).add(systemEvent);
		}
	}
	/*********************************************************************************/
	public void AddSystemEvent(int p_EventType, int p_Flags)
	{
		SystemEvent systemEvent = new SystemEvent();
		ByteBuffer paramByteBuffer = ByteBuffer.allocate(4);

		paramByteBuffer.order(ByteOrder.LITTLE_ENDIAN);
		paramByteBuffer.putInt(0);

		systemEvent.eventType = p_EventType;
		systemEvent.inputFlag = p_Flags;
		systemEvent.param = paramByteBuffer.array();

		synchronized(m_InputQueueLock)
		{
			m_InputDeques.get(m_ActiveInputDeque).add(systemEvent);
		}
	}
	/*********************************************************************************/
}

/*********************************************************************************/
class LauncherContentView extends View
{	
	private LauncherMainActivity m_LauncherMainActivty = null;
	private SystemEventQueue m_SystemEventQueue = new SystemEventQueue();
	
	/*********************************************************************************/
	public LauncherContentView(LauncherMainActivity p_LauncherMainActivty)
	{
		super((Context)p_LauncherMainActivty);

		m_LauncherMainActivty = p_LauncherMainActivty;

		//To get onGenericMotionEvent calls
		setFocusable(true);
		setFocusableInTouchMode(true);
	}
	/*********************************************************************************/
	public SystemEventQueue GetSystemEventQueue()
	{
		return m_SystemEventQueue;
	}
	/*********************************************************************************/
	//internal function to process key events
	private boolean onKeyEvent_Internal(KeyEvent p_KeyEvent, int p_KeyCode, int p_KeyEventType)
	{
		int inputSource = p_KeyEvent.getSource();
		boolean handled = false;
		
		int eventType = 0;
		int inputFlag = 0;
		int param = 0;

		if ((inputSource & InputDevice.SOURCE_GAMEPAD) > 0 ||
		(inputSource & InputDevice.SOURCE_JOYSTICK) > 0)
		{
			eventType = p_KeyEventType;
			inputFlag = SystemEvent.InputFlags.InputEventFlag;
			
			switch(p_KeyCode)
			{
				case KeyEvent.KEYCODE_BUTTON_X:
					param = ((1 << 16) | SystemEvent.ButtonIdentifier.ControllerFaceLeftButton);
					break;
					
				case KeyEvent.KEYCODE_BUTTON_Y:
					param = ((1 << 16) | SystemEvent.ButtonIdentifier.ControllerFaceUpButton);
					break;
					
				case KeyEvent.KEYCODE_BUTTON_A:
					param = ((1 << 16) | SystemEvent.ButtonIdentifier.ControllerFaceDownButton);
					break;
					
				case KeyEvent.KEYCODE_BUTTON_B:
					param = ((1 << 16) | SystemEvent.ButtonIdentifier.ControllerFaceRightButton);
					break;
					
				case KeyEvent.KEYCODE_BUTTON_START:
					param = ((1 << 16) | SystemEvent.ButtonIdentifier.ControllerStart);
					break;
					
				case KeyEvent.KEYCODE_BUTTON_SELECT:
					param = ((1 << 16) | SystemEvent.ButtonIdentifier.ControllerBack);
					break;
					
				case KeyEvent.KEYCODE_BUTTON_THUMBL:
					param = ((1 << 16) | SystemEvent.ButtonIdentifier.ControllerLeftThumbButton);
					break;
					
				case KeyEvent.KEYCODE_BUTTON_THUMBR:
					param = ((1 << 16) | SystemEvent.ButtonIdentifier.ControllerRightThumbButton);
					break;
					
				case KeyEvent.KEYCODE_BUTTON_R1:
					param = ((1 << 16) | SystemEvent.ButtonIdentifier.ControllerRightShoulderButton);
					break;
					
				case KeyEvent.KEYCODE_BUTTON_L1:
					param = ((1 << 16) | SystemEvent.ButtonIdentifier.ControllerLeftShoulderButton);
					break;
					
				case KeyEvent.KEYCODE_DPAD_LEFT:
					param = ((1 << 16) | SystemEvent.ButtonIdentifier.ControllerDPadLeft);
					break;
					
				case KeyEvent.KEYCODE_DPAD_RIGHT:
					param = ((1 << 16) | SystemEvent.ButtonIdentifier.ControllerDPadRight);
					break;
					
				case KeyEvent.KEYCODE_DPAD_UP:
					param = ((1 << 16) | SystemEvent.ButtonIdentifier.ControllerDPadUp);
					break;
					
				case KeyEvent.KEYCODE_DPAD_DOWN:
					param = ((1 << 16) | SystemEvent.ButtonIdentifier.ControllerDPadDown);
					break;
					
			}
			handled = true;
			
			m_SystemEventQueue.AddSystemEvent(eventType, inputFlag, param);
		}
		
		return handled;
	}
	/*********************************************************************************/
	//internal function to process touch movement events
	private boolean onTouchMoveEvent_Internal(MotionEvent p_MotionEvent)
	{
		short x = (short)(p_MotionEvent.getY() * p_MotionEvent.getYPrecision());
		short y = (short)(p_MotionEvent.getX() * p_MotionEvent.getXPrecision());
		
		m_SystemEventQueue.AddSystemEvent(SystemEvent.EventType.TouchMotion, SystemEvent.InputFlags.InputEventFlag, y, x);

		return true; //always handled
	}
	/*********************************************************************************/
	//internal function to process touch events
	private boolean onTouchEvent_Internal(MotionEvent p_MotionEvent)
	{
		short x = (short)(p_MotionEvent.getY() * p_MotionEvent.getYPrecision());
		short y = (short)(p_MotionEvent.getX() * p_MotionEvent.getXPrecision());
		
		m_SystemEventQueue.AddSystemEvent(SystemEvent.EventType.TouchInput, SystemEvent.InputFlags.InputEventFlag, y, x);

		return true; //always handled
	}
	/*********************************************************************************/
	//internal function to process game axis events
	private boolean onGameAxisEvent_Internal(MotionEvent p_MotionEvent)
	{
		final float THUMBSTICK_DEADZONE = 0.1f;
		final float TRIGGER_DEADZONE = 0.1f;
		boolean handled = false;
		
		float leftAxisX = p_MotionEvent.getAxisValue(MotionEvent.AXIS_X);
		float leftAxisY = p_MotionEvent.getAxisValue(MotionEvent.AXIS_Y);
		float rightAxisX = p_MotionEvent.getAxisValue(MotionEvent.AXIS_Z);
		float rightAxisY = p_MotionEvent.getAxisValue(MotionEvent.AXIS_RZ);
		float triggerRight = p_MotionEvent.getAxisValue(MotionEvent.AXIS_RTRIGGER);
		float triggerLeft = p_MotionEvent.getAxisValue(MotionEvent.AXIS_LTRIGGER);
		
		int eventType = 0;
		float param = 0.0f;

		if (Math.abs(leftAxisX) > THUMBSTICK_DEADZONE)
		{
			eventType = SystemEvent.EventType.ControllerLeftThumbX;
			param = leftAxisX;
		}
		else if(Math.abs(leftAxisY) > THUMBSTICK_DEADZONE)
		{
			eventType = SystemEvent.EventType.ControllerLeftThumbY;
			param = leftAxisY;
		}
		else if(Math.abs(rightAxisX) > THUMBSTICK_DEADZONE)
		{
			eventType = SystemEvent.EventType.ControllerRightThumbX;
			param = rightAxisX;
		}
		else if(Math.abs(rightAxisY) > THUMBSTICK_DEADZONE)
		{
			eventType = SystemEvent.EventType.ControllerRightThumbY;
			param = rightAxisY;
		}
		else if(Math.abs(triggerRight) > TRIGGER_DEADZONE)
		{
			eventType = SystemEvent.EventType.ControllerRightShoulderTrigger;
			param = triggerRight;
		}
		else if(Math.abs(triggerLeft) > TRIGGER_DEADZONE)
		{
			eventType = SystemEvent.EventType.ControllerLeftShoulderTrigger;
			param = triggerLeft;
		}
		
		if (eventType != 0)
		{
			m_SystemEventQueue.AddSystemEvent(eventType, SystemEvent.InputFlags.InputEventFlag, param);

			handled = true;
		}
		
		return handled;
	}
	/*********************************************************************************/
	public void onSystemEvent(int p_EventType)
	{
		m_SystemEventQueue.AddSystemEvent(p_EventType, SystemEvent.InputFlags.SystemEventFlag | SystemEvent.InputFlags.WindowEventFlag);
	}
	/*********************************************************************************/
    @Override
	public boolean onGenericMotionEvent(MotionEvent p_MotionEvent) 
    {
    	int inputSource = p_MotionEvent.getSource();
    	boolean handled = false;
    	
        if ((inputSource & InputDevice.SOURCE_GAMEPAD) > 0 || 
        (inputSource & InputDevice.SOURCE_JOYSTICK) > 0)
        {
        	handled = onGameAxisEvent_Internal(p_MotionEvent);
        }
        else if((inputSource & InputDevice.SOURCE_TOUCHSCREEN) > 0)
        {
        	handled = onTouchMoveEvent_Internal(p_MotionEvent);
        }
        
        if (!handled)
        {
        	handled = super.onGenericMotionEvent(p_MotionEvent);
        }

        return handled;
	}
	/*********************************************************************************/
	@Override
	public boolean onKeyDown(int p_KeyCode, KeyEvent p_KeyEvent) 
	{
		boolean handled = onKeyEvent_Internal(p_KeyEvent, p_KeyCode, SystemEvent.EventType.ControllerButtonPressed);
		
		if (!handled)
		{
			handled = super.onKeyDown(p_KeyCode, p_KeyEvent);
		}
		
		return handled;
	}
	/*********************************************************************************/
	@Override
	public boolean onKeyUp(int p_KeyCode, KeyEvent p_KeyEvent) 
	{
		boolean handled = onKeyEvent_Internal(p_KeyEvent, p_KeyCode, SystemEvent.EventType.ControllerButtonReleased);
		
		if (!handled)
		{
			handled = super.onKeyUp(p_KeyCode, p_KeyEvent);
		}
		
		return handled;
	}
	/*********************************************************************************/
	@Override
    public boolean onTouchEvent(MotionEvent p_MotionEvent)
    {
		boolean handled = onTouchEvent_Internal(p_MotionEvent);

		if (!handled)
		{
			handled = super.onTouchEvent(p_MotionEvent);
		}
		
        return handled;
    }
	/*********************************************************************************/
}

public class LauncherMainActivity extends Activity
{
	private LauncherContentView m_ContentView = null;
	private LauncherThread m_LauncherThread = null;
	private AlertDialog m_MessageBox = null;

	private static String LAUNCHER_LIBRARY = "launcher";

	/*********************************************************************************/
	public void exitLauncher()
	{
		finish();
		System.exit(0);
	}
	/*********************************************************************************/
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
       
        m_MessageBox = new AlertDialog.Builder(this).create();
		
        //try to load launcher library
        boolean libraryLoaded = true;

        try
        {
			System.loadLibrary(LAUNCHER_LIBRARY);
        }
        catch (UnsatisfiedLinkError linkerException)
        {
        	libraryLoaded = false;
        	showMessageBox(linkerException.getMessage());
        }

        if (libraryLoaded)
        {
			//showMessageBox("Library loaded!");
	       	m_ContentView = new LauncherContentView(this);
	       	m_LauncherThread = new LauncherThread(m_ContentView, this);
        	setContentView(m_ContentView);

			m_LauncherThread.start();
        }
    }
    /*********************************************************************************/
	@Override
	protected void onPause()
	{
		if (m_ContentView != null)
		{
			m_ContentView.onSystemEvent(SystemEvent.EventType.ApplicationQuit);	
		}
		
		super.onPause();
	}
	/*********************************************************************************/
	private void showMessageBox(String p_Message)
    {
    	AlertDialog alertDialog = new AlertDialog.Builder(this).create();

    	alertDialog.setCancelable(false);
    	alertDialog.setMessage(p_Message);
    	alertDialog.setButton("OK", new DialogInterface.OnClickListener() {
    		@Override
    		public void onClick(DialogInterface dialog, int which){
    			dialog.dismiss();
    		}
    	});

    	alertDialog.show();
    }
	/*********************************************************************************/
}
