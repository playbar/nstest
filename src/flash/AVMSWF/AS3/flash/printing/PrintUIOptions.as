package flash.printing
{
[native(cls="PrintUIOptionsClass", methods="auto")]

public class PrintUIOptions
{
	//Consts
	
	//Properties
	public native function get disablePageRange() : Boolean 
	public native function set disablePageRange(disablePageRange : Boolean):void
	
	public native function get maxPage() : uint
	public native function set maxPage(maxPage : uint):void
	
	public native function get minPage() : uint 
	public native function set minPage(minPage : uint):void
	
	//Methods
	public native function PrintUIOptions()
}
}