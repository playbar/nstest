package flash.desktop
{
[native(cls="UpdaterClass", methods="auto")]

import flash.filesystem.File

public class Updater
{
	//Consts
	
	//Properties
	public native static function get isSupported() : Boolean
	
	//Methods
	public native function Updater()
	public native function update(airFile:File, version:String):void
}
}