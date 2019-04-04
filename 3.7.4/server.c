/* 
实现服务器和客户机的通信收发信息，传输一个数据结构，客户端发->服务器收->服务器发->客户端收
 int socket(int domain, int type, int protocol);
 int bind(int sockfd, const struct sockaddr *addr,
          socklen_t addrlen);
 int inet_pton(int af, const char *src, void *dst);
int listen(int sockfd, int backlog);
int accept(int socket, struct sockaddr *restrict address,
       socklen_t *restrict address_len);

ssize_t recv(int sockfd, void *buf, size_t len, int flags);

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
	 int aptfd = -1;
	 int ret = -1;
	 int backlog = 100;											//监听等待队列长度
	 
	 char sendbuf[10]={0};
	 char recvbuf[10]={0};
	 
	 info stdser;
	 //服务器信息数据结构
	 struct sockaddr_in seraddr={0};
	 seraddr.sin_family = AF_INET;
	 seraddr.sin_port = htons(SERport);										//注意使用网络字节序
	 seraddr.sin_addr.s_addr = inet_addr(SERip);
	 
	  struct sockaddr_in cliaddr={0};
	  socklen_t clilen = 0;
	 //1.建立一个socket网络文件描述符，用于监听
	 sockfd = socket(AF_INET,SOCK_STREAM,0);						//IPV4,TCP,单个协议
	 if(sockfd<0)
	 {
		 perror("socket");
		 return -1;
	 }
	 printf("socket ok %d\n",sockfd);
	 //2.绑定sockt到server ip和port
	 ret = bind(sockfd,(const struct sockaddr *)&seraddr,sizeof(seraddr));		//如果消除警告注意强制类型转化
	 if(ret<0)
	 {
		 perror("socket");
		 return -1;
	 }
	 printf("bind ok\n");
	 //3.进入监听状态
	 listen(sockfd,backlog);
	 if(ret<0)
	 {
		 perror("socket");
		 return -1;
	 }
	 printf("listen ok\n");
	 //4.堵塞等待状态
	 aptfd = accept(sockfd,(struct sockaddr *)&cliaddr,&clilen);
	 if(aptfd<0)
	 {
		 perror("socket");
		 return -1;
	 }
	 printf("accept ok %d\n",aptfd);
	 while(1)
	 {
		//5.服务器接收客户机发的数据
		ret = recv(aptfd,&stdser,sizeof(stdser),0);
		 
		
		//6.服务器解析客户端的数据包，然后干活
		if(stdser.comline == CMD_REG)
		{
			printf("name[%s]\n",stdser.name);
			printf("age[%d]\n",stdser.age);
		}	
		//7.服务器发送应答信号给客户端
		memset(&stdser,0,sizeof(stdser));
		stdser.stat = STA_OK;
		ret = send(aptfd,&stdser,sizeof(stdser),0);

	 }

	 return 0;
 }