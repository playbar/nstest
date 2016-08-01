package flash.html
{
[native(cls="HTMLHostClass", methods="auto")]

import flash.geom.Rectangle

public class HTMLHost
{
	//Consts
	
	//Properties
	public native function get htmlLoader() : HTMLLoader
	
	public native function get windowRect() : Rectangle
	public native function set windowRect(windowRect : Rectangle):void
	
	//Methods
	public native function HTMLHost(defaultBehaviors:Boolean = true)
	public native function createWindow(windowCreateOptions:HTMLWindowCreateOptions):HTMLLoader
	public native function updateLocation(locationURL:String):void
	public native function updateStatus(status:String):void
	public native function updateTitle(title:String):void
	public native function windowBlur():void
	public native function windowClose():void
	public native function windowFocus():void
}
}