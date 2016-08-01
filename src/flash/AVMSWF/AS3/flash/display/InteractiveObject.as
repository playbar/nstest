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
 	[native(cls="InteractiveObjectClass", methods="auto")]

	import flash.events.EventDispatcher
	import flash.display.DisplayObject

	import flash.ui.ContextMenu
	import flash.accessibility.AccessibilityImplementation
	import flash.geom.Rectangle

 	public class InteractiveObject extends  DisplayObject
 	{
 		//Properties
 		 public native function get contextMenu() : ContextMenu 
		 public native function set contextMenu(newContextMenu : ContextMenu): void

		 public native function get doubleClickEnabled() : Boolean 
		 public native function set doubleClickEnabled(newDoubleClickEnabled : Boolean): void

		 public native function get focusRect() : Object 
		 public native function set focusRect(newFocusRect : Object): void

		 public native function get mouseEnabled() : Boolean 
		 public native function set mouseEnabled(newMouseEnabled : Boolean): void

		 public native function get tabEnabled() : Boolean 
		 public native function set tabEnabled(newTabEnabled : Boolean): void

		 public native function get tabIndex() : int 
		 public native function set tabIndex(newTabIndex : int): void

		 //new adds
		 public native function get accessibilityImplementation() : AccessibilityImplementation
		 public native function set accessibilityImplementation(accessibilityImplementation : AccessibilityImplementation):void

		 public native function get needsSoftKeyboard() : Boolean
		 public native function set needsSoftKeyboard(needsSoftKeyboard : Boolean):void

		 public native function get softKeyboardInputAreaOfInterest() : Rectangle
		 public native function set softKeyboardInputAreaOfInterest(softKeyboardInputAreaOfInterest : Rectangle):void
		 //new adds end

 		//Methods
 		 public native function InteractiveObject() 

		 //new adds
		 public native function requestSoftKeyboard():Boolean
		 //new adds end
 	}
 }
