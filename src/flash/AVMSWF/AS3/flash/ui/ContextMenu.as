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

 package flash.ui
 {
 	[native(cls="ContextMenuClass", methods="auto")]

	import flash.events.EventDispatcher
	import flash.display.NativeMenu

	import flash.ui.ContextMenuBuiltInItems
	import flash.ui.ContextMenuClipboardItems
	import flash.net.URLRequest
	import flash.display.NativeMenuItem
	import flash.display.Stage
	

 	public final class ContextMenu extends NativeMenu
 	{
  		//Properties
  		 
  		public native function get builtInItems() : ContextMenuBuiltInItems
		public native function set builtInItems(newBuiltInItems : ContextMenuBuiltInItems): void

		public native function get customItems() : Array
		public native function set customItems(newCustomItems : Array): void

		 //new adds
		public native function get clipboardItems() : ContextMenuClipboardItems
		public native function set clipboardItems(clipboardItems : ContextMenuClipboardItems):void

		public native function get clipboardMenu() : Boolean
		public native function set clipboardMenu(clipboardMenu : Boolean):void

		public static function get isSupported() : Boolean	//s
		{
			return true;
		}

		public native function get items() : Array
		public native function set items(items : Array):void
			
		public native function get link() : URLRequest
		public native function set link(link : URLRequest):void

		public native function get numItems() : int
		public native function set numItems(numItems : int):void
		 //new adds end

 		//Methods
 		public native function ContextMenu()

 		public native function clone():NativeMenu
 		public native function hideBuiltInItems():void

		 //new adds
		public native function addItemAt(item:NativeMenuItem, index:int):NativeMenuItem
		public native function containsItem(item:NativeMenuItem):Boolean
		public native function display(stage:Stage, stageX:Number, stageY:Number):void
		public native function getItemAt(index:int):NativeMenuItem
		public native function getItemIndex(item:NativeMenuItem):int
		public native function removeAllItems():void
		public native function removeItemAt(index:int):NativeMenuItem
		 //new adds end
 	}
 }
