/***********************************************************************************

��Ԫ���ƣ�  ������ʾ   
ģ�鹦�ܣ�  ��ʱ������ʾ��ʾ

�� �� �ƣ�  CExpToolTip 
�����ļ���  ExpToolTip.h,ExpToolTip.cpp

����ʱ�䣺  2002-10-15
����ʱ�䣺  2002-10-22
������Ա��  ����
----------------------------------------------------------------------------------
[�ӿ�]:
BOOL Create(CWnd* pParentWnd)
[����]: ������ʾ����,�������

[����]: 
CWnd* pParentWnd ---- Ϊ�����壬����ָ��

[����]: 
TRUE             ---- �����ɹ�
FALSE            ---- ����ʧ��

--------------------------------------------------------------------

void RelayEvent(CPoint ptMove, CString strTitle, CString strClient)
[����]: ��ʱ��ʾ�ı�

[����]: 
CPoint ptMove     ----- ��ʾ��ʾ���������
CString strTitle  ----- �����ı� 
CString strClient ----- �ͻ����ı�,��\n���У����һ�в���

[����]: ��

----------------------------------------------------------------------------------
[˵��]:

�ȱ��빹��һ�����ʵ����ʵ�������Ϊ��Ա�������ڳ�ʼ����ʱ�����
Create(this)����������ʾ,�˺�����֤��ǰʵ��δ������ʱ��ֻ����
һ�Σ���ε��ý������ԡ�����ں��ʵĵط�����RelayEvent��ʾ��ʾ

***********************************************************************************/

// ExpToolTip.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "qq_auto_login.h"
#include "ExpToolTip.h"

// CExpToolTip

IMPLEMENT_DYNAMIC(CExpToolTip, CWnd)

CExpToolTip::CExpToolTip() :
m_clrTitleFontColor(RGB(49,106,197)),
m_clrClient(RGB(255,255,228)),
m_clrTipFontColor(RGB(0,0,0)),
m_strTitle(_T("")),
m_strClient(_T("")),
m_nIDTimer(0),
m_SizeWindow(0,0),
m_Point(0,0),
m_oldPoint(0,0),
m_bCapture(TRUE),
m_pTitleFont(NULL),
m_pContentFont(NULL),
m_nIDTimer2(2),
m_nOrgTimer(100)
{

}

CExpToolTip::~CExpToolTip()
{
	// ��������    
	if (::IsWindow(m_hWnd))
		DestroyWindow(); 
	if(m_pTitleFont)
	{
		m_pTitleFont->DeleteObject();
		delete m_pTitleFont;
	}
	if (m_pContentFont)  
	{
		m_pContentFont->DeleteObject();
		delete m_pContentFont;
	}
}


BEGIN_MESSAGE_MAP(CExpToolTip, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CExpToolTip ��Ϣ�������

////////////////////////////////////////////////////////////////////////////////////
BOOL CExpToolTip::Create(CWnd *pParentWnd)
{
	ASSERT_VALID(pParentWnd);

	if(!pParentWnd || ::IsWindow(m_hWnd))
	{
		return FALSE; // ���ʵ���Ѿ����ڣ���ô������
	}

	//  ע�ᴰ����
	LPCTSTR lpszTipClass = AfxRegisterWndClass(CS_CLASSDC | CS_SAVEBITS, // ȱʡ��ʽ 
		::LoadCursor(NULL, IDC_ARROW),
		::CreateSolidBrush(m_clrClient),// ����ɫ
		NULL);
	
	//  ����TIP����
	BOOL bRet=CWnd::CreateEx(WS_EX_TOOLWINDOW ,  // ���ߴ�����ʽ
		lpszTipClass,       // ������
		NULL,
		WS_BORDER | WS_POPUP ,// �߿�͵�����ʽ 
		CW_USEDEFAULT,         // �û�ȱʡֵ 
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		pParentWnd->GetSafeHwnd(), 
		NULL, 
		NULL);
	if(bRet)
	{
		SetOwner(pParentWnd);     // ���ø�����     
	}

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////////
int CExpToolTip::CalcWindowsSize(CSize &rzWindow) 
{
	TEXTMETRIC tm;
	int        nPos       = 0;
	int        nMax       = 0;
	int        nLineCount = 0;
	CString    strTemp;

	// ����Ϊ�գ����峤��Ϊ��
	if(m_strTitle.IsEmpty())
	{
		rzWindow.cx = 0;
		rzWindow.cy = 0;
		return 0;
	}

	CDC* pDC=GetDC();

	// ��֤����ĳ���
	CFont* pOldFont = pDC->SelectObject(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
	pDC->GetTextMetrics(&tm);  

	//
	//  ��һ����ַ���
	//
	for(int i = 0;i < m_strClient.GetLength();i++)
	{
		if(m_strClient.GetAt(i)=='\n')
		{
			strTemp = m_strClient.Mid(nPos,i-nPos);
			nPos    = i + 1;

			nMax    = nMax < strTemp.GetLength() ? strTemp.GetLength() : nMax;
			nLineCount++;
		}
		else if(i == (m_strClient.GetLength()-1)) // ��������һ��
		{
			strTemp = m_strClient.Right(i-nPos+1);

			nMax    = nMax < strTemp.GetLength() ? strTemp.GetLength() : nMax;
			nLineCount++;
		}
	}

	nMax = nMax < m_strTitle.GetLength() ?  m_strTitle.GetLength() : nMax; // ����ĳ���
	nLineCount++;

	//
	// ����ߴ�
	//
	rzWindow.cx = nMax * tm.tmMaxCharWidth / 2 + 5;// 5 Ϊ���Ԥ���ľ��� 
	rzWindow.cy = nLineCount * (tm.tmHeight+tm.tmExternalLeading + 2); // 2Ϊ�о�

	pDC->SelectObject(pOldFont);

	ReleaseDC(pDC);
	return (tm.tmHeight + tm.tmExternalLeading + 2); // �ַ��߶�+�о�
}

///////////////////////////////////////////////////////////////////////////////////
void CExpToolTip::OnPaint()
{
	CPaintDC dc(this); 

	int      nPos       = 0;
	int      nLineCount = 0;
	CRgn     rgn;
	CRect    rtTitle;
	CRect    rtDlg;    
	CSize    rzWindow;
	//    CBrush   bhFrame(::GetSysColor(COLOR_3DDKSHADOW));
	CString  strTemp;

	CFont*   pOldFont=NULL;

	if (NULL == m_pContentFont)
	{
		m_pContentFont = new CFont;
		if (!m_pContentFont->CreateStockObject(DEFAULT_GUI_FONT)) // ȡϵͳ����
			return ;
	}

	if (NULL == m_pTitleFont)
	{
		m_pTitleFont = new CFont;
		LOGFONT        LogFont;
		m_pContentFont->GetLogFont(&LogFont);
		//LogFont.lfWeight = FW_BOLD;
		if (!m_pTitleFont->CreateFontIndirect(&LogFont)) // ������������
			return ;
	}
	dc.SetBkMode(TRANSPARENT); 

	// ���㴰��ߴ磬�������ַ��߶�
	int  nTitleHeight = CalcWindowsSize(m_SizeWindow);
	if(nTitleHeight == 0)
		return ;    

	//  ������
	rtTitle.SetRect(0, 0, m_SizeWindow.cx, nTitleHeight);
	pOldFont = dc.SelectObject(m_pTitleFont);
	//dc.SetTextColor(m_clrTitleFontColor);
	dc.SetTextColor(RGB(255,255,255));

	CBrush brush(m_clrTitleFontColor);
	dc.FillRect(&rtTitle,&brush);
	rtTitle.top+=2;

	dc.DrawText(m_strTitle, 
		&rtTitle, 
		DT_CENTER |
		DT_NOPREFIX  |
		DT_EXPANDTABS |  
		DT_SINGLELINE);

	//  �ͻ�������ı�
	dc.SelectObject(m_pContentFont);
	dc.SetTextColor(m_clrTipFontColor);   // �ͻ������ı��Ǻ�ɫ
	for(int i = 0; i < m_strClient.GetLength();i++)
	{
		if(m_strClient.GetAt(i) =='\n')
		{
			strTemp = m_strClient.Mid(nPos, i-nPos);
			nPos    = i + 1;
			nLineCount++;

			if(nLineCount == 1) // ��һ���ı�,����2���о಻���м�����غ�
			{
				dc.TextOut(1, (nTitleHeight * nLineCount + 2), strTemp);
			}
			else
			{
				dc.TextOut(1, (nTitleHeight * nLineCount + 2), strTemp);
			}
		}      
		else if(i == (m_strClient.GetLength() - 1)) // ���һ���ı�
		{
			strTemp = m_strClient.Right(i - nPos + 1);
			nLineCount++;

			if(nLineCount == 1) // ֻ��һ���ı�
			{
				dc.TextOut(1, (nTitleHeight * nLineCount + 2), strTemp);
			}
			else
			{
				dc.TextOut(1, (nTitleHeight * nLineCount + 2), strTemp);
			}
		}
	}    
	brush.DeleteObject();
	rgn.DeleteObject();

	dc.SelectObject(pOldFont)->DeleteObject();
	// Բ�Ǵ���    
	//GetWindowRect(&rtDlg);
	//rgn.CreateRoundRectRgn(0,0,rtDlg.Width(),rtDlg.Height(),10,10);
	//dc.FrameRgn(&rgn,&bhFrame,1,1);
	//SetWindowRgn((HRGN)rgn,TRUE);    
}

/////////////////////////////////////////////////////////////////////////////////////
void CExpToolTip::RelayEvent(const CPoint ptMove, const CString strTitle, const CString strClient)
{ 
	SetTipText(strTitle, strClient);  // ���ñ���
	CalcWindowsSize(m_SizeWindow); // �õ�����ߴ�

	// �������λ����ͬ�������
	if(ptMove == m_oldPoint)
		return;
	m_oldPoint = ptMove;  // ��¼�ϵ��������

	m_Point    = ptMove;   //  �����ƫ��
	m_Point.x += 15;
	m_Point.y += 10;

	if(IsWindowVisible())
	{        
		ShowWindow(SW_HIDE);         // ���������ʾ�ģ���ô����
	}

	if(m_nIDTimer != 0)                //   ����ϴδ���ʱ�䣬��ô���������¿�ʼ 
	{
		KillTimer(m_nIDTimer);
	}    
	m_nIDTimer = SetTimer(1, 500, NULL); // ʱ����Ϊ����
}

void CExpToolTip::RelayEventForOrg(const CPoint ptMove, const CString strTitle, const CString strClient){
	SetTipText(strTitle, strClient);  // ���ñ���
	CalcWindowsSize(m_SizeWindow); // �õ�����ߴ�

	// �������λ����ͬ�������
	if(ptMove == m_oldPoint)
		return;
	m_oldPoint = ptMove;  // ��¼�ϵ��������

	m_Point    = ptMove;   //  �����ƫ��
	m_Point.x -= 300;
	m_Point.y += 10;

	if(IsWindowVisible())
	{        
		ShowWindow(SW_HIDE);         // ���������ʾ�ģ���ô����
	}

	if(m_nIDTimer != 0)                //   ����ϴδ���ʱ�䣬��ô���������¿�ʼ 
	{
		KillTimer(m_nIDTimer);
	}    
	m_nIDTimer = SetTimer(1, 500, NULL); // ʱ����Ϊ����
}


/////////////////////////////////////////////////////////////////////////////////////
void CExpToolTip::OnTimer(UINT nIDEvent)
{    
	CPoint point;
	::GetCursorPos(&point);
	if(nIDEvent == m_nIDTimer && point==m_oldPoint)
	{  
		KillTimer(m_nIDTimer);        // ����ʱ��
		m_nIDTimer2 = SetTimer(2, 5, NULL); //���ڼ������ƶ��Ķ�ʱ��
		if(!m_strTitle.IsEmpty())
		{          
			SetWindowPos(&this->wndTop,  // �ҵ��ؼ�������ʾ����
				m_Point.x,
				m_Point.y,
				m_SizeWindow.cx,
				m_SizeWindow.cy,
				SWP_SHOWWINDOW | 
				SWP_NOACTIVATE);
		}
		return;  
	}
	if(nIDEvent == m_nIDTimer2 && point!=m_oldPoint)
	{  
		KillTimer(m_nIDTimer2);        // ����ʱ��
		if(IsWindowVisible())
		{        
			ShowWindow(SW_HIDE);      // ���������ʾ�ģ���ô����
		}
		return;  
	}
	if(nIDEvent == m_nOrgTimer && point!=m_oldPoint){
		KillTimer(m_nOrgTimer);        // ����ʱ��
		m_nIDTimer2 = SetTimer(2, 5, NULL); //���ڼ������ƶ��Ķ�ʱ��
		if(!m_strTitle.IsEmpty())
		{          
			SetWindowPos(&this->wndTop,  // �ҵ��ؼ�������ʾ����
				m_Point.x - 100,
				m_Point.y,
				m_SizeWindow.cx,
				m_SizeWindow.cy,
				SWP_SHOWWINDOW | 
				SWP_NOACTIVATE);
		}
		return;  
	}
	CWnd::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////////////////

