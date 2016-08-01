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
 	[native(cls="LoaderInfoClass", methods="auto")]
 	
	import flash.system.ApplicationDomain

	import flash.utils.ByteArray
	import flash.events.*

 	public class LoaderInfo extends  EventDispatcher
 	{
 		//Properties
 		 public native function get actionScriptVersion() : uint 

 		 public native function get applicationDomain() : ApplicationDomain 

 		 public native function get bytesLoaded() : uint 
 		 
 		 public native function get bytes() : ByteArray

 		 public native function get bytesTotal() : uint 

 		 public native function get childAllowsParent() : Boolean 

 		 public native function get content() : DisplayObject 

 		 public native function get contentType() : String 

 		 public native function get frameRate() : Number 

 		 public native function get height() : int 

 		 public native function get loader() : Loader 

 		 public native function get loaderURL() : String 

 		 public native function get parameters() : Object 

 		 public native function get parentAllowsChild() : Boolean 

 		 public native function get sameDomain() : Boolean 

 		 public native function get sharedEvents() : EventDispatcher 

 		 public native function get swfVersion() : uint 

 		 public native function get url() : String 

 		 public native function get width() : int 

		 //new adds
//		 public native function get bytes() : ByteArray

		 public native function get childSandboxBridge() : Object
		 public native function set childSandboxBridge(childSandboxBridge : Object):void

		 public native function get isURLInaccessible() : Boolean

		 public native function get parentSandboxBridge() : Object
		 public native function set parentSandboxBridge(parentSandboxBridge : Object):void

		 public native function get uncaughtErrorEvents() : UncaughtErrorEvents
		 //new adds end

		 //Methods
		 //new adds
		 public static native function getLoaderInfoByDefinition(object:Object):LoaderInfo
		 override public function dispatchEvent(event:Event) : Boolean
	        {
	            Error.throwError(IllegalOperationError, 2118);
	            return false;
	        }// end function
		 //new adds end
 	}
 }