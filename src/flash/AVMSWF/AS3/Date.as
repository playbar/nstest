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

package 
{
	[native(cls="DateClass", instance="DateObject", methods="auto")]
	public final dynamic class Date
	{
		// Date.length = 7 per ES3
		// E262 {ReadOnly, DontDelete, DontEnum }
		public static const length:int = 7

		public native static function parse(s):Number	
		public native static function UTC(year, month, date=1, hours=0, minutes=0, seconds=0, ms=0, ... rest):Number

		 native function valueOf():Number
		private native function _toString(index:int):String
		private native function _setTime(value:Number):Number
		private native function _get(index:int):Number

		 function setTime(t=void 0):Number
		{
			return _setTime(t)
		}
		
		prototype.setTime = function(t=void 0):Number
		{
			var d:Date = this
			return d._setTime(t)
		}

		prototype.valueOf = function()
		{
			var d:Date = this
			return d.::valueOf()
		}
		
		 function toString():String				{ return _toString(0) }
		 function toDateString():String			{ return _toString(1) }
		 function toTimeString():String			{ return _toString(2) }
		 function toLocaleString():String		{ return _toString(3) }
		 function toLocaleDateString():String	{ return _toString(4) }
		 function toLocaleTimeString():String	{ return _toString(5) }
		 function toUTCString():String			{ return _toString(6) }

		 native function getUTCFullYear():Number		
		 native function getUTCMonth():Number			
		 native function getUTCDate():Number			
		 native function getUTCDay():Number		
		 native function getUTCHours():Number	
		 native function getUTCMinutes():Number	
		 native function getUTCSeconds():Number	
		 native function getUTCMilliseconds():Number	
		 native function getFullYear():Number	
		 native function getMonth():Number		
		 native function getDate():Number		
		 native function getDay():Number		
		 native function getHours():Number		
		 native function getMinutes():Number	
		 native function getSeconds():Number	
		 native function getMilliseconds():Number	
		 native function getTimezoneOffset():Number	
		 native function getTime():Number			

		private native function _setFullYear(...rest):Number
		private native function _setMonth(...rest):Number
		private native function _setDate(...rest):Number
		private native function _setHours(...rest):Number
		private native function _setMinutes(...rest):Number
		private native function _setSeconds(...rest):Number
		private native function _setMilliseconds(...rest):Number

		private native function _setUTCFullYear(...rest0):Number
		private native function _setUTCMonth(...rest):Number
		private native function _setUTCDate(...rest):Number
		private native function _setUTCHours(...rest):Number
		private native function _setUTCMinutes(...rest):Number
		private native function _setUTCSeconds(...rest):Number
		private native function _setUTCMilliseconds(...rest):Number

		 function setFullYear(year=void 0, month=void 0, date=void 0):Number { return _setFullYear.::apply(this, arguments); }
		 function setMonth(month=void 0, date=void 0):Number { return _setMonth.::apply(this, arguments); }
		 function setDate(date=void 0):Number { return _setDate.::apply(this, arguments); }
		 function setHours(hour=void 0, min=void 0, sec=void 0, ms=void 0):Number { return _setHours.::apply(this, arguments); }
		 function setMinutes(min=void 0, sec=void 0, ms=void 0):Number { return _setMinutes.::apply(this, arguments); }
		 function setSeconds(sec=void 0, ms=void 0):Number { return _setSeconds.::apply(this, arguments); }
		 function setMilliseconds(ms=void 0):Number { return _setMilliseconds.::apply(this, arguments); }

		 function setUTCFullYear(year=void 0, month=void 0, date=void 0):Number { return _setUTCFullYear.::apply(this, arguments); }
		 function setUTCMonth(month=void 0, date=void 0):Number { return _setUTCMonth.::apply(this, arguments); }
		 function setUTCDate(date=void 0):Number { return _setUTCDate.::apply(this, arguments); }
		 function setUTCHours(hour=void 0, min=void 0, sec=void 0, ms=void 0):Number { return _setUTCHours.::apply(this, arguments); }
		 function setUTCMinutes(min=void 0, sec=void 0, ms=void 0):Number { return _setUTCMinutes.::apply(this, arguments); }
		 function setUTCSeconds(sec=void 0, ms=void 0):Number { return _setUTCSeconds.::apply(this, arguments); }
		 function setUTCMilliseconds(ms=void 0):Number { return _setUTCMilliseconds.::apply(this, arguments); }

		prototype.toString = function():String
		{
			var d:Date = this
			return d._toString(0)
		}
		prototype.toDateString = function():String
		{
			var d:Date = this
			return d.::toDateString()
		}
		prototype.toTimeString = function():String
		{
			var d:Date = this
			return d.::toTimeString()
		}
		prototype.toLocaleString = function():String
		{
			var d:Date = this
			return d.::toLocaleString()
		}
		prototype.toLocaleDateString = function():String
		{
			var d:Date = this
			return d.::toLocaleDateString()
		}
		prototype.toLocaleTimeString = function():String
		{
			var d:Date = this
			return d.::toLocaleTimeString()
		}
		prototype.toUTCString = function():String
		{
			var d:Date = this
			return d.::toUTCString()
		}

		prototype.getUTCFullYear = function():Number
		{
			var d:Date = this
			return d.::getUTCFullYear()
		}
		
		prototype.getUTCMonth = function():Number
		{
			var d:Date = this
			return d.::getUTCMonth()
		}
		
		prototype.getUTCDate = function():Number
		{
			var d:Date = this
			return d.::getUTCDate()
		}
		
		prototype.getUTCDay = function():Number
		{
			var d:Date = this
			return d.::getUTCDay()
		}
		
		prototype.getUTCHours = function():Number
		{
			var d:Date = this
			return d.::getUTCHours()
		}
		
		prototype.getUTCMinutes = function():Number
		{
			var d:Date = this
			return d.::getUTCMinutes()
		}
		
		prototype.getUTCSeconds = function():Number
		{
			var d:Date = this
			return d.::getUTCSeconds()
		}
		
		prototype.getUTCMilliseconds = function():Number
		{
			var d:Date = this
			return d.::getUTCMilliseconds()
		}
		
		prototype.getFullYear = function():Number
		{
			var d:Date = this
			return d.::getFullYear()
		}
		
		prototype.getMonth = function():Number
		{
			var d:Date = this
			return d.::getMonth()
		}
		
		prototype.getDate = function():Number
		{
			var d:Date = this
			return d.::getDate()
		}
		
		prototype.getDay = function():Number
		{
			var d:Date = this
			return d.::getDay()
		}
		
		prototype.getHours = function():Number
		{
			var d:Date = this
			return d.::getHours()
		}
		
		prototype.getMinutes = function():Number
		{
			var d:Date = this
			return d.::getMinutes()
		}
		
		prototype.getSeconds = function():Number
		{
			var d:Date = this
			return d.::getSeconds()
		}
		
		prototype.getMilliseconds = function():Number
		{
			var d:Date = this
			return d.::getMilliseconds()
		}
		
		prototype.getTimezoneOffset = function():Number
		{
			var d:Date = this
			return d.::getTimezoneOffset()
		}
		
		prototype.getTime = function():Number
		{
			var d:Date = this
			return d.::getTime()
		}

		prototype.setFullYear = function(year=void 0, month=void 0, date=void 0):Number
		{
			var d:Date = this
			return d.::setFullYear.::apply(d, arguments);
		}
		prototype.setMonth = function(month=void 0, date=void 0):Number
		{
			var d:Date = this
			return d.::setMonth.::apply(d, arguments);
		}
		prototype.setDate = function(date=void 0):Number
		{
			var d:Date = this
			return d.::setDate.::apply(d, arguments);
		}
		prototype.setHours = function(hour=void 0, min=void 0, sec=void 0, ms=void 0):Number
		{
			var d:Date = this
			return d.::setHours.::apply(d, arguments);
		}
		prototype.setMinutes = function(min=void 0, sec=void 0, ms=void 0):Number
		{
			var d:Date = this
			return d.::setMinutes.::apply(d, arguments);
		}
		prototype.setSeconds = function(sec=void 0, ms=void 0):Number
		{
			var d:Date = this
			return d.::setSeconds.::apply(d, arguments);
		}
		prototype.setMilliseconds = function(ms=void 0):Number
		{
			var d:Date = this
			return d.::setMilliseconds.::apply(d, arguments);
		}

		prototype.setUTCFullYear = function(year=void 0, month=void 0, date=void 0):Number
		{
			var d:Date = this
			return d.::setUTCFullYear.::apply(d, arguments);
		}
		prototype.setUTCMonth = function(month=void 0, date=void 0):Number
		{
			var d:Date = this
			return d.::setUTCMonth.::apply(d, arguments);
		}
		prototype.setUTCDate = function(date=void 0):Number
		{
			var d:Date = this
			return d.::setUTCDate.::apply(d, arguments);
		}
		prototype.setUTCHours = function(hour=void 0, min=void 0, sec=void 0, ms=void 0):Number
		{
			var d:Date = this
			return d.::setUTCHours.::apply(d, arguments);
		}
		prototype.setUTCMinutes = function(min=void 0, sec=void 0, ms=void 0):Number
		{
			var d:Date = this
			return d.::setUTCMinutes.::apply(d, arguments);
		}
		prototype.setUTCSeconds = function(sec=void 0, ms=void 0):Number
		{
			var d:Date = this
			return d.::setUTCSeconds.::apply(d, arguments);
		}
		prototype.setUTCMilliseconds = function(ms=void 0):Number
		{
			var d:Date = this
			return d.::setUTCMilliseconds.::apply(d, arguments);
		}

		public function get fullYear():Number { return ::getFullYear(); }
		public function set fullYear(value:Number) { ::setFullYear(value); }

		public function get month():Number { return ::getMonth(); }
		public function set month(value:Number) { ::setMonth(value); }

		public function get date():Number { return ::getDate(); }
		public function set date(value:Number) { ::setDate(value); }

		public function get hours():Number { return ::getHours(); }
		public function set hours(value:Number) { ::setHours(value); }

		public function get minutes():Number { return ::getMinutes(); }
		public function set minutes(value:Number) { ::setMinutes(value); }

		public function get seconds():Number { return ::getSeconds(); }
		public function set seconds(value:Number) { ::setSeconds(value); }

		public function get milliseconds():Number { return ::getMilliseconds(); }
		public function set milliseconds(value:Number) { ::setMilliseconds(value); }

		public function get fullYearUTC():Number { return ::getUTCFullYear(); }
		public function set fullYearUTC(value:Number) { ::setUTCFullYear(value); }

		public function get monthUTC():Number { return ::getUTCMonth(); }
		public function set monthUTC(value:Number) { ::setUTCMonth(value); }

		public function get dateUTC():Number { return ::getUTCDate(); }
		public function set dateUTC(value:Number) { ::setUTCDate(value); }

		public function get hoursUTC():Number { return ::getUTCHours(); }
		public function set hoursUTC(value:Number) { ::setUTCHours(value); }

		public function get minutesUTC():Number { return ::getUTCMinutes(); }
		public function set minutesUTC(value:Number) { ::setUTCMinutes(value); }

		public function get secondsUTC():Number { return ::getUTCSeconds(); }
		public function set secondsUTC(value:Number) { ::setUTCSeconds(value); }

		public function get millisecondsUTC():Number { return ::getUTCMilliseconds(); }
		public function set millisecondsUTC(value:Number) { ::setUTCMilliseconds(value); }

		public function get time():Number { return ::getTime(); }
		public function set time(value:Number) { ::setTime(value); }

		public function get timezoneOffset():Number { return ::getTimezoneOffset(); }
		public function get day():Number { return ::getDay(); }
		public function get dayUTC():Number { return ::getUTCDay(); }


        // Dummy constructor function - This is neccessary so the compiler can do arg # checking for the ctor in strict mode
        // The code for the actual ctor is in DateClass::construct in the avmplus
        public function Date(year = void 0, month = void 0, date = void 0, hours = void 0, minutes = void 0, seconds = void 0, ms = void 0)
        {}
        

		// These are not part of ECMA-262, and thus we will not be exposing
		// them via the new-style get/set functions (this is provided here
		// just to let you know we didn't overlook them)
		//public function get year():Number { return getYear(); }
		//public function get yearUTC():Number { return getUTCYear(); }	

		// The following older ECMA and/or AS2 functions are not supported since
		// they are not Y2K compliant (only get/set 2 digits)
		// getYear
		// setYear
		// getUTCYear
		// setUTCYear

		_dontEnumPrototype(prototype);
	}
}
