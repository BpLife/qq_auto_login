#pragma once


// CExpToolTip

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ExpToolTip.h 
������ʾ��             
--*/

class CExpToolTip : public CWnd
{
	DECLARE_DYNAMIC(CExpToolTip)

public:
	CExpToolTip();
	virtual ~CExpToolTip();

public:
	BOOL Create(CWnd* pParentWnd);                                     // ����TIP����

	void RelayEvent(const CPoint ptMove, const CString strTitle, const CString strClient); // ��ʱ��ʾ
	void RelayEventForOrg(const CPoint ptMove, const CString strTitle, const CString strClient);

	// -------------------------------------------------------------------------------
	// ���ñ�����ɫ
	inline void SetTitleColor(const COLORREF clrTitle) { m_clrTitleFontColor=clrTitle;}
	// ���ÿͻ�����ɫ
	inline void SetClientColor(const COLORREF clrClient) { m_clrClient=clrClient;}  
	// ����������ɫ
	inline void SetFontColor(const COLORREF clrTipFontColor) { m_clrTipFontColor=clrTipFontColor;} 
	// ������ʾ�ı�
	inline void SetTipText(const CString strTitle,const CString strClient)
	{
		m_strTitle  = strTitle;
		m_strClient = strClient;
	}

	inline COLORREF GetTitleColor() const { return m_clrTitleFontColor;}        // ��ñ�����ɫ
	inline COLORREF GetClientColor() const { return m_clrClient;}      // ��ÿͻ�������ɫ 
	inline COLORREF GetFontColor() const { return m_clrTipFontColor;}  // ���������ɫ

	inline CSize    GetWindowSize() const { return m_SizeWindow; }     // ��ô���ߴ�

	// -------------------------------------------------------------------------------
public:

	// ---------------------------------------
	COLORREF   m_clrTitleFontColor;// ������ɫ
	COLORREF   m_clrClient;           // �ͻ�����ɫ
	COLORREF   m_clrTipFontColor;  // ������ɫ

	CString    m_strTitle;           // �����ı�
	CString    m_strClient;           // �ͻ��ı�

	CSize      m_SizeWindow;       // ����ߴ�
	CPoint     m_Point;            // �������
	CPoint     m_oldPoint;         // �ϵ�����
	UINT	   m_nOrgTimer;
	UINT       m_nIDTimer;         // ��ʱ��
	UINT       m_nIDTimer2;        // ���ڼ������ƶ��Ķ�ʱ��
	BOOL       m_bCapture;         // �Ƿ񲶻� 

	CFont*     m_pTitleFont;       // ��������
	CFont*     m_pContentFont;     // �ı����� 

protected:                             
	// ---------------------------------------
	int  CalcWindowsSize(CSize& rzWindow) ; // ���㴰��ߴ�
	// ---------------------------------------

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
};




