package flash.system
{
	[native(cls="WorkerDomainClass",methods="auto")]
	
	import flash.system.Worker
	import flash.utils.ByteArray
	
	public final class WorkerDomain
	{
		//Const
		

		//Properties
		public static native function get current():WorkerDomain
		public static native function get isSupported():Boolean
		//Methods
		
		public native function createWorker(swf:ByteArray, giveAppPrivileges:Boolean = false):Worker
		public native function listWorkers():Vector.<Worker>
	}
}