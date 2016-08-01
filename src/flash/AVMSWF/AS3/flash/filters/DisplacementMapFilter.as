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

 package flash.filters
 {
 	[native(cls="DisplacementMapFilterClass", methods="auto")]

	import flash.filters.BitmapFilter

	import flash.display.BitmapData
	import flash.geom.Point

 	public final class DisplacementMapFilter extends  BitmapFilter
 	{
 		//Properties
 		 public var alpha : Number
 		 public var color : uint
 		 public var componentX : uint
 		 public var componentY : uint
 		 public var mapBitmap : BitmapData
 		 public var mapPoint : Point
 		 public var mode : String
 		 public var scaleX : Number
 		 public var scaleY : Number
 		//Methods
 		 public function DisplacementMapFilter(mapBitmap:BitmapData = null, mapPoint:Point = null, 
 											   componentX:uint = 0, componentY:uint = 0, scaleX:Number = 0.0, 
 											   scaleY:Number = 0.0, mode:String = "wrap", color:uint = 0, alpha:Number = 0.0)
 		 {
 			this.mapBitmap=mapBitmap;
 			this.mapPoint=mapPoint;
 			this.componentX=componentX;
 			this.componentY=componentY;
 			this.scaleX=scaleX;
 			this.scaleY=scaleY;
 			this.mode=mode;
 			this.color=color;
 			this.alpha=alpha;
 		 }

 		 public override function clone():BitmapFilter
 		 {
 			return new DisplacementMapFilter(mapBitmap, mapPoint,componentX, componentY, scaleX, 
 											   scaleY, mode, color, alpha)
 		 }
 	}
 }
