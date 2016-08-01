#ifndef __PROPITEM__
#define __PROPITEM__

typedef struct _AS3PROPITEM
{
	enum {NUMBER,OBJECT,EVENT,STRING,LOGIC,NAMESPACE,OTHER};
	XString16 name;
	XString16 value;
	int		 nType;
	XU32	 nData;
	_AS3PROPITEM* next;
}AS3PROPITEM;


struct MOUSEEVENTDATA
{
	bool	ctrlKey;
	bool	buttonDown;
	XU32	delta;
	double	localX;
	double	localY;
	void	*pRelatedObject;
	bool	shiftKey;
	double	stageX;
	double	stageY;
	void	*pTarget;
	void	*pStop;
	bool	bWithTarget;
};

struct KEYEVENTDATA
{
	/*nChar,
		nKey,
		nLocation,
		nKeyStatus[VK_CONTROL],
		0,
		nKeyStatus[VK_SHIFT],
		(int)pTarget*/
	int nChar;
	int nKey;
	int nLocaltion;
	bool bControl;
	bool bAlt;
	bool bShift;
	void*pTarget;
};


#endif