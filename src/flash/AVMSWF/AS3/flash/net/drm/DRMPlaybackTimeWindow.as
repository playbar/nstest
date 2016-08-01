package flash.net.drm
{
[native(cls="DRMPlaybackTimeWindowClass", methods="auto")]

public class DRMPlaybackTimeWindow
{
	//Consts
	
	//Properties
//	public native function get endDate() : Date
	
//	public native function get period() : uint
	
//	public native function get startDate() : Date
	
	//Methods
        private var m_length:uint;
        private var m_startDate:Date;
        private var m_endDate:Date;

        public function DRMPlaybackTimeWindow()
        {
            return;
        }// end function

        private function init(length:Number, start:Number, end:Number) : void
        {
            this.m_length = length;
            if (start > 0)
            {
                this.m_startDate = new Date(start * 1000);
            }
            if (end > 0)
            {
                this.m_endDate = new Date(end * 1000);
            }
            return;
        }// end function

        public function get period() : uint
        {
            return this.m_length;
        }// end function

        public function get startDate() : Date
        {
            return this.m_startDate;
        }// end function

        public function get endDate() : Date
        {
            return this.m_endDate;
        }// end function
}
}