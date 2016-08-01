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
 
 	[native(cls="BwExternAPIClass",instance="BwExternAPIObject",methods="auto")]
	import flash.utils.ByteArray;

 	public class BwExternAPI
 	{
 	
 		 public function BwExternAPI() {}
 		 
 		static public native function GetBwExternAPIObj() : BwExternAPI
 		//Properties
 		
 		 public native function getDeviceType() : String
 		 
 		 public native function getHPixel() : int
 		 
 		 public native function getVPixel() : int
 		 
 		 public native function getDPI() : int
 		 
 		 //通过SDK进行用户注册
 		 //Type	整数	1为游戏控制界面，2为使用SDK界面
 		 //User_Name	字符串	用户名
 		 //Password	字符串	密码
 		 //Result	整数	0000为成功，其余为错误码
 		 public native function SDKRegister(type:int, username:String, passwd:String ):int
 		 
 		 
 		 //通过SDK进行用户登录
 		 //Type	整数	1为游戏控制界面，2为使用SDK界面
		//User_Name	字符串	用户名
		//Password	字符串	密码
		//Result	整数	0000为成功，其余为错误码
 		 public native function SDKLogin( type:int, username:String, passwd:String ):int
 		 
 		 //通过SDK进行用户退出		
 		 //Type	整数	1为游戏控制界面，2为使用SDK界面
 		 //User_Name	字符串	用户名
 		 public native function SDKLogout( type:int, username:String ):int
 		 
 		 //Type	整数	1为游戏控制界面，2为使用SDK界面
 		 //User_Name	字符串	用户名
 		 //Password	字符串	密码
 		 //Amount	数字	支付金额
 		 //Result	整数	0000为成功，其余为错误码
 		 public native function SDKPayment( type:int, username:String, passwd:String, amount:int ):int
 		 
 		 //Data_Name	字符串	数据名称
 		 //Data_String	二进制串	数据内容
 		 //Result	整数	0000为成功，其余为错误码
 		 public native function SaveDisk( dataName:String, dataString:ByteArray ) : int
 		 
 		 //从客户端读取保存的数据		
 		 //Data_Name	字符串	数据名称
 		 //Data_String	二进制串	数据内容
 		 public native function LoadDisk( dataName:String ) : String
 		 
 		 //清理内存，并启动指定URL		
 		 //Type	整数	1为SWF，2为网页
 		 //URL	字符串	启动的地址
 		 //Result	整数	0000为成功，其余为错误码
 		 public native function Launch( type:int, url:String ) : int
 		 
 		 //通知客户端本SWF是否需要使用播放器工具栏		
 		 //visible	整数	0为显示，1为隐藏
 		 //Result	整数	0000为成功，其余为错误码
 		 public native function ShowToolbar( visible :int ) : int 
 		 
 		 //关闭FLASH
 		 public native function CloseSwf()
 	
 	}
 }