package flash.events
{
	[native(cls="DRMReturnVoucherCompleteEventClass", methods="auto")]
	
	import flash.events.event
	
	public class DRMReturnVoucherCompleteEvent extends Event
	{
		//const
		public static const RETURN_VOUCHER_COMPLETE:String = "returnVoucherComplete"
		
		//Properties
		//private m_licenseID:String;
		//private m_numberOfVouchersReturned:int;
		//private m_policyID:String;
		//private m_serverURL:String;
		public native function get licenseID():String
		public native function set licenseID(value:String):void
	    
		public native function get numberOfVouchersReturned():int
		public native function set numberOfVouchersReturned(value:int):void
	    
		public native function get policyID():String
		public native function set policyID(value:String):void
	    
		public native function get serverURL():String
		public native function set serverURL(value:String):void
		
		//Methods
		public function DRMReturnVoucherCompleteEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, inServerURL:String = null, inLicenseID:String = null, inPolicyID:String = null, inNumberOfVouchersReturned:int = 0)
		{
			super( type, bubbles, cancelable );
			this.licenseID                = inLicenseID;
			this.numberOfVouchersReturned = inNumberOfVouchersReturned;
			this.policyID                 = inPolicyID;
			this.serverURL                = inServerURL;
			return;
		}
		
		override native public function clone():Event
	}
}