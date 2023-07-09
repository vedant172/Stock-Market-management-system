#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "common.h"
//include <netinet/in.h>
//include <arpa/inet.h>

/* Misc constants */
#define	MAXLINE	 8192  /* Max text line length */
#define LISTENQ  1024  /* Second argument to listen() */

pthread_rwlock_t rw_lock;

int open_listenfd(char *port)
{
    struct addrinfo hints, *listp, *p;
    int listenfd, optval=1;
	char host[MAXLINE],service[MAXLINE];
    int flags;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;             /* Accept connections */
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* ... on any IP address AI_PASSIVE - used on server for TCP passive connection, AI_ADDRCONFIG - to use both IPv4 and IPv6 addresses */
    hints.ai_flags |= AI_NUMERICSERV;            /* ... using port number instead of service name*/
    getaddrinfo(NULL, port, &hints, &listp);

    /* Walk the list for one that we can bind to */
    for (p = listp; p; p = p->ai_next) {
        /* Create a socket descriptor */
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;  /* Socket failed, try the next */

        /* Eliminates "Address already in use" error from bind */
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,    //line:netp:csapp:setsockopt
                   (const void *)&optval , sizeof(int));

		flags = NI_NUMERICHOST | NI_NUMERICSERV; /* Display address string instead of domain name and port number instead of service name */
		getnameinfo(p->ai_addr, p->ai_addrlen, host, MAXLINE, service, MAXLINE, flags);
        printf("host:%s, service:%s\n", host, service);

        /* Bind the descriptor to the address */
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break; /* Success */
        close(listenfd); /* Bind failed, try the next */
    }

    /* Clean up */
    freeaddrinfo(listp);
    if (!p) /* No address worked */
        return -1;

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0) {
        close(listenfd);
	return -1;
    }
    return listenfd;
}

void* echo(void *arg)
{
    int *connfd_temp = (int *)arg,user_key;
    int connfd = *connfd_temp;
    size_t n;
    char buf[MAXLINE],str_quan[100],temp_buf[MAXLINE];

    while(1)
    {
    	 while((n = read(connfd, buf, MAXLINE)) == 0) ;
         buf[n] = '\0';
         if(strcmp(buf,"3")==0)
         {
               pthread_exit(NULL);
         }

	 if(strcmp(buf,"1")==0)
         {
              printf("sigup\n");
              while((n = read(connfd, buf, MAXLINE)) == 0) ;
              buf[n] = '\0';
              strcpy(username,buf);
              if(check_user_name(buf)==0)
              {
                 strcpy(buf,"Username already exist!!");
                 write(connfd, buf, strlen(buf));
                 continue;
              }

              else
              {
                 strcpy(buf,"Y");
                 write(connfd, buf, strlen(buf));
                 while((n = read(connfd, buf, MAXLINE)) == 0) ;
                 buf[n] = '\0';

                 printf("psd: %s\n",buf);
                  set_pass(buf);
                 printf("zadu: %s\n",username);
		  break;

              }
         }



	if(strcmp(buf,"2")==0)
         {
              printf("login\n");
              while((n = read(connfd, buf, MAXLINE)) == 0) ;
              buf[n] = '\0';
              strcpy(username,buf);
              if(check_user_name(buf)==1)
              {
                 strcpy(buf,"wrong username");
                 write(connfd, buf, strlen(buf));
                 continue;
              }

              else
              {
                 strcpy(buf,"Y");
                 write(connfd, buf, strlen(buf));
                 while((n = read(connfd, buf, MAXLINE)) == 0) ;
                 buf[n] = '\0';

                 printf("psd: %s\n",buf);
                  if(check_pass(buf)==0)
                  {
                  	strcpy(buf,"Password doesn't match");
                 	write(connfd, buf, strlen(buf));
                 	continue;
                  }

		  else
		  {
		     strcpy(buf,"Y");
                    write(connfd, buf, strlen(buf));
                    break;
		  }

              }
         }
       }


    while(1) {

        while((n = read(connfd, buf, MAXLINE)) == 0) ;
         buf[n] = '\0';
         if(strcmp(buf,"7")==0)
         {
               pthread_exit(NULL);
         }
	buf[n] = '\0';

        if(strcmp(buf,"1")==0)
        {
            while((n = read(connfd, buf, MAXLINE)) == 0);
	     buf[n] = '\0';

             strcpy(stock_name,buf);
             price =-1;
             quan =-1;
             int status;

             pthread_rwlock_rdlock(&rw_lock);
             SHOW();
             pthread_rwlock_unlock(&rw_lock);

             if(price==-1 && quan==-1)
             {
                strcpy(buf,"\0");
                strcpy(buf,"stock is not available");
                  write(connfd, buf, strlen(buf));
             }

             else
             {
                  strcpy(buf,"\0");
             	  sprintf(buf,"%s %d\n%s %d","price:",price,"quan: ",quan);
             	  write(connfd, buf, strlen(buf));
             }
        }

          if(strcmp(buf,"2")==0)
          {
             while((n = read(connfd, buf, MAXLINE)) == 0);
	     buf[n] = '\0';

             strcpy(stock_name,buf);
             price =-1;
             quan =-1;
             int status;

             pthread_rwlock_rdlock(&rw_lock);
             SHOW();
             pthread_rwlock_unlock(&rw_lock);

             if(price==-1 && quan==-1)
             {
                strcpy(buf,"\0");
                strcpy(buf,"N");
                write(connfd, buf, strlen(buf));
             }

             else
             {
               printf("stock YES\n");
                strcpy(buf,"\0");
             	 sprintf(buf,"%s %d\n%s %d","price:",price,"quan: ",quan);
             	 write(connfd, buf, strlen(buf));

                while((n = read(connfd, buf, MAXLINE)) == 0);
	        buf[n] = '\0';

	        int quan_stock_buy;
	        sscanf(buf,"%d",&quan_stock_buy);

	        if(quan_stock_buy>quan)
	        {
	           strcpy(buf,"\0");
                   strcpy(buf,"Invaild input");
                   write(connfd, buf, strlen(buf));
	        }
	        else
	        {
	           pthread_rwlock_wrlock(&rw_lock);
	           BUY(quan_stock_buy);
	           pthread_rwlock_unlock(&rw_lock);

	            strcpy(buf,"\0");
	            sprintf(buf,"%s %d\n%s %d\n%s %d","Price of share: ",price,"No. of shares alloted: ",quan_stock_buy,"Total amount to be  paid: ",price*quan_stock_buy);
	            write(connfd, buf, strlen(buf));

	        }
             }

          }

          if(strcmp(buf,"3")==0)
          {
             while((n = read(connfd, buf, MAXLINE)) == 0);
	     buf[n] = '\0';

             strcpy(stock_name,buf);
             price =-1;
             quan =-1;
             int status;

             pthread_rwlock_rdlock(&rw_lock);
             SHOW();
             pthread_rwlock_unlock(&rw_lock);

             if(price==-1 && quan==-1)
             {
                strcpy(buf,"\0");
                strcpy(buf,"N");
                write(connfd, buf, strlen(buf));
             }

             else
             {
                //printf("stock YES\n");
                 strcpy(buf,"\0");
             	 sprintf(buf,"%s %d\n%s %d","price:",price,"quan: ",quan);
             	 write(connfd, buf, strlen(buf));

                while((n = read(connfd, buf, MAXLINE)) == 0);
	        buf[n] = '\0';

	        int quan_stock_sell;
	        sscanf(buf,"%d",&quan_stock_sell);

	           pthread_rwlock_wrlock(&rw_lock);
	           SELL(quan_stock_sell);
	           pthread_rwlock_unlock(&rw_lock);

	            strcpy(buf,"\0");
	            sprintf(buf,"%s %d\n%s %d\n%s %d","Price of share: ",price,"No. of shares: ",quan_stock_sell,"Total amount to be collected: ",price*quan_stock_sell);
	            write(connfd, buf, strlen(buf));

             }
          }
    }
    printf("end\n");
}

int main(int argc, char **argv)
{
    int ret;
    ret = pthread_rwlock_init(&rw_lock, NULL);

    pthread_t threadID[20];
    int listenfd, connfd[20];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr; /* Enough room for any addr */
    char client_hostname[MAXLINE], client_port[MAXLINE];
    listenfd = open_listenfd(argv[1]);

    int i=0;
    while (1) {
	printf("Waiting for a new Client to connect\n");
        clientlen = sizeof(struct sockaddr_storage); /* Important! */
        connfd[i] = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);

        getnameinfo((struct sockaddr *) &clientaddr, clientlen,
        	client_hostname, MAXLINE, client_port, MAXLINE, 0);

        printf("Connected to (%s, %s)\n", client_hostname, client_port);
	printf("Start Communication with Client: %d\n",i);
        //echo(connfd);

        pthread_create(&threadID[i],NULL,echo,(void*)&connfd[i]);
        i++;
    }

    for(int j=0; j<i; j++)
    {
        pthread_join(threadID[j],NULL);
        printf("End Communication with Client: %d\n",j);
        close(connfd[i]);
    }
    ret = pthread_rwlock_destroy(&rw_lock);
    exit(0);
}

