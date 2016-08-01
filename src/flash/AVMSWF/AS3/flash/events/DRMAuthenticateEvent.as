package flash.events
{
[native(cls="DRMAuthenticateEventClass", methods="auto")]

import flash.net.NetStream
//import flash.utils.ByteArray

public class DRMAuthenticateEvent extends Event
{
	//Consts
	public static const AUTHENTICATION_TYPE_DRM : String = "drm"
	public static const AUTHENTICATION_TYPE_PROXY : String = "proxy"
	public static const DRM_AUTHENTICATE : String = "drmAuthenticate"
	
	//Properties
//	public native function get authenticationType() : String
	
//	public native function get header() : String
	
//	public native function get netstream() : NetStream
	
//	public native function get passwordPrompt() : String
	
//	public native function get urlPrompt() : String
	
//	public native function get usernamePrompt() : String
	
	//Methods
//	public native function DRMAuthenticateEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, header:String = "", userPrompt:String = "", passPrompt:String = "", urlPrompt:String = "", authenticationType:String = "", netstream:NetStream = null)
//	public native override function clone():Event
//	public native override function toString():String
        private var m_header:String;
        private var m_userPrompt:String;
        private var m_passPrompt:String;
        private var m_urlPrompt:String;
        private var m_authenticationType:String;
        private var m_netstream:NetStream;
	
	public function DRMAuthenticateEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, header:String = "", userPrompt:String = "", passPrompt:String = "", urlPrompt:String = "", authenticationType:String = "", netstream:NetStream = null)
        {
            super(type, bubbles, cancelable);
            this.m_header = header;
            this.m_userPrompt = userPrompt;
            this.m_passPrompt = passPrompt;
            this.m_urlPrompt = urlPrompt;
            this.m_authenticationType = authenticationType;
            this.m_netstream = netstream;
            return;
        }// end function

        override public function clone() : Event
        {
            return new DRMAuthenticateEvent(type, bubbles, cancelable, this.m_header, this.m_userPrompt, this.m_passPrompt, this.m_urlPrompt, this.m_authenticationType, this.m_netstream);
        }// end function

        override public function toString() : String
        {
            return formatToString("DRMAuthenticateEvent", "type", "bubbles", "cancelable", "eventPhase", "header", "usernamePrompt", "passwordPrompt", "urlPrompt", "authenticationType");
        }// end function

        public function get header() : String
        {
            return this.m_header;
        }// end function

        public function get usernamePrompt() : String
        {
            return this.m_userPrompt;
        }// end function

        public function get passwordPrompt() : String
        {
            return this.m_passPrompt;
        }// end function

        public function get urlPrompt() : String
        {
            return this.m_urlPrompt;
        }// end function

        public function get authenticationType() : String
        {
            return this.m_authenticationType;
        }// end function

        public function get netstream() : NetStream
        {
            return this.m_netstream;
        }// end function
}
}