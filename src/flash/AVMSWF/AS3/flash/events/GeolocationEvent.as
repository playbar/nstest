package flash.events
{
[native(cls="GeolocationEventClass", methods="auto")]

public class GeolocationEvent extends Event
{
	//Consts
	public static const UPDATE : String = "update"
	
	//Properties
//	public native function get altitude() : Number
//	public native function set altitude(altitude : Number):void
	
//	public native function get heading() : Number
//	public native function set heading(heading : Number):void
	
//	public native function get horizontalAccuracy() : Number
//	public native function set horizontalAccuracy(horizontalAccuracy : Number):void
	
//	public native function get latitude() : Number
//	public native function set latitude(latitude : Number):void
	
//	public native function get longitude() : Number
//	public native function set longitude(longitude : Number):void
	
//	public native function get speed() : Number
//	public native function set speed(speed : Number):void
	
//	public native function get timestamp() : Number
//	public native function set timestamp(timestamp : Number):void
	
//	public native function get verticalAccuracy() : Number
//	public native function set verticalAccuracy(verticalAccuracy : Number):void
	
	//Methods
//	public native function GeolocationEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, latitude:Number = 0, longitude:Number = 0, altitude:Number = 0, hAccuracy:Number = 0, vAccuracy:Number = 0, speed:Number = 0, heading:Number = 0, timestamp:Number = 0)
//	public native override function clone():Event
//	public native override function toString():String
        private var m_latitude:Number;
        private var m_longitude:Number;
        private var m_altitude:Number;
        private var m_horizontalAccuracy:Number;
        private var m_verticalAccuracy:Number;
        private var m_speed:Number;
        private var m_heading:Number;
        private var m_timestamp:Number;

        public function GeolocationEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, latitude:Number = 0, longitude:Number = 0, altitude:Number = 0, hAccuracy:Number = 0, vAccuracy:Number = 0, speed:Number = 0, heading:Number = 0, timestamp:Number = 0)
        {
            super(type, bubbles, cancelable);
            this.m_latitude = latitude;
            this.m_longitude = longitude;
            this.m_altitude = altitude;
            this.m_horizontalAccuracy = hAccuracy;
            this.m_verticalAccuracy = vAccuracy;
            this.m_speed = speed;
            this.m_heading = heading;
            this.m_timestamp = timestamp;
            return;
        }// end function

        override public function clone() : Event
        {
            return new GeolocationEvent(type, bubbles, cancelable, this.m_latitude, this.m_longitude, this.m_altitude, this.m_horizontalAccuracy, this.m_verticalAccuracy, this.m_speed, this.m_heading, this.m_timestamp);
        }// end function

        override public function toString() : String
        {
            return formatToString("GeolocationEvent", "type", "bubbles", "cancelable", "latitude", "longitude", "altitude", "horizontalAccuracy", "verticalAccuracy", "speed", "heading", "timestamp");
        }// end function

        public function get latitude() : Number
        {
            return this.m_latitude;
        }// end function

        public function set latitude(value:Number) : void
        {
            this.m_latitude = value;
            return;
        }// end function

        public function get longitude() : Number
        {
            return this.m_longitude;
        }// end function

        public function set longitude(value:Number) : void
        {
            this.m_longitude = value;
            return;
        }// end function

        public function get altitude() : Number
        {
            return this.m_altitude;
        }// end function

        public function set altitude(value:Number) : void
        {
            this.m_altitude = value;
            return;
        }// end function

        public function get horizontalAccuracy() : Number
        {
            return this.m_horizontalAccuracy;
        }// end function

        public function set horizontalAccuracy(value:Number) : void
        {
            this.m_horizontalAccuracy = value;
            return;
        }// end function

        public function get verticalAccuracy() : Number
        {
            return this.m_verticalAccuracy;
        }// end function

        public function set verticalAccuracy(value:Number) : void
        {
            this.m_verticalAccuracy = value;
            return;
        }// end function

        public function get speed() : Number
        {
            return this.m_speed;
        }// end function

        public function set speed(value:Number) : void
        {
            this.m_speed = value;
            return;
        }// end function

        public function get heading() : Number
        {
            return this.m_heading;
        }// end function

        public function set heading(value:Number) : void
        {
            this.m_heading = value;
            return;
        }// end function

        public function get timestamp() : Number
        {
            return this.m_timestamp;
        }// end function

        public function set timestamp(value:Number) : void
        {
            this.m_timestamp = value;
            return;
        }// end function
}
}