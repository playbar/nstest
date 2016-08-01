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
*   Adobe AS3 Team
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

#include "VMPI.h"
#include <pthread.h>
#ifdef _SYMBIAN
//�����߳�һ�ι���
#include <assert.h>
enum {ETSDnone=0,ETSDcreated,ETSDbusy};
#define KTSDNum 5
static int TSDid[KTSDNum] = {0,0,0,0,0};
#endif

#ifdef _SYMBIAN
bool VMPI_tlsCreate(uintptr_t* tlsId)
{
	int i(0);
	while(i<KTSDNum)
		{
		switch(TSDid[i])
			{
			case ETSDcreated:
				{
				*tlsId = (uintptr_t) i;
				TSDid[i] = ETSDbusy;
				return true;
				}
				break;
			case ETSDnone:
				{
				pthread_key_t key;
				const int r = pthread_key_create(&key, NULL);
				if(r == 0)
					{
					assert(key == i);
					TSDid[i] = ETSDbusy;
					*tlsId = (uintptr_t) key;
					return true;
					}
				}
				break;
			}
		i++;
		}
	return false;
	
//	pthread_key_t key;
//	const int r = pthread_key_create(&key, NULL);
//	
//	if(r == 0)
//	{
//		// we expect the value to default to zero
//// begin modify by hgl 2011-05-27
//		void * re = pthread_getspecific(key);
//#ifdef WIN32
//		GCAssert(re == 0);
//#endif
//// end modify
//		*tlsId = (uintptr_t) key;
//		return true;
//	}
//
//	return false;
}

void VMPI_tlsDestroy(uintptr_t tlsId)
{
	TSDid[tlsId] = ETSDcreated;
//	pthread_key_delete((pthread_key_t)tlsId);
}

void VMPI_tlsStart()
	{
	int i(0);
	while(i<KTSDNum)
		{
		if(TSDid[i] == ETSDnone)
			{
			pthread_key_t key;
			do{
			pthread_key_create(&key,NULL);
			}
			while(key<i);
			TSDid[i] = ETSDcreated;
			}
		i++;
		}
	}

void VMPI_tlsClose()
	{
	for(int i(0);i<KTSDNum;i++)
		{
		pthread_key_delete((pthread_key_t)i);
		}
	}
#else
bool VMPI_tlsCreate(uintptr_t* tlsId)
{
	pthread_key_t key;
	const int r = pthread_key_create(&key, NULL);
	
	if(r == 0)
	{
		// we expect the value to default to zero
// begin modify by hgl 2011-05-27
		void * re = pthread_getspecific(key);
#ifdef WIN32
		GCAssert(re == 0);
#endif
// end modify
		*tlsId = (uintptr_t) key;
		return true;
	}

	return false;
}

void VMPI_tlsDestroy(uintptr_t tlsId)
{
	pthread_key_delete((pthread_key_t)tlsId);
}
#endif

bool VMPI_tlsSetValue(uintptr_t tlsId, void* value)
{
	const int r = pthread_setspecific((pthread_key_t)tlsId, value);
// begin modify by hgl
	//pthread_t tId = pthread_self(); 
	void * pTmp = pthread_getspecific((pthread_key_t)tlsId);
#ifdef WIN32
	GCAssert( pTmp == value);
#endif
// end modify
	return (r == 0);
}

void* VMPI_tlsGetValue(uintptr_t tlsId)
{
	return pthread_getspecific((pthread_key_t)tlsId);
}

vmpi_thread_t VMPI_currentThread()
{
	return (void*)pthread_self();
}
