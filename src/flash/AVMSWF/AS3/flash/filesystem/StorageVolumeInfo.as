package flash.filesystem
{
[native(cls="StorageVolumeInfoClass", methods="auto")]

import flash.events.EventDispatcher

public class StorageVolumeInfo extends EventDispatcher
{
	//Consts
	
	//Properties
	public native static function get isSupported() : Boolean
	
	public native static function get storageVolumeInfo() : StorageVolumeInfo
	
	//Methods
	public native function getStorageVolumes():Vector.<StorageVolume>
}
}