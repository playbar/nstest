package flash.net.drm
{
[native(cls="DRMManagerClass", methods="auto")]

import flash.events.*
import flash.geom.*
import flash.utils.*

public class DRMManager extends EventDispatcher
{
	//Consts
	
	//Properties
//	public native static function get isSupported() : Boolean
	
	//Methods
//	public native function authenticate(serverURL:String, domain:String, username:String, password:String):void
//	public native static function getDRMManager():DRMManager
//	public native function loadPreviewVoucher(contentData:DRMContentData):void
//	public native function loadVoucher(contentData:DRMContentData, setting:String):void
//	public native function resetDRMVouchers():void
//	public native function setAuthenticationToken(serverUrl:String, domain:String, token:ByteArray):void
        private static var theManager:DRMManager = null;

        public function DRMManager()
        {
            return;
        }// end function

        public function authenticate(serverURL:String, domain:String, username:String, password:String) : void
        {
            var _loc_5:* = new DRMAuthenticationContext();
            _loc_5.addEventListener(DRMAuthenticationCompleteEvent.AUTHENTICATION_COMPLETE, this.onAuthenticationComplete);
            _loc_5.addEventListener(DRMAuthenticationErrorEvent.AUTHENTICATION_ERROR, this.onAuthenticationError);
            _loc_5.authenticate(serverURL, domain, username, password);
            return;
        }// end function

        public function setAuthenticationToken(serverUrl:String, domain:String, token:ByteArray) : void
        {
            var _loc_4:* = this.setSAMLTokenInner(serverUrl, domain, token);
            if (_loc_4 != 0)
            {
                this.errorCodeToThrow(_loc_4);
            }
            return;
        }// end function

        public function loadVoucher(contentData:DRMContentData, setting:String) : void
        {
            var _loc_3:DRMVoucherStoreContext = null;
            if (contentData == null)
            {
                throw new ArgumentError();
            }
            if (setting == LoadVoucherSetting.FORCE_REFRESH)
            {
                this.downloadVoucher(contentData, false);
            }
            else if (setting == LoadVoucherSetting.LOCAL_ONLY)
            {
                _loc_3 = new DRMVoucherStoreContext();
                _loc_3.addEventListener(DRMStatusEvent.DRM_STATUS, this.onGetVoucherFromStoreComplete);
                _loc_3.addEventListener(DRMErrorEvent.DRM_ERROR, this.onGetVoucherFromStoreError);
                _loc_3.getVoucherFromStore(contentData);
            }
            else if (setting == LoadVoucherSetting.ALLOW_SERVER)
            {
                _loc_3 = new DRMVoucherStoreContext();
                _loc_3.addEventListener(DRMStatusEvent.DRM_STATUS, this.onGetVoucherFromStoreWithAllowServerComplete);
                _loc_3.addEventListener(DRMErrorEvent.DRM_ERROR, this.onGetVoucherFromStoreWithAllowServerError);
                _loc_3.getVoucherFromStore(contentData);
            }
            else
            {
                throw new ArgumentError();
            }
            return;
        }// end function

        public function loadPreviewVoucher(contentData:DRMContentData) : void
        {
            if (contentData == null)
            {
                throw new ArgumentError();
            }
            this.downloadVoucher(contentData, true);
            return;
        }// end function

        private function downloadVoucher(contentData:DRMContentData, previewVoucher:Boolean = false) : void
        {
            var _loc_3:* = new DRMVoucherDownloadContext();
            _loc_3.addEventListener(DRMStatusEvent.DRM_STATUS, this.onDownloadVoucherComplete);
            _loc_3.addEventListener(DRMErrorEvent.DRM_ERROR, this.onDownloadVoucherError);
            _loc_3.download(contentData, previewVoucher);
            return;
        }// end function

        public function storeVoucher(voucher:ByteArray) : void
        {
            var _loc_2:* = this.storeVoucherInner(voucher);
            if (_loc_2 != 0)
            {
                this.errorCodeToThrow(_loc_2);
            }
            return;
        }// end function

        private native function setSAMLTokenInner(serverUrl:String, domain:String, token:ByteArray) : uint;

        private function onAuthenticationComplete(event:DRMAuthenticationCompleteEvent) : void
        {
            var _loc_2:* = event.clone();
            dispatchEvent(_loc_2);
            var _loc_3:* = event.target as DRMAuthenticationContext;
            _loc_3.removeEventListener(DRMAuthenticationCompleteEvent.AUTHENTICATION_COMPLETE, this.onAuthenticationComplete);
            _loc_3.removeEventListener(DRMAuthenticationErrorEvent.AUTHENTICATION_ERROR, this.onAuthenticationError);
            return;
        }// end function

        private function onAuthenticationError(event:DRMAuthenticationErrorEvent) : void
        {
            var _loc_2:* = event.clone();
            dispatchEvent(_loc_2);
            var _loc_3:* = event.target as DRMAuthenticationContext;
            _loc_3.removeEventListener(DRMAuthenticationCompleteEvent.AUTHENTICATION_COMPLETE, this.onAuthenticationComplete);
            _loc_3.removeEventListener(DRMAuthenticationErrorEvent.AUTHENTICATION_ERROR, this.onAuthenticationError);
            return;
        }// end function

        private function onGetVoucherFromStoreComplete(event:DRMStatusEvent) : void
        {
            this.issueDRMStatusEvent(event.type, event.contentData, event.voucher, true);
            var _loc_2:* = event.target as DRMVoucherStoreContext;
            _loc_2.removeEventListener(DRMStatusEvent.DRM_STATUS, this.onGetVoucherFromStoreComplete);
            _loc_2.removeEventListener(DRMErrorEvent.DRM_ERROR, this.onDownloadVoucherError);
            return;
        }// end function

        private function onGetVoucherFromStoreError(event:DRMErrorEvent) : void
        {
            this.issueDRMErrorEvent(event.contentData, event.errorID, event.subErrorID);
            var _loc_2:* = event.target as DRMVoucherStoreContext;
            _loc_2.removeEventListener(DRMStatusEvent.DRM_STATUS, this.onGetVoucherFromStoreComplete);
            _loc_2.removeEventListener(DRMErrorEvent.DRM_ERROR, this.onDownloadVoucherError);
            return;
        }// end function

        private function onDownloadVoucherComplete(event:DRMStatusEvent) : void
        {
            this.issueDRMStatusEvent(event.type, event.contentData, event.voucher, false);
            var _loc_2:* = event.target as DRMVoucherDownloadContext;
            _loc_2.removeEventListener(DRMStatusEvent.DRM_STATUS, this.onDownloadVoucherComplete);
            _loc_2.removeEventListener(DRMErrorEvent.DRM_ERROR, this.onDownloadVoucherError);
            return;
        }// end function

        private function onGetVoucherFromStoreWithAllowServerComplete(event:DRMStatusEvent) : void
        {
            if (event.voucher != null)
            {
                this.issueDRMStatusEvent(event.type, event.contentData, event.voucher, true);
            }
            else
            {
                this.downloadVoucher(event.contentData);
            }
            var _loc_2:* = event.target as DRMVoucherStoreContext;
            _loc_2.removeEventListener(DRMStatusEvent.DRM_STATUS, this.onGetVoucherFromStoreWithAllowServerComplete);
            _loc_2.removeEventListener(DRMErrorEvent.DRM_ERROR, this.onGetVoucherFromStoreWithAllowServerError);
            return;
        }// end function

        private function onDownloadVoucherError(event:DRMErrorEvent) : void
        {
            this.issueDRMErrorEvent(event.contentData, event.errorID, event.subErrorID);
            var _loc_2:* = event.target as DRMVoucherDownloadContext;
            _loc_2.removeEventListener(DRMStatusEvent.DRM_STATUS, this.onDownloadVoucherComplete);
            _loc_2.removeEventListener(DRMErrorEvent.DRM_ERROR, this.onDownloadVoucherError);
            return;
        }// end function

        private function onGetVoucherFromStoreWithAllowServerError(event:DRMErrorEvent) : void
        {
            this.downloadVoucher(event.contentData);
            var _loc_2:* = event.target as DRMVoucherStoreContext;
            _loc_2.removeEventListener(DRMStatusEvent.DRM_STATUS, this.onGetVoucherFromStoreWithAllowServerComplete);
            _loc_2.removeEventListener(DRMErrorEvent.DRM_ERROR, this.onGetVoucherFromStoreWithAllowServerError);
            return;
        }// end function

        private native function errorCodeToThrow(errorCode:uint) : void;

        private native function issueDRMStatusEvent(specificEventType:String, metadata:DRMContentData, voucher:DRMVoucher, isLocal:Boolean) : void;

        private native function issueDRMErrorEvent(metadata:DRMContentData, errorID:int, subErrorID:int, eventType:String = null) : void;

        private native function storeVoucherInner(voucher:ByteArray) : uint;

        public static function getDRMManager() : DRMManager
        {
            checkRemoteSWFInvocation();
            if (theManager == null)
            {
                theManager = createDRMManager();
            }
            return theManager;
        }// end function

        private native static function createDRMManager() : DRMManager;

        public static function get isSupported() : Boolean
        {
            return true;
        }// end function

        private static function checkRemoteSWFInvocation() : void
        {
            if (isCalledFromRemoteSWF() == true)
            {
                throw new SecurityError();
            }
            return;
        }// end function

        private native static function isCalledFromRemoteSWF() : Boolean;
        
        //new adds 11
        public native static function get networkIdleTimeout():Number
		public native static function set networkIdleTimeout(value:Number):void
		
		public native function addToDeviceGroup(deviceGroup:DRMDeviceGroup, forceRefresh:Boolean = false):void
		public native function removeFromDeviceGroup(deviceGroup:DRMDeviceGroup):void
		public native function resetDRMVouchers():void
		public native function returnVoucher(inServerURL:String, immediateCommit:Boolean, licenseID:String, policyID:String):void
        public native function StoreVoucher(voucher:ByteArray):void
        //new adds 11 end
}
}