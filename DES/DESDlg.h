
// DESDlg.h : header file
//

#pragma once

#include "common.h"
#include "afxwin.h"


// CDESDlg dialog
class CDESDlg : public CDialogEx
{
// Construction
public:
	CDESDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnByteViewChange(UINT nID);
	DECLARE_MESSAGE_MAP()
public:
    int byte_representation;
    CString input_text;
    CString cipher;
    CString key;


    common::byte_view_t byte_view;
    common::blocks_t    key_bytes;
    common::blocks_t    cipher_bytes;
    afx_msg void OnBnClickedButtonRandomKey();
    afx_msg void OnBnClickedButtonEncrypt();
    afx_msg void OnBnClickedButton4idcButtonDecrypt();
    CEdit input_text_edit;
    CEdit cipher_edit;
};
