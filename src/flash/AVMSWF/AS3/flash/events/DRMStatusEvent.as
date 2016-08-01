package flash.events
{
[native(cls="DRMStatusEventClass", methods="auto")]

import flash.net.drm.*

public class DRMStatusEvent extends Event
{
	//Consts
	public static const DRM_STATUS : String = "drmStatus"
	
	//Properties
//	public native function get contentData() : DRMContentData
//	public native function set contentData(contentData : DRMContentData):void
	
//	public native function get detail() : String
	
//	public native function get isAnonymous() : Boolean
	
//	public native function get isAvailableOffline() : Boolean
	
//	public native function get isLocal() : Boolean
//	public native function set isLocal(isLocal : Boolean):void
	
//	public native function get offlineLeasePeriod() : uint
	
//	public native function get policies() : Object
	
//	public native function get voucher() : DRMVoucher
//	public native function set voucher(voucher : DRMVoucher):void
	
//	public native function get voucherEndDate() : Date
	
	//Methods
//	public native override function DRMStatusEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, inMetadata:DRMContentData = null, inVoucher:DRMVoucher = null, inLocal:Boolean = false)
//	public native override function clone():Event
//	public native override function toString():String
        private var m_detail:String;
        private var m_voucher:DRMVoucher;
        private var m_metadata:DRMContentData;
        private var m_isLocal:Boolean;
	
	public function DRMStatusEvent(type:String = "drmStatus", bubbles:Boolean = false, cancelable:Boolean = false, inMetadata:DRMContentData = null, inVoucher:DRMVoucher = null, inLocal:Boolean = false)
        {
            super(type, bubbles, cancelable);
            this.m_detail = new String("DRM.voucherObtained");
            this.m_voucher = inVoucher;
            this.m_metadata = inMetadata;
            this.m_isLocal = inLocal;
            return;
        }// end function

        override public function clone() : Event
        {
            return new DRMStatusEvent(type, bubbles, cancelable, this.m_metadata, this.m_voucher, this.m_isLocal);
        }// end function

        override public function toString() : String
        {
            return formatToString("DRMStatusEvent", "type", "bubbles", "cancelable");
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

        public function get voucher() : DRMVoucher
        {
            return this.m_voucher;
        }// end function

        public function set voucher(value:DRMVoucher) : void
        {
            this.m_voucher = value;
            return;
        }// end function

        public function get isLocal() : Boolean
        {
            return this.m_isLocal;
        }// end function

        public function set isLocal(value:Boolean) : void
        {
            this.m_isLocal = value;
            return;
        }// end function
}
}