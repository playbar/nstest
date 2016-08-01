package flash.globalization
{
[native(cls="LocaleIDClass", methods="auto")]

public class LocaleID
{
	//Consts
	public static const DEFAULT : String = "i-default"
	
	//Properties
	public native function get lastOperationStatus() : String
	
	public native function get name() : String
	
	//Methods
	public native function LocaleID(name:String)
	public native static function determinePreferredLocales(want:Vector.<String>, have:Vector.<String>, keyword:String = "userinterface"):Vector.<String>
	public native function getKeysAndValues():Object
	public native function getLanguage():String
	public native function getRegion():String
	public native function getScript():String
	public native function getVariant():String
	public native function isRightToLeft():Boolean
}
}