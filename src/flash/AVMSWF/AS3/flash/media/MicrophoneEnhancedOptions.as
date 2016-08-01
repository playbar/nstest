package flash.media
{
[native(cls="MicrophoneEnhancedOptionsClass", methods="auto")]

final public class MicrophoneEnhancedOptions
{
	//Consts
	
	//Properties
//	public native function get echoPath() : int
//	public native function set echoPath(echoPath : int):void
	
//	public native function get isVoiceDetected() : int
	
//	public native function get mode() : String
//	public native function set mode(mode : String):void
	
//	public native function get nonLinearProcessing() : Boolean
//	public native function set nonLinearProcessing(nonLinearProcessing : Boolean):void
	
	//Methods
        private var m_mode:String;
        private var m_echoPath:int;
        private var m_nlp:Boolean;
        private var m_isVoiceDetected:int;

        public function MicrophoneEnhancedOptions()
        {
            this.m_mode = MicrophoneEnhancedMode.FULL_DUPLEX;
            this.m_echoPath = 128;
            this.m_nlp = true;
            this.m_isVoiceDetected = -1;
            return;
        }// end function

        public function get mode() : String
        {
            return this.m_mode;
        }// end function

        public function set mode(mode:String) : void
        {
            if (MicrophoneEnhancedMode.FULL_DUPLEX != mode)
            {
	             if (MicrophoneEnhancedMode.HALF_DUPLEX != mode)
	            {
		            if (MicrophoneEnhancedMode.HEADSET != mode)
		            {
			            if (MicrophoneEnhancedMode.SPEAKER_MUTE != mode)
			            {
				            if (MicrophoneEnhancedMode.OFF == mode)
				            {
				                this.m_mode = mode;
				            }            
			            }            
		            }            
	            }           
            }
            return;
        }// end function

        public function get echoPath() : int
        {
            return this.m_echoPath;
        }// end function

        public function set echoPath(echoPath:int) : void
        {
            if (echoPath != 128)
            {
                   if (echoPath == 256)
	            {
	                this.m_echoPath = echoPath;
	            }
            }
            return;
        }// end function

        public function get nonLinearProcessing() : Boolean
        {
            return this.m_nlp;
        }// end function

        public function set nonLinearProcessing(enabled:Boolean) : void
        {
            this.m_nlp = enabled;
            return;
        }// end function

        public function get autoGain() : Boolean
        {
            return false;
        }// end function

        public function set autoGain(enabled:Boolean) : void
        {
            return;
        }// end function

        public function get isVoiceDetected() : int
        {
            return this.m_isVoiceDetected;
        }// end function

        public function set isVoiceDetected(voiceDetected:int)
        {
            this.m_isVoiceDetected = voiceDetected;
            return;
        }// end function
}
}