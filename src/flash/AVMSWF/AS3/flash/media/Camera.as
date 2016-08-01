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

package flash.media
{
	[native(cls="CameraClass", methods="auto")]

	import flash.events.EventDispatcher
	import flash.display.BitmapData
	import flash.geom.Rectangle
	import flash.utils.ByteArray

	public final class Camera extends  EventDispatcher
	{
		//Properties
		 public native function get activityLevel() : Number
		
		 public native function get bandwidth() : int
		
		 public native function get currentFPS() : Number
		
		 public native function get fps() : Number
		
		 public native function get height() : int
		
		 public native function get index() : int
		
		 public native function get keyFrameInterval() : int
		
		 public native function get loopback() : Boolean
		
		 public native function get motionLevel() : int
		
		 public native function get motionTimeout() : int
		
		 public native function get muted() : Boolean
		
		 public native function get name() : String

		 public native static function get names() : Array

		 public native function get quality() : int
		
		 public native function get width() : int

		 //new adds
		 public static function get isSupported() : Boolean
		 {
			return true;
		 }
		 //new adds end

		//Methods
		 public native static function getCamera(name:String = null):Camera
		 public native function setKeyFrameInterval(keyFrameInterval:int):void
		 public native function setLoopback(compress:Boolean = false):void
		 public native function setMode(width:int, height:int, fps:Number, favorArea:Boolean = true):void
		 public native function setMotionLevel(motionLevel:int, timeout:int = 2000):void
		 public native function setQuality(bandwidth:int, quality:int):void	
		 
		 //new adds 11
		 
		 public native function get position():String
		 public native function copyToByteArray(rect:Rectangle, destination:ByteArray):void
		 public native function copyToVector(rect:Rectangle, destination:Vector.<uint>):void
		 public native function drawToBitmapData(destination:BitmapData):void
		 //new adds 11 end	
	}
}
