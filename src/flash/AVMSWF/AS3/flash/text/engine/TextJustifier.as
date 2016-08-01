package flash.text.engine
{
[native(cls="TextJustifierClass", methods="auto")]

import flash.utils.*

public class TextJustifier
{
	//Consts
	
	//Properties
	public native function get lineJustification() : String
	public native function set lineJustification(lineJustification : String):void
	
	public native function get locale() : String
	private native function setLocale(param1:String) : void;
	
	//Methods
//	public native function TextJustifier(locale:String, lineJustification:String)
//	public native function clone():TextJustifier
//	public native static function getJustifierForLocale(locale:String):TextJustifier
        public function TextJustifier(locale:String, lineJustification:String)
        {
            if (getQualifiedClassName(this) == "flash.text.engine::TextJustifier")
            {
                Error.throwError(ArgumentError, 2012, "TextJustifier");
            }
            this.setLocale(locale);
            this.lineJustification = lineJustification;
            return;
        }// end function
	
	public function clone() : TextJustifier
        {
            return null;
        }// end function

        public static function getJustifierForLocale(locale:String) : TextJustifier
        {
            if (locale != null)
            {
	            if (locale.length < 2)
	            {
	                Error.throwError(ArgumentError, 2004, "previousLine");
	            }            
            }
            var _loc_2:* = locale.substr(0, 2);
            if (_loc_2 != "zh")
            {
	            if (_loc_2 != "ja")
	            {
		            if (_loc_2 == "ko")
		            {
		                return new EastAsianJustifier(locale);
		            }            
	            }            
            }
            return new SpaceJustifier(locale);
        }// end function
}
}