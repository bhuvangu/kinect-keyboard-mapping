//#include "StdAfx.h"
#include "stdafx.h"
#include <XnOpenNI.h>
#include <XnLog.h>
#include <XnCppWrapper.h>
#include <XnFPSCalculator.h>
#include <XnPrdNode.h>

using namespace System;
using namespace xn;

#define POSE_TO_USE "Psi" 

xn::UserGenerator g_UserGenerator; 

void XN_CALLBACK_TYPE 

User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie) 
{ 

  printf("New User: %d\n", nId); 

   g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(POSE_TO_USE,nId); 

} 

void XN_CALLBACK_TYPE 

User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie) 

{} 

void XN_CALLBACK_TYPE 

Pose_Detected(xn::PoseDetectionCapability& pose, const XnChar* strPose, XnUserID nId, void* pCookie) 

{ 

  printf("Pose %s for user %d\n", strPose, nId); 

   g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId); 

   g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE); 

} 

void XN_CALLBACK_TYPE 

Calibration_Start(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie) 

{ 

  printf("Starting calibration for user %d\n", nId); 

} 

void XN_CALLBACK_TYPE 

Calibration_End(xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie) 

{ 

  if (bSuccess) 

  { 

    printf("User calibrated\n"); 

    g_UserGenerator.GetSkeletonCap().StartTracking(nId); 

  } 

  else 

  { 

    printf("Failed to calibrate user %d\n", nId); 

   g_UserGenerator.GetPoseDetectionCap().StartPoseDetection( POSE_TO_USE,nId); 

  } 

} 

int main(array<System::String ^> ^args) 

{ 

  XnStatus nRetVal = XN_STATUS_OK; 

  Context context; 

  nRetVal = context.Init(); 

  // TODO: check error code 

  // Create the user generator 

  nRetVal = g_UserGenerator.Create(context); 

  // TODO: check error code 

  XnCallbackHandle h1, h2, h3; 

  g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser,  NULL, h1); 

  g_UserGenerator.GetPoseDetectionCap().RegisterToPoseCallbacks( Pose_Detected, NULL, NULL, h2); 

  g_UserGenerator.GetSkeletonCap().RegisterCalibrationCallbacks( Calibration_Start, Calibration_End, NULL, h3); 

  // Set the profile 

    g_UserGenerator.GetSkeletonCap().SetSkeletonProfile( XN_SKEL_PROFILE_ALL); 

   // Start generating 

   nRetVal = context.StartGeneratingAll(); 

   // TODO: check error code 
   keybd_event(VK_LEFT,0xcb,KEYEVENTF_KEYUP,0);
			     keybd_event(VK_RIGHT,0xcd,KEYEVENTF_KEYUP,0);
				// keybd_event(VK_UP,0xc8,0,0);
  while (TRUE) 

   { 

     // Update to next frame 

     nRetVal = context.WaitAndUpdateAll(); 

     // TODO: check error code 

     // Extract head position of each tracked user 

     XnUserID aUsers[15]; 

     XnUInt16 nUsers = 15; 

     g_UserGenerator.GetUsers(aUsers, nUsers); 

     for (int i = 0; i < nUsers; ++i) 

     { 

       if (g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i])) 

        { 

          XnSkeletonJointPosition left,right,head; 

          g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(  aUsers[i], XN_SKEL_LEFT_HAND, left); 
		  g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(  aUsers[i], XN_SKEL_RIGHT_HAND, right);
		  g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(  aUsers[i], XN_SKEL_HEAD, head);
		 
         // printf(" %f  \n", left.position.Z-right.position.Z); 
		  		  if(left.position.Z - right.position.Z<75 && left.position.Z - right.position.Z>-75)//|| left.position.Z - right.position.Z >-30)
		  {

			  if(head.position.Z-left.position.Z>400)
			  {
			  keybd_event(VK_UP,0xc8,0,0);
			  }
			  else
			  {
				  keybd_event(VK_UP,0xc8,KEYEVENTF_KEYUP,0);
			  }
			  printf("%f  %f\n",left.position.Z,right.position.Z);
			  if(left.position.Y-right.position.Y>100)
			  {
			//	  printf("move left\n");
				  keybd_event(VK_LEFT,0xcb,0,0);

			  }
			  else if(right.position.Y-left.position.Y>100)
			  {
				  keybd_event(VK_RIGHT,0xcd,0,0);
			  }
			  else
			  {
				 keybd_event(VK_LEFT,0xcb,KEYEVENTF_KEYUP,0);
			     keybd_event(VK_RIGHT,0xcd,KEYEVENTF_KEYUP,0);
			  }
		  }

        }
	   


     } 

   } 

   // Clean up 

   context.Shutdown(); 

} 