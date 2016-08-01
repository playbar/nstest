package flash.media
{
[native(cls="CameraRollClass", methods="auto")]

import flash.events.EventDispatcher

import flash.display.BitmapData

public class CameraRoll extends EventDispatcher
{
	//Consts
	
	//Properties
	 public native static function get supportsAddBitmapData() : Boolean
	 
	 public native static function get supportsBrowseForImage() : Boolean
	
	//Methods
	public native function CameraRoll()
	public native function addBitmapData(bitmapData:BitmapData):void
	public native function browseForImage():void
}
}