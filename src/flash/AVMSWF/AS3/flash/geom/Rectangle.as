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

 package flash.geom
 {
 	[native(cls="RectangleClass", methods="auto")]

	import flash.geom.Point

 	public class Rectangle
 	{
 		//Properties
 		 public var x:Number=0;
 		 public var y:Number=0;
 		 public var width:Number=0;
 		 public var height:Number=0;
 		 
 		 public function get bottom() : Number 
 		 {
 			return y+height;
 		 }
		 public function set bottom(newBottom : Number)
		 {
			height=newBottom-y;
		 }

		 public function get bottomRight() : Point 
		 {
			return new Point(x+width,y+height);
		 }
		 public function set bottomRight(pt : Point)
		 {
			width=pt.x-x;
			height=pt.y-y;
		 }

		 //public function get height() : Number 
		 //{	return bottom-top; }
		 //public function set height(newHeight : Number)
		 //{
		//	bottom=top+newHeight;
		 //}

		 public function get left() : Number 
		 {
			return x;
		 }
		 public function set left(newLeft : Number)
		 {
			x=newLeft;
		 }

		 public function get right() : Number 
		 {
			return x+width;
		 }
		 public function set right(newRight : Number)
		 {
			width=newRight-x;
		 }

		 public function get size() : Point 
		 {
			return new Point(width,height);
		 }
		 public function set size(newSize : Point)
		 {
			width=newSize.x;
			height=newSize.y;
		 }

		 public function get top() : Number 
		 {
			return y;
		 }
		 public function set top(newTop : Number)
		 {
			y=newTop;
		 }

		 public function get topLeft() : Point 
		 {
			return new Point(x,y);
		 }
		 public function set topLeft(pt : Point)
		 {
			x=pt.x;
			y=pt.y;
		 }

		 //public function get width() : Number 
		 //{
		//	return right-left;
		// }
		// public function set width(newWidth : Number)
		 //{
		//	right=left+newWidth;
		 //}

		 //public function get x() : Number 
		 //{
		//	return left;
		 //}
		 //public function set x(newX : Number)
		 //{
		//	left=newX;
		// }

		 //public function get y() : Number 
		 //{
		//	return top;
		 //}
		 //public function set y(newY : Number)
		 //{
		//	top=newY;
		 //}

 		//Methods
 		 public function Rectangle(nx:Number = 0, ny:Number = 0, nwidth:Number = 0, nheight:Number = 0) 
 		 {
 			 x=nx;
 			 y=ny;
 			 width=nwidth;
 			 height=nheight;
 		 }

 		 public function clone():Rectangle 
 		 {
 			return new Rectangle(x,y,width,height);
 		 }
 		 public function contains(px:Number, py:Number):Boolean 
 		 {
 			return px>=x&&px<=x+width&&py>=y&&py<=y+height;
 		 }
 		 public function containsPoint(point:Point):Boolean 
 		 {
 			return point.x>=x&&point.x<=x+width&&point.y>=y&&point.y<=y+height;
 		 }
 		 public native function containsRect(rect:Rectangle):Boolean  		  		 
 		 public function equals(rect:Rectangle):Boolean 
 		 {
 			return x==rect.x&&y==rect.y&&width==rect.width&&height==rect.height;
 		 }
 		
 		 public function inflate(dx:Number, dy:Number):void 
 		 {
 			x-=dx;
 			width+=dx*2;
 			y-=dy;
 			height+=dy*2;
 		 }
 		 public function inflatePoint(point:Point):void 
 		 {
 			x-=point.x;
 			y-=point.y;
 			width+=point.x*2;
 			height+=point.y*2;
 		 }
 		 public native function intersection(toIntersect:Rectangle):Rectangle 
 		 public native function intersects(toIntersect:Rectangle):Boolean 
 		 public function isEmpty():Boolean 
 		 {
 			return height==0&&width==0;
 		 }
 		
 		 public function offset(dx:Number, dy:Number):void 
 		 {
 			x+=dx;
 			y+=dy;
 		 }
 		 public function offsetPoint(point:Point):void 
 		 {
 			x+=point.x;
 			y+=point.y;
 		 }
 		 public function setEmpty():void 
 		 {
 			width=0;
 			height=0;
 		 }
 		 public native function toString():String;
 		 /*{
 			var str:String="(x=";
 			str+=String(x);
 			str+=",y=";
 			str+=String(y);
 			str+=",w=";
 			str+=String(width);
 			str+=",h=";
 			str+=String(height);
 			str+=")";
 			return str;
 		 }*/
 		 public native function union(toUnion:Rectangle):Rectangle 	
 		 
 		 //new adds 11
 		 public native function setTo(xa:Number, ya:Number, widtha:Number, heighta:Number):void
 		 //new adds 11 end
 		 	
 	}
 }
