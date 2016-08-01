/*
 * ggutil.h
 *
 *  Created on: 2011-7-4
 *      Author: yock
 */

#ifndef GGUTIL_H_
#define GGUTIL_H_

class gg_alloca
	{
public:
	void* m_hData;
public:
	gg_alloca(int len)
		{
		m_hData = new unsigned char[len];
		}
	~gg_alloca()
		{
		delete m_hData;
		}
	void* GetData(){return m_hData;}
	};


#endif /* GGUTIL_H_ */
