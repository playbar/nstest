package flash.system
{
	[native(cls="WorkerClass",methods="auto")]
	
	import flash.events.EventDispatcher
	import flash.system.WorkerState
	import flash.system.WorkerDomain
	
	public final class Worker extends EventDispatcher
	{
		//const
		
		//Properties
		
		public static native function get current():Worker
		public native function get isPrimordial():Boolean
		public static native function get isSupported():Boolean
		public native function get state():String
		
		//Methods
		
		override public native function addEventListener(type:String, listener:Function, useCapture:Boolean = false, priority:int = 0, useWeakReference:Boolean = false):void
		public native function createMessageChannel(receiver:Worker):MessageChannel
		public native function getSharedProperty(key:String):*
		override public native function removeEventListener(type:String, listener:Function, useCapture:Boolean = false):void
		public native function setSharedProperty(key:String, value:*):void
		public native function start():void
		public native function terminate():Boolean
	}
}