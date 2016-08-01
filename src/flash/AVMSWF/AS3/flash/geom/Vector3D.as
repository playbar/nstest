package flash.geom
{
[native(cls="Vector3DClass", methods="auto")]

public class Vector3D
{
	//Consts
//	public static const X_AXIS : Vector3D
//	public static const Y_AXIS : Vector3D
//	public static const Z_AXIS : Vector3D
	
	//Properties
//	public native function get length() : Number
	
//	public native function get lengthSquared() : Number
	
//	public native function get w() : Number
//	public native function set w(w : Number):void
	
//	public native function get x() : Number
//	public native function set x(x : Number):void
	
//	public native function get y() : Number
//	public native function set y(y : Number):void
	
//	public native function get z() : Number
//	public native function set z(z : Number):void
		
	//Methods
//	public native function Vector3D(x:Number = 0., y:Number = 0., z:Number = 0., w:Number = 0.)
//	public native function add(a:Vector3D):Vector3D
//	public native static function angleBetween(a:Vector3D, b:Vector3D):Number
//	public native function clone():Vector3D
//	public native function crossProduct(a:Vector3D):Vector3D
//	public native function decrementBy(a:Vector3D):void
//	public native static function distance(pt1:Vector3D, pt2:Vector3D):Number
//	public native function dotProduct(a:Vector3D):Number
//	public native function equals(toCompare:Vector3D, allFour:Boolean = false):Boolean
//	public native function incrementBy(a:Vector3D):void
//	public native function nearEquals(toCompare:Vector3D, tolerance:Number, allFour:Boolean = false):Boolean
//	public native function negate():void
//	public native function normalize():Number
//	public native function project():void
//	public native function scaleBy(s:Number):void
//	public native function subtract(a:Vector3D):Vector3D
//	public native function toString():String

        public var x:Number;
        public var y:Number;
        public var z:Number;
        public var w:Number;
        public static const X_AXIS:Vector3D = new Vector3D(1, 0, 0);
        public static const Y_AXIS:Vector3D = new Vector3D(0, 1, 0);
        public static const Z_AXIS:Vector3D = new Vector3D(0, 0, 1);

        public function Vector3D(x:Number = 0, y:Number = 0, z:Number = 0, w:Number = 0)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
            return;
        }// end function

        public function clone() : Vector3D
        {
            return new Vector3D(this.x, this.y, this.z, this.w);
        }// end function

        public function dotProduct(a:Vector3D) : Number
        {
            return this.x * a.x + this.y * a.y + this.z * a.z;
        }// end function

        public function crossProduct(a:Vector3D) : Vector3D
        {
            return new Vector3D(this.y * a.z - this.z * a.y, this.z * a.x - this.x * a.z, this.x * a.y - this.y * a.x, 1);
        }// end function

        public function get length() : Number
        {
            var _loc_1:* = this.x * this.x + this.y * this.y + this.z * this.z;
            if (_loc_1 <= 0)
            {
                return 0;
            }
            return Math.sqrt(_loc_1);
        }// end function

        public function get lengthSquared() : Number
        {
            return this.x * this.x + this.y * this.y + this.z * this.z;
        }// end function

        public function normalize() : Number
        {
            var _loc_1:* = this.length;
            var _loc_2:* = _loc_1 != 0 ? (1 / _loc_1) : (0);
            this.x = this.x * _loc_2;
            this.y = this.y * _loc_2;
            this.z = this.z * _loc_2;
            return _loc_1;
        }// end function

        public function scaleBy(s:Number) : void
        {
            this.x = this.x * s;
            this.y = this.y * s;
            this.z = this.z * s;
            return;
        }// end function

        public function incrementBy(a:Vector3D) : void
        {
            this.x = this.x + a.x;
            this.y = this.y + a.y;
            this.z = this.z + a.z;
            return;
        }// end function

        public function decrementBy(a:Vector3D) : void
        {
            this.x = this.x - a.x;
            this.y = this.y - a.y;
            this.z = this.z - a.z;
            return;
        }// end function

        public function add(a:Vector3D) : Vector3D
        {
            return new Vector3D(this.x + a.x, this.y + a.y, this.z + a.z);
        }// end function

        public function subtract(a:Vector3D) : Vector3D
        {
            return new Vector3D(this.x - a.x, this.y - a.y, this.z - a.z);
        }// end function

        public function negate() : void
        {
            this.x = -this.x;
            this.y = -this.y;
            this.z = -this.z;
            return;
        }// end function

        public function equals(toCompare:Vector3D, allFour:Boolean = false) : Boolean
        {
            if (this.x == toCompare.x)
            {
	            if (this.y == toCompare.y)
	            {
		            if (this.z == toCompare.z)
		            {
		            		return allFour ? (this.w == toCompare.w) : (true);            
		            }           
	            }            
            }
        }// end function

        public function nearEquals(toCompare:Vector3D, tolerance:Number, allFour:Boolean = false) : Boolean
        {
            var _loc_4:* = this.x - toCompare.x;
            _loc_4 = _loc_4 < 0 ? (-_loc_4) : (_loc_4);
            var _loc_5:* = _loc_4 < tolerance;
            if (_loc_5)
            {
                _loc_4 = this.y - toCompare.y;
                _loc_4 = _loc_4 < 0 ? (-_loc_4) : (_loc_4);
                _loc_5 = _loc_4 < tolerance;
                if (_loc_5)
                {
                    _loc_4 = this.z - toCompare.z;
                    _loc_4 = _loc_4 < 0 ? (-_loc_4) : (_loc_4);
                    _loc_5 = _loc_4 < tolerance;
                    if (_loc_5)
                    {
	                    if (allFour)
	                    {
	                        var _loc_6:* = toCompare.w;
	                        this.w = toCompare.w;
	                        _loc_4 = _loc_6;
	                        _loc_4 = _loc_4 < 0 ? (-_loc_4) : (_loc_4);
	                        _loc_5 = _loc_4 < tolerance;
	                    }                    
                    }
                }
            }
            return _loc_5;
        }// end function

        public function project() : void
        {
            var _loc_1:* = 1 / this.w;
            this.x = this.x * _loc_1;
            this.y = this.y * _loc_1;
            this.z = this.z * _loc_1;
            return;
        }// end function

        public function toString() : String
        {
            var _loc_1:* = "Vector3D(" + this.x + ", " + this.y + ", " + this.z;
            _loc_1 = _loc_1 + ")";
            return _loc_1;
        }// end function

        public function copyFrom(sourceVector3D:Vector3D) : void
        {
            this.x = sourceVector3D.x;
            this.y = sourceVector3D.y;
            this.z = sourceVector3D.z;
            return;
        }// end function

        public function setTo(xa:Number, ya:Number, za:Number) : void
        {
            this.x = xa;
            this.y = ya;
            this.z = za;
            return;
        }// end function

        public static function angleBetween(a:Vector3D, b:Vector3D) : Number
        {
            var _loc_3:* = a.x * b.x + a.y * b.y + a.z * b.z;
            var _loc_4:* = a.length;
            var _loc_5:* = b.length;
            return Math.acos(_loc_3 / (_loc_4 * _loc_5));
        }// end function

        public static function distance(pt1:Vector3D, pt2:Vector3D) : Number
        {
            return pt1.subtract(pt2).length;
        }// end function
        
        //new adds 11
        public native function CopyFrom(sourceVector3D:Vector3D):void
        public native function SetTo(xa:Number, ya:Number, za:Number):void
        //new adds 11 end
}
}