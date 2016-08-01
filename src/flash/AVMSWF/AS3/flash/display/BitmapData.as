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

 package flash.display
 {
 
 	[native(cls="BitmapDataClass",instance="BitmapDataObject",methods="auto")]

	import flash.geom.Rectangle;
	import flash.geom.Point;
	import flash.geom.Matrix;
	import flash.geom.ColorTransform;
	import flash.display.BitmapData;
	import flash.utils.ByteArray;
	import flash.filters.BitmapFilter;

 	public class BitmapData implements IBitmapDrawable
 	{
 		//Properties
 		 public native function get height() : int

 		 public native function get rect() : Rectangle

 		 public native function get transparent() : Boolean

 		 public native function get width() : int

 		//Methods

 		 public native function BitmapData(width:int, height:int, transparent:Boolean = true, fillColor:uint = 0xFFFFFFFF)

 		 public native function applyFilter(sourceBitmapData:BitmapData, sourceRect:Rectangle, destPoint:Point, filter:BitmapFilter):void
 		 public native function clone():BitmapData
 		 public native function colorTransform(rect:Rectangle, colorTransform:ColorTransform):void
 		 public native function compare(otherBitmapData:BitmapData):Object
 		 public native function copyChannel(sourceBitmapData:BitmapData, sourceRect:Rectangle, destPoint:Point, sourceChannel:uint, destChannel:uint):void

 		 public native function copyPixels(sourceBitmapData:BitmapData, sourceRect:Rectangle, destPoint:Point, alphaBitmapData:BitmapData = null, alphaPoint:Point = null, mergeAlpha:Boolean = false):void
 		 public native function dispose():void
 		 public native function draw(source:Object, matrix:Matrix = null, colorTransform:ColorTransform = null, blendMode:String = null, clipRect:Rectangle = null, smoothing:Boolean = false):void
 		 public native function fillRect(rect:Rectangle, color:uint):void
 		 public native function floodFill(x:int, y:int, color:uint):void

 		 public native function generateFilterRect(sourceRect:Rectangle, filter:BitmapFilter):Rectangle
 		 public native function getColorBoundsRect(mask:uint, color:uint, findColor:Boolean = true):Rectangle
 		 public native function getPixel(x:int, y:int):uint
 		 public native function getPixel32(x:int, y:int):uint
 		 public native function getPixels(rect:Rectangle):ByteArray

 		 public native function hitTest(firstPoint:Point, firstAlphaThreshold:uint, secondObject:Object, secondBitmapDataPoint:Point = null, secondAlphaThreshold:uint = 1):Boolean
 		 public native function lock():void
 		 public native function merge(sourceBitmapData:BitmapData, sourceRect:Rectangle, destPoint:Point, redMultiplier:uint, greenMultiplier:uint, blueMultiplier:uint, alphaMultiplier:uint):void
 		 public native function noise(randomSeed:int, low:uint = 0, high:uint = 255, channelOptions:uint = 7, grayScale:Boolean = false):void
 		 public native function paletteMap(sourceBitmapData:BitmapData, sourceRect:Rectangle, destPoint:Point, redArray:Array = null, greenArray:Array = null, blueArray:Array = null, alphaArray:Array = null):void

 		 public native function perlinNoise(baseX:Number, baseY:Number, numOctaves:uint, randomSeed:int, stitch:Boolean, fractalNoise:Boolean, channelOptions:uint = 7, grayScale:Boolean = false, offsets:Array = null):void
 		 public native function pixelDissolve(sourceBitmapData:BitmapData, sourceRect:Rectangle, destPoint:Point, randomSeed:int = 0, numPixels:int = 0, fillColor:uint = 0):int
 		 public native function scroll(x:int, y:int):void
 		 public native function setPixel(x:int, y:int, color:uint):void

 		 public native function setPixel32(x:int, y:int, color:uint):void
 		 public native function setPixels(rect:Rectangle, inputByteArray:ByteArray):void
 		 public native function threshold(sourceBitmapData:BitmapData, sourceRect:Rectangle, destPoint:Point, operation:String, threshold:uint, color:uint = 0, mask:uint = 0xFFFFFFFF, copySource:Boolean = false):uint
 		 public native function unlock(changeRect:Rectangle = null):void

		 //new adds
		 public native function getVector(rect:Rectangle):Vector.<uint>
		 public native function histogram(hRect:Rectangle = null):Vector.<Vector.<Number>>
		 public native function setVector(rect:Rectangle, inputVector:Vector.<uint>):void
		 //new adds end
		 
		 //new adds 11
		 public native function copyPixelsToByteArray(rect:Rectangle, data:ByteArray):void
		 public native function drawWithQuality(source:IBitmapDrawable, matrix:Matrix = null, colorTransform:flash.geom.ColorTransform = null, blendMode:String = null, clipRect:Rectangle = null, smoothing:Boolean = false, quality:String = null):void
 		 public native function encode(rect:Rectangle, compressor:Object, byteArray:ByteArray = null):ByteArray
 		 //new adds 11 end
 	}
 }