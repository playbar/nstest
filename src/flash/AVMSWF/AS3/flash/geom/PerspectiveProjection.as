package flash.geom
{
[native(cls="PerspectiveProjectionClass", methods="auto")]

public class PerspectiveProjection
{
	//Consts
	
	//Properties
	public native function get fieldOfView() : Number
	public native function set fieldOfView(fieldOfView : Number):void
	
	public native function get focalLength() : Number
	public native function set focalLength(focalLength : Number):void
	
	public native function get projectionCenter() : Point
	public native function set projectionCenter(projectionCenter : Point):void
	
	//Methods
	public native function PerspectiveProjection()
	public native function toMatrix3D():Matrix3D
}
}