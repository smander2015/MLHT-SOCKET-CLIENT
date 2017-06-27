#include "GUI.h"


HWND hwndbutton[BUTTON_NUM];//���ڰ�ť


GUI *gui=new GUI();


LRESULT CALLBACK MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
	switch (msg){
	case WM_CREATE:
		gui->Init(hwnd);
		break;
	case WM_PAINT:
		gui->ScreenPaint(hwnd);
		break;
	case WM_COMMAND:
		switch(HIWORD(wParam)){
		case BN_CLICKED:
			for(int i=0;i<BUTTON_NUM;i++)
				if((HWND)lParam == hwndbutton[i]){
					gui->CommandExecute(hwnd,i);
					break;
				}
			break;
		}
		break;
	case WM_DESTROY:
		ExitProcess(0);
	default:
		break;
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow){
	//ע���������� 
	WNDCLASS wc;
	if(!hPrevInstance){
		wc.style = CS_HREDRAW | CS_VREDRAW;                     
		wc.lpfnWndProc = MainWndProc;    
		wc.cbClsExtra = 0;                 
		wc.cbWndExtra = 0;               
		wc.hInstance = hinstance;      
		wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);    
		wc.hCursor = LoadCursor(NULL,IDC_ARROW);                        //���Ϊ��ͷ
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);         //��ɫ����     
		wc.lpszMenuName =  "WndMenu";    
		wc.lpszClassName = "WndClass";                                  //����������
		RegisterClass(&wc); 
	}

   /********************��ʼ�������ھ��hMainWnd********************/
	HWND hMainWnd;
	hMainWnd=CreateWindow( 
		"WndClass",                                                      //��������
		"��ħ֮ս4.5 HACK CLIENT",                                       //���ڱ�����
		WS_OVERLAPPEDWINDOW,                                             //���ڷ��
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,             
		NULL,               
		hinstance,          
		NULL);
	ShowWindow(hMainWnd, SW_SHOWNORMAL); 
	UpdateWindow(hMainWnd); 

   /********************��Ϣѭ���ṹ********************/    
	MSG msg;
	while(GetMessage(&msg,NULL,0,0))//��ȡ��Ϣ�����msg�ṹ��
    {
   		TranslateMessage(&msg);//���������Ϣ
		DispatchMessage(&msg);//�򴰿ں���������Ϣ���ô��ں�������
	}
	return msg.wParam;
}