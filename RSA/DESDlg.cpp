
// DESDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DES.h"
#include "DESDlg.h"
#include "afxdialogex.h"

#include "rsa_impl.h"

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
    , pub_key(0)
    , pr_key(0)
    , mod_key(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    srand(clock() % UINT_MAX);

//     rsa::gen_erath();

//     for (int i = 0; i < 10000000; i++)
//     {
//         rsa::int_t n, e, d, p, q, phi; int d1;
//         rsa::random_keys(n, e, d);
// 
//         for (rsa::byte_t j = 0; j < 255; j++)
//         {
//             rsa::byte_t k = j;
//             rsa::int_t c = rsa::pows(k, e, n);
//             rsa::byte_t k2 = rsa::pows(c, d, n);
//             if (k != k2)
//             {
//                 assert(k != k2);
//             }
//         }
//     }
}

void CDESDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_RADIO_VIEW_HEX, byte_representation);
    DDX_Text(pDX, IDC_INPUT_TEXT, input_text);
    DDX_Text(pDX, IDC_OUTPUT_CIPHER, cipher);
    DDX_Control(pDX, IDC_INPUT_TEXT, input_text_edit);
    DDX_Control(pDX, IDC_OUTPUT_CIPHER, cipher_edit);
    DDX_Text(pDX, IDC_PUBLIC_KEY, pub_key);
    DDX_Text(pDX, IDC_PRIVATE_KEY, pr_key);
    DDX_Text(pDX, IDC_MODULO, mod_key);
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
    if (!cipher.IsEmpty())
    {
        common::string_t str = common::to_string_view(cipher_bytes, byte_view);
        cipher = str.c_str();
    }
    UpdateData(FALSE);
}


void CDESDlg::OnBnClickedButtonRandomKey()
{
    UpdateData(TRUE);
    rsa::random_keys(n, e, d);
    pub_key = e;
    pr_key = d;
    mod_key = n;
    UpdateData(FALSE);
}


void CDESDlg::OnBnClickedButtonEncrypt()
{
    UpdateData(TRUE);
    if (pub_key == 0) OnBnClickedButtonRandomKey();
    common::string_t str(CT2A(input_text.GetBuffer()));
    cipher_bytes = common::to_bstring(str);
    for (size_t i = 0; i < cipher_bytes.size(); i++)
    {
        cipher_bytes[i] = rsa::pows(cipher_bytes[i], e, n);
    }
    str = common::to_string_view(cipher_bytes, byte_view);
    CString s(str.c_str());
    cipher = s;
    UpdateData(FALSE);
}


void CDESDlg::OnBnClickedButton4idcButtonDecrypt()
{
    UpdateData(TRUE);
    common::string_t str(CT2A(cipher.GetBuffer()));
    common::ints_t bstr = common::from_string_view(str, byte_view);
    for (size_t i = 0; i < cipher_bytes.size(); i++)
    {
        bstr[i] = rsa::pows(bstr[i], d, n);
    }
    str = common::to_string(bstr);
    CString s(str.c_str());
    input_text = s;
    UpdateData(FALSE);
}
