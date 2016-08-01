package flash.accessibility
{
import flash.geom.*;

[native(cls="AccessibilityImplementationClass", methods="auto")]

public class AccessibilityImplementation
{
	//Consts
	
	//Properties
	public native function get errno() : uint
	public native function set errno(errno : uint):void
	
	public native function get stub() : Boolean
	public native function set stub(stub : Boolean):void
	
	//Methods
	public native static function AccessibilityImplementation()	//¾²Ì¬¹¹Ôìº¯Êý¡£
	public native function accDoDefaultAction(childID:uint):void
	public native function accLocation(childID:uint):*
	public native function accSelect(operation:uint, childID:uint):void
	public native function get_accDefaultAction(childID:uint):String
	public native function get_accFocus():uint
	public native function get_accName(childID:uint):String
	public native function get_accRole(childID:uint):uint
	public native function get_accSelection():Array
	public native function get_accState(childID:uint):uint
	public native function get_accValue(childID:uint):String
	public native function getChildIDArray():Array
	public native function isLabeledBy(labelBounds:Rectangle):Boolean
}
}