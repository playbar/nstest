package flash.desktop
{
[native(cls="NativeProcessClass", methods="auto")]

import flash.events.EventDispatcher

import flash.utils.IDataInput
import flash.utils.IDataOutput
import flash.desktop.NativeProcessStartupInfo

public class NativeProcess extends EventDispatcher
{
	//Consts
	
	//Properties
	public native static function get isSupported() : Boolean
	
	public native function get running() : Boolean
	
	public native function get standardError() : IDataInput
	
	public native function get standardInput() : IDataOutput
	
	public native function get standardOutput() : IDataInput
	
	//Methods
	public native function NativeProcess()
	public native function closeInput():void
	public native function exit(force:Boolean = false):void
	public native function start(info:NativeProcessStartupInfo):void
}
}