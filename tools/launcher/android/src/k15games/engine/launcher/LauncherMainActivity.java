package k15games.engine.launcher;

import android.app.Activity;
import android.app.AlertDialog;
import android.view.KeyEvent;
import android.view.View;
import android.view.MotionEvent;
import android.view.InputDevice;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;

import java.util.ArrayDeque;
import java.util.Deque;
import java.nio.ByteBuffer;

/*********************************************************************************/
final class SystemEvent
{
	public int eventType = EventType.NoEvent;
	public byte[] param = null;
	public int inputFlag = 0;
	
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
}
/*********************************************************************************/



/*********************************************************************************/
class LauncherThread extends Thread
{
	private final float NANOSECONDS_TO_MILLISECONDS = 1 / 1000000;
	private boolean m_Running = true;
	private LauncherContentView m_ContentView = null;
	
	public LauncherThread(LauncherContentView p_ContentView)
	{
		m_ContentView = p_ContentView;
	}

	@Override
	public void run() 
	{
		K15_InitEngine();
		K15_RunEngine(); //blocks until engine shutdown
		K15_ShutdownEngine();
	}
	
	public native void K15_RunEngine();
	public native void K15_InitEngine();
	public native void K15_ShutdownEngine();
}
/*********************************************************************************/



/*********************************************************************************/
class LauncherContentView extends View
{	
	private LauncherMainActivity m_LauncherMainActivty = null;
	private Deque<SystemEvent> m_InputDeque = new ArrayDeque<SystemEvent>();
	
	/*********************************************************************************/
	public Deque<SystemEvent> GetInputDeque()
	{
		return m_InputDeque;
	}
	/*********************************************************************************/
	
	
	
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
	//internal function to process key events
	private boolean onKeyEvent_Internal(KeyEvent p_KeyEvent, int p_KeyCode, int p_KeyEventType)
	{
		SystemEvent inputEvent = new SystemEvent();
		
		int inputSource = p_KeyEvent.getSource();
		boolean handled = false;
		
		if ((inputSource & InputDevice.SOURCE_GAMEPAD) > 0 ||
		(inputSource & InputDevice.SOURCE_JOYSTICK) > 0)
		{
			inputEvent.eventType = p_KeyEventType;
			inputEvent.inputFlag = SystemEvent.InputFlags.InputEventFlag;
			
			switch(p_KeyCode)
			{
				case KeyEvent.KEYCODE_BUTTON_X:
					inputEvent.param = ByteBuffer.allocate(4).putInt((1 << 16) | SystemEvent.ButtonIdentifier.ControllerFaceLeftButton).array();
					break;
					
				case KeyEvent.KEYCODE_BUTTON_Y:
					inputEvent.param = ByteBuffer.allocate(4).putInt((1 << 16) | SystemEvent.ButtonIdentifier.ControllerFaceUpButton).array();
					break;
					
				case KeyEvent.KEYCODE_BUTTON_A:
					inputEvent.param = ByteBuffer.allocate(4).putInt((1 << 16) | SystemEvent.ButtonIdentifier.ControllerFaceDownButton).array();
					break;
					
				case KeyEvent.KEYCODE_BUTTON_B:
					inputEvent.param = ByteBuffer.allocate(4).putInt((1 << 16) | SystemEvent.ButtonIdentifier.ControllerFaceRightButton).array();
					break;
					
				case KeyEvent.KEYCODE_BUTTON_START:
					inputEvent.param = ByteBuffer.allocate(4).putInt((1 << 16) | SystemEvent.ButtonIdentifier.ControllerStart).array();
					break;
					
				case KeyEvent.KEYCODE_BUTTON_SELECT:
					inputEvent.param = ByteBuffer.allocate(4).putInt((1 << 16) | SystemEvent.ButtonIdentifier.ControllerBack).array();
					break;
					
				case KeyEvent.KEYCODE_BUTTON_THUMBL:
					inputEvent.param = ByteBuffer.allocate(4).putInt((1 << 16) | SystemEvent.ButtonIdentifier.ControllerLeftThumbButton).array();
					break;
					
				case KeyEvent.KEYCODE_BUTTON_THUMBR:
					inputEvent.param = ByteBuffer.allocate(4).putInt((1 << 16) | SystemEvent.ButtonIdentifier.ControllerRightThumbButton).array();
					break;
					
				case KeyEvent.KEYCODE_BUTTON_R1:
					inputEvent.param = ByteBuffer.allocate(4).putInt((1 << 16) | SystemEvent.ButtonIdentifier.ControllerRightShoulderButton).array();
					break;
					
				case KeyEvent.KEYCODE_BUTTON_L1:
					inputEvent.param = ByteBuffer.allocate(4).putInt((1 << 16) | SystemEvent.ButtonIdentifier.ControllerLeftShoulderButton).array();
					break;
					
				case KeyEvent.KEYCODE_DPAD_LEFT:
					inputEvent.param = ByteBuffer.allocate(4).putInt((1 << 16) | SystemEvent.ButtonIdentifier.ControllerDPadLeft).array();
					break;
					
				case KeyEvent.KEYCODE_DPAD_RIGHT:
					inputEvent.param = ByteBuffer.allocate(4).putInt((1 << 16) | SystemEvent.ButtonIdentifier.ControllerDPadRight).array();
					break;
					
				case KeyEvent.KEYCODE_DPAD_UP:
					inputEvent.param = ByteBuffer.allocate(4).putInt((1 << 16) | SystemEvent.ButtonIdentifier.ControllerDPadUp).array();
					break;
					
				case KeyEvent.KEYCODE_DPAD_DOWN:
					inputEvent.param = ByteBuffer.allocate(4).putInt((1 << 16) | SystemEvent.ButtonIdentifier.ControllerDPadDown).array();
					break;
					
			}
			handled = true;
			
			m_InputDeque.add(inputEvent);
		}
		
		return handled;
	}
	/*********************************************************************************/
	//internal function to process touch movement events
	private boolean onTouchMoveEvent_Internal(MotionEvent p_MotionEvent)
	{
		SystemEvent inputEvent = new SystemEvent();
		
		short x = (short)(p_MotionEvent.getY() * p_MotionEvent.getYPrecision());
		short y = (short)(p_MotionEvent.getX() * p_MotionEvent.getXPrecision());
		
		inputEvent.eventType = SystemEvent.EventType.TouchMotion;
		inputEvent.param = ByteBuffer.allocate(4).putShort(0, x).putShort(1, y).array();
		inputEvent.inputFlag = SystemEvent.InputFlags.InputEventFlag;
		
		m_InputDeque.add(inputEvent);
		
		return true; //always handled
	}
	/*********************************************************************************/
	//internal function to process touch events
	private boolean onTouchEvent_Internal(MotionEvent p_MotionEvent)
	{
		SystemEvent inputEvent = new SystemEvent();
		
		short x = (short)(p_MotionEvent.getY() * p_MotionEvent.getYPrecision());
		short y = (short)(p_MotionEvent.getX() * p_MotionEvent.getXPrecision());
		
		inputEvent.eventType = SystemEvent.EventType.TouchInput;
		inputEvent.param = ByteBuffer.allocate(4).putShort(0, x).putShort(1, y).array();
		inputEvent.inputFlag = SystemEvent.InputFlags.InputEventFlag;
		
		m_InputDeque.add(inputEvent);
		
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
		
		SystemEvent inputEvent = null;
		
		if (Math.abs(leftAxisX) > THUMBSTICK_DEADZONE)
		{
			inputEvent = new SystemEvent();
			inputEvent.eventType = SystemEvent.EventType.ControllerLeftThumbX;
			inputEvent.param = ByteBuffer.allocate(4).putFloat(leftAxisX).array();
		}
		else if(Math.abs(leftAxisY) > THUMBSTICK_DEADZONE)
		{
			inputEvent = new SystemEvent();
			inputEvent.eventType = SystemEvent.EventType.ControllerLeftThumbY;
			inputEvent.param = ByteBuffer.allocate(4).putFloat(leftAxisY).array();
		}
		else if(Math.abs(rightAxisX) > THUMBSTICK_DEADZONE)
		{
			inputEvent = new SystemEvent();
			inputEvent.eventType = SystemEvent.EventType.ControllerRightThumbX;
			inputEvent.param = ByteBuffer.allocate(4).putFloat(rightAxisX).array();
		}
		else if(Math.abs(rightAxisY) > THUMBSTICK_DEADZONE)
		{
			inputEvent = new SystemEvent();
			inputEvent.eventType = SystemEvent.EventType.ControllerRightThumbY;
			inputEvent.param = ByteBuffer.allocate(4).putFloat(rightAxisY).array();
		}
		else if(Math.abs(triggerRight) > TRIGGER_DEADZONE)
		{
			inputEvent = new SystemEvent();
			inputEvent.eventType = SystemEvent.EventType.ControllerRightShoulderTrigger;
			inputEvent.param = ByteBuffer.allocate(4).putFloat(triggerRight).array();
		}
		else if(Math.abs(triggerLeft) > TRIGGER_DEADZONE)
		{
			inputEvent = new SystemEvent();
			inputEvent.eventType = SystemEvent.EventType.ControllerLeftShoulderTrigger;
			inputEvent.param = ByteBuffer.allocate(4).putFloat(triggerLeft).array();
		}
		
		if (inputEvent != null)
		{
			inputEvent.inputFlag = SystemEvent.InputFlags.InputEventFlag;
			m_InputDeque.add(inputEvent);
			
			handled = true;
		}
		
		return handled;
	}
	/*********************************************************************************/
	public void onSystemEvent(int p_EventType)
	{
		SystemEvent systemEvent = new SystemEvent();
		
		systemEvent.eventType = p_EventType;
		systemEvent.inputFlag = SystemEvent.InputFlags.SystemEventFlag | SystemEvent.InputFlags.WindowEventFlag;
		
		m_InputDeque.add(systemEvent);
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
			showMessageBox("Library loaded!");
	       	m_ContentView = new LauncherContentView(this);
	       	m_LauncherThread = new LauncherThread(m_ContentView);
        	setContentView(m_ContentView);
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