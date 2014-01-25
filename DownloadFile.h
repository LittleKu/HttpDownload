#pragma once
#include "winhttprequest.h"

class CDownloadFile : public CWinHttpRequest
{
public:
    typedef void (__stdcall *DownloadCallback)(REQUEST_STATUS status, LPVOID lpCurBuf, DWORD dwCurSize, LPVOID lpParam);
public:
    CDownloadFile(void);
    virtual ~CDownloadFile(void);
    void SetCallback(DownloadCallback callBack, LPVOID lpParam);
    BOOL DownloadFile(LPCTSTR lpszUrl, LPCTSTR lpszSavePath);
    DWORD GetTotalSize()const;
    DWORD GetDownloadedSize()const;
protected:
	BOOL GetContinueLen(DWORD& dwContinueIndex);
	void Close();
    virtual void OnDataArrived(REQUEST_STATUS status, LPVOID lpCurBuf, DWORD dwCurSize);
private:
    HANDLE m_hFile;
    DWORD m_dwTotalFileSize;    // �ļ��ܴ�С
    DWORD m_dwDownloaded;       // �Ѿ����ص��ֽ���
    CString m_sFilePath;
    DownloadCallback m_callBack;
    LPVOID m_callBackParam;
};

