package flash.system
{
[native(cls="SystemUpdaterClass", methods="auto")]

import flash.events.EventDispatcher

public class SystemUpdater extends EventDispatcher
{
	//Consts
	
	//Properties
	
	//Methods
	public native function SystemUpdater()
	public native function cancel():void
	public native function update(type:String):void
}
}