// qq_auto_loginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "qq_auto_login.h"
#include "qq_auto_loginDlg.h"
#include ".\qq_auto_logindlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cqq_auto_loginDlg �Ի���

Cqq_auto_loginDlg::Cqq_auto_loginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cqq_auto_loginDlg::IDD, pParent)
	, m_nLoginQQNum(5)
	, m_nCurLoginQQIndex(0)
	, m_nPauseTime(10)
	, m_bStartAutoMode(TRUE)
	, m_pDlgTip(NULL)
	, m_nQQ2013(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	string qq_exe_path, qq_account_path;
	theConfig.GetString("PATH", "QQ_EXE_PATH", qq_exe_path, "config.ini");
	theConfig.GetString("PATH", "QQ_ACCOUNT_PATH", qq_account_path, "config.ini");
	theConfig.GetInt("QQ_NUM", "NUM", m_nLoginQQNum, "config.ini");
	theConfig.GetInt("TIMER",  "ELAPSE", m_nPauseTime, "config.ini");
	theConfig.GetInt("QQ_NUM", "CUR_INDEX", m_nCurLoginQQIndex, "config.ini");
	theConfig.GetInt("QQ_NUM", "QQ2013", m_nQQ2013, "config.ini");

	m_strQQExePath = qq_exe_path.c_str();
	m_strQQAccountPath = qq_account_path.c_str();
}

Cqq_auto_loginDlg::~Cqq_auto_loginDlg()
{
	theConfig.SetInt("QQ_NUM", "CUR_INDEX", m_nCurLoginQQIndex, "config.ini");

	if (NULL != m_pDlgTip)
	{
		delete m_pDlgTip;
	}
};

void Cqq_auto_loginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nLoginQQNum);
	DDV_MinMaxInt(pDX, m_nLoginQQNum, 1, 10);
	DDX_Text(pDX, IDC_EDIT2, m_nPauseTime);
	DDV_MinMaxInt(pDX, m_nPauseTime, 1, INT_MAX);
	DDX_Text(pDX, IDC_EDIT3, m_strQQExePath);
	DDX_Text(pDX, IDC_EDIT4, m_strQQAccountPath);
}

BEGIN_MESSAGE_MAP(Cqq_auto_loginDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
END_MESSAGE_MAP()


// Cqq_auto_loginDlg ��Ϣ�������

BOOL Cqq_auto_loginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	m_pDlgTip=new CExpToolTip;
	m_pDlgTip->SetTitleColor(RGB(128,128,255));
	m_pDlgTip->Create(this);//������Ϣ��ʾ��
		
	if (1 == m_nQQ2013)
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);
	}
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void Cqq_auto_loginDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cqq_auto_loginDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR Cqq_auto_loginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//�ֹ�ģʽ
void Cqq_auto_loginDlg::OnBnClickedButton1()	
{
	//�Ƚ�����ǰ����QQ����
	Fun_TerminateProcess("qq.exe");

	UpdateData();
	if (m_strQQAccountPath == "" || m_strQQExePath == "")
	{
		MessageBox("��������QQ2012������·���Լ�QQ�ʺ���������·��");
		return ;
	}

	//��ȡQQ�ʺ�
	ReadQQAccount(m_strQQAccountPath,  m_mapQQ);
	if (m_mapQQ.size() <= 0)
	{
		MessageBox("��ȡ����QQ�ʺ������루���ܸ�ʽ���ԣ���ʽ��QQ�����������|�ŷָ������£� 123456|11111");
		return ;
	}
	
	//����ļ����QQ�ʺ�С������Ҫ���е�QQ��Ŀ��������Ҫ���е�QQ��Ŀ����QQ�ʺ���
	if (m_nLoginQQNum > m_mapQQ.size())
	{
		m_nLoginQQNum = m_mapQQ.size();
		UpdateData(FALSE);
	}

	//�ȶ�λ��ɨ�赽��QQ����
	if (m_nCurLoginQQIndex >= m_mapQQ.size())
	{
		m_nCurLoginQQIndex = 0;			
	}
	vector_type::iterator it = m_mapQQ.begin();
	for (int i=0; i<m_nCurLoginQQIndex; i++, it++);
	
	//����ָ������Ŀ��QQ
	int nPreLoginQQNum = 0;
	while (it != m_mapQQ.end())
	{
		if (nPreLoginQQNum < m_nLoginQQNum )
		{
			AotoLoginQQ(m_nQQ2013,  m_strQQExePath, it->first.c_str(), it->second.c_str());
		}
		else
		{
			break;
		}
		m_nCurLoginQQIndex++;
		nPreLoginQQNum++;
		it++;
	}
	
	//�������� 
	theConfig.SetInt("QQ_NUM", "NUM", m_nLoginQQNum, "config.ini");
	theConfig.SetInt("TIMER",  "ELAPSE", m_nPauseTime, "config.ini");
	theConfig.SetInt("QQ_NUM", "CUR_INDEX", m_nCurLoginQQIndex, "config.ini");	
	theConfig.SetInt("QQ_NUM", "QQ2013", m_nQQ2013, "config.ini");
}

HBRUSH Cqq_auto_loginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TIP)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void Cqq_auto_loginDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1000)
	{
		//��������QQ����
		Fun_TerminateProcess("qq.exe");

		//�ȶ�λ��ɨ�赽��QQ����
		if (m_nCurLoginQQIndex >= m_mapQQ.size())
		{
			m_nCurLoginQQIndex = 0;			
		}
		vector_type::iterator it = m_mapQQ.begin();
		for (int i=0; i<m_nCurLoginQQIndex; i++, it++);
		
		int nPreLoginQQNum = 0;
		while (it != m_mapQQ.end())
		{
			if (nPreLoginQQNum < m_nLoginQQNum )
			{
				AotoLoginQQ(m_nQQ2013,  m_strQQExePath, it->first.c_str(), it->second.c_str());
			}
			else
			{
				break;
			}
			m_nCurLoginQQIndex++;
			nPreLoginQQNum++;
			it++;
			if (m_nCurLoginQQIndex >= m_mapQQ.size())
			{
				m_nCurLoginQQIndex = 0;
				it = m_mapQQ.begin();
			}
		}

		theConfig.SetInt("QQ_NUM", "CUR_INDEX", m_nCurLoginQQIndex, "config.ini");
	}
	CDialog::OnTimer(nIDEvent);
}

void Cqq_auto_loginDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE, "exe", "QQ.exe",OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,"��ִ���ļ�(*.exe)|*.exe||", this);
 	if (dlg.DoModal() == IDOK)
	{
		m_strQQExePath = dlg.GetPathName();

		theConfig.SetString("PATH", "QQ_EXE_PATH", string(m_strQQExePath), "config.ini");
		UpdateData(FALSE);
	}
}

void Cqq_auto_loginDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE, "exe", "QQ.txt",OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,"�ı��ļ�(*.txt)|*.txt||", this);
	if (dlg.DoModal() == IDOK)
	{
		m_strQQAccountPath = dlg.GetPathName();
		theConfig.SetString("PATH", "QQ_ACCOUNT_PATH", string(m_strQQAccountPath), "config.ini");
		UpdateData(FALSE);
	}
}

void Cqq_auto_loginDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_bStartAutoMode)
	{		
		OnBnClickedButton1();
		SetTimer(1000, 1000*60*m_nPauseTime, NULL);
		SetDlgItemText(IDC_BUTTON2, "ֹͣ�Զ�ģʽ");
	}
	else
	{
		KillTimer(1000);
		SetDlgItemText(IDC_BUTTON2, "�����Զ�ģʽ");		
	}

	m_bStartAutoMode = !m_bStartAutoMode;
}

void Cqq_auto_loginDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��������QQ����
	Fun_TerminateProcess("qq.exe");
}

void Cqq_auto_loginDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nCurLoginQQIndex = 0;
	theConfig.SetInt("QQ_NUM", "CUR_INDEX", m_nCurLoginQQIndex, "config.ini");
}

BOOL Cqq_auto_loginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	CRect rc1, rc2, rc3, rc4;
	GetDlgItem(IDC_BUTTON1)->GetWindowRect(rc1);
	GetDlgItem(IDC_BUTTON2)->GetWindowRect(rc2);
	GetDlgItem(IDC_BUTTON5)->GetWindowRect(rc3);
	GetDlgItem(IDC_BUTTON6)->GetWindowRect(rc4);

	ScreenToClient(rc1);
	ScreenToClient(rc2);
	ScreenToClient(rc3);
	ScreenToClient(rc4);
	CString strManalMode, strAutoMode, strCloseAllQQ, strClearQQMemory;
	strManalMode.Format("������ǰ�������е�����QQ���̣�Ȼ�����Ѽ����˳����������%d��", m_nLoginQQNum);
	strAutoMode.Format("������ǰ�������е�����QQ���̣�Ȼ�����Ѽ����˳����������%d��\n����ͣ%d���ӣ�Ȼ�������ǰ����QQ���̣������������ѭ����ֱ�����ֹͣ�Զ�ģʽ", m_nLoginQQNum, m_nPauseTime);
	strCloseAllQQ.Format("������ǰ���������е�����QQ����");
	strClearQQMemory.Format("��˰�ť���´ε�¼��QQ�����õ��ļ��еĵ�һ�����¿�ʼ");
	
	if (WM_MOUSEMOVE == pMsg->message)
	{
		CPoint point(pMsg->pt);
		ScreenToClient(&point);
		//�ж���λ���ڷ�Χ��,���Ҳ��ڱ�ͷ��
		if (rc1.PtInRect(point) )
		{
			m_pDlgTip->RelayEvent(pMsg->pt, "ԭ��", strManalMode );
		}

		if (rc2.PtInRect(point) )
		{
			m_pDlgTip->RelayEvent(pMsg->pt, "ԭ��", strAutoMode );
		}

		if (rc3.PtInRect(point) )
		{
			m_pDlgTip->RelayEvent(pMsg->pt, "ԭ��", strCloseAllQQ);
		}

		if (rc4.PtInRect(point) )
		{
			m_pDlgTip->RelayEvent(pMsg->pt, "ԭ��", strClearQQMemory );
		}

	}
	return CDialog::PreTranslateMessage(pMsg);
}

void Cqq_auto_loginDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	m_nQQ2013 = 1;
}

void Cqq_auto_loginDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	m_nQQ2013 = 0;
}
