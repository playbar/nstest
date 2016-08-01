package flash.globalization
{
[native(cls="NumberParseResultClass", methods="auto")]

public class NumberParseResult
{
	//Consts
	
	//Properties
	public native function get endIndex() : int
	
	public native function get startIndex() : int
	
	public native function get value() : Number
	
	//Methods
	public native function NumberParseResult(value:Number = NaN, startIndex:int = 0x7fffffff, endIndex:int = 0x7fffffff)
}
}