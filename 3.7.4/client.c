/* 
实现服务器和客户机的通信收发信息，传输一个数据结构，客户端发->服务器收->服务器发->客户端收
 int socket(int domain, int type, int protocol);
 int bind(int sockfd, const struct sockaddr *addr,
          socklen_t addrlen);
 int inet_pton(int af, const char *src, void *dst);
int listen(int sockfd, int backlog);
int accept(int socket, struct sockaddr *restrict address,
       socklen_t *restrict address_len);

	   
       int connect(int sockfd, const struct sockaddr *addr,
                   socklen_t addrlen);
	   
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
 */
 
 
 #include <stdio.h>
  #include <sys/types.h>          
       #include <sys/socket.h>
		#include <arpa/inet.h>

       #include <netinet/in.h>
#include <string.h>
	   
#define SERip "192.168.206.128"
#define SERport 6005

#define CMD_REG	10	   
#define	STA_OK	110
typedef struct student_info
{
	char name[10];
	int age;
	int comline;
	int stat;
}info;

	   
 int main(void)
 {
	 int sockfd = -1;
	 int ret = -1;
	 char sendbuf[10]={0};
	 char recvbuf[10]={0};
	 
	 info stdcli;

	 //服务器信息数据结构
	 struct sockaddr_in seraddr={0};
	 seraddr.sin_family = AF_INET;
	 seraddr.sin_port = htons(SERport);										//注意使用网络字节序
	 seraddr.sin_addr.s_addr = inet_addr(SERip);
	 
	  struct sockaddr_in cliaddr={0};
	  int clilen = sizeof(cliaddr);
	 //1.建立一个socket网络文件描述符，用于连接后的读写
	 sockfd = socket(AF_INET,SOCK_STREAM,0);						//IPV4,TCP,单个协议
	 if(sockfd<0)
	 {
		 perror("socket");
		 return -1;
	 }
	 printf("socket ok %d\n",sockfd);
	 //2.连接sockt到server ip和port
	 ret = connect(sockfd,(const struct sockaddr *)&seraddr,sizeof(seraddr));		//如果消除警告注意强制类型转化
	 if(ret<0)
	 {
		 perror("connect");
		 return -1;
	 }
	 printf("connect ok\n");
	 while(1)
	 {
		 // ret = send(sockfd,buf,sizeof(buf)/sizeof(buf[0]),0);
		//3.客户端发数据给服务器
		printf("please input name:\n");
		scanf("%s",stdcli.name);
		printf("please input age:\n");
		scanf("%d",&stdcli.age);
		stdcli.comline = CMD_REG;

		ret = send(sockfd,&stdcli,sizeof(stdcli),0);	
		
		//4.客户端接受服务器给的应答
		memset(&stdcli,0,sizeof(stdcli));
		ret = recv(sockfd,&stdcli,sizeof(stdcli),0);
		
		
		//5.客户端解析服务器的应答，再做进一步处理
		if(stdcli.stat == STA_OK)
		{
			printf("success\n"); 
		}
		else
		{
			printf("error\n"); 
		}
	 }
	
	
	 return 0;
 }