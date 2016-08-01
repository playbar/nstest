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

 package flash.net
 {
 	[native(cls="URLRequestClass", methods="auto")]

 	public final class URLRequest
 	{
 		//Properties
 		 public native function get contentType() : String 
		 public native function set contentType(newContentType : String): void

		 public native function get data() : Object 
		 public native function set data(newData : Object): void

		 public native function get method() : String 
		 public native function set method(newMethod : String): void

		 public native function get requestHeaders() : Array 
		 public native function set requestHeaders(newRequestHeaders : Array): void

		 public native function get url() : String 
		 public native function set url(newUrl : String): void

		 //new adds
		public native function get authenticate() : Boolean
		public native function set authenticate(authenticate : Boolean):void

		public native function get cacheResponse() : Boolean
		public native function set cacheResponse(cacheResponse : Boolean):void

		public native function get digest() : String
		public native function set digest(digest : String):void

		public native function get followRedirects() : Boolean
		public native function set followRedirects(followRedirects : Boolean):void

		public native function get idleTimeout() : Number
		public native function set idleTimeout(idleTimeout : Number):void

		public native function get manageCookies() : Boolean
		public native function set manageCookies(manageCookies : Boolean):void

		public native function get useCache() : Boolean
		public native function set useCache(useCache : Boolean):void

		public native function get userAgent() : String
		public native function set userAgent(userAgent : String):void
		 //new adds end

 		//Methods
 		 public function URLRequest(url:String = null) 
 		 {
 		     if (url != null)
	            {
	                this.url = url;
	            }
	            this.requestHeaders = [];
	            return;
 		 }
 		 
 		 
 		//new adds 11
 		public native function useRedirectedURL(sourceRequest:URLRequest, wholeURL:Boolean = false, pattern:* = null, replace:String = null):void
 		
 		//new adds 11 end
 	}
 }
