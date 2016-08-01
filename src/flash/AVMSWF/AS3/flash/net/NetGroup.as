package flash.net
{
[native(cls="NetGroupClass", methods="auto")]

import flash.events.EventDispatcher

import flash.net.NetConnection
import flash.net.NetGroupInfo

public class NetGroup extends EventDispatcher
{
	//Consts
	
	//Properties
//	public native function get estimatedMemberCount() : Number
	
//	public native function get info() : NetGroupInfo
	
//	public native function get localCoverageFrom() : String
	
//	public native function get localCoverageTo() : String
	
//	public native function get neighborCount() : Number
	
//	public native function get receiveMode() : String
//	public native function set receiveMode(receiveMode : String):void
	
//	public native function get replicationStrategy() : String
//	public native function set replicationStrategy(replicationStrategy : String):void
	
	//Methods
//	public native function NetGroup(connection:NetConnection, groupspec:String)
//	public native function addHaveObjects(startIndex:Number, endIndex:Number):void
//	public native function addMemberHint(peerID:String):Boolean
//	public native function addNeighbor(peerID:String):Boolean
//	public native function addWantObjects(startIndex:Number, endIndex:Number):void
//	public native function close():void
//	public native function convertPeerIDToGroupAddress(peerID:String):String
//	public native function denyRequestedObject(requestID:int):void
//	public native function post(message:Object):String
//	public native function removeHaveObjects(startIndex:Number, endIndex:Number):void
//	public native function removeWantObjects(startIndex:Number, endIndex:Number):void
//	public native function sendToAllNeighbors(message:Object):String
//	public native function sendToNearest(message:Object, groupAddress:String):String
//	public native function sendToNeighbor(message:Object, sendMode:String):String
//	public native function writeRequestedObject(requestID:int, object:Object):void
        private static const kClose:uint = 0;
        private static const kPost:uint = 344;
        private static const kSendToNearest:uint = 345;
        private static const kSendToNeighbor:uint = 346;
        private static const kGetReceiveMode:uint = 347;
        private static const kSetReceiveMode:uint = 348;
        private static const kGetEstimatedMemberCount:uint = 349;
        private static const kGetNeighborCount:uint = 350;
        private static const kGetLocalCoverageFrom:uint = 353;
        private static const kGetLocalCoverageTo:uint = 354;
        private static const kGetReplicationStrategy:uint = 355;
        private static const kSetReplicationStrategy:uint = 356;
        private static const kUpdateHaveBlocks:uint = 357;
        private static const kUpdateWantBlocks:uint = 358;
        private static const kWriteRequestedBlock:uint = 359;
        private static const kDenyRequestedBlock:uint = 360;
        private static const kAddNeighbor:uint = 361;
        private static const kAddMemberHint:uint = 362;

        public native function NetGroup(connection:NetConnection, groupspec:String);

//        private function ctor(connection:NetConnection, groupspec:String) : void;

        private function onStatus(info) : void
        {
            dispatchEvent(new NetStatusEvent("status", false, false, info));
            return;
        }// end function

        public function close() : void
        {
            this.invoke(kClose);
            return;
        }// end function

        private native function invoke(index:uint, ... args);

        public function get replicationStrategy() : String
        {
            return this.invoke(kGetReplicationStrategy);
        }// end function

        public function set replicationStrategy(s:String) : void
        {
            this.invoke(kSetReplicationStrategy, s);
            return;
        }// end function

        public function addHaveObjects(startIndex:Number, endIndex:Number) : void
        {
            if (startIndex >= 0)
            {
	            if (startIndex <= 9007199254740992)
	            {
		            if (endIndex >= 0)
		            {
		                   if (endIndex > 9007199254740992)
			            {
			                Error.throwError(RangeError, 2006);
			            }
			            else
			            {
			                this.invoke(kUpdateHaveBlocks, true, startIndex, endIndex);
			            }
		            }            
	            }            
            }
            return;
        }// end function

        public function removeHaveObjects(startIndex:Number, endIndex:Number) : void
        {
            if (startIndex >= 0)
            {
	            if (startIndex <= 9007199254740992)
	            {
		            if (endIndex >= 0)
		            {
			            if (endIndex > 9007199254740992)
			            {
			                Error.throwError(RangeError, 2006);
			            }
			            else
			            {
			                this.invoke(kUpdateHaveBlocks, false, startIndex, endIndex);
			            }            
		            }            
	            }            
            }
            return;
        }// end function

        public function addWantObjects(startIndex:Number, endIndex:Number) : void
        {
            if (startIndex >= 0)
            {
	            if (startIndex <= 9007199254740992)
	            {
		            if (endIndex >= 0)
		            {
			            if (endIndex > 9007199254740992)
			            {
			                Error.throwError(RangeError, 2006);
			            }
			            else
			            {
			                this.invoke(kUpdateWantBlocks, true, startIndex, endIndex);
			            }            
		            }            
	            }            
            }
            return;
        }// end function

        public function removeWantObjects(startIndex:Number, endIndex:Number) : void
        {
            if (startIndex >= 0)
            {
	            if (startIndex <= 9007199254740992)
	            {
		            if (endIndex >= 0)
		            {
			            if (endIndex > 9007199254740992)
			            {
			                Error.throwError(RangeError, 2006);
			            }
			            else
			            {
			                this.invoke(kUpdateWantBlocks, false, startIndex, endIndex);
			            }            
		            }            
	            }            
            }
            return;
        }// end function

        public function writeRequestedObject(requestID:int, object:Object) : void
        {
            this.invoke(kWriteRequestedBlock, requestID, object);
            return;
        }// end function

        public function denyRequestedObject(requestID:int) : void
        {
            this.invoke(kDenyRequestedBlock, requestID);
            return;
        }// end function

        public function get estimatedMemberCount() : Number
        {
            return this.invoke(kGetEstimatedMemberCount);
        }// end function

        public function get neighborCount() : Number
        {
            return this.invoke(kGetNeighborCount);
        }// end function

        public function get receiveMode() : String
        {
            return this.invoke(kGetReceiveMode);
        }// end function

        public function set receiveMode(mode:String)
        {
            return this.invoke(kSetReceiveMode, mode);
        }// end function

        public native function get info() : NetGroupInfo;

        public native function convertPeerIDToGroupAddress(peerID:String) : String;

        public native function get localCoverageFrom() : String;

        public native function get localCoverageTo() : String;

        public function post(message:Object) : String
        {
            return this.invoke(kPost, message);
        }// end function

        public function sendToNearest(message:Object, groupAddress:String) : String
        {
            return this.invoke(kSendToNearest, message, groupAddress, null, null);
        }// end function

        public function sendToNeighbor(message:Object, sendMode:String) : String
        {
            return this.invoke(kSendToNeighbor, message, sendMode);
        }// end function

        public function sendToAllNeighbors(message:Object) : String
        {
            return this.invoke(kSendToNeighbor, message, "allNeighbors");
        }// end function

        public function addNeighbor(peerID:String) : Boolean
        {
            return this.invoke(kAddNeighbor, peerID);
        }// end function

        public function addMemberHint(peerID:String) : Boolean
        {
            return this.invoke(kAddMemberHint, peerID);
        }// end function
}

}