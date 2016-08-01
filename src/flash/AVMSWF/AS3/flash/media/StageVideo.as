package flash.media
{
[native(cls="StageVideoClass", methods="auto")]

import flash.events.EventDispatcher

import flash.geom.Point
import flash.geom.Rectangle
import flash.net.NetStream

public class StageVideo extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get colorSpaces() : Vector.<String>
	
	public native function get depth() : int
	public native function set depth(depth : int):void
	
	public native function get pan() : Point
	public native function set pan(pan : Point):void
	
	public native function get videoHeight() : int
	
	public native function get videoWidth() : int
	
	public native function get viewPort() : Rectangle
	public native function set viewPort(viewPort : Rectangle):void
	
	public native function get zoom() : Point
	public native function set zoom(zoom : Point):void
	
	//Methods
	public native function attachNetStream(netStream:NetStream):void
	
	//new adds 11
	public native function attachCamera(theCamera:Camera):void
	//new adds 11 end
}
}