#include "protocol.h"

#pragma comment(lib, "ws2_32.lib") 
using namespace std;

int main(void)
{
	// 加载socket动态链接库(dll)
	WORD wVersionRequested;
	WSADATA wsaData;	// 这结构是用于接收Wjndows Socket的结构信息的
	hostent * pHostent = NULL;
	int err;
	 
	wVersionRequested = MAKEWORD( 2, 2 );	// 请求1.1版本的WinSock库
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return -1;			// 返回值为零的时候是表示成功申请WSAStartup
	}
	 
	if ( LOBYTE( wsaData.wVersion ) != 2 ||	HIBYTE( wsaData.wVersion ) != 2 ) {
		// 检查这个低字节是不是1，高字节是不是1以确定是否我们所请求的1.1版本
		// 否则的话，调用WSACleanup()清除信息，结束函数
		WSACleanup( );
		return -1; 
	}
	
	// 创建socket操作，建立流式套接字，返回套接字号sockClient
	// SOCKET socket(int af, int type, int protocol);
	// 第一个参数，指定地址簇(TCP/IP只能是AF_INET，也可写成PF_INET)
	// 第二个，选择套接字的类型(流式套接字)，第三个，特定地址家族相关协议（0为自动）

	// 将套接字sockClient与远程主机相连
	// int connect( SOCKET s,  const struct sockaddr* name,  int namelen);
	// 第一个参数：需要进行连接操作的套接字
	// 第二个参数：设定所需要连接的地址信息
	// 第三个参数：地址的长度

	//pHostent = gethostbyname("5033598.all123.net");
	//puts(pHostent->h_addr_list[0]);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.s_addr = inet_addr("127.0.0.1");
	//addrSrv.sin_addr.s_addr = *((unsigned long *)pHostent->h_addr);	// 本地回路地址是127.0.0.1; 
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6001);

	int count = 0;
	int sendSuccess = 0;
	time_t start,end;
	start = time(NULL);
	char temp[] = "hello world!";
	puts("program start");
	system("pause");
	puts("sending");
	while(count < 50){
		SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);		
		connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

		if ((count = send(sockClient, temp, strlen(temp), 0)) == strlen(temp)) {
			printf("send success:num = %d\n", count);
			sendSuccess++;
		}
		else {
			printf("send failed:num = %d\n", count);
		}

		char temp[256] = { 0 };
		int num = recv(sockClient, temp, 256, 0);
		printf("recv:%d content:\n", num);
		for (int i = 0; i < num; ++i) {
			putchar(temp[i]);
		}
		putchar('\n');

		closesocket(sockClient);
		system("pause");
		//Sleep(1);
		count ++;
	}

	end=time(NULL);
	printf("send:%d,success:%d,rate=%f%%\n" ,count , sendSuccess ,100*( (double)sendSuccess/count )); 
	printf("time:%fseconds",difftime(end,start));
	WSACleanup();	// 终止对套接字库的使用
	return 0;
}
