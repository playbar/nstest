 package BWLoader
 {
 
 	[native(cls="BXFile",instance="File",methods="auto")]
	import flash.utils.ByteArray;
 	public class BXFile extends Object
 	{
 		static public const SEEK_SET:int=0;
 		static public const SEEK_CUR:int=1;
 		static public const SEEK_END:int=2;
 		static public const READ:int = 1;
 		static public const WRITE:int = 2;
 		static public const CREATE:int = 4;
 		static public const APPEND:int = 8;
 		static public const SWF:int = 0;
 		static public const NETGAME:int = 1;
 		static public const WEB:int = 2;
 		static public const APP:int = 3;
 		public native function open(file:String,mode:int):Boolean
 		public function BXFile():void
 		{
 		}
 		public native function close():Boolean
 		public native function getLength():int
 		public native function seek(pos:int,type=SEEK_SET):int;
 		public native function write(data:ByteArray,s:int,l:int):int;
 		public native function read(data:ByteArray,s:int,l:int):int;
 		static public native function deleteFile(file:String):Boolean
 		static public native function copyFile(src:String,dst:String):Boolean
 		//static public native function createDirectory(name:String):Boolean
 		//static public native function removeDirectory(name:String):Boolean
 		//static public native function getFileList(path:String,fileList:Array,pathList:Array):void
 		//static public native function isFile(name:String):Boolean
 		static public native function exec(name:String,type:int):Boolean
 		static public native function createShortcutIcon(file:String,image:String,name:String):Boolean
 		static public native function reload():Boolean
 		static public native function isExist(file:String):Boolean
 	}
 }