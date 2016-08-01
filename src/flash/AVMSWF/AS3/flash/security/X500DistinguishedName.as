package flash.security
{
    [native(cls="X500DistinguishedNameClass", methods="auto")]
	
    public class X500DistinguishedName
    {

        public function X500DistinguishedName()
        {
            return;
        }// end function

        public native function get commonName() : String;

        public native function get organizationName() : String;

        public native function get organizationalUnitName() : String;

        public native function get localityName() : String;

        public native function get stateOrProvinceName() : String;

        public native function get countryName() : String;

        public native function toString() : String;

    }
}
