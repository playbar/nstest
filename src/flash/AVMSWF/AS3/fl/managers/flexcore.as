

package fl.managers
{
    import flash.display.*;
    import flash.events.*;
    import flash.text.*;
    import flash.ui.*;
    import flash.utils.*;

    public class FocusManager extends Object
    {
        private var _showFocusIndicator:Boolean = true;
        private var _defaultButtonEnabled:Boolean = true;
        private var activated:Boolean = false;
        private var calculateCandidates:Boolean = true;
        private var lastFocus:InteractiveObject;
        private var lastAction:String;

        public function FocusManager(param1:DisplayObjectContainer)
        {
            activated = false;
            calculateCandidates = true;
            _showFocusIndicator = true;
            _defaultButtonEnabled = true;            
            return;
        }// end function

        public function get showFocusIndicator() : Boolean
        {
            return _showFocusIndicator;
        }// end function

        

        public function set form(param1:DisplayObjectContainer) : void
        {
            _form = param1;
            return;
        }// end function

        
        }// end function

        public function findFocusManagerComponent(param1:InteractiveObject) : InteractiveObject
        {
            return param1;
        }// end function

        
        public function sendDefaultButtonEvent() : void
        {            
            return;
        }// end function

        public function getFocus() : InteractiveObject
        {
           return null;
        }// end function

        
        public function setFocus(param1:InteractiveObject) : void
        {            
            return;
        }// end function

        
        public function hideFocus() : void
        {
            return;
        }// end function

        
        public function get defaultButton() : Sprite
        {
            return null;
        }// end function

        
        public function showFocus() : void
        {
            return;
        }// end function

        public function set defaultButtonEnabled(param1:Boolean) : void
        {
            _defaultButtonEnabled = param1;
            return;
        }// end function

        public function getNextFocusManagerComponent(param1:Boolean = false) : InteractiveObject
        {
            return null;
        }// end function

        

        public function get nextTabIndex() : int
        {
            return 0;
        }// end function

        

        public function set showFocusIndicator(param1:Boolean) : void
        {
            _showFocusIndicator = param1;
            return;
        }// end function

        public function get form() : DisplayObjectContainer
        {
            return null;
        }// end function

       
        public function activate() : void
        {
            
            activated = true;
            
            return;
        }// end function

        public function deactivate() : void
        {
            
            activated = false;
            return;
        }// end function

        public function get defaultButtonEnabled() : Boolean
        {
            return _defaultButtonEnabled;
        }// end function
    
}

