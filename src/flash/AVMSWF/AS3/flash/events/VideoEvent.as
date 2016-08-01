package flash.events
{
[native(cls="VideoEventClass", methods="auto")]

public class VideoEvent extends Event
{
	//Consts
	public static const RENDER_STATE : String = "renderState"
	public static const RENDER_STATUS_ACCELERATED : String = "accelerated"
	public static const RENDER_STATUS_SOFTWARE : String = "software"
	public static const RENDER_STATUS_UNAVAILABLE : String = "unavailable"
	
	//Properties
//	public native function get status() : String
	
	//Methods
//	public native function VideoEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, status:String = null)

        private var m_status:String;
        public const codecInfo:String;
	
	public function VideoEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, status:String = null)
        {
            super(type, bubbles, cancelable);
            this.m_status = status;
            return;
        }// end function

        public function get status() : String
        {
            return this.m_status;
        }// end function
}
}