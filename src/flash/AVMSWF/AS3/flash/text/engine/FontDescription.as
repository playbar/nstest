package flash.text.engine
{
[native(cls="FontDescriptionClass", methods="auto")]

public class FontDescription
{
	//Consts
	
	//Properties
	public native function get cffHinting() : String
	public native function set cffHinting(cffHinting : String):void
	
	public native function get fontLookup() : String
	public native function set fontLookup(fontLookup : String):void
	
	public native function get fontName() : String
	public native function set fontName(fontName : String):void
	
	public native function get fontPosture() : String
	public native function set fontPosture(fontPosture : String):void
	
	public native function get fontWeight() : String
	public native function set fontWeight(fontWeight : String):void
	
	public native function get locked() : Boolean
	public native function set locked(locked : Boolean):void
	
	public native function get renderingMode() : String
	public native function set renderingMode(renderingMode : String):void
	
	//Methods
//	public native function FontDescription(fontName:String = "_serif", fontWeight:String = "normal", fontPosture:String = "normal", fontLookup:String = "device", renderingMode:String = "cff", cffHinting:String = "horizontalStem")
//	public native function clone():FontDescription
	public native static function isDeviceFontCompatible(fontName:String, fontWeight:String, fontPosture:String):Boolean
	public native static function isFontCompatible(fontName:String, fontWeight:String, fontPosture:String):Boolean

        public function FontDescription(fontName:String = "_serif", fontWeight:String = "normal", fontPosture:String = "normal", fontLookup:String = "device", renderingMode:String = "cff", cffHinting:String = "horizontalStem")
        {
            this.fontName = fontName;
            this.fontWeight = fontWeight;
            this.fontPosture = fontPosture;
            this.fontLookup = fontLookup;
            this.renderingMode = renderingMode;
            this.cffHinting = cffHinting;
            return;
        }// end function
	
	public function clone() : FontDescription
        {
            return new FontDescription(this.fontName, this.fontWeight, this.fontPosture, this.fontLookup, this.renderingMode, this.cffHinting);
        }// end function
}
}