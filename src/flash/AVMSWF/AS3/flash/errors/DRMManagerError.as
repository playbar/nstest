package flash.errors
{
[native(cls="DRMManagerErrorClass", instance="DRMManagerErrorObject", methods="auto")]

public class DRMManagerError extends Error
{
	//Consts
	
	//Properties
//	public native function get subErrorID() : int
	
	//Methods
//	public native function DRMManagerError(message:String, id:int, subErrorID:int)
//	public native function toString():String
        private var _subErrorID:int;

        public function DRMManagerError(message:String, id:int, subErrorID:int)
        {
            super(message, id);
            this._subErrorID = subErrorID;
            return;
        }// end function

        public function get subErrorID() : int
        {
            return this._subErrorID;
        }// end function

        public function toString() : String
        {
            var _loc_1:* = "DRMManagerError: \'" + message + "\', error ID:\'" + errorID.toString() + "\', subErrorID:\'" + this._subErrorID.toString() + "\'";
            return _loc_1;
        }// end function
}
}