package
{
//	public native function Vector(sourceArray:Object):Vector.<T>
}

package flash.media
{
	public native function scanHardware():void
}

package adobe.utils
{
	public native function MMEndCommand(endStatus:Boolean, notifyString:String):void
}

package flash.sampler
{
	public native function clearSamples():void
	public native function getGetterInvocationCount(obj:Object, qname:QName):Number
	public native function getInvocationCount(obj:Object, qname:QName):Number
	public native function getLexicalScopes(obj:Function):Array
	public native function getMasterString(str:String):String
	
	public native function getMemberNames(o:Object, instanceNames:Boolean = false):Object
	public native function getSampleCount():Number
	public native function getSamples():Object
	public native function getSavedThis(obj:Function):Object
	public native function getSetterInvocationCount(obj:Object, qname:QName):Number
	
	public native function getSize(o:*):Number
	public native function isGetterSetter(obj:Object, qname:QName):Boolean
	public native function pauseSampling():void
	public native function sampleInternalAllocs(b:Boolean):void
	public native function setSamplerCallback(f:Function):void
	
	public native function startSampling():void
	public native function stopSampling():void
}