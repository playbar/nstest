package flash.globalization
{
[native(cls="CollatorClass", methods="auto")]

public class Collator
{
	//Consts
	
	//Properties
	public native function get actualLocaleIDName() : String
	
	public native function get ignoreCase() : Boolean
	public native function set ignoreCase(ignoreCase : Boolean):void
	
	public native function get ignoreCharacterWidth() : Boolean
	public native function set ignoreCharacterWidth(ignoreCharacterWidth : Boolean):void
	
	public native function get ignoreDiacritics() : Boolean
	public native function set ignoreDiacritics(ignoreDiacritics : Boolean):void
	
	public native function get ignoreKanaType() : Boolean
	public native function set ignoreKanaType(ignoreKanaType : Boolean):void
	
	public native function get ignoreSymbols() : Boolean
	public native function set ignoreSymbols(ignoreSymbols : Boolean):void
	
	public native function get lastOperationStatus() : String
	
	public native function get numericComparison() : Boolean
	public native function set numericComparison(numericComparison : Boolean):void
	
	public native function get requestedLocaleIDName() : String
	
	//Methods
	public native function Collator(requestedLocaleIDName:String, initialMode:String = "sorting")
	public native function compare(string1:String, string2:String):int
	public native function equals(string1:String, string2:String):Boolean
	public native static function getAvailableLocaleIDNames():Vector.<String>
}
}