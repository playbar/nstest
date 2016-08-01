 package BWLoader
 {
 
 	[native(cls="WaveStreamClass",instance="WaveStreamObject",methods="auto")]
	import flash.media.Microphone;
	import flash.utils.ByteArray;
	import flash.events.EventDispatcher;
	import flash.events.Event;
	
	public class BXWaveEvent extends Event
	{
		//public var data:ByteArray;
		//public var channels:int;
		static public const WAVEIN_EVENT:String="wave_in";
		public native function getData():ByteArray;
		public native function getChannels():int;
	}
	
 	public class BXWaveStream extends EventDispatcher
 	{ 	
 		 static public function get isEnabled():Boolean
		 {
			 return true;
		 }
 		 public native function BXWaveStream(mic:Microphone):void
 		 public native function getDCTData(data:ByteArray):void
 		 public native function getMicrophone():Microphone
 		 public native function start():Boolean
 		 public native function stop():Boolean
 	}
 	
 	public class BXAcc extends Object
 	{
 		static public native function init():Boolean;
 		static public native function get accX():Number;
 		static public native function get accY():Number;
 		static public native function get accZ():Number;
 		static public native function get gObject():Object;
 		static public native function get aObject():Object;
 	} 	
 }