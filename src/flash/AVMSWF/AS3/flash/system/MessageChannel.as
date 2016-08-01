package flash.system
{
	[native(cls="MessageChannelClass",methods="auto")]
	
	import flash.events.EventDispatcher
	import flash.system.MessageChannelState
	
	public class MessageChannel extends EventDispatcher
	{
		//const
		
		//Properties
		public native function get messageAvailable():Boolean
		public native function get state():String
		
		//methods
		
		override public native function addEventListener(type:String, listener:Function, useCapture:Boolean = false, priority:int = 0, useWeakReference:Boolean = false):void
		public native function close():void
		public native function receive(blockUntilReceived:Boolean = false):*
		override public native function removeEventListener(type:String, listener:Function, useCapture:Boolean = false):void
		public native function send(arg:*, queueLimit:int = -1):void
		public native function toString():String
	}
}