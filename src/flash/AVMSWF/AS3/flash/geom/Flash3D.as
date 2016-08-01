package flash.geom
{
	[native(cls="Vector3DClass", methods="auto")]
	public class Vector3D
	{
	public static var X_AXIS=new Vector3D(1,0,0);
	public static var Y_AXIS=new Vector3D(0,1,0);
	public static var Z_AXIS=new Vector3D(0,0,1);
	public  var x:Number;
	public  var y:Number;
	public  var z:Number;
	public  var w:Number;
	public function get length():Number
	{
		var l:Number=x*x+y*y+z*z;
		return Math.sqrt(l);
	}
	public function get lengthSquared():Number
	{
		return x*x+y*y+z*z;
	}
	public function Vector3D(x:Number = 0., y:Number = 0., z:Number = 0., w:Number = 0.)
	{
		this.x=x;
		this.y=y;
		this.z=z;
		this.w=w;
	}
	public function add(a:Vector3D):Vector3D
	{
		var v=new Vector3D(x,y,z,0);
		v.x+=a.x;
		v.y+=a.y;
		v.z+=a.z;
		return v;
	}
	public static function angleBetween(a:Vector3D, b:Vector3D):Number
	{
		var l:Number=b.length;
		var l1:Number=a.length;
		if(l==0||l1==0) return Math.PI/2;
		var m:Number=(a.x*b.x+a.y*b.y+a.z*b.z)/(l*l1);
		return Math.acos(m);
	}
	public function clone():Vector3D
	{
		return new Vector3D(x,y,z,w);
	}
	public function crossProduct(a:Vector3D):Vector3D
	{
		//²æ»ý¹«Ê½£ºu x v = (u2v3-v2u3 , u3v1-v3u1 , u1v2-u2v1 )
		return new Vector3D(y*a.z-a.y*z,z*a.x-a.z*x,x*a.y-y*a.x);
	}
	public function decrementBy(a:Vector3D):void
	{
		x-=a.x;
		y-=a.y;
		z-=a.z;
	}
	public static function distance(a:Vector3D, b:Vector3D):Number
	{
		//float  z;
		var dx:Number=a.x-b.x;
		var dy:Number=a.y-b.y;
		var dz:Number=a.z-b.z;
		return Math.sqrt(dx*dx+dy*dy+dz*dz);
	}
	public function dotProduct(a:Vector3D):Number
	{
		//u * v = u1v1+u2v2+u3v3
		return x*a.x+y*a.y+z*a.z;
	}
	public function equals(a:Vector3D, all:Boolean = false):Boolean
	{
		if(x!=a.x||y!=a.y||z!=a.z) return false;
		return all?w==a.w:true;
	}
	public function incrementBy(a:Vector3D):void
	{
		x+=a.x;
		y+=a.y;
		z+=a.z;
	}
	public function nearEquals(a:Vector3D, d:Number, all:Boolean = false):Boolean
	{
		var dd:Number=x-a.x;
		if(dd<0) dd=-dd;
		if(dd>d) return false;
		dd=y-a.y;
		if(dd<0) dd=-dd;
		if(dd>d) return false;
		dd=z-a.z;
		if(dd<0) dd=-dd;
		if(dd>d) return false;
		if(!all) return true;
		dd=y-a.y;
		if(dd<0) dd=-dd;
		if(dd>d) return false;
		return true;
	}
	public function negate():void
	{
		x=-x;
		y=-y;
		z=-z;
	}
	public function normalize():Number
	{
		var l:Number=length();
		if(l!=0)
		{
			x=x/l;
			y=y/l;
			z=z/l;
		}
		return l;
	}
	public function project():void
	{
		if(w!=0)
		{
			x=x/w;
			y=y/w;
			z=z/w;
		}
	}
	public function scaleBy(s:Number):void
	{
		x*=s;
		y*=s;
		z*=s;
	}
	public function subtract(a:Vector3D):Vector3D
	{
		return new Vector3D(x-a.x,y-a.y,z-a.z);
	}
	public function toString():String
	{
		return "(x="+String(x)+",y="+String(y)+",z="+String(z)+")";
	}
	}
	[native(cls="Matrix3DClass", methods="auto")]
	public class Matrix3D
	{
		public var rowData:Vector.<Number>
		public function get determinant():Number
		{
			//a11*a22*a33*a44+a12*a23*a34*a41+a13*a21*a32*a43+a14*
			return rowData[0]*rowData[5]*rowData[10]*rowData[15]+
				   rowData[1]*rowData[6]*rowData[11]*rowData[12]+
				   rowData[2]*rowData[7]*rowData[8]*rowData[13]+
				   rowData[3]*rowData[4]*rowData[9]*rowData[14]-
				   rowData[12]*rowData[9]*rowData[6]*rowData[3]-
				   rowData[8]*rowData[5]*rowData[3]*rowData[15]-
				   rowData[4]*rowData[1]*rowData[14]*rowData[11]-
				   rowData[0]*rowData[13]*rowData[10]*rowData[7];
		}
		public function get position():Vector3D
		{
			return Vector3D(rowData[3],rowData[7],rowData[11],rowData[15]);
		}
		public function set position(a:Vector3D):void
		{
			rowData[3]=a.x;
			rowData[7]=a.y;
			rowdata[11]=a.z;
			rowData[15]=a.w;
		}
		public function Matrix3D(v:Vector.<Number> = null)
		{
			rowData=new Vector.<Number>(16);
			if(v!=null)
			{
				for(var i=0;i<16;i++)
				{
					rowData[i]=v[i];
				}
			}
		}
		public function append(lhs:Matrix3D):void
		{
			
		}
		public function appendRotation(degrees:Number, axis:Vector3D, pivotPoint:Vector3D = null):void
		{
			
		}
		public function appendScale(xScale:Number, yScale:Number, zScale:Number):void
		{
		}
		public function appendTranslation(x:Number, y:Number, z:Number):void
		{
		}
		public function clone():Matrix3D
		{
			return new Matrix3D(rowData);
		}
		public function decompose(orientationStyle:String = "eulerAngles"):Vector.<Vector3D>
		{
			var v:Vector.<Vector3D>=new Vector.<Vector3D>(3);
			return v;
		}
		public function deltaTransformVector(v:Vector3D):Vector3D
		{
			return v;
		}
		public function identity():void
		{
			rowData[0]=1;
			rowData[5]=1;
			rowData[7]=1;
			rowData[15]=1;
			rowData[1]=rowData[2]=rowData[3]=0;
			rowData[4]=rowData[6]=rowData[7]=0;
			rowData[8]=rowData[9]=rowData[11]=0;
			rowData[12]=rowData[13]=rowData[14]=0;
		}
		public function identity():void
		{
			
		}
		public function invert():Boolean
		{
			return true;
		}
		public function pointAt(pos:Vector3D, at:Vector3D = null, up:Vector3D = null):void
		{
		}
		public function prepend(rhs:Matrix3D):void
		{
		}
		public function prependRotation(degrees:Number, axis:Vector3D, pivotPoint:Vector3D = null):void
		{
		}
		public function prependScale(xScale:Number, yScale:Number, zScale:Number):void
		{
		}
		public function prependTranslation(x:Number, y:Number, z:Number):void
		{
		}
		public function recompose(components:Vector.<Vector3D>, orientationStyle:String = "eulerAngles"):Boolean
		{
		}
		public function transformVector(v:Vector3D):Vector3D
		{
			return v;
		}
		public function transformVectors(vin:Vector.<Number>, vout:Vector.<Number>):void
		{			
		}
		public function transpose():void
		{
		}
		
	}
	public class PerspectiveProjection 
	{
		public function get fieldOfView():Number
		{
			return 0;
		}
		public function set fieldOfView(value:Number):void
		{
		}
		public function get focalLength():Number
		{
			return 100;
		}
		public function set focalLength(value:Number):void
		{
		}
		public function get projectionCenter():Point
		{
		}
	    public function set projectionCenter(value:Point):void
		{
			return new Point(200,200);
		}
		public function PerspectiveProjection()
		{
		}
		public function toMatrix3D():Matrix3D
		{
			var data:Vector.<Number>=new Vector.<Number>(16);
			return new Matrix3D(data);
		}

	}
	public class Orientation3D 
	{
		public static const AXIS_ANGLE:String = "axisAngle"
		public static const EULER_ANGLES:String = "eulerAngles"
		public static const QUATERNION:String = "quaternion"
	}
	[native(cls="Utils3DClass", methods="auto")]
	public class Utils3D
	{
		
		public static function pointTowards(percent:Number, mat:Matrix3D, pos:Vector3D, at:Vector3D = null, up:Vector3D = null):Matrix3D
		{
			
		}
		public static function projectVector(m:Matrix3D, v:Vector3D):Vector3D
		{
		
		}
		public static function projectVectors(m:Matrix3D, verts:Vector.<Number>, projectedVerts:Vector.<Number>, uvts:Vector.<Number>):void
		{
		
		}
	}
}