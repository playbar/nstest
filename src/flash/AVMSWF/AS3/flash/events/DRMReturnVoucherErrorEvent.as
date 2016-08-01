package flash.events
{
	[native(cls="DRMReturnVoucherErrorEventClass", methods="auto")]
	
	import flash.events.Event
	import flash.events.ErrorEvent
	import flash.events.TextEvent
	
	public class DRMReturnVoucherErrorEvent extends ErrorEvent
	{
		//const
		public static const RETURN_VOUCHER_ERROR:String = "returnVoucherError"
		
		//Properties
		//private m_licenseID:String;
		//private m_policyID:String;
		//private m_serverURL:String;
		//private m_subErrorID:int;
		public native function get licenseID():String
		public native function set licenseID(value:String):void
	    
		public native function get policyID():String
		public native function set policyID(value:String):void
	    
		public native function get serverURL():String
		public native function set serverURL(value:String):void
	    
		public native function get subErrorID():int
		public native function set subErrorID(value:int):void
		
		//Methods
		
		public function DRMReturnVoucherErrorEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, inDetail:String = "", inErrorID:int = 0, inSubErrorID:int = 0, inServerURL:String = null, inLicenseID:String = null, inPolicyID:String = null)
		{
			super ( type, bubbles, cancelable, inDetail, inErrorID );
			this.licenseID = inLicenseID;
			this.policyID  = inPolicyID;
			this.serverURL = inServerURL;
			this.subErrorID = inSubErrorID;
			return;
		}
		override public native function clone():Event
	}
}