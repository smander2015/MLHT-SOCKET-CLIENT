#include"MLHT_CLIENT.h"


//#define SERVER_IP "192.168.56.102"
#define SERVER_IP "192.168.117.131"
//#define SERVER_IP "10.64.136.114"
//#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9000


#define SCREEN_WIDTH (GetSystemMetrics(SM_CXSCREEN)>=1920?1280:(GetSystemMetrics(SM_CXSCREEN)>=1600?1024:800)) //��Ļ���
#define SCREEN_HEIGHT (GetSystemMetrics(SM_CYSCREEN)>=1080?720:(GetSystemMetrics(SM_CYSCREEN)>=900?768:600))   //��Ļ�߶�
#define SCREEN_X (GetSystemMetrics(SM_CXSCREEN)-SCREEN_WIDTH)/2     //��Ļ������
#define SCREEN_Y (GetSystemMetrics(SM_CYSCREEN)-SCREEN_HEIGHT)/2    //��Ļ������


#define BUTTON_WIDTH    (SCREEN_WIDTH/8)		//���ڿ��ư�ť��
#define BUTTON_HEIGHT   (SCREEN_HEIGHT/12)      //���ڿ��ư�ť��
#define BUTTON_DISTANCE  BUTTON_HEIGHT			//���ڿ��ư�ť�ݼ��=���ڿ��ư�ť��+���ڿ��ư�ť���
#define BUTTON_X		(SCREEN_WIDTH/10)       //���ڿ��������ϰ�ť��y����
#define BUTTON_Y		(SCREEN_HEIGHT/10)      //���ڿ��������ϰ�ť��x����


#define BUTTON_NUM 11             //���ڿ��ư�ť��


#define BUTTON_CLIENT_CONNECT 0  //���������Ӱ�ť���
#define BUTTON_FILE_UPLOAD    1  //�ļ��ϴ���ť���
#define BUTTON_FILE_DOWNLOAD  2  //�ļ����ذ�ť���
#define BUTTON_SHELL_WINDOW   3  //Զ�����濪��/�رհ�ť���

#define BUTTON_PROCESS_HIDE   4  //Զ�����濪��/�رհ�ť���
#define BUTTON_FILE_HIDE	  5  //Զ�����濪��/�رհ�ť���
#define BUTTON_SHELL_INPUT	  6  //Զ�����濪��/�رհ�ť���

#define BUTTON_SHELL_LS		  7  //Զ�����濪��/�رհ�ť���
#define BUTTON_SHELL_MKDIR    8  //Զ�����濪��/�رհ�ť���
#define BUTTON_SHELL_RMDIR    9  //Զ�����濪��/�رհ�ť���
#define BUTTON_SHELL_RM		 10  //Զ�����濪��/�رհ�ť���


#define SHELL_WIDTH (SCREEN_WIDTH/4)						//SHELL���ڿ��ư�ť��
#define SHELL_HEIGHT (SCREEN_HEIGHT/4)  					//SHELL���ڿ��ư�ť��
#define SHELL_X BUTTON_X									//SHELL���ڿ��������ϰ�ť��y����
#define SHELL_Y BUTTON_Y+3*(BUTTON_DISTANCE+BUTTON_HEIGHT)  //SHELL���ڿ��������ϰ�ť��x����


extern HWND hwndbutton[BUTTON_NUM];   //���ڰ�ť


class GUI{
public:
	MLHT_CLIENT* mc;
	
	bool isconnected;						//�������Ƿ�������
	bool processhide;
	bool filehide;

public:
	GUI(){
		isconnected = processhide = filehide = false;
	}

	void Init(HWND);						//�����ڳ�ʼ��
	void ScreenPaint(HWND);					//��Ļ����
	void CommandExecute(HWND,int);			//ָ��ִ��

	char* GetFilename(char* FullPathName);	//��ȡȫ·���е��ļ���

};