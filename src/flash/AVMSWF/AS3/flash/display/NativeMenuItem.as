package flash.display
{
[native(cls="NativeMenuItemClass", methods="auto")]

import flash.events.EventDispatcher

public class NativeMenuItem extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get checked() : Boolean
	public native function set checked(checked : Boolean):void
	
	public native function get data() : Object
	public native function set data(data : Object):void
	
	public native function get enabled() : Boolean
	public native function set enabled(enabled : Boolean):void
	
	public native function get isSeparator() : Boolean
	
	public native function get keyEquivalent() : String
	public native function set keyEquivalent(keyEquivalent : String):void
	
	public native function get keyEquivalentModifiers() : Array
	public native function set keyEquivalentModifiers(keyEquivalentModifiers : Array):void
	
	public native function get label() : String
	public native function set label(label : String):void
	
	public native function get menu() : NativeMenu
	
	public native function get mnemonicIndex() : int
	public native function set mnemonicIndex(mnemonicIndex : int):void
	
	public native function get name() : String
	public native function set name(name : String):void
	
	public native function get submenu() : NativeMenu
	public native function set submenu(submenu : NativeMenu):void
	
	//Methods
	public native function NativeMenuItem(label:String = "", isSeparator:Boolean = false)
	public native function clone():NativeMenuItem
	public native function toString():String			//override
}
}