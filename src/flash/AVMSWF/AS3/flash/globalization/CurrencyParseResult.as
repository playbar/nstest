package flash.globalization
{
[native(cls="CurrencyParseResultClass", methods="auto")]

public class CurrencyParseResult
{
	//Consts
	
	//Properties
	public native function get currencyString() : String
	
	public native function get value() : Number
	
	//Methods
	public native function CurrencyParseResult(value:Number = NaN, symbol:String = "")
}
}