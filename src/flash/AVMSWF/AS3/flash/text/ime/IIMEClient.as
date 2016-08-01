package flash.text.ime
{
[native(cls="IIMEClientClass", methods="auto")]

import flash.geom.Rectangle

public interface IIMEClient
{
	//Consts
	
	//Properties
	function get compositionEndIndex() : int
	
	function get compositionStartIndex() : int
	
	function get selectionActiveIndex() : int
	
	function get selectionAnchorIndex() : int
	
	function get verticalTextLayout() : Boolean
	
	//Methods
	function confirmComposition(text:String = null, preserveSelection:Boolean = false):void
	function getTextBounds(startIndex:int, endIndex:int):Rectangle
	function getTextInRange(startIndex:int, endIndex:int):String
	function selectRange(anchorIndex:int, activeIndex:int):void
	function updateComposition(text:String, attributes:Vector.<CompositionAttributeRange>, compositionStartIndex:int, compositionEndIndex:int):void
}
}