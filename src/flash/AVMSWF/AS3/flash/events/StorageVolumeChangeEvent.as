package flash.events
{
[native(cls="StorageVolumeChangeEventClass", methods="auto")]

import flash.filesystem.File
import flash.filesystem.StorageVolume

public class StorageVolumeChangeEvent extends Event
{
	//Consts
	public static const STORAGE_VOLUME_MOUNT : String = "storageVolumeMount"
	public static const STORAGE_VOLUME_UNMOUNT : String = "storageVolumeUnmount"
	
	//Properties
	public native function get rootDirectory() : File
	
	public native function get storageVolume() : StorageVolume
	
	//Methods
	public native function StorageVolumeChangeEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, path:File = null, volume:StorageVolume = null)
	public native override function clone():Event
	public native override function toString():String
}
}