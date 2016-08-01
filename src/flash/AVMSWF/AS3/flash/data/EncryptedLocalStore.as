package flash.data
{
[native(cls="EncryptedLocalStoreClass", methods="auto")]

import flash.utils.ByteArray

public class EncryptedLocalStore
{
	//Consts
	
	//Properties
	public native static function get isSupported() : Boolean
	
	//Methods
	public native static function getItem(name:String):ByteArray
	public native static function removeItem(name:String):void
	public native static function reset():void
	public native static function setItem(name:String, data:ByteArray, stronglyBound:Boolean = false):void
}
}