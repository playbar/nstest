package flash.html
{
[native(cls="HTMLLoaderClass", methods="auto")]

import flash.events.EventDispatcher
import flash.display.DisplayObject
import flash.display.InteractiveObject
import flash.display.DisplayObjectContainer
import flash.display.Sprite

import flash.html.HTMLHost
import flash.system.ApplicationDomain
import flash.display.NativeWindowInitOptions
import flash.net.URLRequest
import flash.html.HTMLHistoryItem
import flash.geom.Rectangle

public class HTMLLoader extends Sprite
{
	//Consts
	
	//Properties
	public native function get authenticate() : Boolean
	public native function set authenticate(authenticate : Boolean):void
	
	public native function get cacheResponse() : Boolean
	public native function set cacheResponse(cacheResponse : Boolean):void
	
	public native function get contentHeight() : Number
	
	public native function get contentWidth() : Number
	
	public native function get hasFocusableContent() : Boolean
	
	public native override function get height() : Number		//O
	public native override function set height(height : Number):void
	
	public native function get historyLength() : uint
	
	public native function get historyPosition() : uint
	public native function set historyPosition(historyPosition : uint):void
	
	public native function get htmlHost() : HTMLHost
	public native function set htmlHost(htmlHost : HTMLHost):void
	
	public native function get idleTimeout() : Number
	public native function set idleTimeout(idleTimeout : Number):void
	
	public native static function get isSupported() : Boolean		//s
	
	public native function get loaded() : Boolean
	
	public native function get location() : String
	
	public native function get manageCookies() : Boolean
	public native function set manageCookies(manageCookies : Boolean):void
	
	public native function get navigateInSystemBrowser() : Boolean
	public native function set navigateInSystemBrowser(navigateInSystemBrowser : Boolean):void
	
	public native function get paintsDefaultBackground() : Boolean
	public native function set paintsDefaultBackground(paintsDefaultBackground : Boolean):void
	
	public native static function get pdfCapability() : int		//s
	
	public native function get placeLoadStringContentInApplicationSandbox() : Boolean
	public native function set placeLoadStringContentInApplicationSandbox(placeLoadStringContentInApplicationSandbox : Boolean):void
	
	public native function get runtimeApplicationDomain() : ApplicationDomain
	public native function set runtimeApplicationDomain(runtimeApplicationDomain : ApplicationDomain):void
	
	public native function get scrollH() : Number
	public native function set scrollH(scrollH : Number):void
	
	public native function get scrollV() : Number
	public native function set scrollV(scrollV : Number):void
	
	public native static function get swfCapability() : int		//s
	
	public native function get textEncodingFallback() : String
	public native function set textEncodingFallback(textEncodingFallback : String):void
	
	public native function get textEncodingOverride() : String
	public native function set textEncodingOverride(textEncodingOverride : String):void
	
	public native function get useCache() : Boolean
	public native function set useCache(useCache : Boolean):void
	
	public native function get userAgent() : String
	public native function set userAgent(userAgent : String):void
	
	public native override function get width() : Number
	public native override function set width(width : Number):void	//O
	
	public native function get window() : Object
	
	//Methods
	public native function HTMLLoader()
	public native function cancelLoad():void
	public native static function createRootWindow(visible:Boolean = true, windowInitOptions:NativeWindowInitOptions = null, scrollBarsVisible:Boolean = true, bounds:Rectangle = null):HTMLLoader
	public native function getHistoryAt(position:uint):HTMLHistoryItem
	public native function historyBack():void
	public native function historyForward():void
	public native function historyGo(steps:int):void
	public native function load(urlRequestToLoad:URLRequest):void
	public native function loadString(htmlContent:String):void
	public native function reload():void
}
}