package flash.net.drm
{
[native(cls="VoucherAccessInfoClass", methods="auto")]

public class VoucherAccessInfo
{
	//Consts
	
	//Properties
//	public native function get authenticationMethod() : String
	
//	public native function get displayName() : String
	
//	public native function get domain() : String
	
	//Methods
        private var m_displayName:String;
        private var m_authMethod:String;
        private var m_domain:String;
        private var m_deviceGroup:DRMDeviceGroup;
        private var m_policyID:String;

        public function VoucherAccessInfo()
        {
            return;
        }// end function

        public function get displayName() : String
        {
            return this.m_displayName;
        }// end function

        public function get authenticationMethod() : String
        {
            return this.m_authMethod;
        }// end function

        public function get domain() : String
        {
            return this.m_domain;
        }// end function

        private function setFields(inDisplayName:String, inAuthMethod:String, inDomain:String, inDeviceGroup:DRMDeviceGroup, inPolicyID:String)
        {
            this.m_displayName = inDisplayName;
            this.m_authMethod = inAuthMethod;
            this.m_domain = inDomain;
            this.m_deviceGroup = inDeviceGroup;
            this.m_policyID = inPolicyID;
            return;
        }// end function
        
        //new adds 11
        public function get deviceGroup():DRMDeviceGroup
        {
			return this.m_deviceGroup;
        }// end function
        
        public function get policyID():String
        {
			return this.m_policyID;
        }// end function
        
        //new adds 11 end 
}
}