#pragma once


// CFolderDialog

class CFolderDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CFolderDialog)

public:
	static bool GetFolder(CString& folderpath, const wchar_t* szCaption = NULL, HWND hOwner = NULL);

public:
	CFolderDialog(CString* pPath, CWnd* pParentWnd = NULL);
	CFolderDialog(BOOL bOpenFileDialog, // 对于 FileOpen 为 TRUE，对于 FileSaveAs 为 FALSE
	LPCTSTR lpszDefExt = NULL,
	LPCTSTR lpszFileName = NULL,
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	LPCTSTR lpszFilter = NULL,
	CWnd* pParentWnd = NULL);
	virtual ~CFolderDialog();
	
public:

    static WNDPROC m_wndProc;
    CString* m_pPath;
protected:
	DECLARE_MESSAGE_MAP()
private:
    virtual void OnInitDone();
    virtual void OnFileNameChange();
    virtual void OnFolderChange();
    void ChangeFolder();
};


