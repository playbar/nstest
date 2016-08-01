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

 package flash.system
 {
 	[native(cls="SystemClass", methods="auto")]

 	final public class System
 	{
		//Properties
		public native static function get ime() : IME 

		public native static function get totalMemory() : uint 

		public native static function get useCodePage() : Boolean 
		public native static function set useCodePage(newUseCodePage : Boolean): void

		//new adds
		public native static function get freeMemory() : Number

		public native static function get privateMemory() : Number

		public native static function get totalMemoryNumber() : Number
		//new adds end
 	
 		//Methods
 		public native static function setClipboard(string:String):void 
 		public native static function gc():void

		//new adds
		public native static function disposeXML(node:XML):void
		public native static function exit(code:uint):void
		public native static function pause():void
		public native static function resume():void
		//new adds end
		
		//new adds 11
		public static native function pauseForGCIfCollectionImminent(imminence:Number = 0.75):void
		//new adds 11 end
 	}
 }
