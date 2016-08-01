package flash.external
{
[native(cls="ExtensionContextClass", methods="auto")]

import flash.events.EventDispatcher

import flash.filesystem.File

public class ExtensionContext extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get actionScriptData() : Object
	public native function set actionScriptData(actionScriptData : Object):void
	
	//Methods
	public native function call(functionName:String, ... args):Object
	public native static function createExtensionContext(extensionID:String, contextType:String):ExtensionContext
	public native function dispose():void
	public native static function getExtensionDirectory(extensionID:String):File
}
}