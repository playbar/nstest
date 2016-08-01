package flash.geom
{
[native(cls="Utils3DClass", methods="auto")]

public class Utils3D
{
	//Consts
	
	//Properties
	
	//Methods
	public native static function pointTowards(percent:Number, mat:Matrix3D, pos:Vector3D, at:Vector3D = null, up:Vector3D = null):Matrix3D
	public native static function projectVector(m:Matrix3D, v:Vector3D):Vector3D
	public native static function projectVectors(m:Matrix3D, verts:Vector.<Number>, projectedVerts:Vector.<Number>, uvts:Vector.<Number>):void
}
}