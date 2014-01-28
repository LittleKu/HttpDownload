#pragma once
#include "winhttprequest.h"

class CDownloadFile : public CWinHttpRequest
{
public:
    typedef void (__stdcall *DownloadCallback)(REQUEST_STATUS status, LPVOID lpCurBuf, DWORD dwCurSize, LPVOID lpParam);
public:
    CDownloadFile(void);
    virtual ~CDownloadFile(void);

    /**
     * ���ûص���������ʾ����ʱʹ��
     * @param callBack  �û����õĻص����������ݵ���ʱ���Զ�����
     * @param lpParam �û�ָ���Ļص������Ĳ���
     */
    void SetCallback(DownloadCallback callBack, LPVOID lpParam);

    /**
     * �����ļ�
     * @param  lpszUrl ���ص�ַ
     * @param  lpszSavePath �����·��
     * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
     */
    BOOL DownloadFile(LPCTSTR lpszUrl, LPCTSTR lpszSavePath);

    /**
     * ��ȡ�ļ��ܴ�С
     * @return ���ط��������ļ����ܴ�С
     */
    DWORD GetTotalSize()const;

    /**
     * ��ȡ�Ѿ����ص��ֽ���
     * @return �Ѿ����ص��ֽ���
     */
    DWORD GetDownloadedSize()const;
protected:
    /**
     * ��ȡ�Ѿ����ص��ļ��ֽ���,�����ϵ�����
     * @param  dwContinueIndex
     * @return �жϵ������ļ�����TRUE,ʧ�ܷ���FALSE
     */
    BOOL GetContinueLen(DWORD& dwContinueIndex);

    /**
     * �ر�HTTP����,�ر��ļ����
     */
    void Close();

    /**
     * ��������ʱ�ص�����
     * @param status ����״̬��,������,���ش���,�������
     * @param lpCurBuf ��ǰ���ص�������
     * @param dwCurSize ��ǰ���ص������ݳ���
     */
    virtual void OnDataArrived(REQUEST_STATUS status, LPVOID lpCurBuf, DWORD dwCurSize);
private:
    HANDLE m_hFile;             // �ļ����
    DWORD m_dwTotalFileSize;    // �ļ��ܴ�С
    DWORD m_dwDownloaded;       // �Ѿ����ص��ֽ���
    CString m_sFilePath;        // �����ļ�����·��
    DownloadCallback m_callBack;    // �ص�����
    LPVOID m_callBackParam;     // ���ػص�����
};

