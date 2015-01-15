       #include <sys/types.h>
       #include <sys/socket.h>
       #include <netdb.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h>
       #include <string.h>
	#include<dirent.h>
       #define BUF_SIZE 4096
	#define MAX_SERVERS 9

char arg_to_server[132];
char server[MAX_SERVERS][16];
int port[MAX_SERVERS][8];
int ser_total = 1; /* except for case 1, we deal with single server */
struct dirent *dptr,*dptr1;



int get_and_parse_input() 
{

		int choice,i;
		char arg[128]="who -a";
		 DIR *dirp;
		char buf[10]="/proc";

		printf("\n\033[1;34mChoices:\n1.Display proc info of server(s)\t2.Display users of server 'X'\n3.Block a user on server 'X'\t\t4.Block owner of process 'P' on server 'X'\nEnter choice:\033[0m");
		scanf("%d", &choice);
		switch(choice) 
		{
			case 1:
			{
        		/* Display proc info of server(s) */
        		printf("\033[1;36m############ To get proc details from server(s), Enter the following Details ##############\033[0m\n");
				/*
 				*
 				*
 				* TODO : Implement the following
				 * 
 				* read no of servers: ser_total, if > MAX print error & quit
 				* read the proc argument into arg
 				* read iteratively following details for all serves 
 				* 	a) server ip int server[i] 
 				* 	b) port no into port[i]
		 		* concatenate D: to to the arg value and store in arg_to_server
 				*
 				*/
		

		        printf("\033[1;34mNo_of servers(MAX:9)\t:\033[0m"); scanf("%d", &ser_total);
		        if (ser_total <= 0 || ser_total > MAX_SERVERS) {
		        printf("server count invalid / exceeded \n");
		        return -1;}
		        

			printf("\n \033[1;31mSelect from the following /proc argument\033[0m\n");
			

			
        		
        		if((dirp=opendir(buf))==NULL)
        		{
                		printf("Directory /proc doesn't exists...");
                		return -1;
        		}
        		while(dptr=readdir(dirp))
        		{
				
               			 printf("%s\t",dptr->d_name);
				
        		}

	
       			 closedir(dirp);




		        printf("\n\n\033[1;31mSelect an arg of the form /proc/....The above displayed list of files or directories can be taken as a reference\033[0m\t:"); 
			
			scanf("%s", &arg);
		        for(i=0;i<ser_total; i++) {
		        printf("\033[1;34mEnter server[%d] Details: \033[0m\n", i);
		        printf("\033[1;34mserver ip:\033[0m"); scanf("%s", &server[i]);
		        printf("\033[1;34mport:\033[0m"); scanf("%s", &port[i]);
		        }
			strcpy(arg_to_server, "D:");
		        strcat(arg_to_server, arg);

        		break;
			}   //end of case 1
#if 1
			case 2:
			{
        			/* Display current users in machine 'X' */
        			/* arg contains the who -a command */
        		printf("\033[1;36m############ To get user list from a server, Enter the following Details ##############\033[0m\n");
			/*
			 *
 			*
 			* TODO : Implement the following
			 * 
			 * read server ip into server[0] b) port no into port[0]
 			* concatenate W: to to the arg value and store in arg_to_server
			 *
			 */
				strcpy(arg_to_server, "W:");
				printf("\n\033[1;34mEnter ip:\033[0m"); scanf("%s",&server[0]);
				printf("\n\033[1;34mEnter port:\033[0m"); scanf("%s",&port[0]);
				strcat(arg_to_server,arg);
				printf("\033[1;34mAfter concat \033[0m%s\n",arg_to_server);
				
				fflush(stdout);

        		break;
			}
			case 3:
			{
        			/* block user */
        			printf("\033[1;36m############ To Block an user on the server, Enter the following Details ##############\033[0m\n");
				/*
 				*
 				*
 				* TODO : Implement the following
 				* 
 				* read user name into arg
 				* read server ip into server[0] 
 				* port no into port[0]
 				* concatenate B: to to the arg value and store in arg_to_server
 				*
 				*/
				strcpy(arg_to_server, "B:");
				printf("\n\033[1;34mEnter ip:\033[0m"); scanf("%s",&server[0]);
				printf("\n\033[1;34mEnter port:\033[0m"); scanf("%s",&port[0]);
				printf("\033[1;34mUser to be blocked:\033[0m "); scanf("%s",&arg);
				strcat(arg_to_server,arg);
				printf("\033[1;34mAfter concat \033[0m%s\n",arg_to_server);
				
				fflush(stdout);


        		break;
			}
			case 4:
			{
        		/* block user of a process */
        		printf("\033[1;36m############ To Block Owner of a process on the server, Enter the following Details ##############\033[0m\n");
			/*
 			*
 			*
 			* TODO : Implement the following
 			* 
 			* read process id into arg
 			* read server ip into server[0] 
 			* port no into port[0]
 			* concatenate P: to to the arg value and store in arg_to_server
 			*
 			*/
				strcpy(arg_to_server, "P:");
                                printf("\n\033[1;34mEnter ip:\033[0m"); scanf("%s",&server[0]);
                                printf("\n\033[1;34mEnter port:\033[0m"); scanf("%s",&port[0]);
                                printf("\033[1;34mProcess id (of User to be blocked):\033[0m "); scanf("%s",&arg);
                                strcat(arg_to_server,arg);
                                printf("\033[1;34mAfter concat\033[0m %s\n",arg_to_server);
                               
                                fflush(stdout);

        		break;
			}
#endif
	default:
        		printf("Invalid option, exiting the program\n");
        		return -1;

	}////switch
	

        switch(choice) {
        case 1:
                printf("\n\n\033[1;35m################\t Proc FS Information \t##############################\033[0m\n\n");
                break;
        case 2:
                printf("\n\n\033[1;35m################\t List of Users \t##############################\033[0m\n\n");
                printf("\n\t\t\t List of Users \n\n");
                break;
        case 3:
                printf("\n\n\033[1;35m################\t Blocking an User \t##############################\033[0m\n\n");
                printf("\n\t\t\t Blocking an User \n\n");
                break;
        case 4:
                printf("\n\n\033[1;35m################\t Blocking owner of a particular process \t##############################\033[0m\n\n");
                break;
        }
        
}




	int main(int argc, char *argv[])
       {
           struct addrinfo hints;
           struct addrinfo *result, *rp;
           int sfd, s, j, i;
           size_t len;
           ssize_t nread;
           char buf[BUF_SIZE];
	

		
		if (get_and_parse_input() < 0) {
		printf("Error in input ... Exiting\n");
		return 0;
		}
	

           /* Obtain address(es) matching host/port */
	
           memset(&hints, 0, sizeof(struct addrinfo));
           hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
           hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
           hints.ai_flags = 0;
           hints.ai_protocol = 0;          /* Any protocol */


	   for (i=0; i < ser_total; i++) { // for no of servers
		printf("\nFrom server:%s, port:%s\n", server[i], port[i]); fflush(stdout);

           s = getaddrinfo(server[i], port[i], &hints, &result);
           if (s != 0) {
               fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
               exit(EXIT_FAILURE);
           }
	/* 
	 * getaddrinfo() returns a list of address structures.
	 * Try each address until we successfully connect(2).
	 * If socket(2) (or connect(2)) fails, we (close the socket
	 * and) try the next address. 
	 */

           for (rp = result; rp != NULL; rp = rp->ai_next) {
               sfd = socket(rp->ai_family, rp->ai_socktype,
                            rp->ai_protocol);
               if (sfd == -1)
                   continue;

               if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
                   break;                  /* Success */

               close(sfd);
           }

           if (rp == NULL) {               /* No address succeeded */
               fprintf(stderr, "Could not connect\n");
               exit(EXIT_FAILURE);
           }

           freeaddrinfo(result);           /* No longer needed */


               len = strlen(arg_to_server) + 1;
                       /* +1 for terminating null byte */

               if (len + 1 > BUF_SIZE) {
                   fprintf(stderr,
                           "long message ... aborting\n");
                   return 0;
               }

               if (write(sfd, arg_to_server, len) != len) {
                   fprintf(stderr, "partial/failed write\n");
                   exit(EXIT_FAILURE);
               }

	while (1) 
	{ // read from socket and store in buffer
	       nread = read(sfd, buf, BUF_SIZE);
               if (nread == -1) {
                   perror("read");
                   exit(EXIT_FAILURE);
               }
		if(!strcmp(buf,"EOF")) { // server completed the transaction 
		break;
		}
		printf("\033[0;30m%s\033[0m", buf); // print the data received from buffer

	} //while
        //       printf("Got ack from server for %ld bytes: %s\n", (long) nread, buf);
	} // for ser_total

           exit(EXIT_SUCCESS);
 }





