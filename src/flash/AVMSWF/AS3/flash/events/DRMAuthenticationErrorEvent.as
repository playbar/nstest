package flash.events
{
[native(cls="DRMAuthenticationErrorEventClass", methods="auto")]

public class DRMAuthenticationErrorEvent extends ErrorEvent
{
	//Consts
	public static const AUTHENTICATION_ERROR : String = "authenticationError"
	
	//Properties
//	public native function get domain() : String
//	public native function set domain(domain : String):void
	
//	public native function get serverURL() : String
//	public native function set serverURL(serverURL : String):void
	
//	public native function get subErrorID() : int
//	public native function set subErrorID(subErrorID : int):void
	
	//Methods
//	public native function DRMAuthenticationErrorEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, inDetail:String = "", inErrorID:int = 0, inSubErrorID:int = 0, inServerURL:String = null, inDomain:String = null)
//	public native override function clone():Event
        private var m_subErrorID:int;
        private var m_serverURL:String;
        private var m_domain:String;
        public function DRMAuthenticationErrorEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, inDetail:String = "", inErrorID:int = 0, inSubErrorID:int = 0, inServerURL:String = null, inDomain:String = null)
        {
            super(type, bubbles, cancelable, inDetail, inErrorID);
            this.m_subErrorID = inSubErrorID;
            this.m_serverURL = inServerURL;
            this.m_domain = inDomain;
            return;
        }// end function

        override public function clone() : Event
        {
            return new DRMAuthenticationErrorEvent(type, bubbles, cancelable, text, errorID, this.subErrorID, this.serverURL, this.domain);
        }// end function

        public function get subErrorID() : int
        {
            return this.m_subErrorID;
        }// end function

        public function set subErrorID(value:int) : void
        {
            this.m_subErrorID = value;
            return;
        }// end function

        public function get serverURL() : String
        {
            return this.m_serverURL;
        }// end function

        public function set serverURL(value:String) : void
        {
            this.m_serverURL = value;
            return;
        }// end function

        public function get domain() : String
        {
            return this.m_domain;
        }// end function

        public function set domain(value:String) : void
        {
            this.m_domain = value;
            return;
        }// end function
}
}