package flash.media
{
[native(cls="StageWebViewClass", methods="auto")]

import flash.events.EventDispatcher

import flash.display.Stage
import flash.geom.Rectangle
import flash.display.BitmapData

public class StageWebView extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get isHistoryBackEnabled() : Boolean
	
	public native function get isHistoryForwardEnabled() : Boolean
	
	public native static function get isSupported() : Boolean
	
	public native function get location() : String
	
	public native function get stage() : Stage
	public native function set stage(stage : Stage):void
	
	public native function get title() : String
	
	public native function get viewPort() : Rectangle
	public native function set viewPort(viewPort : Rectangle):void
	
	//Methods
	public native function StageWebView()
	public native function assignFocus(direction:String = "none"):void
	public native function dispose():void
	public native function drawViewPortToBitmapData(bitmap:BitmapData):void
	public native function historyBack():void
	public native function historyForward():void
	public native function loadString(text:String, mimeType:String = "text/html"):void
	public native function loadURL(url:String):void
	public native function reload():void
	public native function stop():void
}
}