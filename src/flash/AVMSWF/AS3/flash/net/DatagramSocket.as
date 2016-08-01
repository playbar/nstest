package flash.net
{
[native(cls="DatagramSocketClass", methods="auto")]

import flash.events.EventDispatcher

import flash.utils.ByteArray

public class DatagramSocket extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get bound() : Boolean
	
	public native function get connected() : Boolean
	
	public native static function get isSupported() : Boolean
	
	public native function get localAddress() : String
	
	public native function get localPort() : int
	
	public native function get remoteAddress() : String
	
	public native function get remotePort() : int
	
	//Methods
	public native function DatagramSocket()
	public native function bind(localPort:int = 0, localAddress:String = "0.0.0.0"):void
	public native function close():void
	public native function connect(remoteAddress:String, remotePort:int):void
	public native function receive():void
	public native function send(bytes:ByteArray, offset:uint = 0, length:uint = 0, address:String = null, port:int = 0):void
}
}