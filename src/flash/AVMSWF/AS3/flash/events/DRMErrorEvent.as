package flash.events
{
[native(cls="DRMErrorEventClass", methods="auto")]

import flash.net.drm.*

public class DRMErrorEvent extends ErrorEvent
{
	//Consts
	public static const DRM_ERROR : String = "drmError"
	
	//Properties
//	public native function get contentData() : DRMContentData
//	public native function set contentData(contentData : DRMContentData):void
	
//	public native function get drmUpdateNeeded() : Boolean
	
//	public native function get subErrorID() : int
	
//	public native function get systemUpdateNeeded() : Boolean
	
	//Methods
//	public native function DRMErrorEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, inErrorDetail:String = "", inErrorCode:int = 0, insubErrorID:int = 0, inMetadata:DRMContentData = null, inSystemUpdateNeeded:Boolean = false, inDrmUpdateNeeded:Boolean = false)
//	public native override function clone():Event
//	public native override function toString():String
        private var m_metadata:DRMContentData;
        private var m_subErrorID:int;
        private var m_systemUpdateNeeded:Boolean;
        private var m_drmUpdateNeeded:Boolean;

        public static const DRM_LOAD_DEVICEID_ERROR:String = "drmLoadDeviceIdError";
	
	public function DRMErrorEvent(type:String = "drmError", bubbles:Boolean = false, cancelable:Boolean = false, inErrorDetail:String = "", inErrorCode:int = 0, insubErrorID:int = 0, inMetadata:DRMContentData = null, inSystemUpdateNeeded:Boolean = false, inDrmUpdateNeeded:Boolean = false)
        {
            super(type, bubbles, cancelable, inErrorDetail, inErrorCode);
            this.m_subErrorID = insubErrorID;
            this.m_metadata = inMetadata;
            this.m_systemUpdateNeeded = inSystemUpdateNeeded;
            this.m_drmUpdateNeeded = inDrmUpdateNeeded;
            return;
        }// end function

        override public function clone() : Event
        {
            return new DRMErrorEvent(type, bubbles, cancelable, text, errorID, this.subErrorID, this.m_metadata, this.m_systemUpdateNeeded, this.m_drmUpdateNeeded);
        }// end function

        override public function toString() : String
        {
            return formatToString("DRMErrorEvent", "type", "bubbles", "cancelable", "eventPhase", "errorID", "subErrorID", "text", "systemUpdateNeeded", "drmUpdateNeeded");
        }// end function

        public function get subErrorID() : int
        {
            return this.m_subErrorID;
        }// end function

        public function get contentData() : DRMContentData
        {
            return this.m_metadata;
        }// end function

        public function set contentData(value:DRMContentData) : void
        {
            this.m_metadata = value;
            return;
        }// end function

        public function get systemUpdateNeeded() : Boolean
        {
            return this.m_systemUpdateNeeded;
        }// end function

        public function get drmUpdateNeeded() : Boolean
        {
            return this.m_drmUpdateNeeded;
        }// end function
}
}