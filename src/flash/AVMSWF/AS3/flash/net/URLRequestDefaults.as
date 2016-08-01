package flash.net
{
[native(cls="URLRequestDefaultsClass", methods="auto")]

public class URLRequestDefaults
{
	//Consts
	
	//Properties
	public native static function get authenticate() : Boolean
	public native static function set authenticate(authenticate : Boolean):void
	
	public native static function get cacheResponse() : Boolean
	public native static function set cacheResponse(cacheResponse : Boolean):void
	
	public native static function get followRedirects() : Boolean
	public native static function set followRedirects(followRedirects : Boolean):void
	
	public native static function get idleTimeout() : Number
	public native static function set idleTimeout(idleTimeout : Number):void
	
	public native static function get manageCookies() : Boolean
	public native static function set manageCookies(manageCookies : Boolean):void
	
	public native static function get useCache() : Boolean
	public native static function set useCache(useCache : Boolean):void
	
	public native static function get userAgent() : String
	public native static function set userAgent(userAgent : String):void
	
	//Methods
	public native static function setLoginCredentialsForHost(hostname:String, user:String, password:String):*
}
}