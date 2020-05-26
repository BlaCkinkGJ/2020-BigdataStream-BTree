
// SearchAppDlg.h: 헤더 파일
//

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <queue>

#include "BTNode.h"
#include "recfile.h"
#include "recordng.h"
#include "BTree.h"

// 외부 기호 참조 오류 정정용
#include "BTNode.cpp"
#include "BTree.cpp"
#include "SimpInd.cpp"

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>

using namespace std;

const int BTreeSize = 5;//btree order m = 5

// CSearchAppDlg 대화 상자
class CSearchAppDlg : public CDialogEx
{
	// 생성입니다.
public:
	CSearchAppDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CSearchAppDlg();

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
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CEdit m_idEdit;
	CEdit m_labelEdit;
	CEdit m_titleEdit;
	CEdit m_composerEdit;
	CEdit m_artistEdit;
	CEdit m_findIdEdit;
	CListBox m_itemList;
	afx_msg void OnBnClickedInsertButton();

private:
	Recording m_record;
	FixedFieldBuffer* m_buffer;
	RecordFile<Recording>* m_recFile;
	BTree<string>* m_btree;
public:
	afx_msg void OnBnClickedFindButton();
	template<class KeyType, class RecType>
	int RetrieveRecording(Recording& recording, string key, BTree<KeyType>& RecordingIndex, RecordFile<RecType>& RecordingFile);
};
