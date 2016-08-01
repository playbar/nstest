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
 	[native(cls="NetConnectionClass", methods="auto")]

	import flash.events.EventDispatcher

	import flash.net.Responder

 	public class NetConnection extends  EventDispatcher
 	{
 		//Properties
 		 public native function get client() : Object 
		 public native function set client(newClient : Object): void

		 public native function get connected() : Boolean 

		 public native function get connectedProxyType() : String 

		 public native static function get defaultObjectEncoding() : uint 
		 public native static function set defaultObjectEncoding(newDefaultObjectEncoding : uint): void

		 public native function get objectEncoding() : uint 
		 public native function set objectEncoding(newObjectEncoding : uint): void

		 public native function get proxyType() : String 
		 public native function set proxyType(newProxyType : String): void

		 public native function get uri() : String 

		 public native function get usingTLS() : Boolean 

		 //new adds
		public native function get farID() : String

		public native function get farNonce() : String

		public native function get httpIdleTimeout() : Number
		public native function set httpIdleTimeout(httpIdleTimeout : Number):void

		public native function get maxPeerConnections() : uint
		public native function set maxPeerConnections(maxPeerConnections : uint):void

		public native function get nearID() : String

		public native function get nearNonce() : String

		public native function get protocol() : String

		public native function get unconnectedPeerStreams() : Array
		 //new adds end

 		//Methods
		 public native function NetConnection() 

 		 public native function addHeader(operation:String, mustUnderstand:Boolean = false, param:Object = null):void 
 		 public function call(command:String, responder:Responder, ... arguments):void 
 		 {
 			callN(command,responder,arguments);
 		 }
 		 public native function callN(command:String,responder:Responder,arguments:Array):void
 		 public native function close():void 
 		 public function connect(command:String, ... arguments):void 
 		 {
 			connectN(command,arguments);
 		 }
 		 public native function connectN(command:String,arguments:Array):void
 	}
 }
