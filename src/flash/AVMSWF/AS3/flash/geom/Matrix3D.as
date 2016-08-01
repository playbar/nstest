package flash.geom
{
[native(cls="Matrix3DClass", methods="auto")]

public class Matrix3D
{
	//Consts
	
	//Properties
	public native function get determinant() : Number
	
	public native function get position() : Vector3D
	public native function set position(position : Vector3D):void
	
	public native function get rawData() : Vector.<Number>
	public native function set rawData(rawData : Vector.<Number>):void
	
	//Methods
	public native function Matrix3D(v:Vector.<Number> = null)
	public native function append(lhs:Matrix3D):void
	public native function appendRotation(degrees:Number, axis:Vector3D, pivotPoint:Vector3D = null):void
	public native function appendScale(xScale:Number, yScale:Number, zScale:Number):void
	public native function appendTranslation(x:Number, y:Number, z:Number):void

	public native function clone():Matrix3D
	public native function decompose(orientationStyle:String = "eulerAngles"):Vector.<Vector3D>
	public native function deltaTransformVector(v:Vector3D):Vector3D
	public native function identity():void
	public native static function interpolate(thisMat:Matrix3D, toMat:Matrix3D, percent:Number):Matrix3D

	public native function interpolateTo(toMat:Matrix3D, percent:Number):void
	public native function invert():Boolean
	public native function pointAt(pos:Vector3D, at:Vector3D = null, up:Vector3D = null):void
	public native function prepend(rhs:Matrix3D):void
	public native function prependRotation(degrees:Number, axis:Vector3D, pivotPoint:Vector3D = null):void

	public native function prependScale(xScale:Number, yScale:Number, zScale:Number):void
	public native function prependTranslation(x:Number, y:Number, z:Number):void
	public native function recompose(components:Vector.<Vector3D>, orientationStyle:String = "eulerAngles"):Boolean
	public native function transformVector(v:Vector3D):Vector3D
	public native function transformVectors(vin:Vector.<Number>, vout:Vector.<Number>):void

	public native function transpose():void
	
	//new adds 11
	public native function copyColumnFrom(column:uint, vector3D:Vector3D):void
	public native function copyColumnTo(column:uint, vector3D:Vector3D):void
	public native function copyFrom(sourceMatrix3D:Matrix3D):void
	public native function copyRawDataFrom(vector:Vector.<Number>, index:uint = 0, transpose:Boolean = false):void
	public native function copyRawDataTo(vector:Vector.<Number>, index:uint = 0, transpose:Boolean = false):void
	public native function copyRowFrom(row:uint, vector3D:Vector3D):void
	public native function copyRowTo(row:uint, vector3D:Vector3D):void
	public native function copyToMatrix3D(dest:Matrix3D):void
	//new adds 11 end
}
}