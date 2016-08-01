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
 	[native(cls="PointClass", methods="auto")]

 	public class Point
 	{
 		//Properties
 		 public var x:Number=0;
 		 public var y:Number=0;
 		 public native function get length() : Number 

		 
 		//Methods
 		 public function Point(newX:Number = 0, newY:Number = 0) 
 		 {
 			x=newX;
 			y=newY;
 		 }

 		 public function add(v:Point):Point 
 		 {
 			return new Point(x+v.x,y+v.y);
 		 }
 		 public function clone():Point 
 		 {
 			return new Point(x,y);
 		 }
 		 public native static function distance(pt1:Point, pt2:Point):Number 
 		 public function equals(pt:Point):Boolean 
 		 {
 			return x==pt.x&&y==pt.y;
 		 }
 		 public static function interpolate(pt1:Point, pt2:Point, f:Number):Point 
 		 {
 			var x:Number=pt2.x+(pt1.x-pt2.x)*f;
 			var y:Number=pt2.y+(pt1.y-pt2.y)*f;
 			return new Point(x,y);
 		 }

 		 public native function normalize(thickness:Number):void 
 		 public function offset(dx:Number, dy:Number):void 
 		 {
 			x+=dx;
 			y+=dy;
 		 }
 		 public native static function polar(len:Number, angle:Number):Point 
 		 public function subtract(v:Point):Point 
 		 {
 			return new Point(x-v.x,y-v.y);
 		 }
 		 public native function toString():String 
 		 /*{
 			var s:String="(x=";
 			s+=String(x);
 			s+=",y=";
 			s+=String(y);
 			s+=")";
 			return s;
 		 }*/
 		 
 		 //new adds 11
 		 public native function copyFrom(sourcePoint:Point):void
 		 public native function setTo(xa:Number, ya:Number):void
 		 //new adds 11 end
 	}
 }
