// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "DownloadFile.h"
#include <atlctrls.h>
#include "WinHttpGet.h"
#include "WinHttpPost.h"

class CMainDlg : public CDialogImpl<CMainDlg>
{
private:
//    CWinHttpRequest httpRequest;
    CDownloadFile m_downloadFile;
//  CDownloadFile* m_pDownloadFile;
    CProgressBarCtrl m_progressCtrl;
    VERB_TYPE m_verbType;

public:
    enum { IDD = IDD_MAINDLG };

    BEGIN_MSG_MAP(CMainDlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
    COMMAND_ID_HANDLER(IDOK, OnOK)
    COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
    COMMAND_ID_HANDLER(IDC_START_DOWNLOAD, OnStartDwonload)
    COMMAND_ID_HANDLER(IDC_STOP_DOWNLOAD, OnStopDwonload)
    COMMAND_ID_HANDLER(IDC_QUIT, OnCancel)
    COMMAND_ID_HANDLER(IDC_BTN_SUBMIT, OnSubmit)
    COMMAND_HANDLER(IDC_RADIO1, BN_CLICKED, OnBnClickedRadio1)
    COMMAND_HANDLER(IDC_RADIO2, BN_CLICKED, OnBnClickedRadio2)
    END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//  LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//  LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//  LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

    static void __stdcall OnDataProcess(REQUEST_STATUS status, LPVOID lpCurBuf, DWORD dwCurSize, LPVOID lpParam)
    {
        CMainDlg* pThis = (CMainDlg*)lpParam;
        CProgressBarCtrl progressCtrl = pThis->m_progressCtrl;
        CDownloadFile* pDownloadFile = &pThis->m_downloadFile;

        if(REQUEST_HEADERS_AVAILABLE == status)
        {
            progressCtrl.SetPos((int)(1.0 * pDownloadFile->GetDownloadedSize() / pDownloadFile->GetTotalSize() * 100));
        }
        else if(REQUEST_READCOMPLATE == status || REQUEST_READERROR == status)
        {
            if(REQUEST_READCOMPLATE == status)
            {
                progressCtrl.SetPos(100);
                ::MessageBox(NULL, _T("�������"), _T("�ɹ�"), MB_OK);
            }
            else
                ::MessageBox(NULL, _T("���ش���"), _T("ʧ��"), MB_OK);

        }
        else if(REQUEST_READING == status)
        {
            pThis->m_progressCtrl.SetPos((int)(1.0 * pDownloadFile->GetDownloadedSize() / pDownloadFile->GetTotalSize() * 100));
        }
    }

    LRESULT OnDwonload(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
//         IWinHttpWrapper* pWinHttp = new CWinHttpBase();
//         if(NULL != pWinHttp)
//         {
//             const TCHAR* sUrl = _T("http://ww4.sinaimg.cn/bmiddle/89ad7439jw1ea5low3d1bj20c86esqv5.jpg");
//             const TCHAR* sSavePath = _T("Demo.jpg");
//             pWinHttp->Download(sUrl, sSavePath, 0, 0);
//         }

        return 0;
    }

    CStringW Utf8ToUnicode(CStringA lpszUtf8)
    {
        LPWSTR lpwBuf = NULL;

        try
        {
            //�õ�ת������ַ�������
            DWORD dwBufLen = MultiByteToWideChar(CP_UTF8, 0, lpszUtf8, -1, NULL, NULL);
            //new buffer
            lpwBuf = new wchar_t[dwBufLen];

            if(NULL == lpwBuf)
            {
                return _T("");
            }

            memset(lpwBuf, 0, dwBufLen * sizeof(wchar_t));
            MultiByteToWideChar(CP_UTF8, 0, lpszUtf8, -1, lpwBuf, dwBufLen);
            CStringW s = lpwBuf;
            delete[] lpwBuf;
            return s;
        }
        catch(...)
        {
            return _T("");
        }

        return lpwBuf;
    }

    void Test_GET()
    {
        const TCHAR* sUrl = _T("http://localhost/index.php");
        CWinHttpGet m_httpGet;
        m_httpGet.PostRequest(sUrl);
        CStringA sRes = m_httpGet.GetResponse();
        CString s = Utf8ToUnicode(sRes);
    }

    void Test_POST()
    {
        CWinHttpPost httpPost;
        const TCHAR* postUrl = _T("http://www.sxddck.com/admin/login.php");
        const TCHAR* postParam = _T("username=admin&password=admin&verifycode=&Submit=%E7%99%BB+%E9%99%86");
        httpPost.PostRequest(postUrl, postParam);
        CStringA sRes = httpPost.GetResponse();
        CString s = Utf8ToUnicode(sRes);
    }

    LRESULT OnStartDwonload(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        Test_POST();
//      Test_GET();
        return 0;
        CEdit editUrl = GetDlgItem(IDC_EDIT1);
        CEdit editSavePath = GetDlgItem(IDC_EDIT2);
        CString sUrl;
        editUrl.GetWindowText(sUrl);
        CString sSavePath;
        editSavePath.GetWindowText(sSavePath);

        m_downloadFile.SetCallback(OnDataProcess, this);
        m_downloadFile.DownloadFile(sUrl, sSavePath);
        return 0;
    }

    LRESULT OnStopDwonload(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        m_downloadFile.CancelRequest();
        return 0;
    }

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        // center the dialog on the screen
        CenterWindow();

        // set icons
        HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
        SetIcon(hIcon, TRUE);
        HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
        SetIcon(hIconSmall, FALSE);

        m_progressCtrl = GetDlgItem(IDC_PROGRESS1);
        m_progressCtrl.SetRange(0, 100);
        m_progressCtrl.SetPos(0);

        const TCHAR* sUrl = _T("http://dl_dir.qq.com/invc/qqplayer/QQPlayerMini_Setup_3.2.845.500.exe");
        CEdit editUrl = GetDlgItem(IDC_EDIT1);
        editUrl.SetWindowText(sUrl);

        const TCHAR* sSavePath = _T("test.exe");
        CEdit editSavePath = GetDlgItem(IDC_EDIT2);
        editSavePath.SetWindowText(sSavePath);

        m_verbType = VERB_TYPE_GET;
        CButton btnRadio = GetDlgItem(IDC_RADIO1);
        btnRadio.SetCheck(BST_CHECKED);
        return TRUE;
    }

    LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
        dlg.DoModal();
        return 0;
    }

    LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        // TODO: Add validation code
        EndDialog(wID);
        return 0;
    }

    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        EndDialog(wID);
        return 0;
    }

    LRESULT OnSubmit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        CString sUrl;
        CEdit editUrl = GetDlgItem(IDC_EDIT3);
        editUrl.GetWindowText(sUrl);

        CString sParam;
        CEdit editParam = GetDlgItem(IDC_EDIT4);
        editParam.GetWindowText(sParam);

        CStringA sTemp;
        CString sResult;

        if(VERB_TYPE_GET == m_verbType)
        {
            CWinHttpGet m_httpGet;

            if(m_httpGet.PostRequest(sUrl))
            {
                sTemp = m_httpGet.GetResponse();
                sResult = Utf8ToUnicode(sTemp);
            }
        }
        else if(VERB_TYPE_POST == m_verbType)
        {
            CWinHttpPost httpPost;
            if(httpPost.PostRequest(sUrl, sParam))
            {
                sTemp = httpPost.GetResponse();
                sResult = Utf8ToUnicode(sTemp);
            }
        }

        CEdit editResult = GetDlgItem(IDC_EDIT5);
        editResult.SetWindowText(CA2W(sTemp));
        return 0;
    }
    LRESULT OnBnClickedRadio1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        m_verbType = VERB_TYPE_GET;
        return 0;
    }

    LRESULT OnBnClickedRadio2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        m_verbType = VERB_TYPE_POST;
        return 0;
    }
};
