// FolderDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SpiritTool.h"
#include "FolderDialog.h"


// CFolderDialog


IMPLEMENT_DYNAMIC(CFolderDialog, CFileDialog)

	
WNDPROC CFolderDialog::m_wndProc = NULL;

bool CFolderDialog::GetFolder(CString& folderpath, const wchar_t* szCaption,HWND hOwner)
{
	
	bool retVal = false;


    // The BROWSEINFO struct tells the shell
    // how it should display the dialog.
    BROWSEINFO bi;
    memset(&bi, 0, sizeof(bi));
    bi.ulFlags   = BIF_USENEWUI;
    bi.hwndOwner = hOwner;
    bi.lpszTitle = szCaption;

    // must call this if using BIF_USENEWUI
    ::OleInitialize(NULL);

    // Show the dialog and get the itemIDList for the selected folder.
    LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);

    if(pIDL != NULL)
    {
        // Create a buffer to store the path, then get the path.
        wchar_t buffer[_MAX_PATH] = {'\0'};
        if(::SHGetPathFromIDList(pIDL, buffer) != 0)
        {
            // Set the string value.
            folderpath = buffer;
            retVal = true;
        }       
        // free the item id list
        CoTaskMemFree(pIDL);
    }

    ::OleUninitialize();

    return retVal;
}

CFolderDialog::CFolderDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{

}

// Function name    : CFolderDialog::CFolderDialog
// Description      : Constructor
// Return type      :
// Argument         : CString* pPath ; represent string where selected folder wil be saved
CFolderDialog::CFolderDialog(CString* pPath, CWnd* pParentWnd) : CFileDialog(true, NULL, _T("*..*"), 6UL, NULL, pParentWnd)
{
    m_pPath = pPath;
}


CFolderDialog::~CFolderDialog()
{
}


BEGIN_MESSAGE_MAP(CFolderDialog, CFileDialog)
END_MESSAGE_MAP()

// CFolderDialog 消息处理程序
LRESULT CALLBACK WindowProcNew(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message ==  WM_COMMAND)
    {
        if(HIWORD(wParam) == BN_CLICKED)
        {
            if(LOWORD(wParam) == IDOK)
            {
                if(CFileDialog* pDlg = (CFileDialog*)CWnd::FromHandle(hwnd))
                {
                    TCHAR path[MAX_PATH];
                    GetCurrentDirectory(MAX_PATH, path);
                    *((CFolderDialog*)pDlg->GetDlgItem(0))->m_pPath = CString(path);
                    pDlg->EndDialog(IDOK);
                    return NULL;
                }
            }
        }
    }
    return CallWindowProc(CFolderDialog::m_wndProc, hwnd, message, wParam, lParam);
}



// Function name    : CFolderDialog::OnInitDone
// Description      : For update the wiew of CFileDialog
// Return type      : void
void CFolderDialog::OnInitDone()
{


    //Rearrange the controls in the bottom.
    CWnd* pFD = GetParent();

	/*
    HideControl(edt1);

    //HideControl(stc3);    //Select file static text
    //HideControl(cmb13);   //Current file combobox

    HideControl(cmb1);      //File filter combobox

    HideControl(stc2);      //Filter static text

    //Enlarge Listview control
    CRect rectList2;
    pFD->GetDlgItem(lst1)->GetWindowRect(rectList2);
    pFD->ScreenToClient(rectList2);
    pFD->GetDlgItem(lst1)->SetWindowPos(0, 0, 0, rectList2.Width(), abs(rectList2.top - (rectCancel.top - 4)), SWP_NOMOVE | SWP_NOZORDER);

    //Set Static text and position
    CRect rectText;
    pFD->GetDlgItem(stc3)->GetWindowRect(rectText);
    pFD->ScreenToClient(rectText);
    pFD->GetDlgItem(stc3)->SetWindowPos(0, rectList2.left, rectCancel.top + 6, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    SetControlText(stc3, _T("Selected:"));

    //ComboBox of current file
    CRect rectComBo;
    pFD->GetDlgItem(cmb13)->GetWindowRect(rectComBo);
    pFD->ScreenToClient(rectComBo);
    pFD->GetDlgItem(cmb13)->SetWindowPos(0, rectText.left + rectText.Width() - 40, rectCancel.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	*/
	//Get Cancel Button Position
    CRect rectCancel;
    pFD->GetDlgItem(IDCANCEL)->GetWindowRect(rectCancel);
    pFD->ScreenToClient(rectCancel);

    //Set OK Button Position
    CRect rectOK;
    pFD->GetDlgItem(IDOK)->GetWindowRect(rectOK);
    pFD->ScreenToClient(rectOK);
    pFD->GetDlgItem(IDOK)->SetWindowPos(0, rectCancel.left - rectOK.Width() - 2, rectCancel.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    SetControlText(IDOK, _T("Select"));

    pFD->SetWindowText(_T("Choose folder"));
    pFD->CenterWindow();
    m_wndProc = (WNDPROC)SetWindowLong(pFD->m_hWnd, GWL_WNDPROC, (long)WindowProcNew);
}



//Change the combobox context when select folder.
void CFolderDialog::OnFileNameChange()
{
    ChangeFolder();
}

//If the folder contains no sub folder, the OnFileNameChange will not be triggered.
//Add this OnFolderChange to double sure even if there is no sub folder.
void CFolderDialog::OnFolderChange()
{
    ChangeFolder();
}

//Change the combobox text to current selected folder
void CFolderDialog::ChangeFolder()
{
    TCHAR path[MAX_PATH] = {0};
    GetCurrentDirectory(MAX_PATH, path);
//    SetControlText(1152, path);
}





