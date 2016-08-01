package flash.filesystem
{
[native(cls="FileStreamClass", methods="auto")]

import flash.events.EventDispatcher

import flash.utils.ByteArray

public class FileStream extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get bytesAvailable() : uint
	
	public native function get endian() : String
	public native function set endian(endian : String):void
	
	public native function get objectEncoding() : uint
	public native function set objectEncoding(objectEncoding : uint):void
	
	public native function get position() : Number
	public native function set position(position : Number):void
	
	public native function get readAhead() : Number
	public native function set readAhead(readAhead : Number):void
	
	//Methods
	public native function FileStream()
	public native function close():void
	public native function open(file:File, fileMode:String):void
	public native function openAsync(file:File, fileMode:String):void
	public native function readBoolean():Boolean
	public native function readByte():int
	public native function readBytes(bytes:ByteArray, offset:uint = 0, length:uint = 0):void
	public native function readDouble():Number
	public native function readFloat():Number
	public native function readInt():int
	public native function readMultiByte(length:uint, charSet:String):String
	public native function readObject():*
	public native function readShort():int
	public native function readUnsignedByte():uint
	public native function readUnsignedInt():uint
	public native function readUnsignedShort():uint
	public native function readUTF():String
	public native function readUTFBytes(length:uint):String
	public native function truncate():void
	public native function writeBoolean(value:Boolean):void
	public native function writeByte(value:int):void
	public native function writeBytes(bytes:ByteArray, offset:uint = 0, length:uint = 0):void
	public native function writeDouble(value:Number):void
	public native function writeFloat(value:Number):void
	public native function writeInt(value:int):void
	public native function writeMultiByte(value:String, charSet:String):void
	public native function writeObject(object:*):void
	public native function writeShort(value:int):void
	public native function writeUnsignedInt(value:uint):void
	public native function writeUTF(value:String):void
	public native function writeUTFBytes(value:String):void
}
}