package flash.desktop
{
[native(cls="IFilePromiseClass", methods="auto")]

import flash.utils.IDataInput
import flash.events.ErrorEvent

public interface IFilePromise
{
	//Consts
	
	//Properties
	function get isAsync() : Boolean
	
	function get relativePath() : String
	
	//Methods
	function close():void
	function open():IDataInput
	function reportError(e:ErrorEvent):void
}
}