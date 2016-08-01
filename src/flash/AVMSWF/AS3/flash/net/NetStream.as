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
	[native(cls="NetStreamClass", methods="auto")]

	import flash.media.*
	import flash.net.*

	import flash.events.EventDispatcher
	import flash.utils.ByteArray

	public class NetStream extends  EventDispatcher
	{
		//Properties
		 public native function get bufferLength() : Number 

		 public native function get bufferTime() : Number 
		 public native function set bufferTime(newBufferTime : Number): void

		 public native function get bytesLoaded() : uint 

		 public native function get bytesTotal() : uint 

		 public native function get checkPolicyFile() : Boolean 
		 public native function set checkPolicyFile(newCheckPolicyFile : Boolean): void

		 public native function get client() : Object 
		 public native function set client(newClient : Object): void

		 public native function get currentFPS() : Number 

		 public native function get liveDelay() : Number 

		 public native function get objectEncoding() : uint 

		 public native function get soundTransform() : SoundTransform 
		 public native function set soundTransform(newSoundTransform : SoundTransform): void

		 public native function get time() : Number 

		 //new adds
		public native function get audioReliable() : Boolean
		public native function set audioReliable(audioReliable : Boolean):void

		public native function get audioSampleAccess() : Boolean
		public native function set audioSampleAccess(audioSampleAccess : Boolean):void

		public native function get backBufferLength() : Number

		public native function get backBufferTime() : Number
		public native function set backBufferTime(backBufferTime : Number):void

		public native function get bufferTimeMax() : Number
		public native function set bufferTimeMax(bufferTimeMax : Number):void

		public native function get dataReliable() : Boolean
		public native function set dataReliable(dataReliable : Boolean):void

		public native function get farID() : String

		public native function get farNonce() : String

		public native function get inBufferSeek() : Boolean
		public native function set inBufferSeek(inBufferSeek : Boolean):void

		public native function get info() : NetStreamInfo

		public native function get maxPauseBufferTime() : Number
		public native function set maxPauseBufferTime(maxPauseBufferTime : Number):void

		public native function get multicastAvailabilitySendToAll() : Boolean
		public native function set multicastAvailabilitySendToAll(multicastAvailabilitySendToAll : Boolean):void

		public native function get multicastAvailabilityUpdatePeriod() : Number
		public native function set multicastAvailabilityUpdatePeriod(multicastAvailabilityUpdatePeriod : Number):void

		public native function get multicastFetchPeriod() : Number
		public native function set multicastFetchPeriod(multicastFetchPeriod : Number):void

		public native function get multicastInfo() : NetStreamMulticastInfo

		public native function get multicastPushNeighborLimit() : Number
		public native function set multicastPushNeighborLimit(multicastPushNeighborLimit : Number):void

		public native function get multicastRelayMarginDuration() : Number
		public native function set multicastRelayMarginDuration(multicastRelayMarginDuration : Number):void

		public native function get multicastWindowDuration() : Number
		public native function set multicastWindowDuration(multicastWindowDuration : Number):void

		public native function get nearNonce() : String

		public native function get peerStreams() : Array

		public native function get videoReliable() : Boolean
		public native function set videoReliable(videoReliable : Boolean):void

		public native function get videoSampleAccess() : Boolean
		public native function set videoSampleAccess(videoSampleAccess : Boolean):void
		 //new adds end

		//new adds 11
		public native function get useHardwareDecoder():Boolean
		public native function set useHardwareDecoder(value:Boolean):void
		
	    public native function get useJitterBuffer():Boolean
		public native function set useJitterBuffer(value:Boolean):void
		
	    public native function get videoStreamSettings():VideoStreamSettings
		public native function set videoStreamSettings(value:VideoStreamSettings):void
		//new adds 11 end
		
		//Methods
		 public native function NetStream(connection:NetConnection) 

		 public native function attachAudio(microphone:Microphone):void 
		 public native function attachCamera(theCamera:Camera, snapshotMilliseconds:int = -1):void 
		 public native function close():void 
		 public native function pause():void 
		 public function play(... arguments):void 
		 {
			playN(arguments);
		 }
		 public native function playN(arguments:Array):void		 
		
		 public native function publish(name:String = null, type:String = null):void 
		 public native function receiveAudio(flag:Boolean):void 
		 public native function receiveVideo(flag:Boolean):void 
		 public native function resume():void 
		 public native function seek(offset:Number):void 
		
		 public function send(handlerName:String, ... arguments):void 
		 {
			sendN(handlerName,arguments);
		 }
		 public native function sendN(handlerName:String,arguments:Array);		 
		 public native function togglePause():void 

		 //new adds
		public native function appendBytes(bytes:ByteArray):void
		public native function appendBytesAction(netStreamAppendBytesAction:String):void
		public native function attach(connection:NetConnection):void
		public native function onPeerConnect(subscriber:NetStream):Boolean
		public native function play2(param:NetStreamPlayOptions):void
		public native function preloadEmbeddedData(param:NetStreamPlayOptions):void
		public native function receiveVideoFPS(FPS:Number):void
		public native static function resetDRMVouchers():void
		public native function setDRMAuthenticationCredentials(userName:String, password:String, type:String):void
		public native function step(frames:int):void
		 //new adds end
		 
		 //new adds 11
		 public native function dispose():void
		 
		 //new adds 11 end
		
	}
}
