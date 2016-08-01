package flash.globalization
{
[native(cls="CurrencyFormatterClass", methods="auto")]

public class CurrencyFormatter
{
	//Consts
	
	//Properties
	public native function get actualLocaleIDName() : String
	
	public native function get currencyISOCode() : String
	
	public native function get currencySymbol() : String
	
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
	
	public native function get negativeCurrencyFormat() : uint
	public native function set negativeCurrencyFormat(negativeCurrencyFormat : uint):void
	
	public native function get negativeSymbol() : String
	public native function set negativeSymbol(negativeSymbol : String):void
	
	public native function get positiveCurrencyFormat() : uint
	public native function set positiveCurrencyFormat(positiveCurrencyFormat : uint):void
	
	public native function get requestedLocaleIDName() : String
	
	public native function get trailingZeros() : Boolean
	public native function set trailingZeros(trailingZeros : Boolean):void
	
	public native function get useGrouping() : Boolean
	public native function set useGrouping(useGrouping : Boolean):void
	
	//Methods
	public native function CurrencyFormatter(requestedLocaleIDName:String)
	public native function format(value:Number, withCurrencySymbol:Boolean = false):String
	public native function formattingWithCurrencySymbolIsSafe(requestedISOCode:String):Boolean
	public native static function getAvailableLocaleIDNames():Vector.<String>
	public native function parse(inputString:String):CurrencyParseResult
	public native function setCurrency(currencyISOCode:String, currencySymbol:String):void
}
}