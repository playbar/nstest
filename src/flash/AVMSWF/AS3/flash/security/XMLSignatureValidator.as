package flash.security
{
[native(cls="XMLSignatureValidatorClass", methods="auto")]

import flash.events.EventDispatcher

import flash.utils.ByteArray

public class XMLSignatureValidator extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get digestStatus() : String
	
	public native function get identityStatus() : String
	
	public native static function get isSupported() : Boolean
	
	public native function get referencesStatus() : String
	
	public native function get referencesValidationSetting() : String
	public native function set referencesValidationSetting(referencesValidationSetting : String):void
	
	public native function get revocationCheckSetting() : String
	public native function set revocationCheckSetting(revocationCheckSetting : String):void
	
	public native function get signerCN() : String
	
	public native function get signerDN() : String
	
	public native function get signerExtendedKeyUsages() : Array
	
	public native function get signerTrustSettings() : Array
	
	public native function get uriDereferencer() : IURIDereferencer
	public native function set uriDereferencer(uriDereferencer : IURIDereferencer):void
	
	public native function get useSystemTrustStore() : Boolean
	public native function set useSystemTrustStore(useSystemTrustStore : Boolean):void
	
	public native function get validityStatus() : String
	
	//Methods
	public native function XMLSignatureValidator()
	public native function addCertificate(cert:ByteArray, trusted:Boolean):*
	public native function verify(signature:XML):void
}
}