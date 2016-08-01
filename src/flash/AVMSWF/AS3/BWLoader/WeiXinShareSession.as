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
 
 	[native(cls="WeiXinShareSessionClass",instance="WeiXinShareSessionObject",methods="auto")]
	import flash.utils.ByteArray;
	import flash.display.BitmapData;
 	public class WeiXinShareSession
 	{
 		public function WeiXinShareSession() {}
 		 
 		static public native function GetWeiXinShareSessionObj() : WeiXinShareSession
 		
 		
 	     //终端是否安装了微信
 	     public native function isWeiXinIstalled():Boolean
 		 //发送text消息给息给微信, text为消息内容
 		 public native function sendText(sendType:int, text:String ):int
 		 //发送photo消息给息给微信, picData为bitmap图片数据
 		 public native function sendPhoto(sendType:int, picData:BitmapData ):int
 		 //发送link消息给息给微信， 参数依次为标题，描述，缩略图，点击图片的web连接
 		 public native function sendLink(sendType:int, title:String, description:String, thumbPicData:BitmapData, webpageUrl:String):int
 		 //发送music消息给息给微信， 参数依次为标题，描述，缩略图，音乐网页的url地址，音乐数据的url地址
 		 public native function sendMusic(sendType:int, title:String, description:String, thumbPicData:BitmapData, musicUrl:String, musicDataUrl:String):int
 		 //发送video消消息给息给微信，参数依次为标题，描述，缩略图，视频网页的url地址
 		 public native function sendVedio(sendType:int, title:String, description:String, thumbPicData:BitmapData, videoUrl:String):int
 		 //发送app消息给息给微信，参数依次为标题，描述，缩略图，第三方程序的自定义数据（微信终端会回传给第三方程序）,url,app文件数据（微信好友需要点击下载数据，回传给第三方app）
 		 public native function sendAppMessage(sendType:int, title:String, description:String, thumbPicData:BitmapData, extInfo:String, url:String, fileData:ByteArray):int
 		 //发送非gif消息给息给微信,参数依次为缩略图，emotionData为非gif数据
 		 public native function sendNotGifContent(sendType:int, thumbPicData:BitmapData,emotionData:ByteArray):int
 		 //发送gif消息给息给微信,参数依次为缩略图，emotionData为gif数据
 		 public native function sendGifContent(sendType:int, thumbPicData:BitmapData,emotionData:ByteArray):int
 		 //发送文件消息给息给微信，参数依次为标题，描述，缩略图，文件数据
 		 public native function sendFileContent(sendType:int, title:String, description:String, thumbPicData:BitmapData, fileData:ByteArray, fileExt:String):int
  	}
 }