#ifndef _FLEXINIT_
#define _FLEXINIT_

namespace FLEX
{
	extern bool LoadIfNeed(String* className,XAVM2*pAVM2);
	extern ScriptPlayer* InitFlex(XSWFPlayer*root,XAVM2*pAVM2,ShellCodeContext*pContext,int nFlex);
	extern void			 ReleaseFlex(ScriptPlayer*);
}

#endif