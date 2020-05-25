
// SearchAppDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SearchApp.h"
#include "SearchAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

#define TEST
#ifdef TEST
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
#define NumberOfRecords 10
using namespace std;
const int BTreeSize = 5;//btree order m = 5
template<class KeyType, class RecType>
int RetrieveRecording(Recording& recording, string key, BTree<KeyType>& RecordingIndex, RecordFile<RecType>& RecordingFile)
{
	int result;
	result = RecordingFile.Read(recording, RecordingIndex.Search(key));
	if (result == -1) return FALSE;
	result = recording.Unpack(RecordingFile.GetBuffer());
	return result;
}

int run()
{
	int result, i;
	int choice;
	string searchKey;
	Recording record;
	FixedFieldBuffer Buffer(300, 100);
	RecordFile<Recording> Recfile(Buffer);
	Recfile.Create("recording.dat", ios::out | ios::trunc);
	BTree <string> bt(BTreeSize);
	char fileName[] = "btindex.dat";
	result = bt.Create(fileName, ios::in | ios::out | ios::trunc);
	if (!result) { cout << "Creating of BTree index fails" << endl; return 0; }
	do
	{
		cout << "1.Insert all records" << endl;//동작해야 함
		cout << "2.Delete a record" << endl;//동작해야 함
		cout << "3.Search with a key" << endl;//동작해야 함
		cout << "4.Display all record" << endl;//in-order traversal //동작해야 함
		cout << "5.Exit" << endl;
		cout << "Enter your choice : ";
		cin >> choice;
		switch (choice)
		{
		case 1:
			int recaddr;
			Recording* R[NumberOfRecords];
			R[0] = new Recording("LON", "2312", "Romeo and Juliet", "Prokofiev", "Maazel");
			R[1] = new Recording("RCA", "2626", "Quartet in C Sharp Minor", "Beethoven", "Julliard");
			R[2] = new Recording("WAR", "23699", "Touchstone", "Corea", "Corea");
			R[3] = new Recording("ANG", "3795", "Symphony No. 9", "Beethoven", "Giulini");
			R[4] = new Recording("COL", "38358", "Nebraska", "Springsteen", "Springsteen");
			R[5] = new Recording("DG", "18807", "Symphony No. 9", "Beethoven", "Karajan");
			R[6] = new Recording("MER", "75016", "Coq d'or Suite", "Rimsky-Korsakov", "Leinsdorf");
			R[7] = new Recording("COL", "31809", "Symphony No. 9", "Dvorak", "Bernstein");
			R[8] = new Recording("DG", "139201", "Violin Concerto", "Beethoven", "Ferras");
			R[9] = new Recording("FF", "245", "Good News", "Sweet Honey in the Rock", "Sweet Honey in the Rock");
			for (int i = 0; i < NumberOfRecords; i++)
			{
				R[i]->Init(Buffer);
				R[i]->Pack(Buffer);
				recaddr = Recfile.Write(*R[i]);
				cout << "Recordin2 R[" << i << "] at recaddr " << recaddr << endl;
				string value = R[i]->IdNum;
				result = bt.Insert(value, recaddr);
			}
			break;
		case 2:
			//cout << bt.Delete("COL") << endl;
			break;
		case 3:
			cout << "Search key = " << endl;
			cin >> searchKey;
			RetrieveRecording(record, searchKey, bt, Recfile);
			cout << record << endl;//operator <<을 구현해야 한다
			break;
		case 4:
			cout << bt;//operator <<을 friend function으로 구현한다
			//bt의 모든 key value을 출력해야 한다.
			break;
		default:
			cout << "Wrong choice\n";
		}
	} while (choice < 5);
	system("pause");
	return 1;
}
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSearchAppDlg 대화 상자



CSearchAppDlg::CSearchAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SEARCHAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSearchAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INSERT_EDIT, m_insertEdit);
	DDX_Control(pDX, IDC_LIST3, m_listBox);
	DDX_Control(pDX, IDC_PICTURE_BOX, m_pictureBox);
	DDX_Control(pDX, IDC_OPERATION_COMBO, m_operCombo);
	DDX_Control(pDX, IDC_OUTPUT_COMBO, m_outCombo);
}

BEGIN_MESSAGE_MAP(CSearchAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INSERT_BUTTON, &CSearchAppDlg::OnBnClickedInsertButton)
	ON_BN_CLICKED(IDC_INSERT_BUTTON2, &CSearchAppDlg::OnBnClickedInsertButton2)
	ON_CBN_SELCHANGE(IDC_OUTPUT_COMBO, &CSearchAppDlg::OnCbnSelchangeOutputCombo)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSearchAppDlg 메시지 처리기

BOOL CSearchAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
#ifdef TEST
	run();
#endif
	m_operCombo.AddString(_T("입력")); // INSERT_DATA 0
	m_operCombo.AddString(_T("삭제")); // DELETE_DATA 1
	m_operCombo.SetCurSel(0);

	m_outCombo.AddString(_T("중위 탐색")); // IN_ORDER 0 
	m_outCombo.AddString(_T("전위 탐색")); // PRE_ORDER 1
	m_outCombo.SetCurSel(0);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSearchAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSearchAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSearchAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSearchAppDlg::DrawScreen()
{
#if 0
	keys.clear();
	tree.getTreePoints(keys, m_order);

	CRect rect;
	m_pictureBox.GetClientRect(&rect);
	CDC* pDC = m_pictureBox.GetWindowDC();
	pDC->SetWindowExt(100, 100);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->FillSolidRect(&rect, RGB(255, 255, 255));
	for (const auto& _key : keys) {
		CString temp;
		int key = _key.first;
		int x = _key.second.second;
		int y = _key.second.first;
		const int xlen = 40;
		const int ylen = 30;
		const int xsize = xlen;
		const int ysize = ylen - 10;

		x = xlen * x;
		y = ylen * y;

		CRect bound(x - 1, y - 1, x + xsize + 1, y + ysize + 1);
		CRect textLocation(x, y, x + xlen, y + ylen);
		AvlNode<int>* _node = tree.search(key);
		int _leftKey = -1, _rightKey = -1;

		temp.Format(_T("%d"), key);

		if (_node == nullptr) {
			continue;
		}

		if (_node->getLeftChild())
			_leftKey = _node->getLeftChild()->getKey();
		if (_node->getRightChild())
			_rightKey = _node->getRightChild()->getKey();

		pDC->Rectangle(&bound);
		pDC->DrawText(temp, &textLocation, DT_CENTER);

		if (_leftKey != -1) {
			int _key = _leftKey;
			int fromX = x;
			int fromY = y + ysize + 1;
			int toX = xlen * keys.find(_key)->second.second + (xsize + 1) - 1;
			int toY = ylen * keys.find(_key)->second.first - 1;
			const int len = 3;
			pDC->MoveTo(fromX, fromY);
			pDC->Ellipse(fromX - len, fromY - len, fromX + len, fromY + len);
			pDC->LineTo(toX, toY);
			pDC->Ellipse(toX - len, toY - len, toX + len, toY + len);
			// cout << fromX << ", " << fromY << " ==> " << toX << ", " << toY << endl;
		}
		if (_rightKey != -1) {
			int _key = _rightKey;
			int fromX = x + xsize + 1;
			int fromY = y + ysize + 1;
			int toX = xlen * keys.find(_key)->second.second - 1;
			int toY = ylen * keys.find(_key)->second.first - 1;
			const int len = 3;
			pDC->MoveTo(fromX, fromY);
			pDC->Ellipse(fromX - len, fromY - len, fromX + len, fromY + len);
			pDC->LineTo(toX, toY);
			pDC->Ellipse(toX - len, toY - len, toX + len, toY + len);
			// cout << fromX << ", " << fromY << " ==> " << toX << ", " << toY << endl;
		}
		UpdateWindow();
	}
	m_pictureBox.ReleaseDC(pDC);
	UpdateWindow();
#endif
}



void CSearchAppDlg::OnBnClickedInsertButton()
{
#if 0
	CString strData;
	int key;

	m_insertEdit.GetWindowTextW(strData);
	if (strData.IsEmpty()) {
		return;
	}

	key = _ttoi(strData);

	if (m_operCombo.GetCurSel() == CSearchAppDlg::INSERT_DATA) {
		tree.Insert(key);
	}
	else if (m_operCombo.GetCurSel() == CSearchAppDlg::DELETE_DATA) {
		tree.Delete(key);
	}

	m_listBox.ResetContent();
	tree.inOrder();
	strData.Format(_T("inOrder ==> %s"), CString(tree.resultStream.str().c_str()));
	m_listBox.AddString(strData);
	tree.preOrder();
	strData.Format(_T("preOrder ==> %s"), CString(tree.resultStream.str().c_str()));
	m_listBox.AddString(strData);

	DrawScreen();

	m_insertEdit.SetWindowTextW(_T(""));
#endif
}

// TODO: display도 구현할 것!


void CSearchAppDlg::OnBnClickedInsertButton2()
{
#if 0
	vector<CString> v;
	int i = 0;
	tree.display(v);
	m_listBox.ResetContent();
	for (const auto& value : v) {
		m_listBox.InsertString(i, value);
		i++;
	}
#endif
}


BOOL CSearchAppDlg::PreTranslateMessage(MSG* pMsg)
{
	// enter
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)) {
		OnBnClickedInsertButton();
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSearchAppDlg::OnCbnSelchangeOutputCombo()
{
#if 0
	int outputMethod = m_outCombo.GetCurSel();
	switch (outputMethod) {
	case CSearchAppDlg::IN_ORDER:
		m_order = Order::IN_ORDER;
		break;
	case CSearchAppDlg::PRE_ORDER:
		m_order = Order::PRE_ORDER;
		break;
	default:
		m_order = Order::IN_ORDER;
	}
	DrawScreen();
#endif
}


void CSearchAppDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CWnd* pCtrl = GetDlgItem(IDC_PICTURE_BOX);
	if (!pCtrl) {
		return;
	}
	DrawScreen();
}
