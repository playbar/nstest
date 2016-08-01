package flash.net
{
[native(cls="NetStreamMulticastInfoClass", methods="auto")]

public class NetStreamMulticastInfo
{
	//Consts
	
	//Properties
//	public native function get bytesPushedFromPeers() : Number
	
//	public native function get bytesPushedToPeers() : Number
	
//	public native function get bytesReceivedFromIPMulticast() : Number
	
//	public native function get bytesReceivedFromServer() : Number
	
//	public native function get bytesRequestedByPeers() : Number
	
//	public native function get bytesRequestedFromPeers() : Number
	
//	public native function get fragmentsPushedFromPeers() : Number
	
//	public native function get fragmentsPushedToPeers() : Number
	
//	public native function get fragmentsReceivedFromIPMulticast() : Number
	
//	public native function get fragmentsReceivedFromServer() : Number
	
//	public native function get fragmentsRequestedByPeers() : Number
	
//	public native function get fragmentsRequestedFromPeers() : Number
	
//	public native function get receiveControlBytesPerSecond() : Number
	
//	public native function get receiveDataBytesPerSecond() : Number
	
//	public native function get receiveDataBytesPerSecondFromIPMulticast() : Number
	
//	public native function get receiveDataBytesPerSecondFromServer() : Number
	
//	public native function get sendControlBytesPerSecond() : Number
	
//	public native function get sendControlBytesPerSecondToServer() : Number
	
//	public native function get sendDataBytesPerSecond() : Number
	
	//Methods
//	public native function toString():String

        private var m_sendDataBytesPerSecond:Number;
        private var m_sendControlBytesPerSecond:Number;
        private var m_receiveDataBytesPerSecond:Number;
        private var m_receiveControlBytesPerSecond:Number;
        private var m_bytesPushedToPeers:Number;
        private var m_fragmentsPushedToPeers:Number;
        private var m_bytesRequestedByPeers:Number;
        private var m_fragmentsRequestedByPeers:Number;
        private var m_bytesPushedFromPeers:Number;
        private var m_fragmentsPushedFromPeers:Number;
        private var m_bytesRequestedFromPeers:Number;
        private var m_fragmentsRequestedFromPeers:Number;
        private var m_sendControlBytesPerSecondToServer:Number;
        private var m_receiveDataBytesPerSecondFromServer:Number;
        private var m_bytesReceivedFromServer:Number;
        private var m_fragmentsReceivedFromServer:Number;
        private var m_receiveDataBytesPerSecondFromIPMulticast:Number;
        private var m_bytesReceivedFromIPMulticast:Number;
        private var m_fragmentsReceivedFromIPMulticast:Number;

        public function NetStreamMulticastInfo(sendDataBytesPerSecond:Number, sendControlBytesPerSecond:Number, receiveDataBytesPerSecond:Number, receiveControlBytesPerSecond:Number, bytesPushedToPeers:Number, fragmentsPushedToPeers:Number, bytesRequestedByPeers:Number, fragmentsRequestedByPeers:Number, bytesPushedFromPeers:Number, fragmentsPushedFromPeers:Number, bytesRequestedFromPeers:Number, fragmentsRequestedFromPeers:Number, sendControlBytesPerSecondToServer:Number, receiveDataBytesPerSecondFromServer:Number, bytesReceivedFromServer:Number, fragmentsReceivedFromServer:Number, receiveDataBytesPerSecondFromIPMulticast:Number, bytesReceivedFromIPMulticast:Number, fragmentsReceivedFromIPMulticast:Number)
        {
            this.m_sendDataBytesPerSecond = sendDataBytesPerSecond;
            this.m_sendControlBytesPerSecond = sendControlBytesPerSecond;
            this.m_receiveDataBytesPerSecond = receiveDataBytesPerSecond;
            this.m_receiveControlBytesPerSecond = receiveControlBytesPerSecond;
            this.m_bytesPushedToPeers = bytesPushedToPeers;
            this.m_fragmentsPushedToPeers = fragmentsPushedToPeers;
            this.m_bytesRequestedByPeers = bytesRequestedByPeers;
            this.m_fragmentsRequestedByPeers = fragmentsRequestedByPeers;
            this.m_bytesPushedFromPeers = bytesPushedFromPeers;
            this.m_fragmentsPushedFromPeers = fragmentsPushedFromPeers;
            this.m_bytesRequestedFromPeers = bytesRequestedFromPeers;
            this.m_fragmentsRequestedFromPeers = fragmentsRequestedFromPeers;
            this.m_sendControlBytesPerSecondToServer = sendControlBytesPerSecondToServer;
            this.m_receiveDataBytesPerSecondFromServer = receiveDataBytesPerSecondFromServer;
            this.m_bytesReceivedFromServer = bytesReceivedFromServer;
            this.m_fragmentsReceivedFromServer = fragmentsReceivedFromServer;
            this.m_receiveDataBytesPerSecondFromIPMulticast = receiveDataBytesPerSecondFromIPMulticast;
            this.m_bytesReceivedFromIPMulticast = bytesReceivedFromIPMulticast;
            this.m_fragmentsReceivedFromIPMulticast = fragmentsReceivedFromIPMulticast;
            return;
        }// end function

        public function get sendDataBytesPerSecond() : Number
        {
            return this.m_sendDataBytesPerSecond;
        }// end function

        public function get sendControlBytesPerSecond() : Number
        {
            return this.m_sendControlBytesPerSecond;
        }// end function

        public function get receiveDataBytesPerSecond() : Number
        {
            return this.m_receiveDataBytesPerSecond;
        }// end function

        public function get receiveControlBytesPerSecond() : Number
        {
            return this.m_receiveControlBytesPerSecond;
        }// end function

        public function get bytesPushedToPeers() : Number
        {
            return this.m_bytesPushedToPeers;
        }// end function

        public function get fragmentsPushedToPeers() : Number
        {
            return this.m_fragmentsPushedToPeers;
        }// end function

        public function get bytesRequestedByPeers() : Number
        {
            return this.m_bytesRequestedByPeers;
        }// end function

        public function get fragmentsRequestedByPeers() : Number
        {
            return this.m_fragmentsRequestedByPeers;
        }// end function

        public function get bytesPushedFromPeers() : Number
        {
            return this.m_bytesPushedFromPeers;
        }// end function

        public function get fragmentsPushedFromPeers() : Number
        {
            return this.m_fragmentsPushedFromPeers;
        }// end function

        public function get bytesRequestedFromPeers() : Number
        {
            return this.m_bytesRequestedFromPeers;
        }// end function

        public function get fragmentsRequestedFromPeers() : Number
        {
            return this.m_fragmentsRequestedFromPeers;
        }// end function

        public function get sendControlBytesPerSecondToServer() : Number
        {
            return this.m_sendControlBytesPerSecondToServer;
        }// end function

        public function get receiveDataBytesPerSecondFromServer() : Number
        {
            return this.m_receiveDataBytesPerSecondFromServer;
        }// end function

        public function get bytesReceivedFromServer() : Number
        {
            return this.m_bytesReceivedFromServer;
        }// end function

        public function get fragmentsReceivedFromServer() : Number
        {
            return this.m_fragmentsReceivedFromServer;
        }// end function

        public function get receiveDataBytesPerSecondFromIPMulticast() : Number
        {
            return this.m_receiveDataBytesPerSecondFromIPMulticast;
        }// end function

        public function get bytesReceivedFromIPMulticast() : Number
        {
            return this.m_bytesReceivedFromIPMulticast;
        }// end function

        public function get fragmentsReceivedFromIPMulticast() : Number
        {
            return this.m_fragmentsReceivedFromIPMulticast;
        }// end function

        public function toString() : String
        {
            return "sendDataBytesPerSecond=" + this.m_sendDataBytesPerSecond + " sendControlBytesPerSecond=" + this.m_sendControlBytesPerSecond + " receiveDataBytesPerSecond=" + this.m_receiveDataBytesPerSecond + " receiveControlBytesPerSecond=" + this.m_receiveControlBytesPerSecond + " bytesPushedToPeers=" + this.m_bytesPushedToPeers + " fragmentsPushedToPeers=" + this.m_fragmentsPushedToPeers + " bytesRequestedByPeers=" + this.m_bytesRequestedByPeers + " fragmentsRequestedByPeers=" + this.m_fragmentsRequestedByPeers + " bytesPushedFromPeers=" + this.m_bytesPushedFromPeers + " fragmentsPushedFromPeers=" + this.m_fragmentsPushedFromPeers + " bytesRequestedFromPeers=" + this.m_bytesRequestedFromPeers + " fragmentsRequestedFromPeers=" + this.m_fragmentsRequestedFromPeers + " sendControlBytesPerSecondToServer=" + this.m_sendControlBytesPerSecondToServer + " receiveDataBytesPerSecondFromServer=" + this.m_receiveDataBytesPerSecondFromServer + " bytesReceivedFromServer=" + this.m_bytesReceivedFromServer + " fragmentsReceivedFromServer=" + this.m_fragmentsReceivedFromServer + " receiveDataBytesPerSecondFromIPMulticast=" + this.m_receiveDataBytesPerSecondFromIPMulticast + " bytesReceivedFromIPMulticast=" + this.m_bytesReceivedFromIPMulticast + " fragmentsReceivedFromIPMulticast=" + this.m_fragmentsReceivedFromIPMulticast;
        }// end function
}
}