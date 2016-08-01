#pragma once

#include <string.h>

template <class T>
class CPointerArray
{
public:
	CPointerArray(void);
	~CPointerArray(void);
public:
	int		getSize();
	void	addData(const T* element);
	T*		removeData(int index);
	T*		getData(int index);
	void	insertData(int index,const T* element);
	void	release();
protected:
	T		**m_data;
	int		  m_size;
};

template <class T>
CPointerArray<T>::CPointerArray(void)
{
	m_data = NULL;
	m_size = 0;
}

template <class T>
CPointerArray<T>::~CPointerArray(void)
{
	release();
}

template <class T>
int		CPointerArray<T>::getSize()
{
	return m_size;
}

template <class T>
void	CPointerArray<T>::addData(const T* element)
{
	if(element == NULL)
		return;
	T** pptemp;
	pptemp = m_data;
	m_size++;
	m_data = new T*[m_size];
	memcpy(m_data,pptemp,(m_size-1)<<2);
	m_data[m_size-1] = (T*)element;
	delete pptemp;
}

template <class T>
T*		CPointerArray<T>::removeData(int index)
{
	if(index<0 || index>m_size-1) return NULL;
	T** pptemp;
	pptemp = m_data+index;
	T* element;
	element = pptemp[0];
	if(index+1<m_size)
		memmove(pptemp,pptemp+1,(m_size-(index+1))<<2);
	m_size--;
	return element;
}

template <class T>
T*		CPointerArray<T>::getData(int index)
{
	if(index<0 || index>m_size-1) return NULL;
	return m_data[index];
}

template <class T>
void	CPointerArray<T>::insertData(int index,const T* element)
{
	if(index<0) index = 0;
	if(index>m_size) index = m_size;
	T** pptemp;
	pptemp = m_data;
	m_data = new T*[m_size+1];
	if(index != 0)
		memmove(m_data,pptemp,index<<2);
	m_data[index] = element;
	if(index != m_size)
		memmove(m_data+index+1,pptemp+index,(m_size-index)<<2);
	m_size++;
	delete pptemp;
}

template <class T>
void	CPointerArray<T>::release()
{
	for(int i=0 ; i<m_size ; i++)
	{
		delete m_data[i];
	}
	delete m_data;
}