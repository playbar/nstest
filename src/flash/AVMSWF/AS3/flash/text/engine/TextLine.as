package flash.text.engine
{
[native(cls="TextLineClass", methods="auto")]

import flash.events.*
import flash.geom.*
import flash.display.*
import flash.ui.*
import flash.net.drm

public class TextLine extends DisplayObjectContainer
{
	//Consts
//	public static const MAX_LINE_WIDTH : int = 1000000
	
	//Properties
//	public native function get ascent() : Number
	
//	public native function get atomCount() : int
	
//	public native function get descent() : Number
	
//	public native function get hasGraphicElement() : Boolean
	
//	public native function get hasTabs() : Boolean
	
//	public native function get mirrorRegions() : Vector.<TextLineMirrorRegion>
	
//	public native function get nextLine() : TextLine
	
//	public native function get previousLine() : TextLine
	
//	public native function get rawTextLength() : int
	
//	public native function get specifiedWidth() : Number
	
//	public native function get textBlock() : TextBlock
	
//	public native function get textBlockBeginIndex() : int
	
//	public native function get textHeight() : Number
	
//	public native function get textWidth() : Number

//	public native function get totalAscent() : Number
	
//	public native function get totalDescent() : Number
	
//	public native function get totalHeight() : Number
	
//	public native function get unjustifiedTextWidth() : Number
	
//	public native function get userData() : *
//	public native function set userData(userData : *):void
	
//	public native function get validity() : String
//	public native function set validity(validity : String):void
	
	//Methods
//	public native function dump():String
//	public native function flushAtomData():void
//	public native function getAtomBidiLevel(atomIndex:int):int
//	public native function getAtomBounds(atomIndex:int):Rectangle
//	public native function getAtomCenter(atomIndex:int):Number
//	public native function getAtomGraphic(atomIndex:int):DisplayObject
//	public native function getAtomIndexAtCharIndex(charIndex:int):int
//	public native function getAtomIndexAtPoint(stageX:Number, stageY:Number):int
//	public native function getAtomTextBlockBeginIndex(atomIndex:int):int
//	public native function getAtomTextBlockEndIndex(atomIndex:int):int
//	public native function getAtomTextRotation(atomIndex:int):String
//	public native function getAtomWordBoundaryOnLeft(atomIndex:int):Boolean
//	public native function getBaselinePosition(baseline:String):Number
//	public native function getMirrorRegion(mirror:EventDispatcher):TextLineMirrorRegion
        public var userData:Object;
        public static const MAX_LINE_WIDTH:int = 1000000;

        public function TextLine()
        {
            return;
        }// end function

        override public function set focusRect(focusRect:Object) : void
        {
            Error.throwError(IllegalOperationError, 2181);
            return;
        }// end function

        override public function set tabChildren(enable:Boolean) : void
        {
            Error.throwError(IllegalOperationError, 2181);
            return;
        }// end function

        override public function set tabEnabled(enabled:Boolean) : void
        {
            Error.throwError(IllegalOperationError, 2181);
            return;
        }// end function

        override public function set tabIndex(index:int) : void
        {
            Error.throwError(IllegalOperationError, 2181);
            return;
        }// end function

        public native function get textBlock() : TextBlock;

        public native function get hasGraphicElement() : Boolean;

        public native function get hasTabs() : Boolean;

        public native function get nextLine() : TextLine;

        public native function get previousLine() : TextLine;

        public native function get ascent() : Number;

        public native function get descent() : Number;

        public native function get textHeight() : Number;

        public native function get textWidth() : Number;

        public native function get totalAscent() : Number;

        public native function get totalDescent() : Number;

        public native function get totalHeight() : Number;

        public native function get textBlockBeginIndex() : int;

        public native function get rawTextLength() : int;

        public native function get specifiedWidth() : Number;

        public native function get unjustifiedTextWidth() : Number;

        public native function get validity() : String;

        public native function set validity(value:String) : void;

        public native function get atomCount() : int;

        public native function get mirrorRegions() : Vector.<TextLineMirrorRegion>;

        public function getMirrorRegion(mirror:EventDispatcher) : TextLineMirrorRegion
        {
            var _loc_4:TextLineMirrorRegion = null;
            var _loc_2:* = this.mirrorRegions;
            var _loc_3:int = 0;
            while (_loc_3 < _loc_2.length)
            {
                
                _loc_4 = _loc_2[_loc_3];
                if (_loc_4.mirror == mirror)
                {
                    return _loc_4;
                }
                _loc_3 = _loc_3 + 1;
            }
            return null;
        }// end function

        public function flushAtomData() : void
        {
            return;
        }// end function

        public native function getAtomIndexAtPoint(stageX:Number, stageY:Number) : int;

        public native function getAtomIndexAtCharIndex(charIndex:int) : int;

        public native function getAtomBounds(atomIndex:int) : Rectangle;

        public native function getAtomBidiLevel(atomIndex:int) : int;

        public native function getAtomTextRotation(atomIndex:int) : String;

        public native function getAtomTextBlockBeginIndex(atomIndex:int) : int;

        public native function getAtomTextBlockEndIndex(atomIndex:int) : int;

        public native function getAtomCenter(atomIndex:int) : Number;

        public native function getAtomWordBoundaryOnLeft(atomIndex:int) : Boolean;

        public native function getAtomGraphic(atomIndex:int) : DisplayObject;

        public native function getBaselinePosition(baseline:String) : Number;

        public native function dump() : String;

        override public function set contextMenu(value:ContextMenu) : void
        {
            Error.throwError(IllegalOperationError, 2181);
            return;
        }// end function
}
}