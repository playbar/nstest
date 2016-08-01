/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine.].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 2004-2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe  Team
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

 package flash.net
 {
 	[native(cls="SocketClass", methods="auto")]

	import flash.events.EventDispatcher
	import flash.utils.IDataInput;
	import flash.utils.IDataOutput;
	import flash.utils.ByteArray

 	public class Socket extends  EventDispatcher implements IDataInput,IDataOutput
 	{
 		//Properties
 		 public native function get timeout():uint
 		 public native function set timeout(newVar:uint):void
 		 public native function get bytesAvailable() : uint 

		 public native function get connected() : Boolean 

		 public native function get endian() : String 
		 public native function set endian(newEndian : String): void

		 public native function get objectEncoding() : uint 
		 public native function set objectEncoding(newObjectEncoding : uint): void

		 //new adds
		public native function get localAddress() : String

		public native function get localPort() : int

		public native function get remoteAddress() : String

		public native function get remotePort() : int

//		public native function get timeout() : uint
//		public native function set timeout(timeout : uint):void
		 //new adds end

 		//Methods
 		 public native function Socket(host:String = null, port:int = 0) 

 		 public native function close():void 
 		 public native function connect(host:String, port:int):void 
 		 public native function flush():void 
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
		 public native function writeBoolean(value:Boolean):void 
		 public native function writeByte(value:int):void 
		 public native function writeBytes(bytes:ByteArray, offset:uint = 0, length:uint = 0):void 

		 public function writeUnsignedByte(value:uint):void
		 {
			writeByte(value);
		 }
		 public function writeUnsignedShort(value:uint):void
		 {
			writeShort(value);
		 }
		 public native function writeDouble(value:Number):void 
		 public native function writeFloat(value:Number):void 
		 public native function writeInt(value:int):void 
		 public native function writeMultiByte(value:String, charSet:String):void 
		 public native function writeObject(object:*):void 

		 public native function writeShort(value:int):void 
		 public native function writeUnsignedInt(value:uint):void 
		 public native function writeUTF(value:String):void 
		 public native function writeUTFBytes(value:String):void 
		 
		 //new adds 11
		 public native function get bytesPending():uint
		 //new adds 11 end
 	}
 }
