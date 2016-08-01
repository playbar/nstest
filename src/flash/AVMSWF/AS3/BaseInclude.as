package
{
	public native function nativetrace(arguments:Array):void;
	public function trace(... arguments):void
	{
		nativetrace(arguments);
	}
}
package flash.profiler
{
	public native function showRedrawRegions(on:Boolean, color:uint = 0xFF0000):void 
}


package flash.net
{
	public native function getClassByAlias (aliasName:String):Class
	public native function navigateToURL(request:URLRequest, window:String = null):void 
	public native function registerClassAlias(aliasName:String, classObject:Class):void 
	public native function sendToURL(request:URLRequest):void
}

package flash.utils
{
	public native function clearInterval(id:uint):void//Cancels a specified setInterval() call. flash.utils 
	public native function clearTimeout(id:uint):void //Cancels a specified setTimeout() call. flash.utils 
	//public native function describeType(value:*):XML //Produces an XML object that describes the ActionScript object named as the parameter of the method. flash.utils 
    public native function escapeMultiByte(value:String):String //Returns an escaped copy of the input string encoded as either UTF-8 or system code page, depending on the value of System.useCodePage. flash.utils 
    public native function getDefinitionByName(name:String):Object //Returns a reference to the class object of the class specified by the name parameter. flash.utils 
    //public native function getQualifiedClassName(value:*):String //Returns the fully qualified class name of an object. flash.utils 
    //public native function getQualifiedSuperclassName(value:*):String //Returns the fully qualified class name of the base class of the object specified by the value parameter. flash.utils 
    public native function getTimer():int //Returns the number of milliseconds that have elapsed since Flash Player was initialized, and is used to compute relative time. flash.utils 
    public native function nativeSetInterval(closure:Function,delay:Number,arguments:Array):uint;
    public function setInterval(closure:Function, delay:Number, ... arguments):uint //Runs a function at a specified interval (in milliseconds). flash.utils 
    {
		return nativeSetInterval(closure,delay,arguments);
    }
    public native function nativeSetTimeout(closure:Function, delay:Number, arguments:Array):uint
    public function setTimeout(closure:Function, delay:Number, ... arguments):uint //Runs a specified function after a specified delay (in milliseconds). flash.utils 
    {
		return nativeSetTimeout(closure,delay,arguments);
    }
    public native function unescapeMultiByte(value:String):String //Returns an unescaped copy of the input string, which is decoded from either system code page page or UTF-8 depending on the value of System.useCodePage. 
}

//include "Flash3D.as"