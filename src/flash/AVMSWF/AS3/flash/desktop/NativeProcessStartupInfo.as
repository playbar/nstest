package flash.desktop
{
[native(cls="NativeProcessStartupInfoClass", methods="auto")]

import flash.filesystem.File

public class NativeProcessStartupInfo
{
	//Consts
	
	//Properties
	public native function get arguments() : Vector.<String>
	public native function set arguments(arguments : Vector.<String>):void
	
	public native function get executable() : File
	public native function set executable(executable : File):void
	
	public native function get workingDirectory() : File
	public native function set workingDirectory(workingDirectory : File):void
	
	//Methods
	public native function NativeProcessStartupInfo()
}
}