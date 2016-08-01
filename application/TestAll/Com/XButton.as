package Com
{
	import flash.display.MovieClip;
	import flash.text.TextField;
	import flash.events.MouseEvent;
	public class XButton extends MovieClip
	{
		private  var isDown:Boolean = false;
		public function XButton()
		{
			gotoAndStop("up");
			//trace("Button");
			addEventListener(MouseEvent.ROLL_OUT,onOut);
			addEventListener(MouseEvent.ROLL_OVER,onOver);
			addEventListener(MouseEvent.MOUSE_DOWN,onDown);
			addEventListener(MouseEvent.MOUSE_UP,onUp);
		}
		
		public function set label(t:String):void
		{
			txt.text = t;
		}
		
		private function onOut(e:MouseEvent)
		{
			if(isDown) gotoAndStop("down");
			else	   gotoAndStop("up");
		}
		
		private function onOver(e:MouseEvent)
		{
			gotoAndStop("over");
		}
		
		private function onDown(e:MouseEvent)
		{
			gotoAndStop("down");
			isDown = true;
		}
		
		private function onUp(e:MouseEvent)
		{
			gotoAndStop("over");
			isDown = false;
		}
		
	}
}