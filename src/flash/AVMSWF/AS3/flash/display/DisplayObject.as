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
 * Portions created by the Initial Developer are Copyright (C) 2008
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
 	[native(cls="DisplayObjectClass", methods="auto")]

	import flash.events.EventDispatcher

	import flash.accessibility.AccessibilityProperties
	import flash.geom.Rectangle
	import flash.geom.Transform
	import flash.geom.Point
	import flash.display.LoaderInfo;
//	import Boolean			//toplevel

	import flash.geom.Matrix
	import flash.geom.Vector3D

 	public class DisplayObject extends  EventDispatcher implements IBitmapDrawable
 	{
 		 //
 		 static public function hookUIInit():Function
 		 {
 			return function(obj:Sprite=null):void
 			{
 				try
 				{
 					if(obj)
 					{
 						obj.drawNow();
 					}
 				}
 				catch(e)
 				{
 				}
 			}
 		 }
 		//Properties
 		 public native function get accessibilityProperties() : AccessibilityProperties
		 public native function set accessibilityProperties(newAccessibilityProperties : AccessibilityProperties): void

		 public native function get alpha() : Number
		 public native function set alpha(newAlpha : Number) : void;

		 public native function get blendMode() : String
		 public native function set blendMode(newBlendMode : String): void

		 public native function get cacheAsBitmap() : Boolean
		 public native function set cacheAsBitmap(newCacheAsBitmap : Boolean): void

		 public native function get filters() : Array
		 public native function set filters(newFilters : Array): void

		 public native function get height() : Number
		 public native function set height(newHeight : Number): void

		 public native function get loaderInfo() : LoaderInfo

		 public native function get mask() : DisplayObject
		 public native function set mask(newMask : DisplayObject): void

		 public native function get mouseX() : Number

		 public native function get mouseY() : Number

		 public native function get name() : String
		 public native function set name(newName : String): void

		 public native function get opaqueBackground() : Object
		 public native function set opaqueBackground(newOpaqueBackground : Object): void

		 public native function get parent() : DisplayObjectContainer

		 public native function get root() : DisplayObject

		 public native function get rotation() : Number
		 public native function set rotation(newRotation : Number): void

		 public native function get scale9Grid() : Rectangle
		 public native function set scale9Grid(newScale9Grid : Rectangle): void

		 public native function get scaleX() : Number
		 public native function set scaleX(newScaleX : Number) : void

		 public native function get scaleY() : Number
		 public native function set scaleY(newScaleY : Number): void

		 public native function get scrollRect() : Rectangle
		 public native function set scrollRect(newScrollRect : Rectangle): void

		 public native function get stage() : Stage

		 public native function get transform() : Transform
		 public native function set transform(newTransform : Transform): void

		 public native function get visible() : Boolean
		 public native function set visible(newVisible : Boolean): void

		 public native function get width() : Number
		 public native function set width(newWidth : Number): void

		 public native function get x() : Number
		 public native function set x(newX : Number): void

		 public native function get y() : Number
		 public native function set y(newY : Number): void

		 //new adds
		 public native function set blendShader(blendShader : Shader):void

		 public native function get cacheAsBitmapMatrix() : Matrix
		 public native function set cacheAsBitmapMatrix(cacheAsBitmapMatrix : Matrix):void

		 public native function get rotationX() : Number
		 public native function set rotationX(rotationX : Number):void

		 public native function get rotationY() : Number
		 public native function set rotationY(rotationY : Number):void

		 public native function get rotationZ() : Number
		 public native function set rotationZ(rotationZ : Number):void

		 public native function get scaleZ() : Number
		 public native function set scaleZ(scaleZ : Number):void

		 public native function get z() : Number
		 public native function set z(z : Number):void
		 //new adds end

 		//Methods
 		 public native function getBounds(targetCoordinateSpace:DisplayObject):Rectangle
 		 public native function getRect(targetCoordinateSpace:DisplayObject):Rectangle
 		 public native function globalToLocal(point:Point):Point
 		 public native function hitTestObject(obj:DisplayObject):Boolean
 		 public native function hitTestPoint(x:Number, y:Number, shapeFlag:Boolean = false):Boolean
 		 public native function localToGlobal(point:Point):Point
 		 public native function DisplayObject();
 		 public native function drawNow():void;
 		 
		 //new adds
		 public native function globalToLocal3D(point:Point):Vector3D
		 public native function local3DToGlobal(point3d:Vector3D):Point
		 //new adds end
		 
		 //new adds 11
		 public native function get metaData():Object
		 public native function set metaData(value:Object):void
		 //new adds 11 end
 	}
 }
