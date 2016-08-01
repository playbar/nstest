package flash.text
{
	[native(cls="StageTextClass",methods="auto")]
	
	import flash.events.EventDispatcher
	import flash.text.AutoCapitalize
	import flash.text.engine.FontPosture
	import flash.text.engine.FontWeight
	import flash.text.StageTextInitOptions
	import flash.text.ReturnKeyLabel
	import flash.text.SoftKeyboardType
	import flash.text.TextFormatAlign
	import flash.geom.Rectangle
	import flash.display.BitmapData
	import flash.display.Stage
	import flash.*
	
	public final class StageText extends EventDispatcher
	{
		//Const
		
		//Properties
		
		public native function get autoCapitalize():String
		public native function set autoCapitalize(value:String):void
		
		public native function get autoCorrect():Boolean
		public native function set autoCorrect(value:Boolean):void
		
		public native function get color():uint
		public native function set color(value:uint):void
    
		public native function get displayAsPassword():Boolean
		public native function set displayAsPassword(value:Boolean):void
    
        public native function get editable():Boolean
		public native function set editable(value:Boolean):void
    
		public native function get fontFamily():String
		public native function set fontFamily(value:String):void
	    
		public native function get fontPosture():String
		public native function set fontPosture(value:String):void
	    
		public native function get fontSize():int
		public native function set fontSize(value:int):void
	    
		public native function get fontWeight():String
		public native function set fontWeight(value:String):void
	    
		public native function get locale():String
		public native function set locale(value:String):void
	    
		public native function get maxChars():int
		public native function set maxChars(value:int):void
	    
		public native function get multiline():Boolean
	    
		public native function get restrict():String
		public native function set restrict(value:String):void
	    
		public native function get returnKeyLabel():String
		public native function set returnKeyLabel(value:String):void
	    
		public native function get selectionActiveIndex():int
	    
		public native function get selectionAnchorIndex():int
	    
		public native function get softKeyboardType():String
		public native function set softKeyboardType(value:String):void
	    
		public native function get stage():Stage
		public native function set stage(value:Stage):void
	    
		public native function get text():String
		public native function set text(value:String):void
	    
		public native function get textAlign():String
		public native function set textAlign(value:String):void
	    
		public native function get viewPort():Rectangle
		public native function set viewPort(value:Rectangle):void
	    
		public native function get visible():Boolean
		public native function set visible(value:Boolean):void
    
		//Methods
		
		public function StageText(initOptions:flash.text.StageTextInitOptions = null)
		{
			super(initOptions);
			return;
		}
		
		
		public native function assignFocus():void
		public native function dispose():void
		public native function drawViewPortToBitmapData(bitmap:BitmapData):void
		public native function selectRange(anchorIndex:int, activeIndex:int):void
		
	}
}