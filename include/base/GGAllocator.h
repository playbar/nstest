/*
 *  GGAllocator.h
 *  jqbar
 *
 *  Created by lining on 10-11-12.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _GG_ALLOCATOR_H_
#define _GG_ALLOCATOR_H_

#include <new>
#include <loki/SmallObj.h>

typedef Loki::SmallObject< ::Loki::SingleThreaded > LokiSmallObject;
typedef LokiSmallObject::ObjAllocatorSingleton::MyAllocatorSingleton LokiSmallObjectSingleton;

class GGAllocator : public LokiSmallObject
{
public:
	void test();
	
//	static void* operator new(size_t size);
//	static void* operator new[](size_t size);
//	static void operator delete(void* p);
//	static void operator delete[](void* p);
	
};


#endif