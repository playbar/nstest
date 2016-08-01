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

package {
	
// E4X definitions.  based on ECMA-357
	
[native(cls="XMLClass", instance="XMLObject", methods="auto")]
public final dynamic class XML extends Object
{
	// { ReadOnly, DontDelete, DontEnum }
	public static const length = 1
	
	// { DontDelete, DontEnum }
	public native static function get ignoreComments():Boolean
	public native static function set ignoreComments(newIgnore:Boolean): void
	
	// { DontDelete, DontEnum }
	public native static function get ignoreProcessingInstructions():Boolean
	public native static function set ignoreProcessingInstructions(newIgnore:Boolean): void
	
	// { DontDelete, DontEnum }
	public native static function get ignoreWhitespace():Boolean
	public native static function set ignoreWhitespace(newIgnore:Boolean): void

	// { DontDelete, DontEnum }
	public native static function get prettyPrinting():Boolean
	public native static function set prettyPrinting(newPretty:Boolean): void

	// { DontDelete, DontEnum }
	public native static function get prettyIndent():int
	public native static function set prettyIndent(newIndent:int): void

	 static function settings ():Object
	{
		return {
			ignoreComments: XML.ignoreComments,
			ignoreProcessingInstructions: XML.ignoreProcessingInstructions,
			ignoreWhitespace: XML.ignoreWhitespace,
			prettyPrinting: XML.prettyPrinting,
			prettyIndent: XML.prettyIndent
		};
	}
	
	 static function setSettings(o:Object=null):void
	{
		if (o == null) // undefined or null
		{
			XML.ignoreComments = true;
			XML.ignoreProcessingInstructions = true;
			XML.ignoreWhitespace = true;
			XML.prettyPrinting = true;
			XML.prettyIndent = 2;
			return;
		}

		if (("ignoreComments" in o) && (o.ignoreComments is Boolean))			
			XML.ignoreComments = o.ignoreComments;
		if (("ignoreProcessingInstructions" in o) && (o.ignoreProcessingInstructions is Boolean))
			XML.ignoreProcessingInstructions = o.ignoreProcessingInstructions;
		if (("ignoreWhitespace" in o) && (o.ignoreWhitespace is Boolean))
			XML.ignoreWhitespace = o.ignoreWhitespace;
		if (("prettyPrinting" in o) && (o.prettyPrinting is Boolean))
			XML.prettyPrinting = o.prettyPrinting;
		if (("prettyIndent" in o) && (o.prettyIndent is Number))		
			XML.prettyIndent = o.prettyIndent;
	}
	
	 static function defaultSettings():Object
	{
		return {
			ignoreComments: true,
			ignoreProcessingInstructions: true,
			ignoreWhitespace: true,
			prettyPrinting: true,
			prettyIndent: 2
		};
	}
	
	// override (hide) functions from object
	// ISSUE why do we override valueOf?  it does the same thing as the one in Object

	 native function toString ():String

	// override  methods from Object
	override  native function hasOwnProperty (P=void 0):Boolean
	override  native function propertyIsEnumerable (P=void 0):Boolean

	// XML functions
	 native function addNamespace (ns):XML;
	 native function appendChild (child):XML;
	 native function attribute (arg):XMLList;
	 native function attributes():XMLList;
	 native function child (propertyName):XMLList;
	 native function childIndex():int;
	 native function children ():XMLList;
	 native function comments ():XMLList;
	 native function contains (value):Boolean;
	 native function copy ():XML;
	 native function descendants (name="*"):XMLList; // name is optional
	 native function elements (name="*"):XMLList; // name is optional
	 native function hasComplexContent ():Boolean;
	 native function hasSimpleContent ():Boolean;
	 native function inScopeNamespaces ():Array;
	 native function insertChildAfter (child1, child2):*; // undefined or XML
	 native function insertChildBefore (child1, child2):*; // undefined or XML
	 function length ():int { return 1; }
	 native function localName ():Object; // null or String;
	 native function name ():Object; // null or String;
	private native function _namespace (prefix:*, argc:int):*; 
	 function namespace (prefix = null):* // prefix is optional
	{
		// can't use .apply() here, XML getproperty hacking confuses name lookup
		return arguments.length ? _namespace(prefix, 1) : _namespace(null, 0);
	}
	 native function namespaceDeclarations ():Array;
	 native function nodeKind ():String;
	 native function normalize ():XML;
	 native function parent ():*; // undefined or String
	 native function processingInstructions (name="*"):XMLList; // name is optional
	 native function prependChild (value):XML;
	 native function removeNamespace (ns):XML;
	 native function replace (propertyName, value):XML;
	 native function setChildren (value):XML;
	 native function setLocalName (name):void;
	 native function setName (name):void;
	 native function setNamespace (ns):void;
	 native function text ():XMLList;
	 native function toXMLString ():String;

	// notification extensions
	 native function notification():Function;
	 native function setNotification(f:Function);

    // Dummy constructor function - This is neccessary so the compiler can do arg # checking for the ctor in strict mode
    // The code for the actual ctor is in XMLClass::construct in the avmplus
    public function XML(value = void 0)
    {}
	//
	// dynamic, proto-hackable properties from E-357
	//
	
   	XML.settings = function():Object {
		return ::settings()
	}
	XML.setSettings = function(o=undefined):void {
		::setSettings(o)
	}
	XML.defaultSettings = function():Object {
		return ::defaultSettings()
	}

	 function valueOf():XML { return this }

	// this is what rhino appears to do
	prototype.valueOf = Object.prototype.valueOf
	
	prototype.hasOwnProperty = function(P=void 0):Boolean {
		if (this === prototype) 
		{
			return this.::hasOwnProperty(P);
		}
		var x:XML = this
		return x.::hasOwnProperty(P)
	}
	
	prototype.propertyIsEnumerable = function(P=void 0):Boolean {
		if (this === prototype) 
		{
			return this.::propertyIsEnumerable(P);
		}
		var x:XML = this
		return x.::propertyIsEnumerable(P)
	}
	
	prototype.toString = function():String {
		if (this === prototype) 
		{
			return "";
		}
		var x:XML = this
		return x.::toString()
	}
	
	prototype.addNamespace = function(ns):XML {
		var x:XML = this
		return x.::addNamespace(ns)
	}
	
	prototype.appendChild = function(child):XML {
		var x:XML = this
		return x.::appendChild(child)
	}
	
	prototype.attribute = function(arg):XMLList {
		var x:XML = this
		return x.::attribute(arg)
	}

	prototype.attributes = function():XMLList {
		var x:XML = this
		return x.::attributes()
	}
	
	prototype.child = function(propertyName):XMLList {
		var x:XML = this
		return x.::child(propertyName)
	}

	prototype.childIndex = function():int {
		var x:XML = this
		return x.::childIndex()
	}

	prototype.children = function():XMLList {
		var x:XML = this
		return x.::children()
	}
	
	prototype.comments = function():XMLList {
		var x:XML = this
		return x.::comments()
	}
	
	prototype.contains = function(value):Boolean {
		var x:XML = this
		return x.::contains(value)
	}
	
	prototype.copy = function():XML {
		var x:XML = this
		return x.::copy()
	}
	
	prototype.descendants = function(name="*"):XMLList {
		var x:XML = this
		return x.::descendants(name)
	}
	
	prototype.elements = function(name="*"):XMLList {
		var x:XML = this
		return x.::elements(name)
	}
	
	prototype.hasComplexContent = function():Boolean {
		var x:XML = this
		return x.::hasComplexContent()
	}
	
	prototype.hasSimpleContent = function():Boolean {
		var x:XML = this
		return x.::hasSimpleContent()
	}
	
	prototype.inScopeNamespaces = function():Array {
		var x:XML = this
		return x.::inScopeNamespaces()
	}
	
	prototype.insertChildAfter = function(child1, child2):*  {
		var x:XML = this
		return x.::insertChildAfter(child1,child2)
	}
	
	prototype.insertChildBefore = function(child1, child2):* {
		var x:XML = this
		return x.::insertChildBefore(child1,child2)
	}
	
	prototype.length = function():int {
		var x:XML = this
		return x.::length()
	}
	
	prototype.localName = function():Object {
		var x:XML = this
		return x.::localName()
	}

	prototype.name = function():Object {
		var x:XML = this
		return x.::name()
	}
	
	prototype.namespace = function(prefix=null):* {
		var x:XML = this
		return x.::namespace.::apply(x, arguments)
	}

	prototype.namespaceDeclarations = function():Array {
		var x:XML = this
		return x.::namespaceDeclarations()
	}
	
	prototype.nodeKind = function():String {
		var x:XML = this
		return x.::nodeKind()
	}
	
	prototype.normalize = function():XML {
		var x:XML = this
		return x.::normalize()
	}
	
	prototype.parent = function():* {
		var x:XML = this
		return x.::parent()
	}

	prototype.processingInstructions = function(name="*"):XMLList {
		var x:XML = this
		return x.::processingInstructions(name)
	}
	
	prototype.prependChild = function(value):XML {
		var x:XML = this
		return x.::prependChild(value)
	}
	
	prototype.removeNamespace = function(ns):XML {
		var x:XML = this
		return x.::removeNamespace(ns)
	}
	
	prototype.replace = function(propertyName, value):XML {
		var x:XML = this
		return x.::replace(propertyName, value)
	}
	
	prototype.setChildren = function(value):XML {
		var x:XML = this
		return x.::setChildren(value)
	}

	prototype.setLocalName = function(name):void {
		var x:XML = this
		x.::setLocalName(name)
	}

	prototype.setName = function(name):void {
		var x:XML = this
		x.::setName(name)
	}

	prototype.setNamespace = function(ns):void {
		var x:XML = this
		x.::setNamespace(ns)
	}

	prototype.text = function():XMLList {
		var x:XML = this
		return x.::text()
	}
	
	prototype.toXMLString = function():String {
		var x:XML = this
		return x.::toXMLString()
	}
	
	
    _dontEnumPrototype(prototype);
}

[native(cls="XMLListClass", instance="XMLListObject", methods="auto")]
public final dynamic class XMLList extends Object
{
	// { ReadOnly, DontDelete, DontEnum }
	public static const length = 1

	 native function toString ():String
	 function valueOf():XMLList { return this }

	// these Override (hide) the same functions from Object
	override  native function hasOwnProperty (P=void 0):Boolean
	override  native function propertyIsEnumerable (P=void 0):Boolean

	// XMLList functions
	 native function attribute (arg):XMLList;
	 native function attributes():XMLList;
	 native function child (propertyName):XMLList;
	 native function children ():XMLList;
	 native function comments ():XMLList;
	 native function contains (value):Boolean;
	 native function copy ():XMLList;

	// E4X 13.4.4.12, pg 76
	 native function descendants (name="*"):XMLList; // name is optional

	 native function elements (name="*"):XMLList; // name is optional
	 native function hasComplexContent ():Boolean;
	 native function hasSimpleContent ():Boolean;
	 native function length ():int;
	 native function name ():Object;  // null or a string;
	 native function normalize ():XMLList;
	 native function parent ():*; // undefined or XML;
	 native function processingInstructions (name="*"):XMLList; // name is optional
	 native function text ():XMLList;
	 native function toXMLString ():String;

	// These are not in the spec but work if the length of the XMLList is one
	// (Function just gets propagated to the first and only list element)
	 native function addNamespace (ns):XML;
	 native function appendChild (child):XML;
	 native function childIndex():int;
	 native function inScopeNamespaces ():Array;
	 native function insertChildAfter (child1, child2):*; // undefined or this
	 native function insertChildBefore (child1, child2):*; // undefined or this
	 native function nodeKind ():String;
	private native function _namespace (prefix:*, argc:int):*; 
	 function namespace (prefix = null):* // prefix is optional
	{
		// can't use .apply() here, XML getproperty hacking confuses name lookup
		return arguments.length ? _namespace(prefix, 1) : _namespace(null, 0);
	}
	 native function localName ():Object; // null or String
	 native function namespaceDeclarations ():Array;
	 native function prependChild (value):XML;
	 native function removeNamespace (ns):XML;
	 native function replace (propertyName, value):XML;
	 native function setChildren (value):XML;
	 native function setLocalName (name):void;
	 native function setName (name):void;
	 native function setNamespace (ns):void;

	// notification extensions(reserved)
	//public native function notification():Function;
	//public native function setNotification(f:Function);

    // Dummy constructor function - This is neccessary so the compiler can do arg # checking for the ctor in strict mode
    // The code for the actual ctor is in XMLList::construct in the avmplus
    public function XMLList(value = void 0)
    {}
    
    
	// this is what rhino appears to do, not sure why bother
	prototype.valueOf = Object.prototype.valueOf
	
	prototype.hasOwnProperty = function(P=void 0):Boolean {
		if (this === prototype) 
		{
			return this.::hasOwnProperty(P);
		}
		var x:XMLList = this
		return x.::hasOwnProperty(P)
	}
	
	prototype.propertyIsEnumerable = function(P=void 0):Boolean {
		if (this === prototype) 
		{
			return this.::propertyIsEnumerable(P);
		}
		var x:XMLList = this
		return x.::propertyIsEnumerable(P)
	}
	
	prototype.toString = function():String {
		if (this === prototype) 
		{
			return "";
		}
		var x:XMLList = this
		return x.::toString()
	}
	
	prototype.addNamespace = function(ns):XML {
		var x:XMLList = this
		return x.::addNamespace(ns)
	}
	
	prototype.appendChild = function(child):XML {
		var x:XMLList = this
		return x.::appendChild(child)
	}
	
	prototype.attribute = function(arg):XMLList {
		var x:XMLList = this
		return x.::attribute(arg)
	}

	prototype.attributes = function():XMLList {
		var x:XMLList = this
		return x.::attributes()
	}
	
	prototype.child = function(propertyName):XMLList {
		var x:XMLList = this
		return x.::child(propertyName)
	}

	prototype.childIndex = function():int {
		var x:XMLList = this
		return x.::childIndex()
	}

	prototype.children = function():XMLList {
		var x:XMLList = this
		return x.::children()
	}
	
	prototype.comments = function():XMLList {
		var x:XMLList = this
		return x.::comments()
	}
	
	prototype.contains = function(value):Boolean {
		var x:XMLList = this
		return x.::contains(value)
	}
	
	prototype.copy = function():XMLList {
		var x:XMLList = this
		return x.::copy()
	}
	
	prototype.descendants = function(name="*"):XMLList {
		var x:XMLList = this
		return x.::descendants(name)
	}
	
	prototype.elements = function(name="*"):XMLList {
		var x:XMLList = this
		return x.::elements(name)
	}
	
	prototype.hasComplexContent = function():Boolean {
		var x:XMLList = this
		return x.::hasComplexContent()
	}
	
	prototype.hasSimpleContent = function():Boolean {
		var x:XMLList = this
		return x.::hasSimpleContent()
	}
	
	prototype.inScopeNamespaces = function():Array {
		var x:XMLList = this
		return x.::inScopeNamespaces()
	}
	
	prototype.insertChildAfter = function(child1, child2):* {
		var x:XMLList = this
		return x.::insertChildAfter(child1,child2)
	}
	
	prototype.insertChildBefore = function(child1, child2):* {
		var x:XMLList = this
		return x.::insertChildBefore(child1,child2)
	}
	
	prototype.length = function():int {
		var x:XMLList = this
		return x.::length()
	}
	
	prototype.localName = function():Object {
		var x:XMLList = this
		return x.::localName()
	}

	prototype.name = function():Object {
		var x:XMLList = this
		return x.::name()
	}
	
	prototype.namespace = function(prefix=null):* {
		var x:XMLList = this
		return x.::namespace.::apply(x, arguments)
	}

	prototype.namespaceDeclarations = function():Array {
		var x:XMLList = this
		return x.::namespaceDeclarations()
	}
	
	prototype.nodeKind = function():String {
		var x:XMLList = this
		return x.::nodeKind()
	}
	
	prototype.normalize = function():XMLList {
		var x:XMLList = this
		return x.::normalize()
	}
	
	prototype.parent = function():* {
		var x:XMLList = this
		return x.::parent()
	}

	prototype.processingInstructions = function(name="*"):XMLList {
		var x:XMLList = this
		return x.::processingInstructions(name)
	}
	
	prototype.prependChild = function(value):XML {
		var x:XMLList = this
		return x.::prependChild(value)
	}
	
	prototype.removeNamespace = function(ns):XML {
		var x:XMLList = this
		return x.::removeNamespace(ns)
	}
	
	prototype.replace = function(propertyName, value):XML {
		var x:XMLList = this
		return x.::replace(propertyName, value)
	}
	
	prototype.setChildren = function(value):XML {
		var x:XMLList = this
		return x.::setChildren(value)
	}

	prototype.setLocalName = function(name):void {
		var x:XMLList = this
		x.::setLocalName(name)
	}

	prototype.setName = function(name):void {
		var x:XMLList = this
		x.::setName(name)
	}

	prototype.setNamespace = function(ns):void {
		var x:XMLList = this
		x.::setNamespace(ns)
	}

	prototype.text = function():XMLList {
		var x:XMLList = this
		return x.::text()
	}
	
	prototype.toXMLString = function():String {
		var x:XMLList = this
		return x.::toXMLString()
	}

    _dontEnumPrototype(prototype);

}

[native(cls="QNameClass", instance="QNameObject", methods="auto")]
public final class QName extends Object
{
	// E262 {DontDelete, ReadOnly, DontEnum}
	public static const length = 2

	// E357 {DontDelete, ReadOnly}
	public native function get localName():String

	// E357 {DontDelete, ReadOnly}
	public native function get uri()

	 function valueOf():QName { return this }

	 function toString():String {
		if (uri === "")
			return localName
		if (uri === null)
			return "*::" + localName
		var cc:uint = uri.charCodeAt(uri.length-1);	
		// strip the version mark, if there is one
		var base_uri:String = uri;
		if(cc >= 0xE000 && cc <= 0xF8FF) {
			base_uri = uri.substr(0, uri.length-1);
		}
		if (base_uri === "")
			return localName
		return base_uri + "::" + localName
	}

	prototype.toString = function():String
	{
		if (this === prototype) return ""
		if (!(this is QName))
			Error.throwError( TypeError, 1004 /*kInvokeOnIncompatibleObjectError*/, "QName.prototype.toString" );
		var q:QName = this
		return q.::toString()
	}

    // Dummy constructor function - This is neccessary so the compiler can do arg # checking for the ctor in strict mode
    // The code for the actual ctor is in QName::construct in the avmplus
    public function QName(namespace = void 0, name = void 0)
    {}
    
    _dontEnumPrototype(prototype);
}

}
