
// DESDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DES.h"
#include "DESDlg.h"
#include "afxdialogex.h"

#include "dh_impl.h"

#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDESDlg dialog


namespace {

    inline CString to_string(role_t str)
    {
        switch (str)
        {
        case role_t::na:
        default:
            return _T("N/A");
        case role_t::A:
            return _T("A");
        case role_t::B:
            return _T("B");
        }
    }

    inline CString to_string(stage_t str, role_t role)
    {
        switch (str)
        {
        case stage_t::na:
        default:
            return _T("N/A");
        case stage_t::n_g_gen:
            return _T("Generate n, g");
        case stage_t::n_g_io:
            return role == role_t::A ? _T("Send n, g") : _T("Receive n, g");
        case stage_t::X_Y_gen:
            return _T("Generate x, X");
        case stage_t::X_Y_i:
            return _T("Receive Y");
        case stage_t::X_Y_o:
            return _T("Send X");
        case stage_t::key_gen:
            return _T("Generate Key");
        }
    }
}


CDESDlg::CDESDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDESDlg::IDD, pParent)
    , role(role_t::na)
    , stage(stage_t::na)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    role_str = to_string(role);
    stage_str = to_string(stage, role);
}

void CDESDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_IO_N, io_n);
    DDX_Text(pDX, IDC_IO_G, io_g);
    DDX_Text(pDX, IDC_IN_Y, io_Y);
    DDX_Text(pDX, IDC_OUT_X, io_x);
    DDX_Text(pDX, IDC_OUT_KEY, io_k);
    DDX_Text(pDX, IDC_ROLE, role_str);
    DDX_Text(pDX, IDC_STAGE, stage_str);
}

BEGIN_MESSAGE_MAP(CDESDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON4IDC_BUTTON_DECRYPT2, &CDESDlg::OnBnClickedButton4idcButtonDecrypt2)
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

void CDESDlg::PrepareData()
{
    role_str = to_string(role);
    stage_str = to_string(stage, role);
    io_n = n;
    io_g = g;
    io_k = k;
    io_x = x;
    io_Y = Y;
}

void CDESDlg::OnBnClickedButton4idcButtonDecrypt2()
{
    switch (stage)
    {
    case stage_t::na:
    case stage_t::n_g_gen:
        role = role_t::A;
        set_stage(stage_t::n_g_io);
        n = dh::random_prime();
        g = dh::primitive_root(n);
        k = x = Y = 0;
        break;
    case stage_t::X_Y_gen:
        set_stage(role == role_t::A ? stage_t::X_Y_o : stage_t::X_Y_i);
        x = dh::random_int();
        X = dh::pows(g, x, n);
        break;
    case stage_t::key_gen:
        set_stage(stage_t::na);
        k = dh::pows(Y, x, n);
        break;
    default:
        set_stage(stage_t::na);
        break;
    }

    PrepareData();
    UpdateData(FALSE);
}


void CDESDlg::OnBnClickedButtonEncrypt()
{
    switch (stage)
    {
    case stage_t::n_g_io:
        if (role == role_t::A)
        {
            set_stage(stage_t::X_Y_gen);
            std::ofstream os("dh.bin", std::ios::binary);
            os << n << " " << g;
            os.close();
        }
        else
        {
            set_stage(stage_t::na);
        }
        break;
    case stage_t::X_Y_o:
        if (lstage == stage_t::X_Y_i)
        {
            set_stage(stage_t::key_gen);
        }
        else
        {
            set_stage(stage_t::X_Y_i);
        }
        {
            std::ofstream os("dh.bin", std::ios::binary);
            os << X;
            os.close();
        }
        break;
    default:
        break;
    }

    PrepareData();
    UpdateData(FALSE);
}


void CDESDlg::OnBnClickedButton4idcButtonDecrypt()
{
    switch (stage)
    {
    case stage_t::na:
    case stage_t::n_g_io:
        role = role_t::B;
        k = x = Y = 0;
        {
            set_stage(stage_t::X_Y_gen);
            std::ifstream os("dh.bin", std::ios::binary);
            os >> n >> g;
            os.close();
        }
        break;
    case stage_t::X_Y_i:
        if (lstage == stage_t::X_Y_o)
        {
            set_stage(stage_t::key_gen);
        }
        else
        {
            set_stage(stage_t::X_Y_o);
        }
        {
            std::ifstream os("dh.bin", std::ios::binary);
            os >> Y;
            os.close();
        }
        break;
    default:
        break;
    }

    PrepareData();
    UpdateData(FALSE);
}
