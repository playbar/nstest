package flash.net
{
[native(cls="NetGroupInfoClass", methods="auto")]

public class NetGroupInfo
{
	//Consts
	
	//Properties
//	public native function get objectReplicationReceiveBytesPerSecond() : Number
	
//	public native function get objectReplicationSendBytesPerSecond() : Number
	
//	public native function get postingReceiveControlBytesPerSecond() : Number
	
//	public native function get postingReceiveDataBytesPerSecond() : Number
	
//	public native function get postingSendControlBytesPerSecond() : Number
	
//	public native function get postingSendDataBytesPerSecond() : Number
	
//	public native function get routingReceiveBytesPerSecond() : Number
	
//	public native function get routingSendBytesPerSecond() : Number
	
	//Methods
//	public native function toString():String
        private var m_postingSendDataBytesPerSecond:Number;
        private var m_postingSendControlBytesPerSecond:Number;
        private var m_postingReceiveDataBytesPerSecond:Number;
        private var m_postingReceiveControlBytesPerSecond:Number;
        private var m_routingSendBytesPerSecond:Number;
        private var m_routingReceiveBytesPerSecond:Number;
        private var m_objectReplicationSendBytesPerSecond:Number;
        private var m_objectReplicationReceiveBytesPerSecond:Number;

        public function NetGroupInfo(postingSendDataBytesPerSecond:Number, postingSendControlBytesPerSecond:Number, postingReceiveDataBytesPerSecond:Number, postingReceiveControlBytesPerSecond:Number, routingSendBytesPerSecond:Number, routingReceiveBytesPerSecond:Number, objectReplicationSendBytesPerSecond:Number, objectReplicationReceiveBytesPerSecond:Number)
        {
            this.m_postingSendDataBytesPerSecond = postingSendDataBytesPerSecond;
            this.m_postingSendControlBytesPerSecond = postingSendControlBytesPerSecond;
            this.m_postingReceiveDataBytesPerSecond = postingReceiveDataBytesPerSecond;
            this.m_postingReceiveControlBytesPerSecond = postingReceiveControlBytesPerSecond;
            this.m_routingSendBytesPerSecond = routingSendBytesPerSecond;
            this.m_routingReceiveBytesPerSecond = routingReceiveBytesPerSecond;
            this.m_objectReplicationSendBytesPerSecond = objectReplicationSendBytesPerSecond;
            this.m_objectReplicationReceiveBytesPerSecond = objectReplicationReceiveBytesPerSecond;
            return;
        }// end function

        public function get postingSendDataBytesPerSecond() : Number
        {
            return this.m_postingSendDataBytesPerSecond;
        }// end function

        public function get postingSendControlBytesPerSecond() : Number
        {
            return this.m_postingSendControlBytesPerSecond;
        }// end function

        public function get postingReceiveDataBytesPerSecond() : Number
        {
            return this.m_postingReceiveDataBytesPerSecond;
        }// end function

        public function get postingReceiveControlBytesPerSecond() : Number
        {
            return this.m_postingReceiveControlBytesPerSecond;
        }// end function

        public function get routingSendBytesPerSecond() : Number
        {
            return this.m_routingSendBytesPerSecond;
        }// end function

        public function get routingReceiveBytesPerSecond() : Number
        {
            return this.m_routingReceiveBytesPerSecond;
        }// end function

        public function get objectReplicationSendBytesPerSecond() : Number
        {
            return this.m_objectReplicationSendBytesPerSecond;
        }// end function

        public function get objectReplicationReceiveBytesPerSecond() : Number
        {
            return this.m_objectReplicationReceiveBytesPerSecond;
        }// end function

        public function toString() : String
        {
            return "postingSendDataBytesPerSecond=" + this.m_postingSendDataBytesPerSecond + " postingSendControlBytesPerSecond=" + this.m_postingSendControlBytesPerSecond + " postingReceiveDataBytesPerSecond=" + this.m_postingReceiveDataBytesPerSecond + " postingReceiveControlBytesPerSecond=" + this.m_postingReceiveControlBytesPerSecond + " routingSendBytesPerSecond=" + this.m_routingSendBytesPerSecond + " routingReceiveBytesPerSecond=" + this.m_routingReceiveBytesPerSecond + " objectReplicationSendBytesPerSecond=" + this.m_objectReplicationSendBytesPerSecond + " objectReplicationReceiveBytesPerSecond=" + this.m_objectReplicationReceiveBytesPerSecond;
        }// end function
}
}