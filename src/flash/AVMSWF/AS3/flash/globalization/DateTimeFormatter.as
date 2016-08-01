package flash.globalization
{
[native(cls="DateTimeFormatterClass", methods="auto")]

public class DateTimeFormatter
{
	//Consts
	
	//Properties
	public native function get actualLocaleIDName() : String
	
	public native function get lastOperationStatus() : String
	
	public native function get requestedLocaleIDName() : String
	
	//Methods
	public native function DateTimeFormatter(requestedLocaleIDName:String, dateStyle:String = "long", timeStyle:String = "long")
	public native function format(dateTime:Date):String
	public native function formatUTC(dateTime:Date):String
	public native static function getAvailableLocaleIDNames():Vector.<String>
	public native function getDateStyle():String
	public native function getDateTimePattern():String
	public native function getFirstWeekday():int
	public native function getMonthNames(nameStyle:String = "full", context:String = "standalone"):Vector.<String>
	public native function getTimeStyle():String
	public native function getWeekdayNames(nameStyle:String = "full", context:String = "standalone"):Vector.<String>
	public native function setDateTimePattern(pattern:String):void
	public native function setDateTimeStyles(dateStyle:String, timeStyle:String):void
}
}