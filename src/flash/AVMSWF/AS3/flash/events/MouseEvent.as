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

 package flash.events
 {
 	[native(cls="MouseEventClass", methods="auto")]

	import flash.events.Event

	import flash.display.InteractiveObject

 	public class MouseEvent extends  Event
 	{
 		//Consts
 		public static const CLICK:String = "click" 
 		public static const DOUBLE_CLICK:String = "doubleClick" 
 		public static const MOUSE_DOWN:String = "mouseDown" 
 		public static const MOUSE_MOVE:String = "mouseMove" 
 		public static const MOUSE_OUT:String = "mouseOut" 
 		
 		public static const MOUSE_OVER:String = "mouseOver" 
 		public static const MOUSE_UP:String = "mouseUp" 
 		public static const MOUSE_WHEEL:String = "mouseWheel" 
 		public static const ROLL_OUT:String = "rollOut" 
 		public static const ROLL_OVER:String = "rollOver" 

		//new adds
		public static const CONTEXT_MENU : String = "contextMenu"
		public static const MIDDLE_CLICK : String = "middleClick"
		public static const MIDDLE_MOUSE_DOWN : String = "middleMouseDown"
		public static const MIDDLE_MOUSE_UP : String = "middleMouseUp"
		public static const RIGHT_CLICK : String = "rightClick"
		public static const RIGHT_MOUSE_DOWN : String = "rightMouseDown"
		public static const RIGHT_MOUSE_UP : String = "rightMouseUp"
		//new adds end

		//new adds 11
		public static const RELEASE_OUTSIDE:String = "releaseOutside"
		
		//new adds 11 end
 		
 		
 		//Properties
 		 public native function get altKey() : Boolean 
		 public native function set altKey(newAltKey : Boolean): void

		 public native function get buttonDown() : Boolean 
		 public native function set buttonDown(newButtonDown : Boolean): void

		 public native function get ctrlKey() : Boolean 
		 public native function set ctrlKey(newCtrlKey : Boolean): void

		 public native function get delta() : int 
		 public native function set delta(newDelta : int): void

		 public native function get localX() : Number 
		 public native function set localX(newLocalX : Number): void

		 public native function get localY() : Number 
		 public native function set localY(newLocalY : Number): void

		 public native function get relatedObject() : InteractiveObject 
		 public native function set relatedObject(newRelatedObject : InteractiveObject): void

		 public native function get shiftKey() : Boolean 
		 public native function set shiftKey(newShiftKey : Boolean): void

		 public native function get stageX() : Number 

		 public native function get stageY() : Number 

		 //new adds
		 public native function get clickCount() : int

		 public native function get commandKey() : Boolean
		 public native function set commandKey(commandKey : Boolean):void

		 public native function get controlKey() : Boolean
		 public native function set controlKey(controlKey : Boolean):void

		 public native function get isRelatedObjectInaccessible() : Boolean
		 public native function set isRelatedObjectInaccessible(isRelatedObjectInaccessible : Boolean):void
		 //new adds end
		 
		 //new adds 11
		 public native function get movementX():Number
		 public native function set movementX(value:Number):void
		 public native function get movementY():Number
		 public native function set movementY(value:Number):void
		 //new adds 11 end

 		//Methods
 		 public native function MouseEvent(type:String, bubbles:Boolean = true, cancelable:Boolean = false, localX:Number = 0, localY:Number = 0, relatedObject:InteractiveObject = null, ctrlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false, buttonDown:Boolean = false, delta:int = 0) 

 		 public native override function clone():Event  		 
 		 public native function updateAfterEvent():void
 		 public native override function toString():String 
		 /*{
 			var s:String="[MouseEvent type=\"";
 			s+=type;
 			s+="\" bubbles=";
 			s+=String(bubbles);
 			s+=" cancelable=";
 			s+=String(cancelable);
 			s+=" eventPhase=";
 			s+=String(eventPhase);
 			s+=" localX=";
 			s+=String(localX);
 			s+=" localY=";
 			s+=String(localY);
 			s+=" stageX=";
 			s+=String(stageX);
 			s+=" stageY=";
 			s+=String(stageY);
 			s+="]";
		 }*/	
 	}
 }
