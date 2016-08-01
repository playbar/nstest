package flash.events
{
[native(cls="StageVideoEventClass", methods="auto")]

public class StageVideoEvent extends Event
{
	//Consts
	public static const RENDER_STATE : String = "renderState"
	public static const RENDER_STATUS_ACCELERATED : String = "accelerated"
	public static const RENDER_STATUS_SOFTWARE : String = "software"
	public static const RENDER_STATUS_UNAVAILABLE : String = "unavailable"	
	
	//Properties
//	public native function get colorSpace() : String
	
//	public native function get status() : String
	
	//Methods
//	public native function StageVideoEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, status:String = null, colorSpace:String = null)

        private var m_status:String;
        private var m_colorSpace:String;
        public const codecInfo:String;
	
	public function StageVideoEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, status:String = null, colorSpace:String = null)
        {
            super(type, bubbles, cancelable);
            this.m_status = status;
            this.m_colorSpace = colorSpace;
            return;
        }// end function

        public function get status() : String
        {
            return this.m_status;
        }// end function

        public function get colorSpace() : String
        {
            return this.m_colorSpace;
        }// end function
}
}