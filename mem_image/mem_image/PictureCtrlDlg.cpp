#include "stdafx.h"
#include "PictureCtrlDlg.h"
#include "frame/YBCtrlMemDC.h"
#include "frame/YBCtrlImageDepository.h"
#include "frame/YBCtrlMisc.h"
#include "loader/YBImageLoader.h"

USING_NAMESPACE_YBCTRL

PictureCtrlDlg::PictureCtrlDlg( const tstring_type& tstrAppPath )
: m_tstrAppPath( tstrAppPath )
, m_dlg4PicCtrl( tstrAppPath ) {
	_loadImage();
	string str = "iVBORw0KGgoAAAANSUhEUgAAAAkAAAAJCAYAAADgkQYQAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAIGNIUk0AAHolAACAgwAA+f8AAIDpAAB1MAAA6mAAADqYAAAXb5JfxUYAAACySURBVHjadJAxDkVAAESfjYRC6LaQuI1SROEUCq1rKZwBF5CoaDSr2NAoRPgd+fn+tPNmMhkjz/PF932PP5qmaTU9zxNFUfyYx3HQti1lWRrmW/o8T6qqYts2AMQb1DQN8zxzXdcD7ftOXdf0fY9Siq7rkFKSJAkAJoBSimEYGMcR13WRUhLHMUKIpykIAtI0xbIstNaEYXgDdxOAbdtEUYTWGsdxvjYaWZYtwN+fgPUzAOYSQuQDUZ8OAAAAAElFTkSuQmCC";
	decodeImageData(str);
	TCHAR* szData = _T("http://image2.958shop.com/p/2011/04/02/100430818717860.bmp");
	TCHAR* path = _T("E:\\123.png");
	if ( !m_download.HttpDownload( szData, path ) ) {
		assert( false );
	}
	int a = 0;
}

PictureCtrlDlg::~PictureCtrlDlg( ) {

}

YBCTRL_WNDMSGMAP_BEGIN( PictureCtrlDlg, YBCtrlBase )
	YBCTRL_ONWNDMSG( WM_PAINT, _onWM_PAINT )
	YBCTRL_ONWNDMSG( WM_ERASEBKGND, _onWM_ERASEBKGND )
	YBCTRL_ONWNDMSG( WM_NCHITTEST, _onWM_NCHITTEST )
YBCTRL_WNDMSGMAP_END()

HWND PictureCtrlDlg::create( int uWidth, int uHeight ) {
	int x;
	int y;
	int nScreenWidth;
	int nScreenHeight;
	HWND hWnd;

	nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	DWORD dwStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP;
	x = (nScreenWidth - uWidth) / 2;
	y = (nScreenHeight - uHeight) / 2;
	//
	hWnd = _createHWND(dwExStyle, dwStyle, x, y, uWidth, uHeight, NULL, (HMENU)NULL, NULL, NULL);
	if (hWnd) {
		m_dlg4PicCtrl.create( 0, 30, uWidth, uHeight - 30, hWnd );
		m_dlg4PicCtrl.setBigBmpSpace();
	}
	return hWnd;
}

void PictureCtrlDlg::_onWM_PAINT(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg) {
	YBCTRLMSG_ENTER_CHECK(WM_PAINT);
	PAINTSTRUCT ps;
	HDC hPaintDC;
	RECT rcClient;

	::GetClientRect(m_hWnd, &rcClient);
	YBCtrlMemDC memDC(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
	_drawBk(memDC.getMemDC());

	hPaintDC = ::BeginPaint(m_hWnd, &ps);
	::BitBlt(hPaintDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY);
	::EndPaint(m_hWnd, &ps);
}
void PictureCtrlDlg::_onWM_ERASEBKGND(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg) {
	YBCTRLMSG_ENTER_CHECK(WM_ERASEBKGND);
	HDC hDC = (HDC)pYBCtrlWndMsg->m_wParam;
	RECT rcClient;

	pYBCtrlWndMsg->m_dwFlag |= (YBCTRLWNDMSG_FLAG_EATMSG | YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = 1;

	::GetClientRect(m_hWnd, &rcClient);
	YBCtrlMemDC memDC(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
	_drawBk(memDC.getMemDC());

	::BitBlt(hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memDC.getMemDC(), 0, 0, SRCCOPY);
}

void PictureCtrlDlg::_onWM_NCHITTEST(ybctrl::YBCtrlWndMsg* pYBCtrlWndMsg) {
	YBCTRLMSG_ENTER_CHECK(WM_NCHITTEST);
	pYBCtrlWndMsg->m_dwFlag = (YBCTRLWNDMSG_FLAG_EATMSG | YBCTRLWNDMSG_FLAG_RESULT_VALID);
	pYBCtrlWndMsg->m_lRes = HTCAPTION;
}

ybctrl::YBCtrlImageStatic* PictureCtrlDlg::_getImageStatic(unsigned int uImageId) {
	YBCtrlImageBase* pYBImageBase = NULL;

	if (!YBCTRLIMAGEDEPOSITORY_INSTANCE->getImage(uImageId, &pYBImageBase))
		return NULL;
	if (!pYBImageBase || (pYBImageBase->getType() != YBCTRL_IMAGE_TYPE_STATIC))
		return NULL;

	return (YBCtrlImageStatic*)pYBImageBase;
}

void PictureCtrlDlg::_drawBk(HDC hDC) {
	RECT rcClient;

	::GetClientRect(m_hWnd, &rcClient);
	YBCtrlImageStatic* pYBImage = NULL;
	pYBImage = _getImageStatic(m_uImageIdBk);

	if (!pYBImage){
		return;
	}

	BackGroundBitBlt(hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pYBImage->getMemDC(),
		0, 0, pYBImage->getWidth(), pYBImage->getHeight(), 0, 0, 0, 0);

}

void PictureCtrlDlg::_loadImage() {
	TCHAR* posTmp = NULL;
	tstring_type tstrImageDirPath;
	tstring_type tstrImageFilePath;
	TCHAR szModulePath[MAX_PATH];
	unsigned int idImage;

	//szModulePath = m_tstrAppPath;
	GetModuleFileName(NULL, szModulePath, MAX_PATH);
	posTmp = _tcsrchr(szModulePath, _T('\\'));
	if (!posTmp){
		assert(false);
		return;
	}
	posTmp += 1;
	*posTmp = 0;

	tstrImageDirPath = szModulePath;
	tstrImageDirPath += _T("Image\\");

	tstrImageFilePath = tstrImageDirPath + _T("YBFileTPDlgBk.png");
	if (!YBImageLoader::getInstance()->loadImage(tstrImageFilePath, idImage)) {
		assert(false);
	}
	m_uImageIdBk = idImage;

}

void PictureCtrlDlg::decodeImageData( string tstrData ) {
	string tstrTmp;
	int OutByte = 0;
	int nlength = 0;

	tstrTmp = m_base64Coder.Decode( tstrData.c_str(), tstrData.length(), OutByte );

	nlength = tstrTmp.length();
	BYTE* buffer = new BYTE[ nlength ];

	for ( int index = 0; index < nlength; ++index ) {
		buffer[ index ] = tstrTmp[ index ];
	}
	m_dlg4PicCtrl.loadImage( buffer, OutByte );
// 	unsigned char BMPHeader[] = { 0x42, 0x4d }; //BMP头文件
// 	unsigned char JPGHeader[] = { 0xff, 0xd8 };   //JPG,JPEG头文件
// 	unsigned char PNGHeader[] = { 0x89, 0x50 }; //GIF头文件
// 	unsigned char ch[ 2 ];
// 	::memcpy( ch, buffer, 2 );
// 	HANDLE hfile;
// 	char chFileOut[] = "d:\\aa.png";
// 	hfile = CreateFile( chFileOut, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL );
// 	if ( hfile == INVALID_HANDLE_VALUE ) {
// 		assert( false );
// 		return;
// 	}
// 	if ( memcmp( BMPHeader, ch, 2 ) == 0 ) {
// 		WriteFile( hfile, buffer, OutByte,  )
// 	} else if ( memcmp( JPGHeader, ch, 2 ) == 0 ) {
// 	
// 	} else if ( memcmp( PNGHeader, ch, 2 ) ==0 ) {
// 		
// 	}
}