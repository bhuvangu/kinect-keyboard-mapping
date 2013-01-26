// ds.cpp : main project file.

#include "stdafx.h"
#include <XnOpenNI.h>
#include <XnLog.h>
#include <XnCppWrapper.h>
#include <XnFPSCalculator.h>

using namespace System;
using namespace xn;
using namespace System::Windows::Forms;
int mkoain(array<System::String ^> ^args)
{

	XnStatus nRetVal = XN_STATUS_OK;
	Context context;
	nRetVal = context.Init();
	// TODO: check error code
	// Create a depth generator
	xn::DepthGenerator depth;
	nRetVal = depth.Create(context);
	// TODO: check error code
	// Set it to VGA maps at 30 FPS
	XnMapOutputMode mapMode;
	mapMode.nXRes = XN_VGA_X_RES;
	mapMode.nYRes = XN_VGA_Y_RES;
	mapMode.nFPS = 30;
	nRetVal = depth.SetMapOutputMode(mapMode);
	// TODO: check error code
	// Start generating
	nRetVal = context.StartGeneratingAll();
	// TODO: check error code
	// Calculate index of middle pixel
	XnUInt32 nMiddleIndex = 
		  XN_VGA_X_RES * XN_VGA_Y_RES/2 + // start of middle line
		  XN_VGA_X_RES/2;                 // middle of this line
	while (TRUE)
	{
		  // Update to next frame
	nRetVal = context.WaitOneUpdateAll(depth);
		  // TODO: check error codeOpenNI User Guide Page | 24
		  const XnDepthPixel* pDepthMap = depth.GetDepthMap();
		  printf("Middle pixel is %u millimeters away\n", 
				 pDepthMap[nMiddleIndex]);
		if(pDepthMap[nMiddleIndex]>750)
		  {
			keybd_event(VK_RIGHT,0xcd,0,0);
			//keybd_event(VK_RIGHT,0xcd,KEYEVENTF_KEYUP,0);
		 }
		else if(pDepthMap[nMiddleIndex]<100)
		{
			//for(int i=0;i<550;i++){
			  //SendKeys::SendWait("{LEFT 1000}");
			//}
			keybd_event(VK_LEFT,0xcb,0,0);
			
		//	keybd_event(VK_LEFT,0xcb,KEYEVENTF_KEYUP,0);
		}
		else
		{
			keybd_event(VK_LEFT,0xcb,KEYEVENTF_KEYUP,0);
			keybd_event(VK_RIGHT,0xcd,KEYEVENTF_KEYUP,0);
		}
	}
	// Clean up
	context.Shutdown();
    return 0;
}
