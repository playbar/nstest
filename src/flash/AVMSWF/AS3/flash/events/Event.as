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
 	[native(cls="EventClass", methods="auto")]

 	public class Event
 	{
 		//Consts
 		public static const ACTIVATE:String = "activate"
 		public static const ADDED:String = "added" 
 		public static const ADDED_TO_STAGE:String = "addedToStage" 
 		public static const CANCEL:String = "cancel" 
 		public static const CHANGE:String = "change" 
 		
		public static const CLOSE:String = "close" 
		public static const COMPLETE:String = "complete" 
		public static const CONNECT:String = "connect" 
		public static const DEACTIVATE:String = "deactivate" 
		public static const ENTER_FRAME:String = "enterFrame" 
		
		public static const FULLSCREEN:String = "fullScreen" 
		public static const ID3:String = "id3" 
		public static const INIT:String = "init" 
		public static const MOUSE_LEAVE:String = "mouseLeave" 
		public static const OPEN:String = "open" 
		
		public static const REMOVED:String = "removed" 
		public static const REMOVED_FROM_STAGE:String = "removedFromStage" 
		public static const RENDER:String = "render" 
		public static const RESIZE:String = "resize" 
		public static const SCROLL:String = "scroll" 
		
		public static const SELECT:String = "select" 
		public static const SOUND_COMPLETE:String = "soundComplete" 
		public static const TAB_CHILDREN_CHANGE:String = "tabChildrenChange" 
		public static const TAB_ENABLED_CHANGE:String = "tabEnabledChange" 
		public static const TAB_INDEX_CHANGE:String = "tabIndexChange" 
		public static const UNLOAD:String = "unload" 
		public static const EXIT_FRAME:String="exitFrame"
		
		//new adds
		public static const CLEAR : String = "clear"
		public static const CLOSING : String = "closing"
		public static const COPY : String = "copy"
		public static const CUT : String = "cut"
		public static const DISPLAYING : String = "displaying"
		public static const EXITING : String = "exiting"
		public static const FRAME_CONSTRUCTED : String = "frameConstructed"
		public static const HTML_BOUNDS_CHANGE : String = "htmlBoundsChange"
		public static const HTML_DOM_INITIALIZE : String = "htmlDOMInitialize"
		public static const HTML_RENDER : String = "htmlRender"
		public static const LOCATION_CHANGE : String = "locationChange"
		public static const NETWORK_CHANGE : String = "networkChange"
		public static const PASTE : String = "paste"
		public static const PREPARING : String = "preparing"
		public static const SELECT_ALL : String = "selectAll"
		public static const STANDARD_ERROR_CLOSE : String = "standardErrorClose"
		public static const STANDARD_INPUT_CLOSE : String = "standardInputClose"
		public static const STANDARD_OUTPUT_CLOSE : String = "standardOutputClose"
		public static const TEXT_INTERACTION_MODE_CHANGE : String = "textInteractionModeChange"
		public static const USER_IDLE : String = "userIdle"
		public static const USER_PRESENT : String = "userPresent"
		//new adds end
		//Check adds
		public static const CONTEXT3D_CREATE:String = "context3DCreate";
		//Check adds end

		//new adds 11
		public static const CHANNEL_MESSAGE:String = "channelMessage"
		public static const CHANNEL_STATE:String = "channelState"
		public static const FRAME_LABEL:String = "frameLabel"
		public static const SUSPEND:String = "suspend"
		public static const TEXTURE_READY:String = "textureReady"
		public static const VIDEO_FRAME:String = "videoFrame"
		public static const WORKER_STATE:String = "workerState"
		//new adds 11 end 
		
		
		//Properties
		 public native function get bubbles() : Boolean

		 public native function get cancelable() : Boolean 
		
		 public native function get currentTarget() : Object 
		
		 public native function get eventPhase() : uint 
		
		 public native function get target() : Object 
		
		 public native function get type() : String 

 		//Methods
 		 public native function Event(type:String, bubbles:Boolean = false, cancelable:Boolean = false) 

 		 public native function clone():Event 
 		 public native function nformatToString(className:String,args:Array):String
 		 public function formatToString(className:String, ... arguments):String
 		 {
 			return nformatToString(className,arguments);
 		 }
 		 public native function isDefaultPrevented():Boolean 
 		 public native function preventDefault():void 
 		 public native function stopImmediatePropagation():void 
 		 public native function stopPropagation():void 
 		 public native function toString():String 
 		 /*{
 			var s:String="[Event type=\"";
 			s+=type;
 			s+="\" bubbles=";
 			s+=String(bubbles);
 			s+=" cancelable=";
 			s+=String(cancelable);
 			s+=" eventPhase=";
 			s+=String(eventPhase);
 			s+="]";
 		 }*/
 	}
 }