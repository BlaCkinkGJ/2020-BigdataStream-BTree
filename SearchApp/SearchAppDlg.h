
// SearchAppDlg.h: 헤더 파일
//

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <queue>

using namespace std;

// CSearchAppDlg 대화 상자
class CSearchAppDlg : public CDialogEx
{
	// 생성입니다.
public:
	CSearchAppDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEARCHAPP_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	map<int, pair<int, int> > keys;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void DrawScreen();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_insertEdit;
	afx_msg void OnBnClickedInsertButton();
	CListBox m_listBox;
	afx_msg void OnBnClickedInsertButton2();
	CStatic m_pictureBox;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CComboBox m_operCombo;
	CComboBox m_outCombo;
	afx_msg void OnCbnSelchangeOutputCombo();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
