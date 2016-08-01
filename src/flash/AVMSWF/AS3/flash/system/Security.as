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
 	[native(cls="SecurityClass", methods="auto")]

 	public final class Security
 	{
 		//Consts
 		public static const LOCAL_TRUSTED:String = "localTrusted" 
 		public static const LOCAL_WITH_FILE:String = "localWithFile" 
 		public static const LOCAL_WITH_NETWORK:String = "localWithNetwork" 
 		public static const REMOTE:String = "remote" 

		//new adds
		public static const APPLICATION : String = "application"
		//new adds end

 		//Properties
 		 public native static function get exactSettings() : Boolean 
		 public native static function set exactSettings(newExactSettings : Boolean): void

		 public native static function get sandboxType() : String 

		 //new adds
		 public native static function get pageDomain() : String
		 //new adds end

 		//Methods
 		 public native static function allowDomainN(... domains):void 
 		 public native static function allowInsecureDomainN(... domains):void 
 		 public static function allowDomain(... domains):void 
 		 {	allowDomainN(domains);	 }
 		 public static function allowInsecureDomain(... domains):void 
 		 {	
 			allowInsecureDomainN(domains);
 		 }
 		 public native static function loadPolicyFile(url:String):void 
 		 public native static function showSettings(panel:String = "default"):void 
 	}
 }
