package flash.filesystem
{
[native(cls="StorageVolumeClass", methods="auto")]

public class StorageVolume
{
	//Consts
	
	//Properties
	public native function get drive() : String
	
	public native function get fileSystemType() : String
	
	public native function get isRemovable() : Boolean
	
	public native function get isWritable() : Boolean
	
	public native function get name() : String
	
	public native function get rootDirectory() : File
	
	//Methods
	public native function StorageVolume(rootDirPath:File, name:String, writable:Boolean, removable:Boolean, fileSysType:String, drive:String)
}
}