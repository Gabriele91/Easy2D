package com.easy2d;

import android.app.NativeActivity;
import android.util.Log;
import android.view.Display;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.view.Window;

public class Easy2DActivity extends NativeActivity {
  
  static {
      System.loadLibrary("Easy2DAndroid");
  }

  //native methods
  public native void onCreateInterface();
  //called in native activity
  public void setScreenOrientation(int orientation){

  	int androidOretation=ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;
  	switch(orientation){

		case 0:  androidOretation=ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;          break;	
		case 1:  androidOretation=ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT;  break;	
		case 2:  androidOretation=ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE; break;	
		case 3:  androidOretation=ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;         break;

		case 4:  androidOretation=ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT;	break;	
		case 5:  androidOretation=ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE;  break;	

  	}
  	this.setRequestedOrientation(androidOretation);
  }

  public int getScreenOrientation(){

    int androidOretation=0;
	switch(this.getRequestedOrientation()){

		case ActivityInfo.SCREEN_ORIENTATION_PORTRAIT:  androidOretation=0;          break;	
		case ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT:  androidOretation=1;  break;	
		case ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE:  androidOretation=2; break;	
		case ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE:  androidOretation=3;         break;

		case ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT:  androidOretation=4;	break;	
		case ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE:  androidOretation=5;  break;	
		
	}
	return androidOretation;

  }

  public void setScreenTitle(String title){
  	this.setTitle(title);
  }
  
  public void setFullScreenMode(){
	//Remove notification bar
	this
	.getWindow()
	.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
  }


  @Override
  public void onCreate(Bundle savedInstanceState) {
  	  this.onCreateInterface();
      super.onCreate(savedInstanceState);
  }

}
