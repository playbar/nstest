package flash.net.drm
{
[native(cls="DRMVoucherClass", methods="auto")]

import flash.events.*;
import flash.geom.*;
import flash.utils.*;

final public class DRMVoucher
{
	//Consts
	
	//Properties
//	public native function get offlineLeaseEndDate() : Date
	
//	public native function get offlineLeaseStartDate() : Date
	
//	public native function get playbackTimeWindow() : flash.net.drm:DRMPlaybackTimeWindow
	
//	public native function get playbackTimeWindow() : DRMPlaybackTimeWindow
	
//	public native function get policies() : Object
	
//	public native function get voucherEndDate() : Date
	
//	public native function get voucherStartDate() : Date
	
	//Methods
	
        private var m_endDate:Date;
        private var m_startDate:Date;
        private var m_offlineLeaseStartDate:Date;
        private var m_offlineLeaseExpirationDate:Date;
        private var m_customPolicies:Object;
        private var m_playbackTimeWindow:DRMPlaybackTimeWindow;

        public function DRMVoucher()
        {
            return;
        }// end function

        public function get voucherStartDate() : Date
        {
            var _loc_1:Number = NaN;
            if (this.m_startDate == null)
            {
                _loc_1 = this.getStartDateInner();
                if (_loc_1 > 0)
                {
                    this.m_startDate = new Date(1000 * _loc_1);
                }
            }
            return this.m_startDate;
        }// end function

        public function get voucherEndDate() : Date
        {
            var _loc_1:Number = NaN;
            if (this.m_endDate == null)
            {
                _loc_1 = this.getEndDateInner();
                if (_loc_1 > 0)
                {
                    this.m_endDate = new Date(1000 * _loc_1);
                }
            }
            return this.m_endDate;
        }// end function

        public function get offlineLeaseStartDate() : Date
        {
            var _loc_1:Number = NaN;
            if (this.m_offlineLeaseStartDate == null)
            {
                _loc_1 = this.getOfflineLeaseStartDateInner();
                if (_loc_1 > 0)
                {
                    this.m_offlineLeaseStartDate = new Date(1000 * _loc_1);
                }
            }
            return this.m_offlineLeaseStartDate;
        }// end function

        public function get offlineLeaseEndDate() : Date
        {
            var _loc_1:Number = NaN;
            if (this.m_offlineLeaseExpirationDate == null)
            {
                _loc_1 = this.getOfflineLeaseExpirationDateInner();
                if (_loc_1 > 0)
                {
                    this.m_offlineLeaseExpirationDate = new Date(1000 * _loc_1);
                }
            }
            return this.m_offlineLeaseExpirationDate;
        }// end function

        private native function getEndDateInner() : Number;

        private native function getStartDateInner() : Number;

        private native function getOfflineLeaseStartDateInner() : Number;

        private native function getOfflineLeaseExpirationDateInner() : Number;

        public function get policies() : Object
        {
            if (this.m_customPolicies == null)
            {
                this.createCustomPolicyObject();
            }
            return this.m_customPolicies;
        }// end function

        private native function createCustomPolicyObject() : void;

        private function setCustomPolicyObject(customPolicyObject:Object) : void
        {
            this.m_customPolicies = customPolicyObject;
            return;
        }// end function

        private native function get playStartTime() : Number;

        private native function get playbackEndTime() : Number;

        private native function get playbackWindow() : Number;

        public function get playbackTimeWindow() : DRMPlaybackTimeWindow
        {
            var _loc_1:uint = 0;
            if (this.m_playbackTimeWindow != null)
            {
                if (this.m_playbackTimeWindow.startDate == null)
                {
	            if (this.playStartTime > 0)
	            {
	                _loc_1 = this.playbackWindow;
	                if (this.playbackWindow > 0)
	                {
	                    this.m_playbackTimeWindow = this.createDRMPlaybackTimeWindow(_loc_1, this.playStartTime, this.playbackEndTime);
	                }
	            }                
                }
            }
            return this.m_playbackTimeWindow;
        }// end function

        public function toByteArray() : ByteArray
        {
            return this.toByteArrayInner();
        }// end function

        private native function createDRMPlaybackTimeWindow(length:Number, start:Number, end:Number) : DRMPlaybackTimeWindow;

        private native function toByteArrayInner() : ByteArray;
        
        //new adds 11
        public native function get licenseID():String
        public native function get policyID():String
        //new adds 11 end
}
}