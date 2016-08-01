package flash.display
{
[native(cls="NativeWindowClass", methods="auto")]

import flash.events.EventDispatcher

import flash.geom.*

public class NativeWindow extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get active() : Boolean
	
	public native function get alwaysInFront() : Boolean
	public native function set alwaysInFront(alwaysInFront : Boolean):void
	
	public native function get bounds() : Rectangle
	public native function set bounds(bounds : Rectangle):void
	
	public native function get closed() : Boolean
	
	public native function get displayState() : String
	
	public native function get height() : Number
	public native function set height(height : Number):void
	
	public native static function get isSupported() : Boolean		//s
	
	public native function get maximizable() : Boolean
	
	public native function get maxSize() : Point
	public native function set maxSize(maxSize : Point):void
	
	public native function get menu() : NativeMenu
	public native function set menu(menu : NativeMenu):void
	
	public native function get minimizable() : Boolean
	
	public native function get minSize() : Point
	public native function set minSize(minSize : Point):void
	
	public native function get owner() : NativeWindow
	
	public native function get resizable() : Boolean
	
	public native function get stage() : Stage
	
	public native static function get supportsMenu() : Boolean	//s
	
	public native static function get supportsNotification() : Boolean	//s
	
	public native static function get supportsTransparency() : Boolean	//s
	
	public native function get systemChrome() : String
	
	public native static function get systemMaxSize() : Point		//s
	
	public native static function get systemMinSize() : Point		//s
	
	public native function get title() : String
	public native function set title(title : String):void
	
	public native function get transparent() : Boolean
	
	public native function get type() : String
	
	public native function get visible() : Boolean
	public native function set visible(visible : Boolean):void
	
	public native function get width() : Number
	public native function set width(width : Number):void
	
	public native function get x() : Number
	public native function set x(x : Number):void
	
	public native function get y() : Number
	public native function set y(y : Number):void
	
	//new adds 11
	public native function get renderMode():String
	
	//new adds 11 end
	
	//Methods
	public native function NativeWindow(initOptions:NativeWindowInitOptions)
 	public native function activate():void
	public native function close():void
	public native function globalToScreen(globalPoint:Point):Point
	public native function listOwnedWindows():Vector.<NativeWindow>
	public native function maximize():void
	public native function minimize():void
	public native function notifyUser(type:String):void
	public native function orderInBackOf(window:NativeWindow):Boolean
	public native function orderInFrontOf(window:NativeWindow):Boolean
	public native function orderToBack():Boolean
	public native function orderToFront():Boolean
	public native function restore():void
	public native function startMove():Boolean
	public native function startResize(edgeOrCorner:String = "BR"):Boolean
}
}