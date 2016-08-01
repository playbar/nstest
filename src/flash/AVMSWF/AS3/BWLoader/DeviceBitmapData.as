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

 package BWLoader
 {
 
 	[native(cls="BitmapDataClass",instance="BitmapDataObject",methods="auto")]
	import flash.display.DisplayObject;
	import flash.geom.Rectangle;
	import flash.geom.Point;
	import flash.geom.Matrix;
	import flash.geom.ColorTransform;
	import flash.display.BitmapData;
	import flash.utils.ByteArray;
	import flash.filters.BitmapFilter;

 	public class BXDeviceBitmapData extends BitmapData
 	{
 	
 		 static public native function GetDeviceBitmapObj(disp:DisplayObject):BXDeviceBitmapData
 		 static public native function GetDeviceBitmap(bx:int,by:int,w:int,h:int):BXDeviceBitmapData
 		//Properties
 		 static public native function isMobile():Boolean
 		
 		 override public native function get height() : int

 		 override public native function get rect() : Rectangle

 		 override public function get transparent() : Boolean
 		 {return false;}

 		 override public native function get width() : int

 		//Methods

 		 function BXDeviceBitmapData(width:int, height:int, transparent:Boolean = true, fillColor:uint = 0xFFFFFFFF)
 		 {
 			Error.throwError(IllegalOperationError, 2181);
 		 }

 		 override public native function applyFilter(sourceBitmapData:BitmapData, sourceRect:Rectangle, destPoint:Point, filter:BitmapFilter):void
 		 override public native function clone():BitmapData
 		 override public native function colorTransform(rect:Rectangle, colorTransform:ColorTransform):void
 		 override public native function compare(otherBitmapData:BitmapData):Object
 		 override public native function copyChannel(sourceBitmapData:BitmapData, sourceRect:Rectangle, destPoint:Point, sourceChannel:uint, destChannel:uint):void

 		 override public native function copyPixels(sourceBitmapData:BitmapData, sourceRect:Rectangle, destPoint:Point, alphaBitmapData:BitmapData = null, alphaPoint:Point = null, mergeAlpha:Boolean = false):void
 		 override public native function dispose():void
 		 override public native function draw(source:Object, matrix:Matrix = null, colorTransform:ColorTransform = null, blendMode:String = null, clipRect:Rectangle = null, smoothing:Boolean = false):void
 		 override public native function fillRect(rect:Rectangle, color:uint):void
 		 override public native function floodFill(x:int, y:int, color:uint):void

 		 override public native function generateFilterRect(sourceRect:Rectangle, filter:BitmapFilter):Rectangle
 		 override public native function getColorBoundsRect(mask:uint, color:uint, findColor:Boolean = true):Rectangle
 		 override public native function getPixel(x:int, y:int):uint
 		 override public native function getPixel32(x:int, y:int):uint
 		 override public native function getPixels(rect:Rectangle):ByteArray

 		 override public native function hitTest(firstPoint:Point, firstAlphaThreshold:uint, secondObject:Object, secondBitmapDataPoint:Point = null, secondAlphaThreshold:uint = 1):Boolean
 		 override public native function lock():void
 		 override public native function merge(sourceBitmapData:BitmapData, sourceRect:Rectangle, destPoint:Point, redMultiplier:uint, greenMultiplier:uint, blueMultiplier:uint, alphaMultiplier:uint):void
 		 override public native function noise(randomSeed:int, low:uint = 0, high:uint = 255, channelOptions:uint = 7, grayScale:Boolean = false):void
 		 override public native function paletteMap(sourceBitmapData:BitmapData, sourceRect:Rectangle, destPoint:Point, redArray:Array = null, greenArray:Array = null, blueArray:Array = null, alphaArray:Array = null):void

 		 override public native function perlinNoise(baseX:Number, baseY:Number, numOctaves:uint, randomSeed:int, stitch:Boolean, fractalNoise:Boolean, channelOptions:uint = 7, grayScale:Boolean = false, offsets:Array = null):void
 		 override public native function pixelDissolve(sourceBitmapData:BitmapData, sourceRect:Rectangle, destPoint:Point, randomSeed:int = 0, numPixels:int = 0, fillColor:uint = 0):int
 		 override public native function scroll(x:int, y:int):void
 		 override public native function setPixel(x:int, y:int, color:uint):void

 		 override public native function setPixel32(x:int, y:int, color:uint):void
 		 override public native function setPixels(rect:Rectangle, inputByteArray:ByteArray):void
 		 override public native function threshold(sourceBitmapData:BitmapData, sourceRect:Rectangle, destPoint:Point, operation:String, threshold:uint, color:uint = 0, mask:uint = 0xFFFFFFFF, copySource:Boolean = false):uint
 		 override public native function unlock(changeRect:Rectangle = null):void

		 //new adds
		 override public native function getVector(rect:Rectangle):Vector.<uint>
		 override public native function histogram(hRect:Rectangle = null):Vector.<Vector.<Number>>
		 override public native function setVector(rect:Rectangle, inputVector:Vector.<uint>):void
		 //new adds end

 	}
 }