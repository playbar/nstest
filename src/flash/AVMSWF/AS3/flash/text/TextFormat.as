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

 package flash.text
 {
 	[native(cls="TextFormatClass", methods="auto")]

 	public class TextFormat
 	{
 		//Properties
 		 public var align:String;
 		 public var blockIndent:Object;
 		 public var bold: Object
 		 public var bullet : Object 
 		 public var color : Object 
 		 public var font : String 
 		 public var indent : Object 
		 public var italic : Object 
		 public var kerning : Object 
		 public var leading : Object 
		 public var leftMargin : Object 
		 public var letterSpacing : Object 
		 public var rightMargin : Object 
		 public var size : Object 
		 public var tabStops : Array 
		 public var target : String 
		 public var underline : Object 
		 public var display : String
		 //var url : String 
		 public native function get url():String;
		 public native function set url(v:String);
		 		 
 		//Methods
 		 public function TextFormat(nfont:String = null, nsize:Object = null, ncolor:Object = null, nbold:Object = null, nitalic:Object = null, nunderline:Object = null, nurl:String = null, ntarget:String = null, nalign:String = null, nleftMargin:Object = null, nrightMargin:Object = null, nindent:Object = null, nleading:Object = null) 
 		 {
 			display="Block";
 			font=nfont;
 			size=nsize;
 			color=ncolor;
 			bold=nbold;
 			italic=nitalic;
 			underline=nunderline;
 			url=nurl;
 			target=ntarget;
 			align=nalign;
 			leftMargin=nleftMargin;
 			rightMargin=nrightMargin;
 			indent=nindent;
 			leading=nleading;
 		 }
 	}
 }
