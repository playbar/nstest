/*
 * GlobalObserver.h
 *
 *  Created on: 2011-9-14
 *      Author: yock
 */

#ifndef GLOBALOBSERVER_H_
#define GLOBALOBSERVER_H_

class GlobalEditObserver
	{
		public:
		virtual void OnGlobalEvent(unsigned long data1,unsigned long data2)=0;
		virtual void SetText(unsigned short *text)=0;
	};

#endif /* GLOBALOBSERVER_H_ */
