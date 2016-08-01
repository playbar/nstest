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
	[native(cls="SimpleButtonClass", methods="auto")]

	import flash.events.EventDispatcher
	import flash.display.DisplayObject
	import flash.display.InteractiveObject

	import flash.media.SoundTransform

	public class SimpleButton extends  InteractiveObject
	{
		//Properties
		 public native function get downState() : DisplayObject 
		 public native function set downState(newDownState : DisplayObject): void

		 public native function get enabled() : Boolean 
		 public native function set enabled(newEnabled : Boolean): void

		 public native function get hitTestState() : DisplayObject 
		 public native function set hitTestState(newHitTestState : DisplayObject): void

		 public native function get overState() : DisplayObject 
		 public native function set overState(newOverState : DisplayObject): void

		 public native function get soundTransform() : SoundTransform
		 public native function set soundTransform(newSoundTransform : SoundTransform): void

		 public native function get trackAsMenu() : Boolean 
		 public native function set trackAsMenu(newTrackAsMenu : Boolean): void

		 public native function get upState() : DisplayObject 
		 public native function set upState(newUpState : DisplayObject): void

		 public native function get useHandCursor() : Boolean 
		 public native function set useHandCursor(newUseHandCursor : Boolean): void

		//Methods
		 public native function SimpleButton(upState:DisplayObject = null, overState:DisplayObject = null, downState:DisplayObject = null, hitTestState:DisplayObject = null) 
	}
}
