package flash.text.engine
{
[native(cls="FontMetricsClass", methods="auto")]

import flash.geom.Rectangle

public class FontMetrics
{
	//Consts
	
	//Properties
//	public native function get emBox() : Rectangle
//	public native function set emBox(emBox : Rectangle):void
	
//	public native function get strikethroughOffset() : Number
//	public native function set strikethroughOffset(strikethroughOffset : Number):void
	
//	public native function get strikethroughThickness() : Number
//	public native function set strikethroughThickness(strikethroughThickness : Number):void
	
//	public native function get subscriptOffset() : Number
//	public native function set subscriptOffset(subscriptOffset : Number):void
	
//	public native function get subscriptScale() : Number
//	public native function set subscriptScale(subscriptScale : Number):void
	
//	public native function get superscriptOffset() : Number
//	public native function set superscriptOffset(superscriptOffset : Number):void
	
//	public native function get superscriptScale() : Number
//	public native function set superscriptScale(superscriptScale : Number):void
	
//	public native function get underlineOffset() : Number
//	public native function set underlineOffset(underlineOffset : Number):void
	
//	public native function get underlineThickness() : Number
//	public native function set underlineThickness(underlineThickness : Number):void
	
	//Methods
//	public native function FontMetrics(emBox:Rectangle, strikethroughOffset:Number, strikethroughThickness:Number, underlineOffset:Number, underlineThickness:Number, subscriptOffset:Number, subscriptScale:Number, superscriptOffset:Number, superscriptScale:Number)

        public var emBox:Rectangle;
        public var strikethroughOffset:Number;
        public var strikethroughThickness:Number;
        public var underlineOffset:Number;
        public var underlineThickness:Number;
        public var subscriptOffset:Number;
        public var subscriptScale:Number;
        public var superscriptOffset:Number;
        public var superscriptScale:Number;
        public var lineGap:Number;

        public function FontMetrics(emBox:Rectangle, strikethroughOffset:Number, strikethroughThickness:Number, underlineOffset:Number, underlineThickness:Number, subscriptOffset:Number, subscriptScale:Number, superscriptOffset:Number, superscriptScale:Number, lineGap:Number = 0)
        {
            this.emBox = emBox;
            this.strikethroughOffset = strikethroughOffset;
            this.strikethroughThickness = strikethroughThickness;
            this.underlineOffset = underlineOffset;
            this.underlineThickness = underlineThickness;
            this.subscriptOffset = subscriptOffset;
            this.subscriptScale = subscriptScale;
            this.superscriptOffset = superscriptOffset;
            this.superscriptScale = superscriptScale;
            this.lineGap = lineGap;
            return;
        }// end function
}
}