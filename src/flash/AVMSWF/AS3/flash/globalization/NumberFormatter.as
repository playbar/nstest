package flash.globalization
{
[native(cls="NumberFormatterClass", methods="auto")]

public class NumberFormatter
{
	//Consts
	
	//Properties
	public native function get actualLocaleIDName() : String
	
	public native function get decimalSeparator() : String
	public native function set decimalSeparator(decimalSeparator : String):void
	
	public native function get digitsType() : uint
	public native function set digitsType(digitsType : uint):void
	
	public native function get fractionalDigits() : int
	public native function set fractionalDigits(fractionalDigits : int):void
	
	public native function get groupingPattern() : String
	public native function set groupingPattern(groupingPattern : String):void
	
	public native function get groupingSeparator() : String
	public native function set groupingSeparator(groupingSeparator : String):void
	
	public native function get lastOperationStatus() : String
	
	public native function get leadingZero() : Boolean
	public native function set leadingZero(leadingZero : Boolean):void
	
	public native function get negativeNumberFormat() : uint
	public native function set negativeNumberFormat(negativeNumberFormat : uint):void
	
	public native function get negativeSymbol() : String
	public native function set negativeSymbol(negativeSymbol : String):void
	
	public native function get requestedLocaleIDName() : String
	
	public native function get trailingZeros() : Boolean
	public native function set trailingZeros(trailingZeros : Boolean):void
	
	public native function get useGrouping() : Boolean
	public native function set useGrouping(useGrouping : Boolean):void
	
	//Methods
	public native function NumberFormatter(requestedLocaleIDName:String)
	public native function formatInt(value:int):String
	public native function formatNumber(value:Number):String
	public native function formatUint(value:uint):String
	public native function getAvailableLocaleIDNames():Vector.<String>
	public native function parse(parseString:String):NumberParseResult
	public native function parseNumber(parseString:String):Number
}
}