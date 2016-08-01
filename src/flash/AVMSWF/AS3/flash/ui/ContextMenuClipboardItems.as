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
 	[native(cls="ContextMenuClipboardItemsClass", methods="auto")]

 	final public class ContextMenuClipboardItems
 	{
  	//Consts
  	
  	//Properties
//  	public native function get clear() : Boolean
//  	public native function set clear(clear : Boolean):void
  	
//  	public native function get copy() : Boolean
//  	public native function set copy(copy : Boolean):void
  	
//  	public native function get cut() : Boolean
// 	public native function set cut(cut : Boolean):void
  	
//  	public native function get paste() : Boolean
//  	public native function set paste(paste : Boolean):void
  	
//  	public native function get selectAll() : Boolean
//  	public native function set selectAll(selectAll : Boolean):void

 		//Methods
//		public native function ContextMenuClipboardItems()
        private var _cut:Boolean = false;
        private var _copy:Boolean = false;
        private var _paste:Boolean = false;
        private var _clear:Boolean = false;
        private var _selectAll:Boolean = true;

        public function ContextMenuClipboardItems()
        {
            return;
        }// end function

        public function get cut() : Boolean
        {
            return this._cut;
        }// end function

        public function set cut(val:Boolean) : void
        {
            this._cut = val;
            return;
        }// end function

        public function get copy() : Boolean
        {
            return this._copy;
        }// end function

        public function set copy(val:Boolean) : void
        {
            this._copy = val;
            return;
        }// end function

        public function get paste() : Boolean
        {
            return this._paste;
        }// end function

        public function set paste(val:Boolean) : void
        {
            this._paste = val;
            return;
        }// end function

        public function get clear() : Boolean
        {
            return this._clear;
        }// end function

        public function set clear(val:Boolean) : void
        {
            this._clear = val;
            return;
        }// end function

        public function get selectAll() : Boolean
        {
            return this._selectAll;
        }// end function

        public function set selectAll(val:Boolean) : void
        {
            this._selectAll = val;
            return;
        }// end function

        public function clone() : ContextMenuClipboardItems
        {
            var _loc_1:* = new ContextMenuClipboardItems();
            _loc_1.cut = this.cut;
            _loc_1.copy = this.copy;
            _loc_1.paste = this.paste;
            _loc_1.clear = this.clear;
            _loc_1.selectAll = this.selectAll;
            return _loc_1;
        }// end function
 	}
 }
