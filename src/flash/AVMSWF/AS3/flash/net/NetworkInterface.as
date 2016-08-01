package flash.net
{
[native(cls="NetworkInterfaceClass", methods="auto")]

public class NetworkInterface
{
	//Consts
	
	//Properties
	public native function get active() : Boolean
	public native function set active(active : Boolean):void
	
	public native function get addresses() : Vector.<InterfaceAddress>
	public native function set addresses(addresses : Vector.<InterfaceAddress>):void
	
	public native function get displayName() : String
	public native function set displayName(displayName : String):void
	
	public native function get hardwareAddress() : String
	public native function set hardwareAddress(hardwareAddress : String):void
	
	public native function get mtu() : int
	public native function set mtu(mtu : int):void
	
	public native function get name() : String
	public native function set name(name : String):void
	
	public native function get parent() : NetworkInterface
	public native function set parent(parent : NetworkInterface):void
	
	public native function get subInterfaces() : Vector.<NetworkInterface>
	public native function set subInterfaces(subInterfaces : Vector.<NetworkInterface>):void
	
	//Methods
}
}