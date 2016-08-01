package flash.system
{
[native(cls="JPEGLoaderContextClass", methods="auto")]

public class JPEGLoaderContext extends LoaderContext
{
	//Consts
	
	//Properties
//	public native function get deblockingFilter() : Number
//	public native function set deblockingFilter(deblockingFilter : Number):void
	
	//Methods
//	public native function JPEGLoaderContext(deblockingFilter:Number = 0.0, checkPolicyFile:Boolean = false, applicationDomain:ApplicationDomain = null, securityDomain:SecurityDomain = null)
        public var deblockingFilter:Number = 0;

        public function JPEGLoaderContext(deblockingFilter:Number = 0, checkPolicyFile:Boolean = false, applicationDomain:ApplicationDomain = null, securityDomain:SecurityDomain = null)
        {
            super(checkPolicyFile, applicationDomain, securityDomain);
            this.deblockingFilter = deblockingFilter;
            return;
        }// end function
}
}