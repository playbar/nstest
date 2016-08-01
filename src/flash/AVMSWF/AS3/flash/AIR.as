package flash.accessibility
{
	import flash.geom.Rectangle;
	[native(cls="ISearchableTextClass",methods="auto")]
	public interface ISearchableText
	{
		function get searchText():String
	}
	[native(cls="ISimpleTextSelection",method="auto")]
	public interface ISimpleTextSelection
	{
		function get selectionActiveIndex():int
		function get selectionAnchorIndex():int
	}
	[native(cls="AccessibilityImplementationClass", methods="auto")]
	public class AccessibilityImplementation extends Object
	{
		  public var errno:uint = 0
		  public var stub:Boolean = false
		  //静态构造函数
		  public function accDoDefaultAction(childID:uint):void
		  {
		  }
		  //一个 IAccessible 方法，执行与此 AccessibilityImplementation 表示的组件关联的默认操作，或执行该组件的子元素之一的默认操作。 AccessibilityImplementation 
		  public function accLocation(childID:uint):*
		  {
			return null;
		  }
		  //MSAA 方法，返回指定 AccessibilityImplementation 中子元素边框的 DisplayObject 或 Rectangle。 AccessibilityImplementation 
		  public function accSelect(operation:uint, childID:uint):void
		  {
		  }
		  //IAccessible 方法，更改此 AccessibilityImplementation 所表示组件中的选定内容。 AccessibilityImplementation 
		  public function get_accDefaultAction(childID:uint):String
		  {
		    return null;
		  }
		  //MSAA 方法，返回此 AccessibilityImplementation 所表示组件的默认操作，或该组件的子元素之一的默认操作。 AccessibilityImplementation 
		  public function get_accFocus():uint
		  {
			return 0;
		  }
		  //MSAA 方法，返回在组件内具有子焦点的子元素（如果有）的无符号整数 ID。 AccessibilityImplementation 
		  public function get_accName(childID:uint):String
		  {
			return null;
		  }
		  //MSAA 方法，返回此 AccessibilityImplementation 所表示组件的名称或组件子元素之一的名称。 AccessibilityImplementation 
		  public function get_accRole(childID:uint):uint
		  {
			return 0;
		  }
		 //MSAA 方法，返回此 AccessibilityImplementation 所表示组件的名称或组件子元素之一的系统角色。 AccessibilityImplementation 
		  public function get_accSelection():Array
		  {
			return new Array(0);
		  }
		  //MSAA 方法，返回包含全部所选子元素的 ID 的数组。 AccessibilityImplementation 
		  public function get_accState(childID:uint):uint
		  {
			return 0;
		  }
		   //IAccessible 方法，返回此 AccessibilityImplementation 所表示组件的当前运行时状态，或该组件的子元素之一的当前运行时状态。 AccessibilityImplementation 
		  public function get_accValue(childID:uint):String
		  {
			return null;
		  }
		  //MSAA 方法，返回此 AccessibilityImplementation 所表示组件的运行时值，或该组件的子元素之一的运行时值。 AccessibilityImplementation 
		  public function getChildIDArray():Array
		  {
			return new Array(0);
		  }
		  //返回一个数组，其中包含 AccessibilityImplementation 中所有子元素的无符号整数 ID。 AccessibilityImplementation 
		  public function hasOwnProperty(name:String):Boolean
		  {
			return false;
		  }
		 //指示对象是否已经定义了指定的属性。 Object 
		  public function isLabeledBy(labelBounds:Rectangle):Boolean
		  {
			return false;
		  }
		  //返回 true 或 false，以指示边框由 x、y、width 和 height 指定的文本对象是否应该视为 AccessibilityImplementation 表示的组件的标签 

	}
}

package flash.display
{
	public interface IGraphicsData 
	{
	}
	public interface IGraphicsFill 
	{
	}
	public interface IGraphicsPath
	{
	}
	public interface IGraphicsStroke
	{
	}
	public final class ColorCorrection extends Object
	{
		//ColorCorrection 类可为 flash.display.Stage.colorCorrection 属性提供值。 

		public static var DEFAULT : String = "default"//[静态] 使用主机的默认颜色校正。 ColorCorrection 
		public static var OFF : String = "off"//[静态] 关闭颜色校正，不论播放器主机环境如何。 ColorCorrection 
		public static var ON : String = "on"//[静态] 打开颜色校正，不论播放器主机环境（如果有）如何。 
	}
	public final class ColorCorrectionSupport extends Object
	{//ColorCorrectionSupport 类可为 flash.display.Stage.colorCorrectionSupport 属性提供值。 
		public static var DEFAULT_OFF : String = "defaultOff"//[静态] 支持颜色校正，默认情况下为关闭状态。 ColorCorrectionSupport 
		public static var DEFAULT_ON : String = "defaultOn"//[静态] 支持颜色校正，默认情况下为打开状态。 ColorCorrectionSupport 
		public static var UNSUPPORTED : String = "unsupported"//[静态] 主机环境不支持颜色校正。 
	}

}
