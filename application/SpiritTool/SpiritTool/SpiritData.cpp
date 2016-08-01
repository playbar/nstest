#include "StdAfx.h"
#include "SpiritData.h"


CSpiritData::CSpiritData(void)
{
	m_hAppname = NULL;
	m_hOutpath = NULL;
	m_hIconpath = NULL;
	m_hApkname = NULL;
	m_hPackagename = NULL;
	m_hSpiritUA = NULL;
	m_hChannelId = NULL;
	m_hImagepath1 = NULL;
	m_hImagepath2 = NULL;
	m_hGifpath = NULL;
	m_hBWpath = NULL;
	m_bDesktop = false;
}


CSpiritData::~CSpiritData(void)
{
#define R(M) {if(M) delete M;M=NULL;}
	R(m_hAppname);
	R(m_hOutpath);
	R(m_hIconpath);
	R(m_hApkname);
	R(m_hPackagename);
	R(m_hSpiritUA);
	R(m_hChannelId);
	R(m_hImagepath1);
	R(m_hImagepath2);
	R(m_hGifpath);
	R(m_hBWpath);
	m_bDesktop = false;
#undef R
}
