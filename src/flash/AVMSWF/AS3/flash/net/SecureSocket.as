package flash.net
{
[native(cls="SecureSocketClass", methods="auto")]
import flash.utils.ByteArray
import flash.security.X509Certificate
public class SecureSocket extends Socket
{
	//Consts
	
	//Properties
	public native static function get isSupported() : Boolean
	public native function get serverCertificate():X509Certificate //new add
	public native function get serverCertificateStatus() : String
	
	//Methods
	public native function SecureSocket()
	public native function addBinaryChainBuildingCertificate(certificate:ByteArray, trusted:Boolean):void  //new add
	public native override function connect(host:String, port:int):void
}
}