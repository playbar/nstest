#ifndef __AS3_EVENT_ARG_STRUCT_
#define __AS3_EVENT_ARG_STRUCT_

typedef struct stBaseArg
{
	Stringp		type;
	bool		bubbles;
	bool		cancelable;
}BaseArg, *pBaseArg;

typedef struct stB_Arg			//b for bool
{
	BaseArg		baseArg;
	bool		bArg;
}B_Arg, *pB_Arg;

typedef struct stS_Arg			//S	for Stringp
{
	BaseArg		baseArg;
	Stringp		sArg;
}S_Arg, *pS_Arg;

typedef struct stI_Arg			//I	for int32_t
{
	BaseArg		baseArg;
	int32_t		iArg;
}I_Arg, *pI_Arg;

typedef struct stUIUI_Arg		//UI for uint32_t
{
	BaseArg		baseArg;
	uint32_t	uiArg1;
	uint32_t	uiArg2;
}UIUI_Arg, *pUIUI_Arg;

typedef struct stO_Arg			//O	for Object
{
	BaseArg		baseArg;
	void		*pObject;
};

typedef struct stSS_Arg
{
	BaseArg		baseArg;
	Stringp		sArg1;
	Stringp		sArg2;
}SS_Arg, *pSS_Arg;

typedef struct stSO_Arg
{
	BaseArg		baseArg;
	Stringp		sArg;
	void		*pObject;
};

typedef struct stOO_Arg
{
	BaseArg		baseArg;
	void		*pObject1;
	void		*pObject2;
};

typedef struct stOBUI_Arg
{
	BaseArg		baseArg;
	void		*pObject;
	bool		bArg;
	uint32_t	uiArg;
};

typedef struct stKeyboardEvent_Arg
{
	BaseArg		baseArg;
	uint32_t	uiArg1;
	uint32_t	uiArg2;
	uint32_t	uiArg3;
	bool		bArg1;
	bool		bArg2;
	bool		bArg3;
}KeyboardEvent_Arg, *pKeyboardEvent_Arg;

typedef	struct stMouseEvent_Arg
{
	BaseArg		baseArg;
	double		dArg1;
	double		dArg2;
	void		*pObject;
	bool		bArg1;
	bool		bArg2;
	bool		bArg3;
	bool		bArg4;
	int32_t		iArg;
}MouseEvent_Arg, *pMouseEvent_Arg;

#endif