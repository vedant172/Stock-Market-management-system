#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
//include <netinet/in.h>
//include <arpa/inet.h>

#define	MAXLINE	 8192  /* Max text line length */
#define max 1000

int open_clientfd(char *hostname, char *port) {
    int clientfd;
    struct addrinfo hints, *listp, *p;
	char host[MAXLINE],service[MAXLINE];
    int flags;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;  /* Open a connection */
    hints.ai_flags = AI_NUMERICSERV;  /* ... using a numeric port arg. */
    hints.ai_flags |= AI_ADDRCONFIG;  /* Recommended for connections where we get IPv4 or IPv6 addresses */
    getaddrinfo(hostname, port, &hints, &listp);

    /* Walk the list for one that we can successfully connect to */
    for (p = listp; p; p = p->ai_next) {
        /* Create a socket descriptor */
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue; /* Socket failed, try the next */

		flags = NI_NUMERICHOST | NI_NUMERICSERV; /* Display address string instead of domain name and port number instead of service name */
		getnameinfo(p->ai_addr, p->ai_addrlen, host, MAXLINE, service, MAXLINE, flags);
        printf("host:%s, service:%s\n", host, service);

        /* Connect to the server */
        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
		{
			printf("Connected to server %s at port %s\n", host, service);
            break; /* Success */
		}
        close(clientfd); /* Connect failed, try another */  //line:netp:openclientfd:closefd
    }

    /* Clean up */
    freeaddrinfo(listp);
    if (!p) /* All connects failed */
        return -1;
    else    /* The last connect succeeded */
        return clientfd;
}


int main(int argc, char **argv)
{
    int clientfd,key,temp,temp_user;
    char *host, *port, buf[MAXLINE],stock_name[100];
    host = argv[1];
    port = argv[2];
    clientfd = open_clientfd(host, port);
     size_t n;
    char username[max],pass[max],confPass[max];
     //fgets(buf, MAXLINE, stdin) != NULL

     while(1)
     {
     	     printf("1.For signUp\n2.For login\n3.To exit\n");
	     printf("Enter option: ");
	     scanf("%d",&temp_user);

	     sprintf(buf,"%d",temp_user);
	     write(clientfd, buf, strlen(buf));
	     if(temp_user == 3) return 0;

	     if(temp_user == 1)
	     {
	     	    printf("Enter username without spaces: ");
		    scanf("%s",username);
		    write(clientfd, username, strlen(username));
		     while((n = read(clientfd, buf, MAXLINE)) == 0);
			   buf[n] = '\0';

		    if(strcmp(buf,"Y")==0)
		    {
		    		printf("\n");
		    		printf("Enter password: ");
		    		scanf("%s",pass);

		    		write(clientfd, pass, strlen(pass));
		    		printf("pass set\n");

		    		break;

		    }
		    else
		    {
		       printf("%s\n",buf);
		       continue;
		    }


	     }

	     if(temp_user == 2)
	     {
	     	    printf("Enter username : ");
		    scanf("%s",username);
		    write(clientfd, username, strlen(username));
		     while((n = read(clientfd, buf, MAXLINE)) == 0);
			   buf[n] = '\0';

		    if(strcmp(buf,"Y")==0)
		    {
		    		printf("\n");
		    		printf("Enter password: ");
		    		scanf("%s",pass);

		    		write(clientfd, pass, strlen(pass));
		    		while((n = read(clientfd, buf, MAXLINE)) == 0);
			        buf[n] = '\0';

		     		if(strcmp(buf,"Y")==0) break;
		     		else
		    		{
		       		printf("%s\n",buf);
		       		continue;
		    		}

		    }
		    else
		    {
		       printf("%s\n",buf);
		       continue;
		    }


	     }


     }

    while (1) {

        printf("choose: 1 get stock detail\n");
   	    printf("choose: 2 buy stock \n");
        printf("choose: 3 sale stock\n");
        printf("choose: 7 Exit\n");
        printf("enter choice: ");
        scanf("%d",&key);
        printf("\n");

        sprintf(buf,"%d",key);
        write(clientfd, buf, strlen(buf));
        if(key == 7) break;
        strcpy(buf,"\0");
        switch(key)
        {

        	case 1:
        	  printf("Enter stock name: ");
		  scanf("%s",stock_name);
        	  write(clientfd, stock_name, strlen(stock_name));

        	  while((n = read(clientfd, buf, MAXLINE)) == 0);
        	   buf[n] = '\0';
        	  printf("%s\n",buf);
        	break;


        	case 2:
        		printf("Enter stock name: ");
			scanf("%s",stock_name);
			write(clientfd, stock_name, strlen(stock_name));

			while((n = read(clientfd, buf, MAXLINE)) == 0);
        	   	buf[n] = '\0';

        	   	if(strcmp(buf,"N")==0)
        	   	{
        	   		printf("stock is not available\n");
        	   	}

        	   	else
        	   	{
        	   	        // while((n = read(clientfd, buf, MAXLINE)) == 0);
				// buf[n] = '\0';
				 printf("%s\n",buf);

        	   		printf("Enter no. of stocks that you want to buy: ");
          			scanf("%d",&temp);
          			sprintf(buf,"%d",temp);
          			write(clientfd, buf, strlen(buf));

          			while((n = read(clientfd, buf, MAXLINE)) == 0);
        	   		buf[n] = '\0';
        	   		printf("%s\n",buf);

        	   	}


        	break;

        	case 3:
        		printf("Enter stock name: ");
			scanf("%s",stock_name);
			write(clientfd, stock_name, strlen(stock_name));

			while((n = read(clientfd, buf, MAXLINE)) == 0);
        	   	buf[n] = '\0';

        	   	if(strcmp(buf,"N")==0)
        	   	{
        	   		printf("stock is not available\n");
        	   	}

        	   	else
        	   	{
        	   	        // while((n = read(clientfd, buf, MAXLINE)) == 0);
				// buf[n] = '\0';
				printf("%s\n",buf);

        	   		printf("Enter no. of stocks that you want to sell: ");
          			scanf("%d",&temp);
          			sprintf(buf,"%d",temp);
          			write(clientfd, buf, strlen(buf));

          			while((n = read(clientfd, buf, MAXLINE)) == 0);
        	   		buf[n] = '\0';
        	   		printf("%s\n",buf);

        	   	}


        	break;


        	default:
        	  printf("please enter right key\n");
        	break;


        }

        //read(clientfd, buf, MAXLINE);
       // fputs(buf, stdout);
	//if (buf[0] == '\n')
	//	break;
    }
    close(clientfd);
    exit(0);
}
