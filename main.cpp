#include "protocol.h"

#pragma comment(lib, "ws2_32.lib") 
using namespace std;

int main(void)
{
	// ����socket��̬���ӿ�(dll)
	WORD wVersionRequested;
	WSADATA wsaData;	// ��ṹ�����ڽ���Wjndows Socket�Ľṹ��Ϣ��
	hostent * pHostent = NULL;
	int err;
	 
	wVersionRequested = MAKEWORD( 2, 2 );	// ����1.1�汾��WinSock��
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return -1;			// ����ֵΪ���ʱ���Ǳ�ʾ�ɹ�����WSAStartup
	}
	 
	if ( LOBYTE( wsaData.wVersion ) != 2 ||	HIBYTE( wsaData.wVersion ) != 2 ) {
		// ���������ֽ��ǲ���1�����ֽ��ǲ���1��ȷ���Ƿ������������1.1�汾
		// ����Ļ�������WSACleanup()�����Ϣ����������
		WSACleanup( );
		return -1; 
	}
	
	// ����socket������������ʽ�׽��֣������׽��ֺ�sockClient
	// SOCKET socket(int af, int type, int protocol);
	// ��һ��������ָ����ַ��(TCP/IPֻ����AF_INET��Ҳ��д��PF_INET)
	// �ڶ�����ѡ���׽��ֵ�����(��ʽ�׽���)�����������ض���ַ�������Э�飨0Ϊ�Զ���

	// ���׽���sockClient��Զ����������
	// int connect( SOCKET s,  const struct sockaddr* name,  int namelen);
	// ��һ����������Ҫ�������Ӳ������׽���
	// �ڶ����������趨����Ҫ���ӵĵ�ַ��Ϣ
	// ��������������ַ�ĳ���

	//pHostent = gethostbyname("5033598.all123.net");
	//puts(pHostent->h_addr_list[0]);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.s_addr = inet_addr("127.0.0.1");
	//addrSrv.sin_addr.s_addr = *((unsigned long *)pHostent->h_addr);	// ���ػ�·��ַ��127.0.0.1; 
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6001);

	//custom code

	protocol::Packet p("NULL", "YYYY/MM/DD HH:MM:SS:MMM");
	p.ContentLength = 10000;
	string header = p.createHeader();
	int headerLength = header.size();
	char * temp = new char[20000];
	sprintf(temp, header.c_str(), headerLength);
	char * ptr = temp + headerLength;

	for (int i = 0; i < 2000; ++i) {
		sprintf(ptr, "%04d#", i);
		ptr += 5;
	}

	int packetLength = headerLength + 10000;
	temp[packetLength] = '\0';
	cout << temp << endl;

	//other data

	int count = 0;
	int sendSuccess = 0;
	time_t start,end;
	start = time(NULL);

	puts("program start");
	system("pause");
	puts("sending");
	while(count < 50){
		SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);		
		connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

		if (send(sockClient, temp, packetLength, 0) == packetLength) {
			printf("send success:num = %d\n", count);
			sendSuccess++;
		}
		else {
			printf("send failed:num = %d\n", count);
		}
		
		/*if(send(sockClient , header.c_str() , header.size() , 0 ) == header.size()){
			printf("send success:num = %d\n",count);
			sendSuccess ++;
		}else{
			printf("send failed:num = %d\n",count);
		}*/

		/*if (send(sockClient, "", 0, 0) == 0) {
			printf("send success:num = %d\n", count);
			sendSuccess++;
		}
		else {
			printf("send failed:num = %d\n", count);
		}*/

		closesocket(sockClient);
		system("pause");
		Sleep(1);
		count ++;
	}

	end=time(NULL);
	printf("send:%d,success:%d,rate=%f%%\n" ,count , sendSuccess ,100*( (double)sendSuccess/count )); 
	printf("time:%fseconds",difftime(end,start));
	WSACleanup();	// ��ֹ���׽��ֿ��ʹ��
	return 0;
}
