package JavaScript
{
	public class Location extends Object
	{
		public native function get hash():String
		public native function set hash(value:String):void
		public native function get host():String
		public native function set host(value:String):void
		public native function get hostname():String
		public native function set hostname(value:String):void
		public native function get href():String
		public native function set href(value:String):void
		public native function get pathname():String
		public native function set pathname(value:String):void
		public native function get port():String
		public native function set port(value:String):void
		public native function get protocol():String
		public native function set protocol(value:String):void
		public native function get search():String
		public native function set search(value:String):void
		public native function assign(value:String):void
		public native function reload():void
		public native function replace(value:String):void
		public native function toString():String
	}
	public class Navigator extends Object
	{
		public native function get appCodeName():String
		public native function get appMinorVersion():String
		public native function get appName():String
		public native function get appVersion():String
		public native function get browserLanguage():String
		public native function get cookieEnabled():String
		public native function get cpuClass():String
		public native function get onLine():String
		public native function get platform():String
		public native function get systemLanguage():String
		public native function get userAgent():String
		public native function get userLanguage():String
		public native function javaEnabled():Boolean		
		public native function taintEnabled():Boolean
	}
	public class Window extends Object
	{		
		//public native function Window()
		public native function get location():Location		
		public native function set location(url:Location):void
		public native function get navigator():Navigator;
	}
	//var navigator:Navigator=new Navigator();
	public native function get window():Window;
	public function get location():Location
	{return window.location;}
	public function get navigator():Navigator
	{return window.navigator;}
	
}