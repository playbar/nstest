package flash.data
{
[native(cls="SQLStatementClass", methods="auto")]

import flash.events.EventDispatcher

import flash.net.Responder

public class SQLStatement extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get executing() : Boolean
	
	public native function get itemClass() : Class
	public native function set itemClass(itemClass : Class):void
	
	public native function get parameters() : Object
	
	public native function get sqlConnection() : SQLConnection
	public native function set sqlConnection(sqlConnection : SQLConnection):void
	
	public native function get text() : String
	public native function set text(text : String):void
	
	//Methods
	public native function SQLStatement()
	public native function cancel():void
	public native function clearParameters():void
	public native function execute(prefetch:int = -1, responder:Responder = null):void
	public native function getResult():SQLResult
	public native function next(prefetch:int = -1, responder:Responder = null):void
}
}