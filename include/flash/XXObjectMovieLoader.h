// XXObjectMovieLoader.h: interface for the XXObjectMovieLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTMOVIELOADER_H__9223D0CE_109F_41A5_8CCF_E4E3648EA3EB__INCLUDED_)
#define AFX_XXOBJECTMOVIELOADER_H__9223D0CE_109F_41A5_8CCF_E4E3648EA3EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectMovieLoader : public XXObject  
{
public:
	XXObjectMovieLoader(ScriptPlayer*pRoot);
	virtual ~XXObjectMovieLoader();

};

#endif // !defined(AFX_XXOBJECTMOVIELOADER_H__9223D0CE_109F_41A5_8CCF_E4E3648EA3EB__INCLUDED_)
