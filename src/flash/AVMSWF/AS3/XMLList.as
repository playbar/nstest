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

package 
{
	[native(cls="XMLListClass", methods="auto")]

	public final dynamic class XMLList
	{
		//Methods
		 public native function XMLList(value:Object) 

		 public native function attribute(attributeName:*):XMLList 
		 public native function attributes():XMLList 
		 public native function child(propertyName:Object):XMLList 
		 public native function children():XMLList 
		 public native function comments():XMLList 

		 public native function contains(value:XML):Boolean 
		 public native function copy():XMLList 
		 public native function descendants(name:Object = *):XMLList 
		 public native function elements(name:Object = *):XMLList 
		 public native function hasComplexContent():Boolean 

		 public native function hasOwnProperty(p:String):Boolean 
		 public native function hasSimpleContent():Boolean 
		 public native function length():int 
		 public native function normalize():XMLList 
		 public native function parent():Object 

		 public native function processingInstructions(name:String = "*"):XMLList 
		 public native function propertyIsEnumerable(p:String):Boolean 
		 public native function text():XMLList 
		 public native function toString():String 
		 public native function toXMLString():String 

		 public native function valueOf():XMLList 
	}
}
