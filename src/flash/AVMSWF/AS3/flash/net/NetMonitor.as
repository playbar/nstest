package flash.net
{
[native(cls="NetMonitorClass", methods="auto")]

import flash.events.EventDispatcher

public class NetMonitor extends EventDispatcher
{
	//Consts
	
	//Properties
	
	//Methods
	public native function listStreams():Vector.<NetStream>
}
}