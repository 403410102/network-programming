#include	"unp.h"

char response[] = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html><html lang='en' class=''>"
"<head>"

"<style class=\"cp-pen-styles\">* {"
"  box-sizing: border-box;"
"}"

"html,"
"body {"
"  background-color: #FEDCC8;"
"}"

".parallax {"
"  -webkit-perspective: 100px;"
"          perspective: 100px;"
"  height: 100vh;"
"  overflow-x: hidden;"
"  overflow-y: auto;"
"  position: absolute;"
"  top: 0;"
"  left: 50%;"
"  right: 0;"
"  bottom: 0;"
"  margin-left: -1500px;"
"}"

".parallax__layer {"
"  position: absolute;"
"  top: 0;"
"  right: 0;"
"  bottom: 0;"
"  left: 0;"
"}"
".parallax__layer img {"
"  display: block;"
"  position: absolute;"
"  bottom: 0;"
"}"

".parallax__cover {"
"  background: #2D112B;"
"  display: block;"
"  position: absolute;"
"  top: 100%;"
"  left: 0;"
"  right: 0;"
"  height: 500px;"
"  z-index: 2;"
"}"

".parallax__layer__0 {"
"  -webkit-transform: translateZ(-300px) scale(4);"
"          transform: translateZ(-300px) scale(4);"
"}"

".parallax__layer__1 {"
"  -webkit-transform: translateZ(-250px) scale(3.5);"
"          transform: translateZ(-250px) scale(3.5);"
"}"

".parallax__layer__2 {"
"  -webkit-transform: translateZ(-200px) scale(3);"
"          transform: translateZ(-200px) scale(3);"
"}"

".parallax__layer__3 {"
"  -webkit-transform: translateZ(-150px) scale(2.5);"
"          transform: translateZ(-150px) scale(2.5);"
"}"

".parallax__layer__4 {"
"  -webkit-transform: translateZ(-100px) scale(2);"
"          transform: translateZ(-100px) scale(2);"
"}"

".parallax__layer__5 {"
"  -webkit-transform: translateZ(-50px) scale(1.5);"
"          transform: translateZ(-50px) scale(1.5);"
"}"

".parallax__layer__6 {"
"  -webkit-transform: translateZ(0px) scale(1);"
"          transform: translateZ(0px) scale(1);"
"}"
"</style></head><body>"
"<div class=\"parallax\">"
"    <div class=\"parallax__layer parallax__layer__0\">"
"        <img src=\"https://sam.beckham.io/images/articles/firewatch/layer_0.png\" />"
"    </div>"
"    <div class=\"parallax__layer parallax__layer__1\">"
"        <img src=\"https://sam.beckham.io/images/articles/firewatch/layer_1.png\" />"
"    </div>"
"    <div class=\"parallax__layer parallax__layer__2\">"
"        <img src=\"https://sam.beckham.io/images/articles/firewatch/layer_2.png\" />"
"    </div>"
"    <div class=\"parallax__layer parallax__layer__3\">"
"        <img src=\"https://sam.beckham.io/images/articles/firewatch/layer_3.png\" />"
"    </div>"
"    <div class=\"parallax__layer parallax__layer__4\">"
"        <img src=\"https://sam.beckham.io/images/articles/firewatch/layer_4.png\" />"
"    </div>"
"    <div class=\"parallax__layer parallax__layer__5\">"
"        <img src=\"https://sam.beckham.io/images/articles/firewatch/layer_5.png\" />"
"    </div>"
"    <div class=\"parallax__layer parallax__layer__6\">"
"        <img src=\"https://sam.beckham.io/images/articles/firewatch/layer_6.png\" />"
"    </div>"
"    <div class=\"parallax__cover\"></div>"
"</div>"

"</body></html>";

int
main(int argc, char **argv)
{
	int					i, maxi, maxfd, listenfd, connfd, sockfd;
	int					nready, client[FD_SETSIZE];
	fd_set				rset, allset;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
	int on=1;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(80);//htons(SERV_PORT);

	if((setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)
	{
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
	}

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	maxfd = listenfd;
	maxi = -1;
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
	for ( ; ; ) {
		rset = allset;
		nready = Select(maxfd+1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset)) {
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
#ifdef	NOTDEF
			printf("new client: %s, port %d\n",
					Inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
					ntohs(cliaddr.sin_port));
#endif

			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) {
					client[i] = connfd;
					break;
				}
			if (i == FD_SETSIZE)
				err_quit("too many clients");

			FD_SET(connfd, &allset);
			if (connfd > maxfd)
				maxfd = connfd;
			if (i > maxi)
				maxi = i;

			if (--nready <= 0)
				continue;
		}

		for (i = 0; i <= maxi; i++) {
			if ( (sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {

				Writen(sockfd, response, sizeof(response) - 1);
				printf("clifd: %d\n",sockfd);
				Shutdown(sockfd,SHUT_RDWR);
				FD_CLR(sockfd, &allset);
				client[i] = -1;
				
				if (--nready <= 0)
					break;
			}
		}
	}
}
/* end fig02 */
