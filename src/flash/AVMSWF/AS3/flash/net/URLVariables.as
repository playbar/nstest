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

 package flash.net
 {
 	[native(cls="URLVariablesClass", methods="auto")]

	import flash.utils.*;

 	public dynamic class URLVariables
 	{
 		//Methods
 		//var context:String;
 		//public var context:Array;
 		public function URLVariables(source:String = null) 
 		{
 			context=new Array();
 			if(source) 
 				decode(source);
 		}

 		public function decode(source:String):void 
 		{
 			if(source==null||context==null)
 				Error.throwError(TypeError,Error.kInvalidArgumentError);
 			var a:Array=source.split('&');
 			var i:uint;
 			for(i =0;i<a.length;i++)
 			{
 				var s:String=a[i];
 				var m:int=s.indexOf("=");
 				if(int<=0) Error.throwError(TypeError,Error.kInvalidArgumentError);
 				var name:String=unescape(s.substr(0,m));
 				var value:String=unescape(s.substr(m+1));
 				this[name]=value;
 				//var v:URLRequestHeader=new URLRequestHeader(escape(s.substr(0,m)),escape(s.substr(m+1)));
 				//context.push(v);
 			}
 		}
 		public function toString():String 
 		{
 			var s="";
 			var b:Boolean=false;
 			var v;
 			var i;
 			for(var m in this)
 			{ 				
 				v=this[m];
 				if(v is Array)
 				{
 				    for(i=0;i<v.length;i++)
 				    {
 						if(b)
 						  s+="&";
 						s+=escapeMultiByte(m);
 						s+="=";
 						s+=escapeMultiByte(v[i]);
 						b=true;
 				    }
 				}
 				else
 				{
 				    if(b)
 				      s+="&";
 					s+=escapeMultiByte(m);
 					s+="=";
 					s+=escapeMultiByte(v);
 					b=true;
 				}
 				
 			}
 			return s;
 		}
 	}
 }