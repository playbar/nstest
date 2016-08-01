/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine.].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 2004-2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe  Team
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

 package flash.display
 {
 	[native(cls="LoaderClass", methods="auto")]

	import flash.events.EventDispatcher
	import flash.display.DisplayObject
	import flash.display.InteractiveObject
	import flash.display.DisplayObjectContainer

	import flash.net.URLRequest
	import flash.system.LoaderContext
	import flash.utils.ByteArray
	import flash.events.UncaughtErrorEvents
	import flash.desktop.IFilePromise
	import flash.errors.IllegalOperationError

 	public class Loader extends  DisplayObjectContainer
 	{
 		//Properties
 		 public override native function get root() : DisplayObject
 		 public native function get content() : DisplayObject 
 		
 		 public native function get contentLoaderInfo() : LoaderInfo 

		 //new adds
		 public native function get uncaughtErrorEvents() : UncaughtErrorEvents
		 //new adds end

 		//Methods
 		 public native function Loader() 

 		 public native function close():void 
 		 public native function load(request:URLRequest, context:LoaderContext = null):void 
 		 public native function loadBytes(bytes:ByteArray, context:LoaderContext = null):void 
 		 public native function unload():void 

		 //new adds
		 public native function loadFilePromise(promise:IFilePromise, context:LoaderContext = null):void
		 public function unloadAndStop(gc:Boolean = true):void
     {
        this.unload();
        return;
     }// end function
        override public function addChild(child:DisplayObject) : DisplayObject
        {
            Error.throwError(IllegalOperationError, 2069);
            return null;
        }// end function

        override public function addChildAt(child:DisplayObject, index:int) : DisplayObject
        {
            Error.throwError(IllegalOperationError, 2069);
            return null;
        }// end function

        override public function removeChild(child:DisplayObject) : DisplayObject
        {
            Error.throwError(IllegalOperationError, 2069);
            return null;
        }// end function

        override public function removeChildAt(index:int) : DisplayObject
        {
            Error.throwError(IllegalOperationError, 2069);
            return null;
        }// end function

        override public function setChildIndex(child:DisplayObject, index:int) : void
        {
            Error.throwError(IllegalOperationError, 2069);
            return;
        }// end function
 	}
 }