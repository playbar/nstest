package flash.text.engine
{
[native(cls="SpaceJustifierClass", methods="auto")]

public class SpaceJustifier extends TextJustifier
{
	//Consts
	
	//Properties
	public native function get letterSpacing() : Boolean
	public native function set letterSpacing(letterSpacing : Boolean):void
	
	public native function get maximumSpacing() : Number
	public native function set maximumSpacing(maximumSpacing : Number):void
	
	public native function get minimumSpacing() : Number
	public native function set minimumSpacing(minimumSpacing : Number):void
	
	public native function get optimumSpacing() : Number
	public native function set optimumSpacing(optimumSpacing : Number):void
	
	//Methods
//	public native function SpaceJustifier(locale:String = "en", lineJustification:String = "unjustified", letterSpacing:Boolean = false)
//	public native override function clone():TextJustifier

        public function SpaceJustifier(locale:String = "en", lineJustification:String = "unjustified", letterSpacing:Boolean = false)
        {
            super(locale, lineJustification);
            this.letterSpacing = letterSpacing;
            return;
        }// end function
	
	override public function clone() : TextJustifier
        {
            var _loc_1:* = new SpaceJustifier(this.locale, this.lineJustification, this.letterSpacing);
            _loc_1.cloneSpacing(this);
            return _loc_1;
        }// end function
    private native function cloneSpacing(param1:SpaceJustifier) : void;
}
}