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

 package flash.geom
 {
 	[native(cls="MatrixClass", methods="auto")]

	import flash.geom.Point

 	public class Matrix
 	{
 		//Properties
 		 public var a:Number=1;
 		 public var b:Number=0;
 		 public var c:Number=0;
 		 public var d:Number=1;
 		 public var tx:Number=0;
 		 public var ty:Number=0;
 	
 		 public function Matrix(na:Number=1,nb:Number=0,nc:Number=0,nd:Number=1,ntx:Number=0,nty:Number=0)
 		 {
 			a=na;
 			b=nb;
 			c=nc;
 			d=nd;
 			tx=ntx;
 			ty=nty;
 		 }

 		 public function clone():Matrix 
 		 {
 			return new Matrix(a,b,c,d,tx,ty);
 		 }
 		 public function concat(m:Matrix):void
 		 {
			 var na:Number = a*m.a;//FixedMul(m1->a,  m2->a);
			 var nd:Number = d*m.d;//FixedMul(m1->d,  m2->d);
			 var nb:Number = 0;
			 var nc:Number = 0;//result.b  = result.c = 0;
			 var ntx:Number=tx*m.a+m.tx;
			 var nty:Number=ty*m.d+m.ty;
			if( b!=0 || c!=0 || m.b!=0 || m.c!=0)
			{
				na+=b*m.c;
				nd+=c*m.b;
				nb+=a*m.b+b*m.d;
				nc+=c*m.a+d*m.c;
				ntx+=ty*m.c;
				nty+=tx*m.b;
			}
			a=na;
			b=nb;
			c=nc;
			d=nd;
			tx=ntx;
			ty=nty;	
 		 }
 		 public function createBox(scaleX:Number, scaleY:Number, rotation:Number = 0, ntx:Number = 0, nty:Number = 0):void 
 		 {
 			ty=nty;
 			tx=ntx;
 			if(rotation==0)
 			{
 				a=scaleX;
 				d=scaleY;
 				b=0;
 				c=0;
 			}
 			else
 			{
 				var sinv:Number=Math.sin(rotation);
 				var cosv:Number=Math.cos(rotation);
 				a=scaleX*cosv;
 				d=scaleY*cosv;
 				b=scaleY*sinv;
 				c=-scaleX*sinv;
 			}
		}

 		 public function createGradientBox(width:Number, height:Number, rotation:Number = 0, ntx:Number = 0, nty:Number = 0):void 
 		 {
 			ty=nty+(height>>1);
 			tx=ntx+(width>>1);
 			width=width/1638.4;
 			height=height/1638.4;
 			if(rotation==0)
 			{
 				a=width;
 				d=height;
 				b=0;
 				c=0;
 			}
 			else
 			{
 				var sinv:Number=Math.sin(rotation);
 				var cosv:Number=Math.cos(rotation);
 				a=width*cosv;
 				d=height*cosv;
 				b=height*sinv;
 				c=-width*sinv;
 			}	
 		 }
 		 public function deltaTransformPoint(point:Point):Point 
 		 {
 			var x:Number=point.x*a+point.y*c;
 			var y:Number=point.x*b+point.y*d;
 			return new Point(x.y);
 		 }
 		 public function identity():void 
 		 {
 		   a=1;
 		   d=1;
 		   b=0;
 		   c=0;
 		   tx=0;
 		   ty=0;
 		 }
 		 public native function invert():void 
 		 public function rotate(angle:Number):void 
 		 {
 			var cv:Number=Math.cos(angle);
 			var sv:Number=Math.sin(angle);
 			var na:Number=a*cv-b*sv;
 			var nb:Number=a*sv+b*cv;
 			var nc:Number=c*cv-d*sv;
 			d=c*sv+d*cv;
 			var ntx:Number=tx*cv-ty*sv;
 			ty=tx*sv+ty*cv;
 			a=na;
 			b=nb;
 			c=nc;
 			tx=ntx;
 		 }
 		 public function scale(sx:Number, sy:Number):void
 		 {
 			  a=a*sx;
 			  d=d*sy;
 			  b=b*sy;
 			  c=c*sx;
 			  tx=tx*sx;
 			  ty=ty*sy;
 		 }
 		 public native function toString():String
 		 /*{
 			var s:String="(a=";
 			s+=String(a);
 			s+=",b=";
 			s+=String(b);
 			s+=",c=";
 			s+=String(c);
 			s+=",d=";
 			s+=String(d);
 			s+=",tx=";
 			s+=String(tx);
 			s+=",ty=";
 			s+=String(ty);
 			s+=")";
 		 }*/
 		 public function transformPoint(point:Point):Point 
 		 {
 			
 			var x:Number=a*point.x+tx;
 			if(c!=0)
 				x+=c*point.y;
 			
			
			var y:Number=d*point.y+ty;	
			if(b!=0)
				y+=b*point.x;
			return new Point(x,y);
 		 }
 		 public function translate(dx:Number, dy:Number):void 
 		 {
 			tx+=dx;
 			ty+=dy;
 		 }
 	
 		//new adds 11
 		public native function copyColumnFrom(column:uint, vector3D:Vector3D):void
 		public native function copyColumnTo(column:uint, vector3D:Vector3D):void
 		public native function copyFrom(sourceMatrix:Matrix):void
 		public native function copyRowFrom(row:uint, vector3D:Vector3D):void
 		public native function copyRowTo(row:uint, vector3D:Vector3D):void
 		public native function setTo(aa:Number, ba:Number, ca:Number, da:Number, txa:Number, tya:Number):void
 		//new adds 11 end	 
 	}
 }
