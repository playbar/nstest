package flash.globalization
{
[native(cls="StringToolsClass", methods="auto")]

public class StringTools
{
	//Consts
	
	//Properties
	public native function get actualLocaleIDName() : String
	
	public native function get lastOperationStatus() : String
	
	public native function get requestedLocaleIDName() : String
	
	//Methods
	public native function StringTools(requestedLocaleIDName:String)
	public native static function getAvailableLocaleIDNames():Vector.<String>
	public native function toLowerCase(s:String):String
	public native function toUpperCase(s:String):String
}
}