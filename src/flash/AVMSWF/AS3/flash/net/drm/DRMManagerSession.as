package flash.net.drm
{
[native(cls="DRMManagerSessionClass", methods="auto")]
    import flash.events.*;
    import flash.utils.*;

    class DRMManagerSession extends EventDispatcher
    {
        private var m_metadata:DRMContentData;
        private var m_checkStatusTimer:Timer;
        public var m_isInSession:Boolean;
        static const STATUS_READY:uint = 0;
        static const STATUS_NOTREADY:uint = 1;
        static const STATUS_FAILED:uint = 2;
        static const STATUS_UNKNOWN:uint = 3;

        function DRMManagerSession()
        {
            this.m_isInSession = false;
            return;
        }// end function

        public function onSessionError() : void
        {
            return;
        }// end function

        public function onSessionComplete() : void
        {
            return;
        }// end function

        public function setTimerUp() : void
        {
            if (this.m_checkStatusTimer == null)
            {
                this.m_checkStatusTimer = new Timer(100, 0);
            }
            this.m_checkStatusTimer.addEventListener(TimerEvent.TIMER, this.onCheckStatus);
            this.m_checkStatusTimer.start();
            return;
        }// end function

        public function get metadata() : DRMContentData
        {
            return this.m_metadata;
        }// end function

        public function set metadata(inData:DRMContentData) : void
        {
            this.m_metadata = inData;
            return;
        }// end function

        public function checkStatus() : uint
        {
            return this.checkStatusInner();
        }// end function

        private function onCheckStatus(event:TimerEvent)
        {
            var _loc_2:* = this.checkStatus();
            if (_loc_2 != STATUS_NOTREADY)
            {
                this.m_isInSession = false;
                this.m_checkStatusTimer.stop();
                if (_loc_2 == STATUS_READY)
                {
                    this.onSessionComplete();
                }
                else
                {
                    this.onSessionError();
                }
                this.m_checkStatusTimer.removeEventListener(TimerEvent.TIMER, this.onCheckStatus);
                this.m_checkStatusTimer = null;
            }
            return;
        }// end function

        public native function getLastError() : uint;

        public native function getLastSubErrorID() : uint;

        public function issueDRMStatusEvent(inMetadata:DRMContentData, voucher:DRMVoucher)
        {
            this.issueDRMStatusEventInner(DRMStatusEvent.DRM_STATUS, inMetadata, voucher, false);
            return;
        }// end function

        private native function issueDRMStatusEventInner(specificEventType:String, metadata:DRMContentData, voucher:DRMVoucher, isLocal:Boolean) : void;

        public native function issueDRMErrorEvent(metadata:DRMContentData, errorID:int, subErrorID:int, eventType:String = null) : void;

        public native function errorCodeToThrow(errorCode:uint) : void;

        private native function checkStatusInner() : uint;

    }
}
