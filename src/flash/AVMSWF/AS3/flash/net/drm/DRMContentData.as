package flash.net.drm
{
[native(cls="DRMContentDataClass", methods="auto")]

import flash.utils.ByteArray

import flash.net.drm.*

public class DRMContentData
{
	//Consts
	
	//Properties
//	public native function get authenticationMethod() : String
	
//	public native function get domain() : String
	
//	public native function get licenseID() : String
	
//	public native function get serverURL() : String
	
	//Methods
//	public native function DRMContentData(rawData:ByteArray = null)
//	public native function getVoucherAccessInfo():Vector.<VoucherAccessInfo>

        private var m_licenseID:String;
        private var m_domain:String;
        private var m_voucherAccessInfo:Vector.<VoucherAccessInfo>;
        private static var m_internalOnly:ByteArray = new ByteArray();

        public function DRMContentData(rawData:ByteArray = null)
        {
            var _loc_2:* = this.setRawMetadataInner(rawData);
            if (_loc_2 != 0)
            {
                this.errorCodeToThrow(_loc_2);
            }
            return;
        }// end function

        public function get serverURL() : String
        {
            return this.FMRMSURL;
        }// end function

        private native function get FMRMSURL() : String;

        public native function get authenticationMethod() : String;

        public function get licenseID() : String
        {
            if (this.m_licenseID == null)
            {
                this.m_licenseID = this.getLicenseIDInner();
            }
            return this.m_licenseID;
        }// end function

        public function get domain() : String
        {
            if (this.m_domain == null)
            {
                this.m_domain = this.getDomainInner();
            }
            return this.m_domain;
        }// end function

        public function getVoucherAccessInfo() : Vector.<VoucherAccessInfo>
        {
            if (this.m_voucherAccessInfo == null)
            {
                this.populateVoucherAccessInfo();
            }
            return this.m_voucherAccessInfo;
        }// end function

        private native function getLicenseIDInner() : String;

        private native function getDomainInner() : String;

        private native function setRawMetadataInner(inRawData:ByteArray) : uint;

        private native function errorCodeToThrow(errorCode:uint) : void;

        private native function populateVoucherAccessInfo() : void;

        private function addVoucherAccessInfo(newVoucherAccessInfo:VoucherAccessInfo) : void
        {
            if (this.m_voucherAccessInfo == null)
            {
                this.m_voucherAccessInfo = new Vector.<VoucherAccessInfo>;
            }
            this.m_voucherAccessInfo.push(newVoucherAccessInfo);
            return;
        }// end function
}
}