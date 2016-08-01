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

package flash.printing
{
	[native(cls="PrintJobClass", methods="auto")]

	import flash.events.EventDispatcher

	import flash.display.Sprite
	import flash.geom.Rectangle
	import flash.printing.PrintJobOptions

	public class PrintJob extends  EventDispatcher
	{
		//Properties
//		 public native function get orientation() : String 

//		 public native function get pageHeight() : int 

//		 public native function get pageWidth() : int 

//		 public native function get paperHeight() : int 

//		 public native function get paperWidth() : int 

		 //new adds
		public native static function get active() : Boolean	//s

		public native function get copies() : int
		public native function set copies(copies : int):void

		public native function get firstPage() : int

		public native function get isColor() : Boolean

//		public static function get isSupported() : Boolean	//s
//		{
//			return true;
//		}

		public native function get jobName() : String
		public native function set jobName(jobName : String):void

		public native function get lastPage() : int

		public native function get maxPixelsPerInch() : Number

		public native function get paperArea() : Rectangle

		public native function get printableArea() : Rectangle

		public native function get printer() : String
		public native function set printer(printer : String):void

		public native static function get printers() : Vector.<String>	//s

		public native static function get supportsPageSetupDialog() : Boolean	//s
		 //new adds end

		//Methods
//		 public native function PrintJob() 

//		 public native function addPage(sprite:Sprite, printArea:Rectangle = null, options:PrintJobOptions = null, frameNum:int = 0):void 
//		 public native function send():void 
//		 public native function start():Boolean 

		 //new adds
		public native function selectPaperSize(paperSize:String):void
		public native function showPageSetupDialog():Boolean
		public native function start2(uiOptions:PrintUIOptions = null, showPrintDialog:Boolean = true):Boolean
		public native function terminate():void
		 //new adds end
		 
        private static const kGetPaperHeight:uint = 1;
        private static const kGetPaperWidth:uint = 3;
        private static const kGetPageHeight:uint = 5;
        private static const kGetPageWidth:uint = 7;
        private static const kGetOrientation:uint = 9;
        private static const kStart:uint = 100;
        private static const kAddPage:uint = 101;
        private static const kSend:uint = 102;

        public function PrintJob()
        {
            return;
        }// end function

        public function get paperHeight() : int
        {
            return this.invoke(kGetPaperHeight);
        }// end function

        public function get paperWidth() : int
        {
            return this.invoke(kGetPaperWidth);
        }// end function

        public function get pageHeight() : int
        {
            return this.invoke(kGetPageHeight);
        }// end function

        public function get pageWidth() : int
        {
            return this.invoke(kGetPageWidth);
        }// end function

        public function get orientation() : String
        {
            return this.invoke(kGetOrientation);
        }// end function

        public function start() : Boolean
        {
            return this.invoke(kStart) == true;
        }// end function

        private native function invoke(index:uint, ... args) : Boolean

        private native function invoke1(index, ... args) : Boolean

        public function send() : void
        {
            this.invoke(kSend);
            return;
        }// end function

        private function toClassicRectangle(printArea:Rectangle)
        {
            if (printArea != null)
            {
                return {xMin:printArea.left, yMin:printArea.top, xMax:printArea.right, yMax:printArea.bottom};
            }
            return null;
        }// end function

        public function addPage(sprite:Sprite, printArea:Rectangle = null, options:PrintJobOptions = null, frameNum:int = 0) : void
        {
            var _loc_5:Object = null;
            if (options != null)
            {
                _loc_5 = {printAsBitmap:options.printAsBitmap};
            }
            if (this.invoke1(kAddPage, sprite, this.toClassicRectangle(printArea), _loc_5, frameNum > 0 ? (frameNum) : (-1)) == false)
            {
                Error.throwError(Error, 2057);
            }
            return;
        }// end function

        public static function get isSupported() : Boolean
        {
            return true;
        }// end function		 
		 
	}
}