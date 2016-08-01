package flash.data
{
[native(cls="SQLConnectionClass", methods="auto")]

import flash.events.EventDispatcher

import flash.net.Responder
import flash.utils.ByteArray

public class SQLConnection extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get autoCompact() : Boolean
	
	public native function get cacheSize() : uint
	public native function set cacheSize(cacheSize : uint):void
	
	public native function get columnNameStyle() : String
	public native function set columnNameStyle(columnNameStyle : String):void
	
	public native function get connected() : Boolean
	
	public native function get inTransaction() : Boolean
	
	public native function get lastInsertRowID() : Number
	
	public native function get pageSize() : uint
	
	public native function get totalChanges() : Number
	
	//new adds 11
	public static native function get isSupported():Boolean
	//new adds 11 end
	
	//Methods
	public native function SQLConnection()
	public native override function addEventListener(type:String, listener:Function, useCapture:Boolean = false, priority:int = 0.0, useWeakReference:Boolean = false):void
	public native function analyze(resourceName:String = null, responder:Responder = null):void
	public native function attach(name:String, reference:Object = null, responder:Responder = null, encryptionKey:ByteArray = null):void
	public native function begin(option:String = null, responder:Responder = null):void
	public native function cancel(responder:Responder = null):void
	public native function close(responder:Responder = null):void
	public native function commit(responder:Responder = null):void
	public native function compact(responder:Responder = null):void
	public native function deanalyze(responder:Responder = null):void
	public native function detach(name:String, responder:Responder = null):void
	public native function getSchemaResult():SQLSchemaResult
	public native function loadSchema(type:Class = null, name:String = null, database:String = "main", includeColumnSchema:Boolean = true, responder:Responder = null):void
	public native function open(reference:Object = null, openMode:String = "create", autoCompact:Boolean = false, pageSize:int = 1024, encryptionKey:ByteArray = null):void
	public native function openAsync(reference:Object = null, openMode:String = "create", responder:Responder = null, autoCompact:Boolean = false, pageSize:int = 1024, encryptionKey:ByteArray = null):void
	public native function reencrypt(newEncryptionKey:ByteArray, responder:Responder = null):void
	public native function releaseSavepoint(name:String = null, responder:Responder = null):void
	public native override function removeEventListener(type:String, listener:Function, useCapture:Boolean = false):void
	public native function rollback(responder:Responder = null):void
	public native function rollbackToSavepoint(name:String = null, responder:Responder = null):void
	public native function setSavepoint(name:String = null, responder:Responder = null):void
}
}