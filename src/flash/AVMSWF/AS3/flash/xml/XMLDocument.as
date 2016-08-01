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

package flash.xml
{
	[native(cls="XMLDocumentClass", methods="auto")]

	import flash.xml.XMLNode

	public class XMLDocument extends  XMLNode
	{
		//Properties
		 public native function get docTypeDecl() : Object
		 public native function set docTypeDecl(newDocTypeDecl : Object): void

		 public native function get idMap() : Object 
		 public native function set idMap(newIdMap : Object): void

		 public native function get ignoreWhite() : Boolean
		 public native function set ignoreWhite(newIgnoreWhite : Boolean): void

		 public native function get xmlDecl() : Object
		 public native function set xmlDecl(newXmlDecl : Object): void

		//Methods
		 public function XMLDocument(source:String = null) 
		 {
			if(source!=null)
				parseXML(source);
		 }

		 public function createElement(name:String):XMLNode 
		 {
			return new XMLNode(XMLNodeType.ELEMENT_NODE,name);
		 }
		 public function createTextNode(text:String):XMLNode 
		 {
			return new XMLNode(XMLNodeType.TEXT_NODE,text);
		 }
		 public native function parseXML(source:String):void 
		 public override function toString():String 
		 {
			var s:String="";
			var o:Object=xmlDecl;
			if(o!=null)
				s=s.concat(String(o));
			o=docTypeDecl;
			if(o!=null)
				s=s.concat(String(o));
			//if(nodeType!=0) 
			for(var i =0;i<childNodes.length;i++)
			{
				//s=s.concat("Hello");
				s=s.concat(childNodes[i].toString());
			}
			return s;
		 }
	}
}
