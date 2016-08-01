package flash.net
{
[native(cls="GroupSpecifierClass", methods="auto")]

import flash.utils.*;

public class GroupSpecifier
{
	//Consts
	
	//Properties
//	public native function get ipMulticastMemberUpdatesEnabled() : Boolean
//	public native function set ipMulticastMemberUpdatesEnabled(ipMulticastMemberUpdatesEnabled : Boolean):void
	
//	public native function get multicastEnabled() : Boolean
//	public native function set multicastEnabled(multicastEnabled : Boolean):void
	
//	public native function get objectReplicationEnabled() : Boolean
//	public native function set objectReplicationEnabled(objectReplicationEnabled : Boolean):void
	
//	public native function get peerToPeerDisabled() : Boolean
//	public native function set peerToPeerDisabled(peerToPeerDisabled : Boolean):void
	
//	public native function get postingEnabled() : Boolean
//	public native function set postingEnabled(postingEnabled : Boolean):void
	
//	public native function get routingEnabled() : Boolean
//	public native function set routingEnabled(routingEnabled : Boolean):void
	
//	public native function get serverChannelEnabled() : Boolean
//	public native function set serverChannelEnabled(serverChannelEnabled : Boolean):void
	
	//Methods
//	public native function GroupSpecifier(name:String)
//	public native function addBootstrapPeer(peerID:String):void
//	public native function addIPMulticastAddress(address:String, port:* = null, source:String = null):void
//	public native function authorizations():String
//	public native static function encodeBootstrapPeerIDSpec(peerID:String):String
//	public native static function encodeIPMulticastAddressSpec(address:String, port:* = null, source:String = null):String
//	public native static function encodePostingAuthorization(password:String):String
//	public native static function encodePublishAuthorization(password:String):String
//	public native function groupspecWithAuthorizations():String
//	public native function groupspecWithoutAuthorizations():String
//	public native function makeUnique():void
//	public native function setPostingPassword(password:String = null, salt:String = null):void
//	public native function setPublishPassword(password:String = null, salt:String = null):void
//	public native function toString():String

        private var m_routing:String;
        private var m_multicast:String;
        private var m_objectReplication:String;
        private var m_posting:String;
        private var m_publishAuthHash:String;
        private var m_postingAuthHash:String;
        private var m_ipMulticastAddresses:String;
        private var m_bootstrapPeers:String;
        private var m_openServerChannel:String;
        private var m_disablePeerToPeer:String;
        private var m_tag:String;
        private var m_unique:String;
        private var m_publishAuth:String;
        private var m_postingAuth:String;
        private var m_ipMulticastMemberUpdates:String;

        public function GroupSpecifier(name:String)
        {
            if (name)
            {
                this.m_tag = toOption(14, byteArrayToHex(stringToBytes(name)));
            }
            else
            {
                throw new ArgumentError("Name can not be empty");
            }
            this.m_unique = "";
            this.routingEnabled = false;
            this.multicastEnabled = false;
            this.objectReplicationEnabled = false;
            this.postingEnabled = false;
            this.setPublishPassword(null);
            this.setPostingPassword(null);
            this.clearIPMulticastAddresses();
            this.clearBootstrapPeers();
            this.serverChannelEnabled = false;
            this.peerToPeerDisabled = false;
            this.ipMulticastMemberUpdatesEnabled = false;
            return;
        }// end function

        public function makeUnique() : void
        {
            this.m_unique = toOption(14, GetCryptoRandomString(32));
            return;
        }// end function

        public function get routingEnabled() : Boolean
        {
            return this.m_routing != "";
        }// end function

        public function set routingEnabled(enabled:Boolean) : void
        {
            if (enabled)
            {
                this.m_routing = "0100";
            }
            else
            {
                this.m_routing = "";
            }
            return;
        }// end function

        public function get multicastEnabled() : Boolean
        {
            return this.m_multicast != "";
        }// end function

        public function set multicastEnabled(enabled:Boolean) : void
        {
            if (enabled)
            {
                this.m_multicast = "0101";
            }
            else
            {
                this.m_multicast = "";
            }
            return;
        }// end function

        public function get objectReplicationEnabled() : Boolean
        {
            return this.m_objectReplication != "";
        }// end function

        public function set objectReplicationEnabled(enabled:Boolean) : void
        {
            if (enabled)
            {
                this.m_objectReplication = "0102";
            }
            else
            {
                this.m_objectReplication = "";
            }
            return;
        }// end function

        public function get postingEnabled() : Boolean
        {
            return this.m_posting != "";
        }// end function

        public function set postingEnabled(enabled:Boolean) : void
        {
            if (enabled)
            {
                this.m_posting = "0103";
            }
            else
            {
                this.m_posting = "";
            }
            return;
        }// end function

        private function clearIPMulticastAddresses() : void
        {
            this.m_ipMulticastAddresses = "";
            return;
        }// end function

        private function clearBootstrapPeers() : void
        {
            this.m_bootstrapPeers = "";
            return;
        }// end function

        public function get peerToPeerDisabled() : Boolean
        {
            return this.m_disablePeerToPeer != "";
        }// end function

        public function set peerToPeerDisabled(disable:Boolean) : void
        {
            if (disable)
            {
                this.m_disablePeerToPeer = "010d";
            }
            else
            {
                this.m_disablePeerToPeer = "";
            }
            return;
        }// end function

        public function get ipMulticastMemberUpdatesEnabled() : Boolean
        {
            return this.m_ipMulticastMemberUpdates != "";
        }// end function

        public function set ipMulticastMemberUpdatesEnabled(enabled:Boolean) : void
        {
            if (enabled)
            {
                this.m_ipMulticastMemberUpdates = "011b";
            }
            else
            {
                this.m_ipMulticastMemberUpdates = "";
            }
            return;
        }// end function

        public function setPublishPassword(password:String = null, salt:String = null) : void
        {
            var _loc_3:ByteArray = null;
            if (password)
            {
                _loc_3 = stringToBytes(password);
                _loc_3.position = 0;
                this.m_publishAuth = toOption(21, byteArrayToHex(_loc_3));
                this.m_publishAuthHash = toOption(5, SaltedSHA256(salt, _loc_3));
            }
            else
            {
                this.m_publishAuthHash = "";
                this.m_publishAuth = "";
            }
            return;
        }// end function

        public function setPostingPassword(password:String = null, salt:String = null) : void
        {
            var _loc_3:ByteArray = null;
            if (password)
            {
                _loc_3 = stringToBytes(password);
                _loc_3.position = 0;
                this.m_postingAuth = toOption(23, byteArrayToHex(_loc_3));
                this.m_postingAuthHash = toOption(7, SaltedSHA256(salt, _loc_3));
            }
            else
            {
                this.m_postingAuthHash = "";
                this.m_postingAuth = "";
            }
            return;
        }// end function

        public function get serverChannelEnabled() : Boolean
        {
            return this.m_openServerChannel != "";
        }// end function

        public function set serverChannelEnabled(enabled:Boolean) : void
        {
            if (enabled)
            {
                this.m_openServerChannel = "010c";
            }
            else
            {
                this.m_openServerChannel = "";
            }
            return;
        }// end function

        public function addBootstrapPeer(peerID:String) : void
        {
            this.m_bootstrapPeers = this.m_bootstrapPeers + toOption(11, peerID);
            return;
        }// end function

        public function addIPMulticastAddress(address:String, port = null, source:String = null) : void
        {
            this.m_ipMulticastAddresses = this.m_ipMulticastAddresses + encodeIPMulticastAddress(address, port, source);
            return;
        }// end function

        public function toString() : String
        {
            return this.groupspecWithAuthorizations();
        }// end function

        public function groupspecWithoutAuthorizations() : String
        {
            return "G:" + this.m_routing + this.m_multicast + this.m_objectReplication + this.m_posting + this.m_publishAuthHash + this.m_postingAuthHash + this.m_openServerChannel + this.m_disablePeerToPeer + this.m_tag + this.m_unique + this.m_ipMulticastMemberUpdates + "00" + this.groupspecExtras();
        }// end function

        private function groupspecExtras() : String
        {
            return this.m_ipMulticastAddresses + this.m_bootstrapPeers;
        }// end function

        public function groupspecWithAuthorizations() : String
        {
            return this.groupspecWithoutAuthorizations() + this.authorizations();
        }// end function

        public function authorizations() : String
        {
            return this.m_publishAuth + this.m_postingAuth;
        }// end function

        public static function encodePostingAuthorization(password:String) : String
        {
            return toOption(23, byteArrayToHex(stringToBytes(password)));
        }// end function

        public static function encodePublishAuthorization(password:String) : String
        {
            return toOption(21, byteArrayToHex(stringToBytes(password)));
        }// end function

        public static function encodeIPMulticastAddressSpec(address:String, port = null, source:String = null) : String
        {
            return encodeIPMulticastAddress(address, port, source);
        }// end function

        public static function encodeBootstrapPeerIDSpec(peerID:String) : String
        {
            return toOption(11, peerID);
        }// end function

        private static function SaltedSHA256(salt:String, bytes:ByteArray) : String
        {
            var _loc_3:ByteArray = null;
            var _loc_4:String = null;
            if (salt)
            {
                _loc_3 = stringToBytes(salt);
                _loc_4 = byteArrayToHex(_loc_3);
                _loc_3.writeBytes(bytes);
                _loc_3.position = 0;
                return _loc_4 + SHA256(_loc_3);
            }
            return SHA256(bytes);
        }// end function

        private static function encodeIPMulticastAddress(address:String, port, source:String) : String
        {
            var val:String;
            var address:* = address;
            var port:* = port;
            var source:* = source;
            var is6:Boolean;
            try
            {
                val = inet_ptohex6(address, port);
                is6;
            }
            catch (e:Error)
            {
                val = inet_ptohex4(address, port);
            }
            if (source)
            {
                val = val + (is6 ? (inet_ptohex6(source, 0)) : (inet_ptohex4(source, 0))).substr(0, -4);
            }
            return toOption(10, val);
        }// end function

        private static function hexByte(byte:uint) : String
        {
            var _loc_2:String = null;
            if (byte > 255)
            {
                throw new RangeError();
            }
            _loc_2 = byte.toString(16);
            if (_loc_2.length < 2)
            {
                _loc_2 = "0" + _loc_2;
            }
            return _loc_2;
        }// end function

        private static function vlu(num:uint) : String
        {
            var _loc_3:uint = 0;
            var _loc_2:String = "";
            do
            {
                
                _loc_3 = num & 127;
                if (_loc_2.length > 0)
                {
                    _loc_3 = _loc_3 | 128;
                }
                _loc_2 = hexByte(_loc_3) + _loc_2;
                num = num >> 7;
            }while (num)
            return _loc_2;
        }// end function

        private static function toOption(optionID:uint, hexBytes:String) : String
        {
            var _loc_3:String = null;
            _loc_3 = vlu(optionID) + hexBytes;
            return vlu(_loc_3.length / 2) + _loc_3;
        }// end function

        private static function inet_ptohex4(address:String, port = null) : String
        {
            var _loc_3:RegExp = null;
            var _loc_4:RegExp = null;
            var _loc_5:Object = null;
            var _loc_6:RegExp = null;
            var _loc_7:RegExp = null;
            var _loc_8:Object = null;
            var _loc_9:int = 0;
            if (port == null)
            {
                _loc_3 = /\[(\d+)\.(\d+)\.(\d+)\.(\d+)\]:(\d+)""\[(\d+)\.(\d+)\.(\d+)\.(\d+)\]:(\d+)/;
                _loc_4 = /(\d+)\.(\d+)\.(\d+)\.(\d+):(\d+)""(\d+)\.(\d+)\.(\d+)\.(\d+):(\d+)/;
                _loc_5 = _loc_3.exec(address);
                if (!_loc_5)
                {
                    _loc_5 = _loc_4.exec(address);
                }
                if (_loc_5)
                {
                    return hexByte(_loc_5[1]) + hexByte(_loc_5[2]) + hexByte(_loc_5[3]) + hexByte(_loc_5[4]) + hexByte(_loc_5[5] / 256) + hexByte(_loc_5[5] & 255);
                }
                throw new ArgumentError("Invalid address");
            }
            else
            {
                _loc_6 = /\[(\d+)\.(\d+)\.(\d+)\.(\d+)\]""\[(\d+)\.(\d+)\.(\d+)\.(\d+)\]/;
                _loc_7 = /(\d+)\.(\d+)\.(\d+)\.(\d+)""(\d+)\.(\d+)\.(\d+)\.(\d+)/;
                _loc_9 = int(port);
                _loc_8 = _loc_6.exec(address);
                if (!_loc_8)
                {
                    _loc_8 = _loc_7.exec(address);
                }
                if (_loc_8)
                {
                    return hexByte(_loc_8[1]) + hexByte(_loc_8[2]) + hexByte(_loc_8[3]) + hexByte(_loc_8[4]) + hexByte(_loc_9 / 256) + hexByte(_loc_9 & 255);
                }
                throw new ArgumentError("Invalid address");
            }
        }// end function

        private static function inet_ptohex6(address:String, port = null) : String
        {
            var _loc_8:int = 0;
            var _loc_11:Object = null;
            var _loc_12:int = 0;
            var _loc_13:int = 0;
            var _loc_15:String = null;
            var _loc_16:String = null;
            var _loc_17:int = 0;
            var _loc_3:* = /(::)|([:\[\]])|(\d+\.\d+\.\d+\.\d+)|([0-9a-fA-F]+)""(::)|([:\[\]])|(\d+\.\d+\.\d+\.\d+)|([0-9a-fA-F]+)/g;
            var _loc_4:* = /(\d+)\.(\d+)\.(\d+)\.(\d+)""(\d+)\.(\d+)\.(\d+)\.(\d+)/;
            var _loc_5:* = address.match(_loc_3);
            var _loc_6:String = "";
            var _loc_7:String = "";
            var _loc_9:Boolean = false;
            var _loc_10:Boolean = false;
            var _loc_14:Boolean = false;
            if (_loc_5 == null)
            {
                throw new ArgumentError();
            }
            _loc_8 = 0;
            while (_loc_8 < _loc_5.length)
            {
                
                _loc_15 = _loc_5[_loc_8];
                if (_loc_15 == "]")
                {
                    _loc_9 = true;
                }
                else if (_loc_15 == "::")
                {
                    if (_loc_10)
                    {
                        throw new RangeError();
                    }
                    _loc_10 = true;
                    _loc_14 = true;
                }
                else if (_loc_15 == ":")
                {
                    _loc_14 = true;
                }
                else if (_loc_15 != "[")
                {
                    if (_loc_9)
                    {
                        if (port)
                        {
                            throw new ArgumentError();
                        }
                        port = _loc_15;
                    }
                    else
                    {
                        _loc_11 = _loc_4.exec(_loc_15);
                        if (_loc_11)
                        {
                            if (_loc_14)
                            {
                                if (!_loc_10)
                                {
	                            if (_loc_6.length != 28)
	                            {
	                                throw new ArgumentError();
	                            }                                
                                }
                            }
                            _loc_9 = true;
                            _loc_7 = _loc_7 + hexByte(_loc_11[1]) + hexByte(_loc_11[2]) + hexByte(_loc_11[3]) + hexByte(_loc_11[4]);
                        }
                        else
                        {
                            _loc_17 = parseInt(_loc_15, 16);	//Top Level
                            if (_loc_17 > 65535)
                            {
                                throw new RangeError();
                            }
                            _loc_16 = hexByte(_loc_17 / 256) + hexByte(_loc_17 & 255);
                            if (_loc_10)
                            {
                                _loc_7 = _loc_7 + _loc_16;
                            }
                            else
                            {
                                _loc_6 = _loc_6 + _loc_16;
                            }
                        }
                    }
                }
                _loc_8 = _loc_8 + 1;
            }
            if (!_loc_14)
            {
                throw new ArgumentError();
            }
            _loc_13 = _loc_6.length + _loc_7.length;
            if (_loc_13 > 32)
            {
                throw new RangeError();
            }
            while (_loc_13++ < 32)
            {
                
                _loc_6 = _loc_6 + "0";
            }
            if (port == null)
            {
                throw new ArgumentError();
            }
            _loc_12 = int(port);
            if (_loc_12 >= 0)
            {
	            if (_loc_12 > 65535)
	            {
	                throw new RangeError();
	            }            
            }
            _loc_7 = _loc_7 + hexByte(_loc_12 / 256) + hexByte(_loc_12 & 255);
            return _loc_6 + _loc_7;
        }// end function

        private static function byteArrayToHex(bytes:ByteArray) : String
        {
            var _loc_3:int = 0;
            var _loc_2:String = "";
            _loc_3 = 0;
            while (_loc_3 < bytes.length)
            {
                
                _loc_2 = _loc_2 + hexByte(bytes[_loc_3]);
                _loc_3 = _loc_3 + 1;
            }
            return _loc_2;
        }// end function

        private static function stringToBytes(str:String) : ByteArray
        {
            var _loc_2:* = new ByteArray();
            _loc_2.writeUTFBytes(str);
            return _loc_2;
        }// end function

        private static function SHA256(arr:ByteArray) : String
        {
            return calcSHA256Digest(arr);
        }// end function

        private native static function calcSHA256Digest(input:ByteArray) : String;

        private native static function GetCryptoRandomString(length:uint) : String;
        
        //new adds 11
        public static native function get maxSupportedGroupspecVersion():int
        public native function get minGroupspecVersion():int
        public native function set minGroupspecVersion(value:int):void
        //new adds 11 end 
}
}