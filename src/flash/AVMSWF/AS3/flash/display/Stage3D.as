/*
	Stage3D类提供一个显示区域和可编程渲染上下文以用于绘制 2D 和 3D 图形。
	Stage3D 使用 Context3D 类供渲染的内容实现高性能的渲染表面。在可能的情况下，
	此表面使用图形处理单元 (GPU)。运行时舞台提供固定数目的 Stage3D 对象。
	实例的数目根据设备类型的不同而有所不同。桌面计算机通常提供四个 Stage3D 实例。
*/
  package flash.display
  {
	[native(cls="Stage3DClass", methods="auto")]

	import flash.events.EventDispatcher
	import flash.display3D.Context3D

	
	public class Stage3D extends EventDispatcher
	{
		//Properties
		//public native function get context3D():Context3D
		
		public native function get visible(): Boolean
		public native function set visible(value:Boolean):void

		public native function get x():Number
		public native function set x(value:Number):void

		public native function get y():Number
		public native function set y(value:Number):void

		//Methods
		public function Stage3D()
		{
			return;
		}//end function

		//public native function requestContext3D(context3DRenderMode:String = "auto",  profile:String = "baseline"):void

	}
}