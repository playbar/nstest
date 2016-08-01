package avmplus
{
	import flash.utils.ByteArray
	
	[native(cls="DomainClass", methods="auto")]
	public final class Domain
	{
		public native function get domainMemory():ByteArray;
		public native function set domainMemory(newValue:ByteArray):void
		public native function Domain();				
	}
}
package
{
	import avmplus.Domain
	public function get currentDomain():Domain
	{
		return new Domain();	
	}
}