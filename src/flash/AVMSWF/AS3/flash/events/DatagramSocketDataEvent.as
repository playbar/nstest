package flash.events
{
[native(cls="DatagramSocketDataEventClass", methods="auto")]

import flash.utils.ByteArray

public class DatagramSocketDataEvent extends Event
{
	//Consts
	public static const DATA : String = "data"
	
	//Properties
	public native function get data() : ByteArray
	public native function set data(data : ByteArray):void
	
	public native function get dstAddress() : String
	public native function set dstAddress(dstAddress : String):void
	
	public native function get dstPort() : int
	public native function set dstPort(dstPort : int):void
	
	public native function get srcAddress() : String
	public native function set srcAddress(srcAddress : String):void
	
	public native function get srcPort() : int
	public native function set srcPort(srcPort : int):void
	
	//Methods
	public native function DatagramSocketDataEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, srcAddress:String = "", srcPort:int = 0, dstAddress:String = "", dstPort:int = 0, data:ByteArray = null)
	public native override function clone():Event
	public native override function toString():String
}
}