/*
 *  GGAlocator.cpp
 *  jqbar
 *
 *  Created by lining on 10-11-12.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "GGAllocator.h"

void GGAllocator::test()
{
	LokiSmallObjectSingleton::Instance().Allocate( 10, true );
}


//
//void* GGAllocator::operator new(size_t size)
//{
//	static int new_count = 0;
//	new_count++;
//	return ::operator new(size);
//}
//
//void* GGAllocator::operator new[] (size_t size)
//{
//	return ::operator new[](size);
//}
//
//void GGAllocator::operator delete(void* p)
//{
//	static int delete_count = 0;
//	delete_count++;
//	return ::operator delete(p);
//}
//
//void GGAllocator::operator delete[] (void* p)
//{
//	return ::operator delete[](p);
//
//}