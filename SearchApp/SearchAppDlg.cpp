
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
static int RetrieveRecording(Recording& recording, string key, BTree<KeyType>& RecordingIndex, RecordFile<RecType>& RecordingFile)
{
	int result;
	result = RecordingIndex.Search(key);
	if (result == -1) {
		cout << "Index를 찾을 수 없습니다." << endl;
		return FALSE;
	}
	result = RecordingFile.Read(recording, result);
	if (result == -1) {
		cout << "File을 읽을 수 없습니다." << endl;
		return FALSE;
	}
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
			R[0] = new Recording("LON", "1", "Romeo and Juliet", "Prokofiev", "Maazel");
			R[1] = new Recording("RCA", "2", "Quartet in C Sharp Minor", "Beethoven", "Julliard");
			R[2] = new Recording("WAR", "3", "Touchstone", "Corea", "Corea");
			R[3] = new Recording("ANG", "4", "Symphony No. 9", "Beethoven", "Giulini");
			R[4] = new Recording("COL", "5", "Nebraska", "Springsteen", "Springsteen");
			R[5] = new Recording("DG", "6", "Symphony No. 9", "Beethoven", "Karajan");
			R[6] = new Recording("MER", "7", "Coq d'or Suite", "Rimsky-Korsakov", "Leinsdorf");
			R[7] = new Recording("COL", "8", "Symphony No. 9", "Dvorak", "Bernstein");
			R[8] = new Recording("DG", "9", "Violin Concerto", "Beethoven", "Ferras");
			R[9] = new Recording("FF", "10", "Good News", "Sweet Honey in the Rock", "Sweet Honey in the Rock");
			for (int i = 0; i < NumberOfRecords; i++)
			{
				R[i]->Init(Buffer);
				R[i]->Pack(Buffer);
				recaddr = Recfile.Write(*R[i]);
				cout << "Recordin2 R[" << i << "] at recaddr " << recaddr << endl;
				result = bt.Insert(R[i]->IdNum, recaddr);
			}
			break;
		case 2:
			//cout << bt.Delete("COL") << endl;
			break;
		case 3:
			cout << "Search key = " << endl;
			cin >> searchKey;
			if (RetrieveRecording(record, searchKey, bt, Recfile) == TRUE)
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

BOOL CSearchAppDlg::PreTranslateMessage(MSG* pMsg)
{
	// enter
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)) {
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
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
