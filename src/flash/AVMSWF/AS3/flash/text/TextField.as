/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine.].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 2004-2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe  Team
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

 package flash.text
 {
 	[native(cls="TextFieldClass", methods="auto")]

	import flash.events.EventDispatcher
	import flash.display.DisplayObject
	import flash.display.InteractiveObject

	import flash.text.TextFormat
	import flash.text.StyleSheet
	import flash.geom.Rectangle
	import flash.text.TextLineMetrics
	import flash.text.TextFormat
	

 	public class TextField extends  InteractiveObject
 	{
 		//Properties
 		 override public native function get x():Number;
 		 override public native function get y():Number;
 		 override public native function set x(value:Number):void;
 		 override public native function set y(value:Number):void;
 		 override public native function get width():Number;
 		 override public native function set width(value:Number):void;
 		 override public native function get height():Number;
 		 override public native function set height(value:Number):void;
 		
 		 public native function get alwaysShowSelection() : Boolean 
		 public native function set alwaysShowSelection(newAlwaysShowSelection : Boolean): void

		 public native function get antiAliasType() : String 
		 public native function set antiAliasType(newAntiAliasType : String): void

		 public native function get autoSize() : String 
		 public native function set autoSize(newAutoSize : String): void

		 public native function get background() : Boolean 
		 public native function set background(newBackground : Boolean): void

		 public native function get backgroundColor() : uint 
		 public native function set backgroundColor(newBackgroundColor : uint): void

		 public native function get border() : Boolean 
		 public native function set border(newBorder : Boolean): void

		 public native function get borderColor() : uint 
		 public native function set borderColor(newBorderColor : uint): void

		 public native function get bottomScrollV() : int

		 public native function get caretIndex() : int 

		 public native function get condenseWhite() : Boolean 
		 public native function set condenseWhite(newCondenseWhite : Boolean): void

		 public native function get defaultTextFormat() : TextFormat 
		 public native function set defaultTextFormat(newDefaultTextFormat : TextFormat): void

		 public native function get displayAsPassword() : Boolean 
		 public native function set displayAsPassword(newDisplayAsPassword : Boolean): void

		 public native function get embedFonts() : Boolean 
		 public native function set embedFonts(newEmbedFonts : Boolean): void

		 public native function get gridFitType() : String 
		 public native function set gridFitType(newGridFitType : String): void

		 public native function get htmlText() : String 
		 public native function set htmlText(newHtmlText : String): void

		 public native function get length() : int 

		 public native function get maxChars() : int 
		 public native function set maxChars(newMaxChars : int): void

		 public native function get maxScrollH() : int 

		 public native function get maxScrollV() : int 

		 public native function get mouseWheelEnabled() : Boolean 
		 public native function set mouseWheelEnabled(newMouseWheelEnabled : Boolean): void

		 public native function get multiline() : Boolean 
		 public native function set multiline(newMultiline : Boolean): void

		 public native function get numLines() : int 

		 public native function get restrict() : String 
		 public native function set restrict(newRestrict : String): void

		 public native function get scrollH() : int 
		 public native function set scrollH(newScrollH : int): void

		 public native function get scrollV() : int 
		 public native function set scrollV(newScrollV : int): void

		 public native function get selectable() : Boolean 
		 public native function set selectable(newSelectable : Boolean): void

		 public native function get selectionBeginIndex() : int 

		 public native function get selectionEndIndex() : int 

		 public native function get sharpness() : Number 
		 public native function set sharpness(newSharpness : Number): void

		 public native function get styleSheet() : StyleSheet 
		 public native function set styleSheet(newStyleSheet : StyleSheet): void

		 public native function get text() : String 
		 public native function set text(newText : String): void

		 public native function get textColor() : uint 
		 public native function set textColor(newTextColor : uint): void

		 public native function get textHeight() : Number 

		 public native function get textWidth() : Number 

		 public native function get thickness() : Number 
		 public native function set thickness(newThickness : Number): void

		 public native function get type() : String 
		 public native function set type(newType : String): void

		 public native function get useRichTextClipboard() : Boolean 
		 public native function set useRichTextClipboard(newUseRichTextClipboard : Boolean): void

		 public native function get wordWrap() : Boolean 
		 public native function set wordWrap(newWordWrap : Boolean): void

		 //new adds
		 public native function get textInteractionMode() : String
		 //new adds end

 		//Methods
 		 public native function TextField() 

 		 public native function appendText(newText:String):void 
 		 public native function getCharBoundaries(charIndex:int):Rectangle 
 		 public native function getCharIndexAtPoint(x:Number, y:Number):int 
 		 public native function getFirstCharInParagraph(charIndex:int):int 
 		 public native function getImageReference(id:String):DisplayObject 

 		 public native function getLineIndexAtPoint(x:Number, y:Number):int 
 		 public native function getLineIndexOfChar(charIndex:int):int 
 		 public native function getLineLength(lineIndex:int):int 
 		 public native function getLineMetrics(lineIndex:int):TextLineMetrics 
 		 public native function getLineOffset(lineIndex:int):int 

 		 public native function getLineText(lineIndex:int):String 
 		 public native function getParagraphLength(charIndex:int):int 
 		 public native function getTextFormat(beginIndex:int = -1, endIndex:int = -1):TextFormat 
 		 public native function replaceSelectedText(value:String):void 
 		 public native function replaceText(beginIndex:int, endIndex:int, newText:String):void 

 		 public native function setSelection(beginIndex:int, endIndex:int):void 
 		 public native function setTextFormat(format:TextFormat, beginIndex:int = -1, endIndex:int = -1):void
		 
		 //new adds
		 public static native function isFontCompatible(fontName:String, fontStyle:String):Boolean
		 //new adds end 		
 	}
 }
