// CEFBrowser.cpp : Defines the entry point for the application.
//
/*
	2018/11/28 by XinFan
	duilib + cef3ʵ�ֵ�web�ؼ��Լ��򵥶��ǩ�����

	cefclientapp.h + cefclienthandler.h -->  cef���
	cefbrowserui.h  -->		cef3��duilib web�ؼ�
	browsermanage.h	-->		�����������ǩ	 
	ceficondown.h	-->		�����վ���ص�icon��ʾ 
	ceftempwnd		-->		����ҳǩ�ر�����
	cefutils.h		-->		����������ô����������  ++  ��duistring��ת
	eventmanage.h	-->		���ǩ�¼�����
*/

#include "stdafx.h"
#include "resource.h"

#include "include/cef_sandbox_win.h"
#include "CefClientApp.h"
#include "MainFrame.h"
#include "CefUtils.h"

bool OnlyOne(LPCWSTR szexeName){
	HANDLE m_hMutex = CreateMutex(NULL, FALSE, szexeName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
		// �����˳�
		return false;
	};
	return true;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("Res"));

	HRESULT Hr = ::CoInitialize(NULL);
	::OleInitialize(NULL);
	if (FAILED(Hr)) return 0;


	CefEnableHighDPISupport();

	void* sandbox_info = NULL;
	CefMainArgs main_args(hInstance);

	CefRefPtr<CefCommandLine> command_line = CreateCommandLine(main_args);

	CefRefPtr<CefApp> app;
	switch (GetProcessType(command_line)) {
	case PROCESS_TYPE_BROWSER:
		app = new CCefClientBrowserApp();
		break;
	case PROCESS_TYPE_RENDERER:
		app = new CCefClientRendererApp();
		break;
	}
	int exit_code = CefExecuteProcess(main_args, app, sandbox_info);
	if (exit_code >= 0) {
		return exit_code;
	}
	// Initialize CEF.
	CefSettings settings;
	settings.no_sandbox = true;
	settings.multi_threaded_message_loop = true;
	CefInitialize(main_args, settings, app.get(), sandbox_info);
	
	
	if (!OnlyOne(EXE_NAME)) return 0;

	CMainFrame* pMainFrame = new CMainFrame();
	if (NULL == pMainFrame) return -1;
	pMainFrame->Create(NULL, FRAME_TITLE, UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 0, 0);
	HICON hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	::SendMessage(pMainFrame->GetHWND(), STM_SETICON, IMAGE_ICON, (LPARAM)(UINT)hIcon);
	pMainFrame->CenterWindow();
	::ShowWindow(*pMainFrame, SW_SHOW);

	CPaintManagerUI::MessageLoop();


	::OleUninitialize();
	::CoUninitialize();
	// Shut down CEF.
	CefShutdown();

	return 0;
}


