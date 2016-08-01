package flash.desktop
{
[native(cls="NativeApplicationClass", methods="auto")]

import flash.events.EventDispatcher

import flash.display.NativeWindow
import flash.desktop.InteractiveIcon
import flash.display.NativeMenu
import flash.events.Event

public class NativeApplication extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get activeWindow() : NativeWindow
	
	public native function get applicationDescriptor() : XML
	
	public native function get applicationID() : String
	
	public native function get autoExit() : Boolean
	public native function set autoExit(autoExit : Boolean):void
	
	public native function get icon() : InteractiveIcon
	
	public native function get idleThreshold() : int
	public native function set idleThreshold(idleThreshold : int):void
	
	public native function get menu() : NativeMenu
	public native function set menu(menu : NativeMenu):void
	
	public native static function get nativeApplication() : NativeApplication		//s
	
	public native function get openedWindows() : Array
	
	public native function get publisherID() : String
	
	public native function get runtimePatchLevel() : uint
	
	public native function get runtimeVersion() : String
	
	public native function get startAtLogin() : Boolean
	public native function set startAtLogin(startAtLogin : Boolean):void
	
	public native static function get supportsDefaultApplication() : Boolean		//s
	
	public native static function get supportsDockIcon() : Boolean	//s
	
	public native static function get supportsMenu() : Boolean	//s
	
	public native static function get supportsStartAtLogin() : Boolean	//s
	
	public native static function get supportsSystemTrayIcon() : Boolean	//s
	
	public native function get systemIdleMode() : String
	public native function set systemIdleMode(systemIdleMode : String):void
	
	public native function get timeSinceLastUserInput() : int
	
	//new adds 11
	public native function get executeInBackground():Boolean
    public native function set executeInBackground(value:Boolean):void
	//new adds 11 end
	
	//Methods
	public native function activate(window:NativeWindow = null):void
  public native override function addEventListener(type:String, listener:Function, useCapture:Boolean = false, priority:int = 0, useWeakReference:Boolean = false):void
 	public native function clear():Boolean
	public native function copy():Boolean
	public native function cut():Boolean
	public native override function dispatchEvent(event:Event):Boolean
	public native function exit(errorCode:int = 0):void
	public native function getDefaultApplication(extension:String):String
	public native function isSetAsDefaultApplication(extension:String):Boolean
	public native function paste():Boolean
	public native function removeAsDefaultApplication(extension:String):void
	public native override function removeEventListener(type:String, listener:Function, useCapture:Boolean = false):void
	public native function selectAll():Boolean
	public native function setAsDefaultApplication(extension:String):void
}
}