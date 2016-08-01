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
 	[native(cls="XMLNodeClass", methods="auto")]

	public class XMLNode
	{
		 //public var attributes:Object;
		 //public var nodeType:uint;
		 //public var childNodes:Array;
		 //public var firstChild:XMLNode;
		 //public var lastChild:XMLNode;
		 //public var localName:String;
		 //public var nextSibling:XMLNode;
		 //public var nodeName:String;		 
		 //public var nodeValue:String;
		 //public var parentNode:XMLNode;
		 //public var previousSibling:XMLNode;
		 public native function get nodeType():uint;
		 public native function get childNodes():Array;
		 public native function get firstChild():XMLNode;
		 public native function get lastChild():XMLNode;
		 public native function get localName():String;
		 public native function get nextSibling():XMLNode;
		 public native function get nodeName():String;		 
		 public native function get nodeValue():String;
		 public native function get parentNode():XMLNode;
		 public native function get previousSibling():XMLNode;
		 
		 public native function set nodeType(param:uint);
		 public native function set childNodes(param:Array);	//	read only
		 public native function set firstChild(param:XMLNode);
		 public native function set lastChild(param:XMLNode);
		 public native function set localName(param:String);	//	read only
		 public native function set nextSibling(param:XMLNode);
		 public native function set nodeName(param:String);		 
		 public native function set nodeValue(param:String);
		 public native function set parentNode(param:XMLNode);
		 public native function set previousSibling(param:XMLNode);
		 
		 //Properties
		 public native function get attributes():Object;
		 public native function set attributes(newObject:Object):void;
		 public native function get namespaceURI() : String 
		 public native function get prefix() : String 

		//Methods
		 public function XMLNode(type:uint=0, value:String=null)		
		 {			 
			 //attributes=null;
			 nodeType=type;//type;
			 childNodes=new Array();
			 firstChild=null;
			 lastChild=null;
			 localName=null;
			 nextSibling=null;
			 nodeName=null;			 
			 nodeValue=null;
			 parentNode=null;
			 previousSibling=null;
			 switch(type)
			 {
				case XMLNodeType.TEXT_NODE:
					 nodeValue=value;
					 break;
				case XMLNodeType.ELEMENT_NODE:
					 nodeName=value;
					 break;
				default:
					 nodeType=XMLNodeType.ELEMENT_NODE;
					 break;
			 }
		 }
		 public native function appendChild(node:XMLNode):void 		 
		 public function cloneNode(deep:Boolean):XMLNode 
		 {
			var node:XMLNode;
			if(nodeType==XMLNodeType.TEXT_NODE)
				node=new XMLNode(nodeType,nodeValue);
			else
				node=new XMLNode(nodeType,nodeName);
			if(deep&&firstChild!=null)
			{
				for(var i=0;i<childNodes.length;i++)
					node.appendChild(childNodes[i].cloneNode(deep));
			}
			node.attributes=attributes;
			return node;
		 }
		 public native function getNamespaceForPrefix(prefix:String):String 
		 public native function getPrefixForNamespace(ns:String):String 
		 public function hasChildNodes():Boolean 
		 {	return childNodes.length>0;	 }
		 public function insertBefore(node:XMLNode, before:XMLNode):void 
		 {
			var index:int=childNodes.indexOf(before,0);
			if(index<0) return;
			childNodes.splice(0,0,node);
			node.parent=this;
			node.nextSibling=before;
			node.previousSibling=before.previousSibling;
			if(before==firstChild)
				firstChild=node;
		 }
		 function removeChild(node:XMLNode):void
		 {
			var index:int=childNodes.indexOf(node,0);
			if(index<0) return;
			childNodes.splice(index,1);
			if(node==firstChild)
				firstChild=firstChild.nextSibling;
			else if(node==lastChild)
			{
				lastChild=lastChild.previousSibling;
				lastChild.nextSibling=null;
			}
			else
			{
				node.previousSibling.nextSibling=node.nextSibling;
			}
			node.previousSibling=null;
			node.nextSibling=null;
			node.parent=null;
		 }
		 public function removeNode():void 
		 {
			if(parent)
				parent.removeChild(this);
		 }
		 public function toString():String
		 {
			return _toString();
		 }
		 public function _toString():String
		 {
			if(nodeType==0) return null;
			var s:String;
			if(nodeType==XMLNodeType.TEXT_NODE)
				s=nodeValue;
			else
			{
				s="<";
				s=s.concat(nodeName);
				if(attributes)
				{
					for(var name in attributes)
					{
						s=s.concat(" ",name,"=\"",attributes[name],"\"");
					}
				}
				if(childNodes==null||childNodes.length<=0)
				{
					s=s.concat(" />\n");
				}
				else s=s.concat(">");
			}
			if(childNodes!=null)
			{
				for(var i=0;i<childNodes.length;i++)
				{
					s=s.concat(childNodes[i].toString());
				}
			
				if(nodeType==XMLNodeType.ELEMENT_NODE&&childNodes.length>0)
				{
					s=s.concat("</",nodeName,">\n");
				}
			}
			return s;
		 }	
	}
 }
