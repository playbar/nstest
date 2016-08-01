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

 package BWLoader
 {
 
 	[native(cls="BXBGLayerClass",instance="BXBGLayerObject",methods="auto")]
	import flash.display.Sprite;
	import flash.display.DisplayObject;
	import flash.events.*;
 	public class BXBGLayer extends Sprite
 	{
 		//static public var instance:BXBGLayer=null;
		override public native function set x(value:Number) : void
		override public native function get x(): Number
		override public native function set y(value:Number) : void
		override public native function get y():Number
		public native function showBG():void
		public native function hideBG():void
		/*override public function set visible(value:Boolean):void
		{
			super.visible=value;
			if(value)
				create();
			else
				release();
		}*/
		//public native function release():void;
		//public native function create():void;
		public native function set scrollMode(value:Boolean):void
		
		public native function BXBGLayer()
		/*{
			super()
			if(!one)
			{
				throw new Error("请通过Common.ui获取实例");
				return;
			}
			//addEventListener(Event.ADD_TO_STAGE,onAdded);
			//addEventListener(Event.REMOVE_FROM_STAGE,onRemoved);
		}*/
		
		/*static public function getInstance():BXBGLayer
		{
			if(instance==null)
				instance=new BXBGLayer(new JustOne());
			return instance;
		}*/
 	}
 }
 