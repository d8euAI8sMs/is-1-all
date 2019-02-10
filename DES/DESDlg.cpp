
// DESDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DES.h"
#include "DESDlg.h"
#include "afxdialogex.h"

#include "des_impl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDESDlg dialog



CDESDlg::CDESDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDESDlg::IDD, pParent)
    , byte_view(common::byte_view_t::hex)
    , byte_representation(static_cast<int>(common::byte_view_t::hex))
    , input_text(_T(""))
    , cipher(_T(""))
    , key(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDESDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_RADIO_VIEW_HEX, byte_representation);
    DDX_Text(pDX, IDC_INPUT_TEXT, input_text);
    DDX_Text(pDX, IDC_OUTPUT_CIPHER, cipher);
    DDX_Text(pDX, IDC_KEY, key);
    DDX_Control(pDX, IDC_INPUT_TEXT, input_text_edit);
    DDX_Control(pDX, IDC_OUTPUT_CIPHER, cipher_edit);
}

BEGIN_MESSAGE_MAP(CDESDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_VIEW_DEC, IDC_RADIO_VIEW_BIN, &OnByteViewChange)
    ON_BN_CLICKED(IDC_BUTTON_RANDOM_KEY, &CDESDlg::OnBnClickedButtonRandomKey)
    ON_BN_CLICKED(IDC_BUTTON_ENCRYPT, &CDESDlg::OnBnClickedButtonEncrypt)
    ON_BN_CLICKED(IDC_BUTTON4IDC_BUTTON_DECRYPT, &CDESDlg::OnBnClickedButton4idcButtonDecrypt)
END_MESSAGE_MAP()


// CDESDlg message handlers

BOOL CDESDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    input_text_edit.SetLimitText(0);
    cipher_edit.SetLimitText(0);

    UpdateData(FALSE);

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDESDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDESDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



afx_msg void CDESDlg::OnByteViewChange(UINT nID)
{
    UpdateData(TRUE);
    byte_view = static_cast<common::byte_view_t>(byte_representation);
    if (!key.IsEmpty())
    {
        common::string_t str = common::to_string(key_bytes, byte_view);
        key = str.c_str();
    }
    if (!cipher.IsEmpty())
    {
        common::string_t str = common::to_string(cipher_bytes, byte_view);
        cipher = str.c_str();
    }
    UpdateData(FALSE);
}


void CDESDlg::OnBnClickedButtonRandomKey()
{
    UpdateData(TRUE);
    key_bytes.clear();
    key_bytes.push_back(common::random_block());
    common::string_t str = common::to_string(key_bytes, byte_view);
    key = str.c_str();
    UpdateData(FALSE);
}


void CDESDlg::OnBnClickedButtonEncrypt()
{
    UpdateData(TRUE);
    if (key.IsEmpty()) {
        OnBnClickedButtonRandomKey();
    }
    common::string_t str(CT2A(input_text.GetBuffer()));
    common::blocks_t input_bytes = common::split(str);
    cipher_bytes = des::encrypt(input_bytes, key_bytes[0]);
    common::string_t output = common::to_string(cipher_bytes, byte_view);
    cipher = output.c_str();
    UpdateData(FALSE);
}


void CDESDlg::OnBnClickedButton4idcButtonDecrypt()
{
    UpdateData(TRUE);
    if (key.IsEmpty())
    {
        OnBnClickedButtonRandomKey();
    }
    common::string_t str(CT2A(cipher.GetBuffer()));
    common::blocks_t input_bytes = common::from_string(str, byte_view);
    common::blocks_t output_bytes = des::decrypt(input_bytes, key_bytes[0]);
    common::string_t output = common::combine(output_bytes);
    input_text = output.c_str();
    UpdateData(FALSE);
}
