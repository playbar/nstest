package flash.media
{
[native(cls="MediaPromiseClass", methods="auto")]

import flash.events.EventDispatcher

import flash.filesystem.File
import flash.utils.IDataInput
import flash.events.ErrorEvent

public class MediaPromise extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get file() : File
	
	public native function get isAsync() : Boolean
	
	public native function get mediaType() : String
	
	public native function get relativePath() : String
	
	//Methods
	public native function close():void
	public native function open():IDataInput
	public native function reportError(e:ErrorEvent):void
}
}