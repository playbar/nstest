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
 	[native(cls="CapabilitiesClass", methods="auto")]

 	public final class Capabilities
 	{
 		//Properties
 		 public native static function get avHardwareDisable() : Boolean

 		 public native static function get hasAccessibility() : Boolean

 		 public native static function get hasAudio() : Boolean

 		 public native static function get hasAudioEncoder() : Boolean

 		 public native static function get hasEmbeddedVideo() : Boolean

 		 public native static function get hasIME() : Boolean

 		 public native static function get hasMP3() : Boolean

 		 public native static function get hasPrinting() : Boolean

 		 public native static function get hasScreenBroadcast() : Boolean

 		 public native static function get hasScreenPlayback() : Boolean

 		 public native static function get hasStreamingAudio() : Boolean

 		 public native static function get hasStreamingVideo() : Boolean

 		 public native static function get hasTLS() : Boolean

 		 public native static function get hasVideoEncoder() : Boolean

 		 public native static function get isDebugger() : Boolean

 		 public native static function get language() : String

 		 public native static function get localFileReadDisable() : Boolean

 		 public native static function get manufacturer() : String

 		 public native static function get os() : String

 		 public native static function get pixelAspectRatio() : Number

 		 public native static function get playerType() : String

 		 public native static function get screenColor() : String

 		 public native static function get screenDPI() : Number

 		 public native static function get screenResolutionX() : Number

 		 public native static function get screenResolutionY() : Number

 		 public native static function get serverString() : String

 		 public native static function get version() : String

		 //new adds
		public native static function get cpuArchitecture() : String

		public native static function get isEmbeddedInAcrobat() : Boolean

		public native static function get languages() : Array

		public native static function get maxLevelIDC() : String

		public native static function get supports32BitProcesses() : Boolean

		public native static function get supports64BitProcesses() : Boolean

		public native static function get touchscreenType() : String
		 //new adds end
		 
		//Methods
		
		//new adds 11
		public static native function hasMultiChannelAudio(type:String):Boolean
		//new adds 11 end
 	}
 }
