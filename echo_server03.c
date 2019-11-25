#include <stdio.h> // 표준 입출력
#include <stdlib.h> // 표준 라이브러리
#include <string.h> // 문자열 처리 함수
#include <unistd.h> // 유닉스 표준
#include <arpa/inet.h> // 인터넷 프로토콜
#include <sys/socket.h> // 소켓통신 함수
#define BUF_SIZE 1024 // 1024바이트 버퍼 크기 사용
void error_handling(char * message);

int main(int argc, char * argv[])
{
	int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len, i;

	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;

	if(argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_adr_sz = sizeof(clnt_adr);
	for(i = 0; i < 5; i++)
	{
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d\n", i+1);

		while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
			write(clnt_sock, message, str_len);

		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

