
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

CSearchAppDlg::~CSearchAppDlg()
{
	delete m_buffer;
	delete m_recFile;
}

void CSearchAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ID_EDIT, m_idEdit);
	DDX_Control(pDX, IDC_LABEL_EDIT, m_labelEdit);
	DDX_Control(pDX, IDC_TITLE_EDIT, m_titleEdit);
	DDX_Control(pDX, IDC_COMPOSER_EDIT, m_composerEdit);
	DDX_Control(pDX, IDC_ARTIST_EDIT, m_artistEdit);
	DDX_Control(pDX, IDC_FIND_ID_EDIT, m_findIdEdit);
	DDX_Control(pDX, IDC_ITEM_LIST, m_itemList);
}

BEGIN_MESSAGE_MAP(CSearchAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_INSERT_BUTTON, &CSearchAppDlg::OnBnClickedInsertButton)
	ON_BN_CLICKED(IDC_FIND_BUTTON, &CSearchAppDlg::OnBnClickedFindButton)
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

	m_buffer = new FixedFieldBuffer(300, 100);
	m_recFile = new RecordFile<Recording>(*m_buffer);
	m_recFile->Create("recording.dat", ios::out | ios::trunc);
	m_btree = new BTree<string>(BTreeSize);
	BOOL result = m_btree->Create("btindex.dat", ios::in | ios::out | ios::trunc);

	return result;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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
		OnBnClickedInsertButton();
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
}


void CSearchAppDlg::OnBnClickedInsertButton()
{
	CString id, label, title, composer, artist;
	m_idEdit.GetWindowTextW(id);
	m_labelEdit.GetWindowTextW(label);
	m_titleEdit.GetWindowTextW(title);
	m_composerEdit.GetWindowTextW(composer);
	m_artistEdit.GetWindowTextW(artist);

	const bool hasEmpty = id.IsEmpty() || label.IsEmpty() || title.IsEmpty() || composer.IsEmpty() || artist.IsEmpty();

	if (hasEmpty) {
		AfxMessageBox(_T("모든 항목을 다 채워주시길 바랍니다."));
		return;
	}

	Recording* R = new Recording((CStringA)label, (CStringA)id, (CStringA)title, (CStringA)composer, (CStringA)artist);
	if (R == nullptr) {
		AfxMessageBox(_T("메모리가 부족합니다."));
		return;
	}

	int recAddr = -1, ret = -1;

	R->Init(*m_buffer);
	R->Pack(*m_buffer);
	recAddr = m_recFile->Write(*R);
	if (recAddr == -1) {
		AfxMessageBox(_T("File 쓰기에 실패했습니다."));
		return;
	}

	ret = m_btree->Insert(R->IdNum, recAddr);
	if (ret != 1) {
		AfxMessageBox(_T("BTree에 Insert하는 데 실패했습니다."));
		return;
	}

	stringstream textStream;
	string token;

	textStream << *m_btree;

	m_itemList.ResetContent();
	while (std::getline(textStream, token)) {
		CString str(token.c_str());
		m_itemList.AddString(str);
	}
	m_idEdit.SetWindowTextW(_T(""));
	m_labelEdit.SetWindowTextW(_T(""));
	m_titleEdit.SetWindowTextW(_T(""));
	m_composerEdit.SetWindowTextW(_T(""));
	m_artistEdit.SetWindowTextW(_T(""));
}


void CSearchAppDlg::OnBnClickedFindButton()
{
	CString cstr;
	m_findIdEdit.GetWindowTextW(cstr);
	string str((CStringA)cstr);
	if (RetrieveRecording(m_record, str, *m_btree, *m_recFile) == TRUE) {
		stringstream result;
		result << m_record;
		CString output(result.str().c_str()), label, title, composer, artist;
		AfxExtractSubString(label, output, 0, '|');
		AfxExtractSubString(title, output, 2, '|');
		AfxExtractSubString(composer, output, 3, '|');
		AfxExtractSubString(artist, output, 4, '|');
		output.Format(_T("라벨: %s\n제목: %s\n작곡가: %s\n아티스트: %s"), label, title, composer, artist);
		MessageBox(output, _T("조회 결과"));
	}
	m_findIdEdit.SetWindowTextW(_T(""));
}

template<class KeyType, class RecType>
int CSearchAppDlg::RetrieveRecording(Recording& recording, string key, BTree<KeyType>& RecordingIndex, RecordFile<RecType>& RecordingFile)
{
	int result;
	result = RecordingIndex.Search(key);
	if (result == -1) {
		AfxMessageBox(_T("Index를 찾을 수 없습니다."));
		return FALSE;
	}
	result = RecordingFile.Read(recording, result);
	if (result == -1) {
		AfxMessageBox(_T("File을 읽는 데 실패했습니다."));
		return FALSE;
	}
	result = recording.Unpack(RecordingFile.GetBuffer());
	return result;
}
