package Com
{
	import flash.display.MovieClip;
	import flash.text.TextField;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.events.Event;
	public class XCheckBox extends Sprite
	{
		private var _isChecked:Boolean = false;
		private var _isActive:Boolean = false;
		static public var Change:String="Change";
		public function XCheckBox()
		{
			ic.gotoAndStop("off");
			addEventListener(MouseEvent.ROLL_OUT,onOut);
			addEventListener(MouseEvent.ROLL_OVER,onOver);
			addEventListener(MouseEvent.MOUSE_DOWN,onDown);
			//addEventListener(MouseEvent.MOUSE_UP,onUp);
		}
		public function get checked():Boolean
		{
			return _isChecked;
		}
		
		public function set checked(b:Boolean):void
		{
			_isChecked = b;
			setStat();
		}
		
		public function set actived(b:Boolean):void
		{
			_isActive = b;
			setStat();
		}
		
		private function onOut(e:MouseEvent)
		{
			actived = false;
		}
		
		private function onOver(e:MouseEvent)
		{
			actived = true;
		}
		
		private function onDown(e:MouseEvent)
		{
			checked = !_isChecked;
			dispatchEvent(new Event(Change));
		}
		
		
		
		public function set label(t:String):void
		{
			txt.text = t;
		}
		private function setStat()
		{
			if(_isChecked)
			{
				if(_isActive)
					ic.gotoAndStop("onactive");
				else
					ic.gotoAndStop("on");
			}
			else
			{
				if(_isActive)
					ic.gotoAndStop("offactive");
				else
					ic.gotoAndStop("off");
			}
		}
		
	}
}