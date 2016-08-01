package flash.sampler
{
[native(cls="NewObjectSampleClass", methods="auto")]

public class NewObjectSample extends Sample
{
	//Consts
	public const id:Number
	public const type : Class
	
	//Properties
	public native function get object() 
	
	public native function get size() : Number
	
	//Methods
}
}