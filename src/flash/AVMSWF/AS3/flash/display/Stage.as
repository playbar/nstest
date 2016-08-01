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
 	[native(cls="StageClass", methods="auto")]

	import flash.events.*
	import flash.display.*

	import flash.text.TextSnapshot
	import flash.geom.Rectangle
	import flash.media.StageVideo

 	public class Stage extends  DisplayObjectContainer
 	{
 		//Properties
 		 public override native function get loaderInfo():LoaderInfo
 		 public override native function get root() : DisplayObject
 		 public native function get align() : String 
		 public native function set align(newAlign : String): void

		 public native function get displayState() : String 
		 public native function set displayState(newDisplayState : String): void

		 public native function get focus() : InteractiveObject 
		 public native function set focus(newFocus : InteractiveObject): void

		 public native function get frameRate() : Number 
		 public native function set frameRate(newFrameRate : Number): void

//		 public native function get height() : Number 
		 public override native function get height() : Number
		 public override native function set height(newHeight : Number): void

		 public override native function get mouseChildren() : Boolean 
		 public override native function set mouseChildren(newMouseChildren : Boolean): void

		 public override native function get numChildren() : int 

		 public native function get quality() : String 
		 public native function set quality(newQuality : String): void

		 public native function get scaleMode() : String 
		 public native function set scaleMode(newScaleMode : String): void

		 public native function get showDefaultContextMenu() : Boolean 
		 public native function set showDefaultContextMenu(newShowDefaultContextMenu : Boolean): void

		 public native function get stageFocusRect() : Boolean 
		 public native function set stageFocusRect(newStageFocusRect : Boolean): void

		 public native function get stageHeight() : int 
		 public native function set stageHeight(newStageHeight : int): void

		 public native function get stageWidth() : int 
		 public native function set stageWidth(newStageWidth : int): void

		 public override native function get tabChildren() : Boolean 
		 public override native function set tabChildren(newTabChildren : Boolean): void

		 public override native function get textSnapshot() : TextSnapshot 

		 public override native function get width() : Number 
		 public override native function set width(newWidth : Number): void

		 //new adds
		 public native function get allowsFullScreen() : Boolean

		 public native function get autoOrients() : Boolean
		 public native function set autoOrients(autoOrients : Boolean):void

		 public native function get color() : uint
		 public native function set color(color : uint):void

		 public native function get colorCorrection() : String
		 public native function set colorCorrection(colorCorrection : String):void

		 public native function get colorCorrectionSupport() : String

		 public native function get deviceOrientation() : String

		 public native function get fullScreenHeight() : uint

		 public native function get fullScreenSourceRect() : Rectangle
		 public native function set fullScreenSourceRect(fullScreenSourceRect : Rectangle):void

		 public native function get fullScreenWidth() : uint

		 public native function get nativeWindow() : NativeWindow

		 public native function get orientation() : String

		 public native function get softKeyboardRect() : Rectangle

		 public native function get stageVideos() : Vector.<StageVideo>

		 public native function get supportedOrientations() : Vector.<String>

		 public native static function get supportsOrientationChange() : Boolean

		 public native function get wmodeGPU() : Boolean
		 //new adds end
		 
		 //new adds 11
		 public native function get allowsFullScreenInteractive():Boolean
		 
		 public native function get constructor():*
		 public native function set constructor(value:*):void
		 
		 public native function get contentsScaleFactor():Number
		 
		 public native function get mouseLock():Boolean
		 public native function set mouseLock(value:Boolean):void
		 
		 public native function get stage3Ds():Vector.<Stage3D>
		 //new adds 11 end

 		//Methods
 		 public native override function addChild(child:DisplayObject):DisplayObject 
 		 public native override function addChildAt(child:DisplayObject, index:int):DisplayObject 
 		 public native override function addEventListener(type:String, listener:Function, useCapture:Boolean = false, priority:int = 0, useWeakReference:Boolean = false):void 
 		 public native override function dispatchEvent(event:Event):Boolean 
 		 public native override function hasEventListener(type:String):Boolean 

 		 public native function invalidate():void 
 		 public native function isFocusInaccessible():Boolean 
 		 public native override function removeChildAt(index:int):DisplayObject 
 		 public native override function setChildIndex(child:DisplayObject, index:int):void 
 		 public native override function swapChildrenAt(index1:int, index2:int):void 

 		 public native override function willTrigger(type:String):Boolean 

		 //new adds
		 public native function assignFocus(objectToFocus:InteractiveObject, direction:String):void
		 public native function setAspectRatio(newAspectRatio:String):void
		 public native function setOrientation(newOrientation:String):void
		 //new adds end
 	}
 }
