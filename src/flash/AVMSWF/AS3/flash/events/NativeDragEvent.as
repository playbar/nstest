package flash.events
{
[native(cls="NativeDragEventClass", methods="auto")]

import flash.desktop.NativeDragOptions
import flash.desktop.Clipboard
import flash.display.InteractiveObject

public class NativeDragEvent extends MouseEvent
{
	//Consts
	public static const NATIVE_DRAG_COMPLETE : String = "nativeDragComplete"
	public static const NATIVE_DRAG_DROP : String = "nativeDragDrop"
	public static const NATIVE_DRAG_ENTER : String = "nativeDragEnter"
	public static const NATIVE_DRAG_EXIT : String = "nativeDragExit"
	public static const NATIVE_DRAG_OVER : String = "nativeDragOver"
	public static const NATIVE_DRAG_START : String = "nativeDragStart"
	public static const NATIVE_DRAG_UPDATE : String = "nativeDragUpdate"
	
	//Properties
	public native function get allowedActions() : NativeDragOptions
	public native function set allowedActions(allowedActions : NativeDragOptions):void
	
	public native function get clipboard() : Clipboard
	public native function set clipboard(clipboard : Clipboard):void
	
	public native function get dropAction() : String
	public native function set dropAction(dropAction : String):void
	
	//Methods
	public native function NativeDragEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = true, localX:Number = NaN, localY:Number = NaN, relatedObject:InteractiveObject = null, clipboard:Clipboard = null, allowedActions:NativeDragOptions = null, dropAction:String = null, controlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false, commandKey:Boolean = false)
	public native override function clone():Event
	public native override function toString():String
}
}