package 
{
    final public class JSON extends Object
    {
        public function JSON()
        {
            return;
        }// end function

        public static native function parse(text:String, reviver:Function = null) : Object
        //{
         //  return new JSONDecoder( text, reviver ).getValue();	
        //}// end function

        public static function stringify(value:Object, replacer = null, space = null) : String
        {
			return new JSONEncoder( value ).getString();
        }// end function

    }
}
