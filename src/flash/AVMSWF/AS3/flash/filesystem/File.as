package flash.filesystem
{
[native(cls="FileClass", methods="auto")]

import flash.events.EventDispatcher
import flash.net.FileReference

public class File extends FileReference
{
	//Consts
	
	//Properties
	public native static function get applicationDirectory() : File		//s
	
	public native static function get applicationStorageDirectory() : File 	//s
	
	public native static function get desktopDirectory() : File		//s
	
	public native static function get documentsDirectory() : File		//s
	
	public native function get downloaded() : Boolean
	public native function set downloaded(downloaded : Boolean):void
	
	public native function get exists() : Boolean
	
	public native function get isDirectory() : Boolean
	
	public native function get isHidden() : Boolean
	
	public native function get isPackage() : Boolean
	
	public native function get isSymbolicLink() : Boolean
	
	public native static function get lineEnding() : String		//s
	
	public native function get nativePath() : String
	public native function set nativePath(nativePath : String):void
	
	public native function get parent() : File
	
	public native static function get separator() : String		//s
	
	public native function get spaceAvailable() : Number
	
	public native static function get systemCharset() : String	//s
	
	public native function get url() : String
	public native function set url(url : String):void
	
	public native static function get userDirectory() : File		//s
	
	//new adds 11
	public static native function get cacheDirectory():File
	public native function get preventBackup():Boolean
    public native function set preventBackup(value:Boolean):void
	//new adds 11 end
	
	//Methods
	public native function File(path:String = null)
	public native function browseForDirectory(title:String):void
	public native function browseForOpen(title:String, typeFilter:Array = null):void
	public native function browseForOpenMultiple(title:String, typeFilter:Array = null):void
	public native function browseForSave(title:String):void
	public native override function cancel():void
	public native function canonicalize():void
	public native function clone():File
	public native function copyTo(newLocation:FileReference, overwrite:Boolean = false):void
	public native function copyToAsync(newLocation:FileReference, overwrite:Boolean = false):void
	public native function createDirectory():void
	public native static function createTempDirectory():File
	public native static function createTempFile():File
	public native function deleteDirectory(deleteDirectoryContents:Boolean = false):void
	public native function deleteDirectoryAsync(deleteDirectoryContents:Boolean = false):void
	public native function deleteFile():void
	public native function deleteFileAsync():void
	public native function getDirectoryListing():Array
	public native function getDirectoryListingAsync():void
	public native function getRelativePath(ref:FileReference, useDotDot:Boolean = false):String
	public native static function getRootDirectories():Array
	public native function moveTo(newLocation:FileReference, overwrite:Boolean = false):void
	public native function moveToAsync(newLocation:FileReference, overwrite:Boolean = false):void
	public native function moveToTrash():void
	public native function moveToTrashAsync():void
	public native function openWithDefaultApplication():void
	public native function resolvePath(path:String):File
}
}