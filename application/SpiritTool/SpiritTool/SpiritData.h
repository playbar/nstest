#pragma once
class CSpiritData
{
public:
	CSpiritData(void);
	~CSpiritData(void);
public:
	char*	m_hAppname;
	char*	m_hOutpath;
	char*	m_hIconpath;
	char*	m_hApkname;
	char*	m_hPackagename;
	char*	m_hSpiritUA;
	char*	m_hChannelId;
	char*	m_hImagepath1;
	char*	m_hImagepath2;
	char*	m_hGifpath;
	char*	m_hBWpath;
	bool	m_bDesktop;
};

