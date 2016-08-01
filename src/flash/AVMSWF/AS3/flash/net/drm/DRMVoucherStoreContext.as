package flash.net.drm
{
[native(cls="DRMVoucherStoreContextClass", methods="auto")]
    class DRMVoucherStoreContext extends DRMManagerSession
    {


        function DRMVoucherStoreContext()
        {
            return;
        }// end function

        public function getVoucherFromStore(inMetadata:DRMContentData) : void
        {
            var _loc_2:uint = 0;
            if (!m_isInSession)
            {
                m_isInSession = true;
                metadata = inMetadata;
                _loc_2 = this.getVoucherFromStoreInner(metadata);
                if (_loc_2 != 0)
                {
                    m_isInSession = false;
                    errorCodeToThrow(_loc_2);
                }
                else
                {
                    setTimerUp();
                }
            }
            return;
        }// end function

        public function get voucher() : DRMVoucher
        {
            return this.getVoucherInner();
        }// end function

        override public function onSessionComplete() : void
        {
            issueDRMStatusEvent(metadata, this.getVoucherInner());
            return;
        }// end function

        override public function onSessionError() : void
        {
            issueDRMErrorEvent(metadata, getLastError(), getLastSubErrorID());
            return;
        }// end function

        private native function getVoucherFromStoreInner(data:DRMContentData) : uint;

        private native function getVoucherInner() : DRMVoucher;

    }
}
