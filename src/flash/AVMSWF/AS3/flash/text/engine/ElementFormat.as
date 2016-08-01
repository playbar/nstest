package flash.text.engine
{
[native(cls="ElementFormatClass", methods="auto")]

final public class ElementFormat
{
	//Consts
	
	//Properties
	public native function get alignmentBaseline() : String
	public native function set alignmentBaseline(alignmentBaseline : String):void
	
	public native function get alpha() : Number
	public native function set alpha(alpha : Number):void
	
	public native function get baselineShift() : Number
	public native function set baselineShift(baselineShift : Number):void
	
	public native function get breakOpportunity() : String
	public native function set breakOpportunity(breakOpportunity : String):void
	
	public native function get color() : uint
	public native function set color(color : uint):void
	
	public native function get digitCase() : String
	public native function set digitCase(digitCase : String):void
	
	public native function get digitWidth() : String
	public native function set digitWidth(digitWidth : String):void
	
	public native function get dominantBaseline() : String
	public native function set dominantBaseline(dominantBaseline : String):void
	
	public native function get fontDescription() : FontDescription
	public native function set fontDescription(fontDescription : FontDescription):void
	
	public native function get fontSize() : Number
	public native function set fontSize(fontSize : Number):void
	
	public native function get kerning() : String
	public native function set kerning(kerning : String):void
	
	public native function get ligatureLevel() : String
	public native function set ligatureLevel(ligatureLevel : String):void
	
	public native function get locale() : String
	public native function set locale(locale : String):void
	
	public native function get locked() : Boolean
	public native function set locked(locked : Boolean):void
	
	public native function get textRotation() : String
	public native function set textRotation(textRotation : String):void
	
	public native function get trackingLeft() : Number
	public native function set trackingLeft(trackingLeft : Number):void
	
	public native function get trackingRight() : Number
	public native function set trackingRight(trackingRight : Number):void
	
	public native function get typographicCase() : String
	public native function set typographicCase(typographicCase : String):void
	
	//Methods
//	public native function ElementFormat(fontDescription:FontDescription = null, fontSize:Number = 12.0, color:uint = 0x000000, alpha:Number = 1.0, textRotation:String = "auto", dominantBaseline:String = "roman", alignmentBaseline:String = "useDominantBaseline", baselineShift:Number = 0.0, kerning:String = "on", trackingRight:Number = 0.0, trackingLeft:Number = 0.0, locale:String = "en", breakOpportunity:String = "auto", digitCase:String = "default", digitWidth:String = "default", ligatureLevel:String = "common", typographicCase:String = "default")
//	public native function clone():ElementFormat
	public native function getFontMetrics():FontMetrics
        public function ElementFormat(fontDescription:FontDescription = null, fontSize:Number = 12, color:uint = 0, alpha:Number = 1, textRotation:String = "auto", dominantBaseline:String = "roman", alignmentBaseline:String = "useDominantBaseline", baselineShift:Number = 0, kerning:String = "on", trackingRight:Number = 0, trackingLeft:Number = 0, locale:String = "en", breakOpportunity:String = "auto", digitCase:String = "default", digitWidth:String = "default", ligatureLevel:String = "common", typographicCase:String = "default")
        {
            this.fontDescription = fontDescription ? (fontDescription) : (new FontDescription());
            this.fontSize = fontSize;
            this.color = color;
            this.alpha = alpha;
            this.textRotation = textRotation;
            this.dominantBaseline = dominantBaseline;
            this.alignmentBaseline = alignmentBaseline;
            this.baselineShift = baselineShift;
            this.kerning = kerning;
            this.trackingRight = trackingRight;
            this.trackingLeft = trackingLeft;
            this.locale = locale;
            this.breakOpportunity = breakOpportunity;
            this.digitCase = digitCase;
            this.digitWidth = digitWidth;
            this.ligatureLevel = ligatureLevel;
            this.typographicCase = typographicCase;
            return;
        }// end function
	
	public function clone() : ElementFormat
        {
            return new ElementFormat(this.fontDescription ? (this.fontDescription.clone()) : (null), this.fontSize, this.color, this.alpha, this.textRotation, this.dominantBaseline, this.alignmentBaseline, this.baselineShift, this.kerning, this.trackingRight, this.trackingLeft, this.locale, this.breakOpportunity, this.digitCase, this.digitWidth, this.ligatureLevel, this.typographicCase);
        }// end function
}
}