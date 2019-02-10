
// DESDlg.h : header file
//

#pragma once

#include "common.h"
#include "afxwin.h"


enum class role_t { na, A, B };
enum class stage_t { na, n_g_gen, n_g_io, X_Y_gen, X_Y_i, X_Y_o, key_gen };

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
	DECLARE_MESSAGE_MAP()
public:

    common::int_t n, g, Y, x, X, k;
    role_t role;
    stage_t stage;
    stage_t lstage;

    void set_stage(stage_t s)
    {
        lstage = stage;
        stage = s;
    }

    long long io_n, io_g, io_Y, io_x, io_k;

    CString role_str;
    CString stage_str;

    void PrepareData();

    afx_msg void OnBnClickedButton4idcButtonDecrypt2();
    afx_msg void OnBnClickedButtonEncrypt();
    afx_msg void OnBnClickedButton4idcButtonDecrypt();
};
