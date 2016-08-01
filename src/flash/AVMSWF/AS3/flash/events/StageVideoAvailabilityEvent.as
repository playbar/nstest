package flash.events
{
[native(cls="StageVideoAvailabilityEventClass", methods="auto")]

public class StageVideoAvailabilityEvent extends Event
{
	//Consts
	public static const STAGE_VIDEO_AVAILABILITY : String = "stageVideoAvailability"
	
	//Properties
//	public native function get availability() : String
	
	//Methods
//	public native function StageVideoAvailabilityEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, availability:String = null)

        private var m_availability:String;

        public function StageVideoAvailabilityEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, availability:String = null)
        {
            super(type, bubbles, cancelable);
            this.m_availability = availability;
            return;
        }// end function

        public function get availability() : String
        {
            return this.m_availability;
        }// end function
}
}