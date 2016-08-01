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
 * Portions created by the Initial Developer are Copyright (C) 2008
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
 	[native(cls="DisplayObjectContainerClass", methods="auto")]

	import flash.events.EventDispatcher
	import flash.display.DisplayObject
	import flash.display.InteractiveObject

	import flash.geom.Point
	import flash.text.TextSnapshot

 	public class DisplayObjectContainer extends  InteractiveObject
 	{
 		//Properties
 		 public native function get mouseChildren() : Boolean
		 public native function set mouseChildren(newMouseChildren : Boolean): void

		 public native function get numChildren() : int

		 public native function get tabChildren() : Boolean
		 public native function set tabChildren(newTabChildren : Boolean): void

		 public native function get textSnapshot() : TextSnapshot

 		//Methods
 		 public native function DisplayObjectContainer()

 		 public native function addChild(child:DisplayObject):DisplayObject
 		 public native function addChildAt(child:DisplayObject, index:int):DisplayObject
 		 public native function areInaccessibleObjectsUnderPoint(point:Point):Boolean
 		 public native function contains(child:DisplayObject):Boolean
 		 public native function getChildAt(index:int):DisplayObject
 		
 		 public native function getChildByName(name:String):DisplayObject
 		 public native function getChildIndex(child:DisplayObject):int
 		 public native function getObjectsUnderPoint(point:Point):Array
 		 public native function removeChild(child:DisplayObject):DisplayObject
 		 public native function removeChildAt(index:int):DisplayObject
 		
 		 public native function setChildIndex(child:DisplayObject, index:int):void
 		 public native function swapChildren(child1:DisplayObject, child2:DisplayObject):void
 		 public native function swapChildrenAt(index1:int, index2:int):void	
 		 
 		 //new adds 11
 		 public native function removeChildren(beginIndex:int = 0, endIndex:int = 0x7fffffff):void
 		 public native function stopAllMovieClips():void
 		 //new adds 11 end
 	}
 }
