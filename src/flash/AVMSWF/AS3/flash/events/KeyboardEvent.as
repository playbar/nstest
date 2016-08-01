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
 	[native(cls="KeyboardEventClass", methods="auto")]

	import flash.events.Event

 	public class KeyboardEvent extends Event
 	{
 		//Consts
 		public static const KEY_DOWN:String = "keyDown" 
 		public static const KEY_UP:String = "keyUp" 

 		//Properties
 		 public native function get altKey() : Boolean 
		 public native function set altKey(newAltKey : Boolean): void

		 public native function get charCode() : uint 
		 public native function set charCode(newCharCode : uint): void

		 public native function get ctrlKey() : Boolean 
		 public native function set ctrlKey(newCtrlKey : Boolean): void

		 public native function get keyCode() : uint 
		 public native function set keyCode(newKeyCode : uint): void

		 public native function get keyLocation() : uint 
		 public native function set keyLocation(newKeyLocation : uint): void

		 public native function get shiftKey() : Boolean 
		 public native function set shiftKey(newShiftKey : Boolean): void

		//new adds
		public native function get commandKey() : Boolean
		public native function set commandKey(commandKey : Boolean):void

		public native function get controlKey() : Boolean
		public native function set controlKey(controlKey : Boolean):void
		//new adds end

 		//Methods
 		 public native function KeyboardEvent(type:String, bubbles:Boolean = true, cancelable:Boolean = false, charCode:uint = 0, keyCode:uint = 0, keyLocation:uint = 0, ctrlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false)

 		 public native override function clone():Event 
 		 public native override function toString():String 
 		 public native function updateAfterEvent():void 
 	}
 }
