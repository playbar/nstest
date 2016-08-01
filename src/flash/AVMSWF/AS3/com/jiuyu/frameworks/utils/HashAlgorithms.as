package com.jiuyu.frameworks.utils
{
    import flash.utils.*;

    public class HashAlgorithms extends Object
    {

        public function HashAlgorithms()
        {
            return;
        }// end function

        public native static function fnvHash(param1:ByteArray) : int
    }
}

package com.my9yu.framework.utils
{
    import flash.utils.*;

    public class HashAlgorithms extends Object
    {

        public function HashAlgorithms()
        {
            return;
        }// end function

        public native static function fnvHash(param1:ByteArray) : int
    }
}

package com.my9yu.framework.utils
{
	import flash.utils.*;
    public class SwfEnDeUtil extends Object
    {
        public static const keyCode:String = ">>>>>>";
        public function SwfEnDeUtil()
        {
            return;
        }// end function
        public native static function unCompress(param1:ByteArray, param2:String) : Object
        public native static function getDecryptDataByByteArray(param1:ByteArray, param2:String, param3:String = "") : Object
    }
}

package com.my9yu.framework.utils
{
    import flash.net.*;
    import flash.utils.*;

    public class ClassUtil extends Object
    {

        public function ClassUtil()
        {
            return;
        }// end function

        public static function clone(param1) : Object
        {
			var _loc_2:* = getQualifiedClassName(param1);
			var _loc_3:* = _loc_2.split("::")[1];
			var _loc_4:* = Class(getDefinitionByName(_loc_2));
            registerClassAlias(_loc_3, _loc_4);
            var _loc_5:* = new ByteArray();
            _loc_5.writeObject(param1);
            _loc_5.position = 0;
            return _loc_5.readObject();
        }
        public static function createNewObject(param1) : Object
        {
			var _loc_2:* = new ByteArray();
            _loc_2.writeObject(param1);
            _loc_2.position = 0;
            return _loc_2.readObject();
        }
        public native static function samePropertiesCopyValue(param1:Object, param2:Object) : Object       
        /*{
            var _loc_4:XML = null;
            var _loc_5:XMLList = null;
            var _loc_6:int = 0;
            var _loc_7:String = null;
            var _loc_8:String = null;
            if (!param1 || !param2)
            {
                return null;
            }
            var _loc_3:* = getQualifiedClassName(param1);
            if (_loc_3 != "Object")
            {
                _loc_4 = describeType(param1);
                _loc_5 = _loc_4.accessor;
                _loc_6 = 0;
                while (_loc_6 < _loc_5.length())
                {
                    
                    _loc_7 = _loc_5[_loc_6].@name;
                    if (param2.hasOwnProperty(_loc_7))
                    {
                        param2[_loc_7] = param1[_loc_7];
                    }
                    else
                    {
                        try
                        {
                            param2[_loc_7] = param1[_loc_7];
                        }
                        catch (err:Error)
                        {
                        }
                    }
                    _loc_6++;
                }
            }
            else
            {
                var _loc_9:int = 0;
                var _loc_10:* = param1;
                //do
                for(_loc_10 in _loc_9)
                {
                    
                    _loc_8 = _loc_10[_loc_9];
                    if (param2.hasOwnProperty(_loc_8))
                    {
                        param2[_loc_8] = param1[_loc_8];
                    }
                    else
                    {
                        try
                        {
                            param2[_loc_8] = param1[_loc_8];
                        }
                        catch (err:Error)
                        {
                        }
                    }
                }//while (_loc_10 in _loc_9)
            }
            return param2;
        }*/
        public native static function objectSamePropertyCopyValueByXML(param1:Object, param2:Object) : void
        /*{
			var _loc_3:XML = null;
            var _loc_4:XMLList = null;
            var _loc_5:XMLList = null;
            var _loc_6:Array = null;
            var _loc_7:int = 0;
            var _loc_8:XML = null;
            var _loc_9:XML = null;
            var _loc_10:XML = null;
            var _loc_11:String = null;
            var _loc_12:String = null;
            if (param1 && param2)
            {
                _loc_3 = describeType(param2);
                if (_loc_3)
                {
                    _loc_4 = _loc_3.variable;
                    _loc_5 = _loc_3.accessor;
                    _loc_6 = [];
                    _loc_7 = 0;
                    _loc_7 = 0;
                    while (_loc_7 < _loc_4.length())
                    {
                        
                        _loc_8 = _loc_4[_loc_7];
                        _loc_6.push(_loc_8);
                        _loc_7++;
                    }
                    _loc_7 = 0;
                    while (_loc_7 < _loc_5.length())
                    {
                        
                        _loc_9 = _loc_5[_loc_7];
                        _loc_6.push(_loc_9);
                        _loc_7++;
                    }
                    _loc_7 = 0;
                    while (_loc_7 < _loc_6.length)
                    {
                        
                        _loc_10 = _loc_6[_loc_7] as XML;
                        if (_loc_10.@access == "readonly")
                        {
                        }
                        else
                        {
                            _loc_11 = _loc_10.@name;
                            _loc_12 = _loc_10.@type;
                            if (param1.hasOwnProperty(_loc_11))
                            {
                                switch(_loc_12)
                                {
                                    case "Boolean":
                                    {
                                        param2[_loc_11] = sloveBooble(param1[_loc_11].toString());
                                        break;
                                    }
                                    case "int":
                                    {
                                        param2[_loc_11] = int(param1[_loc_11]);
                                        break;
                                    }
                                    case "Number":
                                    {
                                        param2[_loc_11] = Number(param1[_loc_11]);
                                        break;
                                    }
                                    case "uint":
                                    {
                                        param2[_loc_11] = uint(param1[_loc_11].toString());
                                        break;
                                    }
                                    case "String":
                                    {
                                        param2[_loc_11] = param1[_loc_11].toString();
                                        break;
                                    }
                                    default:
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                        _loc_7++;
                    }
                }
            }
            return;
        }*/
      private static function sloveBooble(param1) : Boolean
        {
            if (param1 is String)
            {
                if (param1 == "0")
                {
                    return false;
                }
                if (param1 == "1")
                {
                    return true;
                }
                if (String(param1) == "true")
                {
                    return true;
                }
                if (String(param1) == "false")
                {
                    return false;
                }
                return Boolean(param1);
            }
            return Boolean(param1);
        }// end function
        
    }
    
    
}


package com.my9yu.common.utils
{
	import flash.utils.*;
	public class MD5 extends Object
	{
		public function MD5()
        {
            return;
        }// end fu
         public static function hash(param1:String) : String
        {
            var _loc_2:* = new ByteArray();
            _loc_2.writeUTFBytes(param1);
            return hashBinary(_loc_2);
        }// end function

        public static function hashBytes(param1:ByteArray) : String
        {
            return hashBinary(param1);
        }// end function

        public native static function hashBinary(param1:ByteArray) : String
	}
}

/*package com.my9yu.common.utils
{
	public class SamePropertyObjectUtil extends Object
	{
		public function SamePropertyObjectUtil()
        {
            return;
        }// end function

        public native static function copySamePropertyValue(param1:Object, param2:Object) : void       
    }
}*/