package
{
	import flash.display.Sprite;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.events.Event;
	import flash.utils.getTimer;
	import flash.geom.*;
	import flash.filters.*;
	import flash.display.DisplayObject;
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.BitmapDataChannel;
	import flash.utils.ByteArray;
	public class MainWnd extends Sprite
	{
		private var _strFun:Array=["单色","纹理","过渡色","旋转色","9切片","文字","图片功能","特技"];
		private var _strExt:Array=["线条","颜色转换","过滤器","遮盖"];
		private var _strFilters:Array=["Bevel","Blur","ColorMatrix","Convolution","Displace",
									   "DropShadow","Grow","GradientBevel","GradientGrow","Shader"];
		private var _isLine:Boolean = false;
		private var _isTransform:Boolean = false;
		private var _isFilter:Boolean = false;
		private var _isMask:Boolean = false;
		private var _frames:int = 0;
		private var _funID:int = 0;
		private var _frameTimes:Array=[];
		private var _totalFrame = 0;
		private var _funObj:MovieClip = null;
		private var _maskObj:Sprite = null;
		private var _filterButtons:Array;
		private var _filterID:int = 0;
		public function MainWnd()
		{
			//trace("Start");
			_filterButtons=[Bevel,Blur,ColorMatrix,Convolution,DisplacementMap,
							DropShadow,Grow,GradientBevel,GradientGrow];//,Shader];
			var i,ic=_filterButtons.length;
			for(i=0;i<ic;i++)
			{
				_filterButtons[i].label = _strFilters[i];
				_filterButtons[i].addEventListener(XCheckBox.Change,onChangeFilter);
				if(i==0)
				   _filterButtons[i].checked = true;
			}
			stage.addEventListener(Event.ENTER_FRAME,onEnter);
			SimpleColor.label = _strFun[0];
			SimpleColor.addEventListener(MouseEvent.CLICK,onClickSimpleColor);
			Texture.label = _strFun[1];
			Texture.addEventListener(MouseEvent.CLICK,onClickTexture);
			Gradient.label = _strFun[2];
			Gradient.addEventListener(MouseEvent.CLICK,onClickGradient);
			RadialGradient.label = _strFun[3];
			RadialGradient.addEventListener(MouseEvent.CLICK,onClickRadial);
			Grid.label = _strFun[4];
			Grid.addEventListener(MouseEvent.CLICK,onClickGrid);
			FontBut.label = _strFun[5];
			FontBut.addEventListener(MouseEvent.CLICK,onClickFont);
			BitmapFun.label = _strFun[6];
			BitmapFun.addEventListener(MouseEvent.CLICK,onClickBitmap);
			Movie.label = _strFun[7];
			Movie.addEventListener(MouseEvent.CLICK,onClickMovie);
			lineBox.label = _strExt[0];
			transformBox.label = _strExt[1];
			filterBox.label = _strExt[2];
			maskBox.label = _strExt[3];
			onClickSimpleColor(null)
			con.addEventListener(MouseEvent.CLICK,onNext);
			lineBox.addEventListener(XCheckBox.Change,onChange);
			transformBox.addEventListener(XCheckBox.Change,onChange);
			maskBox.addEventListener(XCheckBox.Change,onChange);
			filterBox.addEventListener(XCheckBox.Change,onChange);
			//setStat();
		}
		
		private function showFun():void
		{
			if(_funObj)
				con.removeChild(_funObj);
			if(_maskObj)
				con.removeChild(_maskObj);
			_funObj = null;
			_maskObj = null;
			var disp:MovieClip = null;
			switch(_funID)
			{
				case 0:
					 if(_isLine)
					 	 disp = new FunLines();
					 else
						 disp = new FunSimpleColor();
					 break;
				case 1:
					if(!_isLine)
						disp= new FunTexture();
					else
						disp = new FunTextureLines();
					break;
				case 2:
					if(!_isLine)
						disp = new FunLinenearColor();
					else
						disp = new FunLinenearLine();
					break;
				case 3:
					if(_isLine)
						disp = new FunRadialLines();
					else
						disp = new FunRadialColor();
					break;
				case 4:
					disp = new FunGrid();
					break;
				case 5:
					disp = new FunFont();
					break;
				case 6:
					disp = createBitmapDisp();
					break;
				case 7:
					disp = new FunMovie();
					break;
			}
			if(!disp) return;
			
			_funObj = disp;
			var rc:Rectangle;
			//rc=disp.getRect(null);
			disp.x = 40;
			disp.y = 40;
			if(_funID!=7)
				disp.stop();
			else
			{
				disp.x+=190;
				disp.y+=160;
			}
			
			con.addChild(disp);
			if(_isTransform)
			{
				var cx:ColorTransform=new ColorTransform(0.8,1.0,0.3,0.7,0.0,-0.4,0.3,0);
				disp.transform.colorTransform = cx;
			}
			setFilter();
			if(_isMask)
			{
				_maskObj = new MaskObj();
				_maskObj.x = 40;
				_maskObj.y = 40;
				con.addChild(_maskObj);
				disp.mask = _maskObj;
			}
		}
		
		private function onChange(e:*):void
		{
			_isLine = lineBox.checked;
			_isTransform = transformBox.checked;
			_isFilter = filterBox.checked;
			_isMask = maskBox.checked;
			setStat();
			showFun();
		}
		
		private function createBitmapDisp():MovieClip
		{
			//trace("Bitmap");
			var disp:MovieClip = new MovieClip();
			var bmd:BitmapData = new BitmapData(600,400,true,0xffffff);
			bmd.fillRect(new Rectangle(40,40,300,200),0x7f003f00);
			var ball:BitmapData = new BlueBall(0,0);
			var rc:Rectangle=new Rectangle(0,0,ball.width,ball.height);
			var ball1:BitmapData = new BlueBall(0,0);
			bmd.copyPixels(ball,rc,new Point(300,300),ball1,new Point(0,0),true);
			bmd.copyChannel(ball,rc,new Point(50,50),BitmapDataChannel.BLUE,BitmapDataChannel.GREEN);
			bmd.draw(lineBox,new Matrix(2,0,0,2,200,200));
			bmd.floodFill(240,240,0xffff0000);
			bmd.draw(ball);
			var data:ByteArray=ball.getPixels(rc);
			data.position = 0;
			bmd.setPixels(new Rectangle(60,220,ball.width,ball.height),data);
			
			bmd.merge(ball,rc,new Point(500,20),150,150,150,255);
			//bmd.copy
			ball1.colorTransform(new Rectangle(10,10,ball.width-20,ball.height-30),new ColorTransform(0.4,0.6,0.4,1,0,0,0,0));
			var bc:* = ball.compare(ball1);
			if(bc is BitmapData)
				bmd.draw(bc,new Matrix(1,0,0,1,120,320));
			
			disp.addChild(new Bitmap(bmd));
			return disp;
		}
		
		private function onChangeFilter(e:Event):void
		{
			var i:int;
			var ic:int = _filterButtons.length;
			for(i=0;i<ic;i++)
			{
				var ch:XCheckBox = XCheckBox(_filterButtons[i]);
				if(e.target==ch)
				{
					ch.checked = true;
					_filterID = i;
				}
				else
					ch.checked = false;
			}
			showFun();
		}
		
		private function onNext(e:MouseEvent)
		{
			if(!_funObj) return;
			//trace("Next");
			//trace(_funObj.currentFrame);
			var c:int = _funObj.currentFrame+1;
			if(c>_funObj.totalFrames) c=1;
			_funObj.gotoAndStop(c);
			setFilter();
		}
		
		private function setFilter():void
		{
			if(!_isFilter) return;
			{
				var f:BitmapFilter = null;
				var m:Array;
				switch(_filterID)
				{
					case 0:
					     f=new BevelFilter();
						 break;
					case 1:
					     f=new BlurFilter();
						 break;
					case 2:
						 m=[0.3,0.3,0.3,0,0,
							0.3,0.3,0.3,0,0,
							0.3,0.3,0.3,0,0,
							0,0,0,1,0];
						 f=new ColorMatrixFilter(m);
						 break;
					case 3:
						 f=new ConvolutionFilter();
						 break;
					case 4:
						 f=new DisplacementMapFilter();
						 break;
					case 5:
						 f=new DropShadowFilter();
						 break;
					case 6:
						 f=new GlowFilter();
						 break;
					case 7:
						 f=new GradientBevelFilter();
						 break;
					case 8:
					     f=new GradientGlowFilter();break;
					//case 9:
						// f=new ShaderFilter();
						// break;
					
				}
				if(f==null) return;
				var i:int ,ic:int = _funObj.numChildren;
				var fs:Array = new Array(f);
				for(i=0;i<ic;i++)
				{
					var o:DisplayObject=_funObj.getChildAt(i);
					if(o!=null)
						o.filters = fs;
				}
				//disp.filter = new Array(f);
			}
		}
		
		private function onEnter(e:Event)
		{
			var t:uint = getTimer();
			_frameTimes.push(t);
			if(_frameTimes.length>100)
				_frameTimes.shift();
			var ic:int = _frameTimes.length;
			var dt:uint = _frameTimes[ic-1]-_frameTimes[0];
			_frames = ic*1000/dt;
			//dt*10/ic;
			_totalFrame++;
			if(_totalFrame>50)
			{
				_totalFrame = 0;
				setStat();
			}
		}
		
		private function onClickSimpleColor(e:MouseEvent)
		{
			lineBox.visible = true;
			transformBox.visible = true;
			filterBox.visible = true;
			maskBox.visible = true;
			showFilters(false);
			_funID = 0;
			setStat();
			showFun();
		}
		
		private function onClickTexture(e:MouseEvent)
		{
			lineBox.visible = true;
			transformBox.visible = true;
			filterBox.visible = true;
			maskBox.visible = true;
			showFilters(false);
			_funID = 1;
			setStat();
			showFun();
		}
		
		private function onClickGradient(e:MouseEvent)
		{
			lineBox.visible = true;
			transformBox.visible = true;
			filterBox.visible = true;
			maskBox.visible = true;
			showFilters(false);
			_funID = 2;
			setStat();
			showFun();
		}
		
		private function onClickRadial(e:MouseEvent)
		{
			lineBox.visible = true;
			transformBox.visible = true;
			filterBox.visible = true;
			maskBox.visible = true;
			showFilters(false);
			_funID = 3;
			setStat();
			showFun();
		}
		
		private function onClickGrid(e:MouseEvent)
		{
			lineBox.visible = false;
			transformBox.visible = true;
			filterBox.visible = true;
			maskBox.visible = true;
			showFilters(false);
			_funID = 4;
			setStat();
			showFun();
		}
		
		private function onClickFont(e:MouseEvent)
		{
			lineBox.visible = false;
			transformBox.visible = true;
			filterBox.visible = true;
			maskBox.visible = true;
			showFilters(false);
			_funID = 5;
			setStat();
			showFun();
		}
		
		private function onClickBitmap(e:MouseEvent)
		{
			lineBox.visible = false;
			transformBox.visible = false;
			filterBox.visible = false;
			maskBox.visible = false;
			showFilters(true);
			_funID = 6;
			setStat();
			showFun();
		}
		
		private function onClickMovie(e:MouseEvent)
		{
			lineBox.visible = false;
			transformBox.visible = true;
			filterBox.visible = true;
			maskBox.visible = true;
			showFilters(false);
			_funID = 7;
			setStat();
			showFun();
		}
		
		private function setStat():void
		{
			var s:String = _strFun[_funID];
			if(_isLine)
				s+="-"+_strExt[0];
			if(_isTransform)
				s+="-"+_strExt[1];
			if(_isFilter)
				s+="-"+_strExt[2];
			if(_isMask)
				s+="-"+_strExt[3];
			s+="("+_frames+" fps)";
			msg.text = s;
		}
		private function showFilters(b:Boolean)
		{
			var i,ic=_filterButtons.length;
			for(i=0;i<ic;i++)
				_filterButtons[i].visible = b;
		}
	}
}