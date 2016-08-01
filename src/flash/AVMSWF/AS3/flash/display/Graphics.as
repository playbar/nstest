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
	[native(cls="GraphicsClass", methods="auto")]

	import flash.display.BitmapData
	import flash.geom.Matrix

	public final class Graphics extends Object
	{
		//Methods
		 public native function beginBitmapFill(bitmap:BitmapData, matrix:Matrix = null, repeat:Boolean = true, smooth:Boolean = false):void 
		 public native function beginFill(color:uint, alpha:Number = 1.0):void 
		 public native function beginGradientFill(type:String, colors:Array, alphas:Array, ratios:Array, matrix:Matrix = null, spreadMethod:String = "pad", interpolationMethod:String = "rgb", focalPointRatio:Number = 0):void 
		 public native function clear():void 
		 public native function curveTo(controlX:Number, controlY:Number, anchorX:Number, anchorY:Number):void 
		
		 public native function drawCircle(x:Number, y:Number, radius:Number):void 
		 public native function drawEllipse(x:Number, y:Number, width:Number, height:Number):void 
		 public native function drawRect(x:Number, y:Number, width:Number, height:Number):void 
		 public native function drawRoundRect(x:Number, y:Number, width:Number, height:Number, ellipseWidth:Number, ellipseHeight:Number=NaN):void 
		 public function drawRoundRectComplex(x:Number, y:Number, width:Number, height:Number, ellipseWidth:Number, ellipseW:Number,ellipseHeight:Number=NaN,ellipseH:Number=NaN):void
		 {
			drawRoundRect(x,y,width,height,ellipseWidth,ellipseHeight);
		 }
		 public native function endFill():void 
		
		 public native function lineGradientStyle(type:String, colors:Array, alphas:Array, ratios:Array, matrix:Matrix = null, spreadMethod:String = "pad", interpolationMethod:String = "rgb", focalPointRatio:Number = 0):void 
		 public native function lineStyle(thickness:Number=0, color:uint = 0, alpha:Number = 1.0, pixelHinting:Boolean = false, scaleMode:String = "normal", caps:String = null, joints:String = null, miterLimit:Number = 3):void 
		 public native function lineTo(x:Number, y:Number):void 
		 public native function moveTo(x:Number, y:Number):void 

		 //new adds
		 public native function beginShaderFill(shader:Shader, matrix:Matrix = null):void
		 public native function copyFrom(sourceGraphics:Graphics):void
//		 public native function drawGraphicsData(graphicsData:Vector.<IGraphicsData>):void
		 public native function drawPath(commands:Vector.<int>, data:Vector.<Number>, winding:String = "evenOdd"):void
		 public native function drawTriangles(vertices:Vector.<Number>, indices:Vector.<int> = null, uvtData:Vector.<Number> = null, culling:String = "none"):void
		 public native function lineBitmapStyle(bitmap:BitmapData, matrix:Matrix = null, repeat:Boolean = true, smooth:Boolean = false):void
		 public native function lineShaderStyle(shader:Shader, matrix:Matrix = null):void

        public function Graphics()
        {
            return;
        }// end function

        private function drawPathObject(path:IGraphicsPath) : void
        {
            var _loc_2:GraphicsPath = null;
            var _loc_3:GraphicsTrianglePath = null;
            if (path is GraphicsPath)
            {
                _loc_2 = GraphicsPath(path);
                this.drawPath(_loc_2.commands, _loc_2.data, _loc_2.winding);
            }
            else if (path is GraphicsTrianglePath)
            {
                _loc_3 = GraphicsTrianglePath(path);
                this.drawTriangles(_loc_3.vertices, _loc_3.indices, _loc_3.uvtData, _loc_3.culling);
            }
            return;
        }// end function

        private function beginFillObject(fill:IGraphicsFill) : void
        {
            var _loc_2:GraphicsSolidFill = null;
            var _loc_3:GraphicsGradientFill = null;
            var _loc_4:GraphicsBitmapFill = null;
            var _loc_5:GraphicsShaderFill = null;
            if (fill == null)
            {
                this.endFill();
            }
            else if (fill is GraphicsEndFill)
            {
                this.endFill();
            }
            else if (fill is GraphicsSolidFill)
            {
                _loc_2 = GraphicsSolidFill(fill);
                this.beginFill(_loc_2.color, _loc_2.alpha);
            }
            else if (fill is GraphicsGradientFill)
            {
                _loc_3 = GraphicsGradientFill(fill);
                this.beginGradientFill(_loc_3.type, _loc_3.colors, _loc_3.alphas, _loc_3.ratios, _loc_3.matrix, _loc_3.spreadMethod, _loc_3.interpolationMethod, _loc_3.focalPointRatio);
            }
            else if (fill is GraphicsBitmapFill)
            {
                _loc_4 = GraphicsBitmapFill(fill);
                this.beginBitmapFill(_loc_4.bitmapData, _loc_4.matrix, _loc_4.repeat, _loc_4.smooth);
            }
            else if (fill is GraphicsShaderFill)
            {
                _loc_5 = GraphicsShaderFill(fill);
                this.beginShaderFill(_loc_5.shader, _loc_5.matrix);
            }
            return;
        }// end function

        private function beginStrokeObject(istroke:IGraphicsStroke) : void
        {
            var _loc_4:GraphicsSolidFill = null;
            var _loc_5:GraphicsGradientFill = null;
            var _loc_6:GraphicsBitmapFill = null;
            var _loc_7:GraphicsShaderFill = null;
            var _loc_2:GraphicsStroke = null;
            var _loc_3:IGraphicsFill = null;
            
            if (istroke != null)
            {
                   if (istroke is GraphicsStroke)
	            {
	                _loc_2 = istroke as GraphicsStroke;
	            }
            }
            if (_loc_2)
            {
                    if (_loc_2.fill)
	            {
	                   if (_loc_2.fill is IGraphicsFill)
		            {
		                _loc_3 = _loc_2.fill;
		            }
	            }
            }

            if (_loc_2 != null)
            {
                   if (_loc_3 == null)
	            {
	                this.lineStyle();
	            }
	            else if (_loc_3 is GraphicsSolidFill)
	            {
	                _loc_4 = GraphicsSolidFill(_loc_3);
	                this.lineStyle(_loc_2.thickness, _loc_4.color, _loc_4.alpha, _loc_2.pixelHinting, _loc_2.scaleMode, _loc_2.caps, _loc_2.joints, _loc_2.miterLimit);
	            }
	            else if (_loc_3 is GraphicsGradientFill)
	            {
	                _loc_5 = GraphicsGradientFill(_loc_3);
	                this.lineStyle(_loc_2.thickness, 0, 1, _loc_2.pixelHinting, _loc_2.scaleMode, _loc_2.caps, _loc_2.joints, _loc_2.miterLimit);
	                this.lineGradientStyle(_loc_5.type, _loc_5.colors, _loc_5.alphas, _loc_5.ratios, _loc_5.matrix, _loc_5.spreadMethod, _loc_5.interpolationMethod, _loc_5.focalPointRatio);
	            }
	            else if (_loc_3 is GraphicsBitmapFill)
	            {
	                _loc_6 = GraphicsBitmapFill(_loc_3);
	                this.lineStyle(_loc_2.thickness, 0, 1, _loc_2.pixelHinting, _loc_2.scaleMode, _loc_2.caps, _loc_2.joints, _loc_2.miterLimit);
	                this.lineBitmapStyle(_loc_6.bitmapData, _loc_6.matrix, _loc_6.repeat, _loc_6.smooth);
	            }
	            else if (_loc_3 is GraphicsShaderFill)
	            {
	                _loc_7 = GraphicsShaderFill(_loc_3);
	                this.lineStyle(_loc_2.thickness, 0, 1, _loc_2.pixelHinting, _loc_2.scaleMode, _loc_2.caps, _loc_2.joints, _loc_2.miterLimit);
	                this.lineShaderStyle(_loc_7.shader, _loc_7.matrix);
	            }
            }
            
            return;
        }// end function

        public function drawGraphicsData(graphicsData:Vector.<IGraphicsData>) : void
        {
            var _loc_3:IGraphicsData = null;
            var _loc_4:IGraphicsPath = null;
            var _loc_5:IGraphicsFill = null;
            var _loc_6:IGraphicsStroke = null;
            if (graphicsData == null)
            {
                return;
            }
            var _loc_2:int = 0;
            while (_loc_2 < graphicsData.length)
            {
                
                _loc_3 = graphicsData[_loc_2];
                if (_loc_3 is IGraphicsPath)
                {
                    _loc_4 = IGraphicsPath(_loc_3);
                    this.drawPathObject(_loc_4);
                }
                else if (_loc_3 is IGraphicsFill)
                {
                    _loc_5 = IGraphicsFill(_loc_3);
                    this.beginFillObject(_loc_5);
                }
                else if (_loc_3 is IGraphicsStroke)
                {
                    _loc_6 = IGraphicsStroke(_loc_3);
                    this.beginStrokeObject(_loc_6);
                }
                _loc_2 = _loc_2 + 1;
            }
            return;
        }// end function
		 //new adds end
		 
		//new adds 11
		public native function cubicCurveTo(controlX1:Number, controlY1:Number, controlX2:Number, controlY2:Number, anchorX:Number, anchorY:Number):void
		public native function readGraphicsData(recurse:Boolean = true):Vector.<IGraphicsData>
		//new adds 11 end
	}
}