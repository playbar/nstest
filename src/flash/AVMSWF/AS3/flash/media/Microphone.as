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

 package flash.media
 {
 	[native(cls="MicrophoneClass", methods="auto")]

	import flash.events.EventDispatcher

	import flash.media.SoundTransform

 	public final class Microphone extends  EventDispatcher
 	{
 		//Properties
 		 public native function get activityLevel() : Number 

		 public native function get gain() : Number 
		 public native function set gain(newGain : Number): void

		 public native function get index() : int 

		 public native function get muted() : Boolean 

		 public native function get name() : String 

		 public native static function get names() : Array 

		 public native function get rate() : int 
		 public native function set rate(newRate : int): void

		 public native function get silenceLevel() : Number 

		 public native function get silenceTimeout() : int 

		 public native function get soundTransform() : SoundTransform 
		 public native function set soundTransform(newSoundTransform : SoundTransform): void

		 public native function get useEchoSuppression() : Boolean 

		 //new adds
		 public native function get codec() : String
		 public native function set codec(codec : String):void

		 public native function get enableVAD() : Boolean
		 public native function set enableVAD(enableVAD : Boolean):void

		 public native function get encodeQuality() : int
		 public native function set encodeQuality(encodeQuality : int):void

		 public native function get enhancedOptions() : MicrophoneEnhancedOptions
		 public native function set enhancedOptions(enhancedOptions : MicrophoneEnhancedOptions):void

		 public native function get framesPerPacket() : int
		 public native function set framesPerPacket(framesPerPacket : int):void

		 public static function get isSupported() : Boolean
		 {
			return true;
		 }

		 public native function get noiseSuppressionLevel() : int
		 public native function set noiseSuppressionLevel(noiseSuppressionLevel : int):void
		 //new adds end

 		//Methods
 		 public native static function getMicrophone(index:int = -1):Microphone 
 		 public native function setLoopBack(state:Boolean = true):void 
 		 public native function setSilenceLevel(silenceLevel:Number, timeout:int = -1):void 
 		 public native function setUseEchoSuppression(useEchoSuppression:Boolean):void 

		 //new adds
		 public static native function getEnhancedMicrophone(index:int = -1):Microphone
		 //new adds end
 	}
 }
