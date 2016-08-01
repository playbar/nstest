package flash.display
{
[native(cls="ShaderJobClass", methods="auto")]

import flash.events.EventDispatcher

public class ShaderJob extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get height() : int
	public native function set height(height : int):void
	
	public native function get progress() : Number
	
	public native function get shader() : Shader
	public native function set shader(shader : Shader):void
	
	public native function get target() : Object
	public native function set target(target : Object):void
	
	public native function get width() : int
	public native function set width(width : int):void
	
	//Methods
//	public native function ShaderJob(shader:Shader = null, target:Object = null, width:int = 0, height:int = 0)
	public native function cancel():void
	public native function start(waitForCompletion:Boolean = false):void
        public function ShaderJob(shader:Shader = null, target:Object = null, width:int = 0, height:int = 0)
        {
            if (shader)
            {
                this.shader = shader;
            }
            if (target)
            {
                this.target = target;
            }
            this.width = width;
            this.height = height;
            return;
        }// end function
}
}