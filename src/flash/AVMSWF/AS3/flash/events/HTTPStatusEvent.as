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
 	[native(cls="HTTPStatusEventClass", methods="auto")]

	import flash.events.Event

 	public class HTTPStatusEvent extends  Event
 	{
 		//Consts
 		public static const HTTP_STATUS:String = "httpStatus" 

		//new adds
		public static const HTTP_RESPONSE_STATUS : String = "httpResponseStatus"
		//new adds end

 		//Properties
 		 public native function get status() : int

		 //new adds
		 public native function get responseHeaders() : Array
		 public native function set responseHeaders(responseHeaders : Array):void

		 public native function get responseURL() : String
		 public native function set responseURL(responseURL : String):void
		 //new adds end
		 
		 //new adds 11
		 public native function get redirected():Boolean
		 public native function set redirected(value:Boolean):void
		 //new adds 11 end

 		//Methods
 		 public native function HTTPStatusEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, status:int = 0/*, redirected:Boolean = false*/) 

 		 public native override function clone():Event 
 		 public native override function toString():String 
		 /*{
 			var s:String="[HTTPStatusEvent type=\"";
 			s+=type;
 			s+="\" bubbles=";
 			s+=String(bubbles);
 			s+=" cancelable=";
 			s+=String(cancelable);
 			s+=" eventPhase=";
 			s+=String(eventPhase);
 			s+=" status=";
 			s+=String(status);
 			s+="]";
		 }*/
 	}
 }