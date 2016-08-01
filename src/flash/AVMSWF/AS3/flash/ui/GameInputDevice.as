package flash.ui
{
	[native(cls="GameInputDeviceClass",methods="auto")]
	
	import flash.utils.ByteArray
	import flash.ui.GameInputControl
	
	public final class GameInputDevice
	{
		//Const
		
		public static const MAX_BUFFER_SIZE:int = 32000
		
		//Properties
		
		public native function get enabled():Boolean
		public native function set enabled(value:Boolean):void
	    
		public native function get id():String
	    
		public native function get name():String
	    
		public native function get numControls():int
	    
		public native function get sampleInterval():int
		public native function set sampleInterval(value:int):void
		
		//Methods
		
		public native function getCachedSamples(data:ByteArray, append:Boolean = false):int
		
		public native function getControlAt(i:int):GameInputControl
		
		public native function startCachingSamples(numSamples:int, controls:Vector.<String>):void
		
		public native function stopCachingSamples():void
	}
}