package flash.events
{
[native(cls="DRMAuthenticationCompleteEventClass", methods="auto")]

import flash.utils.*

public class DRMAuthenticationCompleteEvent extends Event
{
	//Consts
	public static const AUTHENTICATION_COMPLETE : String = "authenticationComplete"
	
	//Properties
//	public native function get domain() : String
//	public native function set domain(domain : String):void
	
//	public native function get serverURL() : String
//	public native function set serverURL(serverURL : String):void
	
//	public native function get token() : ByteArray
//	public native function set token(token : ByteArray):void
	
	//Methods
//	public native function DRMAuthenticationCompleteEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, inServerURL:String = null, inDomain:String = null, inToken:ByteArray = null)
//	public native override function clone():Event
        private var m_serverURL:String;
        private var m_domain:String;
        private var m_token:ByteArray;
        public function DRMAuthenticationCompleteEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, inServerURL:String = null, inDomain:String = null, inToken:ByteArray = null)
        {
            super(type, bubbles, cancelable);
            this.m_serverURL = inServerURL;
            this.m_domain = inDomain;
            this.m_token = inToken;
            return;
        }// end function

        override public function clone() : Event
        {
            return new DRMAuthenticationCompleteEvent(type, bubbles, cancelable, this.serverURL, this.domain, this.token);
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

        public function get token() : ByteArray
        {
            return this.m_token;
        }// end function

        public function set token(value:ByteArray) : void
        {
            this.m_token = value;
            return;
        }// end function
	
}
}