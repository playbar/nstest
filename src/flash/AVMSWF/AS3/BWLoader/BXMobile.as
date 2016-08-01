 package BWLoader
 {
 	[native(cls="BXMobile",instance="File",methods="auto")]
 	import flash.display.bitmapData;
 	public class BXMobile extends Object
 	{
 		static public const ROTATE:int = -1;
 		static public const HOZ:int = 0;
 		static public const VER:int = 1;
 	    static public native function setMode(mode:int):Boolean
 	    static public native function get width():int;
 	    static public native function get height():int;
 	    static public native function get name():String;
 	    static public native function getScreenShot(x:int=0,y:int=0,w:int=0,h:int=0):BitmapData;
 	}
 }