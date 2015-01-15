#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#define BUF_SIZE 4096
#include<dirent.h>
#include<sys/stat.h>
#include <fcntl.h>
struct dirent *dptr,*dptr1;
struct stat sbuf;

void send_info_to_client(int sfd, struct sockaddr_storage peer_addr, socklen_t peer_addr_len);
void find_owner_of_process(FILE *fd_info_to_client, char *pid);
void add_to_restricted_list(FILE *fd_info_to_client, char *user);
void read_proc(FILE *fd_info_to_client, char *proc_arg);
void parse_the_msg(char *buf,int sfd, struct sockaddr_storage peer_addr, socklen_t peer_addr_len);



void send_info_to_client(int sfd, struct sockaddr_storage peer_addr, socklen_t peer_addr_len) {
	int fd;
	size_t byte = 0, ret;
	char buf[BUF_SIZE];
/*
 *
 *
 * TODO : Implement the following
 * 
 * open the temporary file info_to_client in read-only & non-blocking  mode
 * read from BUF_SIZE -1 bytes from file into the buf
 * send to client the data in buf: do iteratively uni the copmlete data is sent
 *
 * Check & Handle all error conditions
 */

	printf(" Send info back to client \n"); fflush(stdout);
        if ((fd = open("./info_to_client.txt", O_NONBLOCK | O_RDONLY)) < 0) {
        printf("send_info_to_client: open failed \n");
        return ;
        }

        while(1) {
        byte = read(fd, &buf,(BUF_SIZE - 1));
        if (byte <= 0)
        break;

        if (sendto(sfd, buf, byte, 0,
                           (struct sockaddr *) &peer_addr,
                           peer_addr_len) != byte)
                   fprintf(stderr, "Error sending data to client\n");

        } // while

        close(fd);

 
}
#define MAX_NAME_LEN 128
#define MAX_ARG_SZ 256

void find_owner_of_process(FILE *fd_info_to_client, char *pid) {
	char args_to_script[MAX_ARG_SZ] = "./who_is_the_owner.sh "; /* space at the end of the string is must */
	char owner_of_process[MAX_NAME_LEN];
/*
 * 
 * Learn about how the who_is_the_owner script works
 *
 *
 * TODO : Implement the following
 * 
 *
 * Concatenate pid to args_to_script
 * use system function and execute who_is_the_owner script ( see man system for details)
 * the script iwll store the owner in "tmp_owner_name"
 * open the file tmp_owner_name in read-only and non-blocking mode
 * read the owner name from the file into owner_of_process buffer (max len: (MAX_NAME_LEN - 1)
 * end the string with null char
 * compare the string stored in owner_of_process against "none"
 * 	if matches tell that the user CAN NOT be blocked
 * 	else add the owner_of_process to the restricted list using add_to_restricted_list
 *
 * Inform the outcome to client by putting the reply in file pointed by fd_info_to_client
 *
 * Check & Handle all error conditions
 */

	int result,fd, byte;
	printf("find_owner_of_process :%s \n", pid);
	strcat(args_to_script,pid);
	printf("arg: :%s\n", args_to_script);
	fflush(stdout);
	result=system(args_to_script);//GIVE PATH WHERE SHELL SCRIPT IS
	if (result < 0) printf("failed to execute the command \n");

       if ((fd = open("tmp_owner_name_file", O_NONBLOCK | O_RDONLY)) < 0) {
        printf("find_owner_of_process: open failed \n");
        return ;
        }

        byte = read(fd, &owner_of_process,(MAX_NAME_LEN - 1));

        if (byte < 0) {
        printf("find_owner_of_process: read failed \n");
        return ;
        }

        close(fd);
        owner_of_process[byte] = '\0'; // end the string
	printf("owner :%s\n", owner_of_process);

        //printf("\nOwner of process id [%s]\t : %s \n", pid, owner_of_process);
        fprintf(fd_info_to_client,"\nOwner of process id [%s]\t : %s \n", pid, owner_of_process); // add to info_to_client.txt
        add_to_restricted_list(fd_info_to_client, owner_of_process);
}

void add_to_restricted_list(FILE *fd_info_to_client, char *user) {
	int fd_restricted_list;

	
/*
 * 
 * TODO : Implement the following
 * 
 *
 * if length of string in user is len <= 0 do not add to file, return from function
 * open file /home/deepika/restricted_users.txt in append mode (a+) store descriptor in fd_restricted_list
 * add the user name into the file using fd_restricted_list file descriptor
 *  
 * Inform the outcome to client by putting the reply in file pointed by fd_info_to_client
 *
 * Check & Handle all error conditions
 */


        FILE *fd;
        if(strlen(user) <= 0) {
        printf("Can not add empty string to file\n");
        return;
        }
        fd = fopen("./restricted_users.txt", "a+");
        if(fd == NULL) {
        printf("Block User: file open failed: /home/deepika/Desktop/Project-02apr13/restricted_users.txt\n");
        return ;
        }
        fprintf(fd,"%s\n",user);
        //fwrite(user, 1, strlen(user), fd);
        fclose(fd);
        printf("\nEntry added to blocked list is : %s\n", user);
        fprintf(fd_info_to_client,"\nEntry added to blocked list is : %s\n",user); // add to info_to_client.txt
 
        printf("\nupadte the file :\n");
	
    	 
}
#include <sys/stat.h>
void read_proc(FILE *fd_info_to_client, char *proc_arg) {

/*
 * 
 * TODO : Implement the following
 * 
 *
 * read the /proc fielsystem and get the infromation as requested by the proc_arg
 * if proc_arg is a file then use system function and copy the file to fd_info_to_client 
 * if it is a directory read the contents of the directory to fd_info_to_client
 *
 * Inform the outcome to client by putting the reply in file pointed by fd_info_to_client
 *
 * Check & Handle all error conditions
 *
 */

        struct stat sb;
        char disp_dir[MAX_ARG_SZ] = "ls -C --file-type "; /* space at the end of the string is must */
        char disp_file[MAX_ARG_SZ] = "cp -f "; /* space at the end of the string is must */
        int ret;

           if (!strcmp(proc_arg,"")) {
               printf("\nget_proc_info: NULL argument, Can NOT Proceed\n");
               return;
           }

           if (stat(proc_arg, &sb) == -1) {
               printf("\nget_proc_info: stat failed for input %s\n",proc_arg);
               return;

           }

           printf("File type:                ");

           switch (sb.st_mode & S_IFMT) {
           case S_IFBLK:
                        printf("block device\n");
                        fprintf(fd_info_to_client,"\n[%s] is a block device\n", proc_arg); // put msg in info_to_client.txt
                        break;
           case S_IFCHR:
                        printf("character device\n");
                        fprintf(fd_info_to_client,"\n[%s] is a character device\n", proc_arg); // put msg in info_to_client.txt
                        break;
           case S_IFDIR:
                        printf("directory\n");

                        strcat(disp_dir, proc_arg);
                        strcat(disp_dir, " > info_to_client.txt"); //ls <dir> > info_to_client.txt
                        ret = system(disp_dir);
                        if (ret) {
                        printf("get_proc_info: can not access the directory : [%s]\n",proc_arg);
                        return ;
                        }

                        break;
           case S_IFIFO:
                        printf("FIFO/pipe\n");
                        fprintf(fd_info_to_client,"\n[%s] is a FIFO/pipe\n", proc_arg); // put msg in info_to_client.txt
                        break;
           case S_IFLNK:
                        printf("symlink\n");
                        fprintf(fd_info_to_client,"\n[%s] is a symlink\n", proc_arg); // put msg in info_to_client.txt
                        break;
           case S_IFREG:
                        printf("regular file\n");
                        strcat(disp_file, proc_arg); //cp -f <file>
                        strcat(disp_file, " ");
                        strcat(disp_file, "info_to_client.txt"); //cp -f <file> info_to_client.txt
                        ret = system(disp_file);
                        if (ret) {
                        printf("get_proc_info: can not access the file : [%s]\n",proc_arg);
                        return ;
                        }
                        break;
           case S_IFSOCK:
                        printf("socket\n");
                        fprintf(fd_info_to_client,"\n[%s] is a socket\n", proc_arg); // put msg in info_to_client.txt
                        break;
           default:
                        printf("unknown Filetype\n");
                        fprintf(fd_info_to_client,"\n[%s] Filetype is unlnown\n", proc_arg); // put msg in info_to_client.txt
                        break;
           }

}


void parse_the_msg(char *buf,int sfd, struct sockaddr_storage peer_addr, socklen_t peer_addr_len) {
char real_info[BUF_SIZE], ch,option;
FILE *fd_info_to_client;
char *proc_arg,*pid;
int i=0,result;

/* 
 * 
 * TODO : Implement the following
 * 
 *
 * read the first char in buf and store into ch
 * copy the string from buf[2] to until end of the string into real_info 
 * remove the file info_to_client.txt using system function          ( see man system for details)
 * open file info_to_client.txt in write mode & strore the file descriptor in fd_info_to_client
 * 
 * use switch & case based on ch content 
 * 	D:   	use read_proc function
 * 		read requested proc info and store it in send_info_to_client
 *
 * 	W:   	execute who -a > info_to_client.txt using system function (see man system for details)
 *
 * 	B:   	use add_to_restricted_list function
 * 		add real_info to restricted list using function add_to_restricted_list
 *
 * 	P: 	use find_owner_of_process function
 * 		find owner of pid using function find_owner_of_process
 *
 * close the file info_to_client.txt
 *
 * Inform the outcome to client by putting the reply in file pointed by fd_info_to_client
 *
 * Check & Handle all error conditions
 */

	system("rm -f info_to_client.txt");

	ch = *buf;
	buf = buf + 2;
	strcpy(real_info, buf);



        fd_info_to_client = fopen("info_to_client.txt", "w");
        if(fd_info_to_client == NULL) {
        printf("file open failed for write mode:info_to_client.txt\n");
        return ;
        }
	
	switch(ch)
	{
		case 'D':
			read_proc(fd_info_to_client,real_info);
			fclose(fd_info_to_client);	
			send_info_to_client(sfd, peer_addr, peer_addr_len);
			break;

		case 'W':
			fclose(fd_info_to_client);	
			result=system("who -a >info_to_client.txt");
			if(result < 0 )printf("failed to execute the command\n");
			send_info_to_client(sfd, peer_addr, peer_addr_len);
			break;

		case 'B': 
			add_to_restricted_list(fd_info_to_client,real_info);
			printf("close fd\n"); fflush(stdout);
			fclose(fd_info_to_client);	
			printf("closed fd\n"); fflush(stdout);
			send_info_to_client(sfd, peer_addr, peer_addr_len);
			break;

		case 'P':
			
			find_owner_of_process(fd_info_to_client,real_info);
			fclose(fd_info_to_client);	
			send_info_to_client(sfd, peer_addr, peer_addr_len);
			break;
			
	}		
	
	


}




       int
       main(int argc, char *argv[])
       {
           struct addrinfo hints;
           struct addrinfo *result, *rp;
           int sfd;
	   int s;
           struct sockaddr_storage peer_addr;
           socklen_t peer_addr_len;
           ssize_t nread;
           char buf[BUF_SIZE];

           if (argc != 2) {
               fprintf(stderr, "Usage: %s port\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           memset(&hints, 0, sizeof(struct addrinfo));
           hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
           hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
           hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
           hints.ai_protocol = 0;          /* Any protocol */
           hints.ai_canonname = NULL;
           hints.ai_addr = NULL;
           hints.ai_next = NULL;

           s = getaddrinfo(NULL, argv[1], &hints, &result);
           if (s != 0) {
               fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
               exit(EXIT_FAILURE);
           }

	/* 
	 * getaddrinfo() returns a list of address structures.
	 * Try each address until we successfully bind(2).
	 * If socket(2) (or bind(2)) fails, we (close the socket
	 * and) try the next address. 
	 */

           for (rp = result; rp != NULL; rp = rp->ai_next) {
               sfd = socket(rp->ai_family, rp->ai_socktype,
                       rp->ai_protocol);
               if (sfd == -1)
                   continue;

               if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
                   break;                  /* Success */

               close(sfd);
           }
           if (rp == NULL) {               /* No address succeeded */
               fprintf(stderr, "Could not bind\n");
               exit(EXIT_FAILURE);
           }

           freeaddrinfo(result);           /* No longer needed */

           /* Read datagrams and echo them back to sender */

           for (;;) {
               peer_addr_len = sizeof(struct sockaddr_storage);
               nread = recvfrom(sfd, buf, BUF_SIZE, 0,
                       (struct sockaddr *) &peer_addr, &peer_addr_len);
               if (nread == -1)
                   continue;               /* Ignore failed request */

               char host[NI_MAXHOST], service[NI_MAXSERV];

               s = getnameinfo((struct sockaddr *) &peer_addr,
                               peer_addr_len, host, NI_MAXHOST,
                               service, NI_MAXSERV, NI_NUMERICSERV);
              if (s == 0)
                   printf("Received %ld bytes from %s:%s\n",
                           (long) nread, host, service);
                  
               else
                   fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
		/*
 
		printf("the received message is %s\n",buf);
		printf("Sending ack to client for %d bytes\n", nread);

               if (sendto(sfd, buf, nread, 0,
                           (struct sockaddr *) &peer_addr,
                           peer_addr_len) != nread)
                   fprintf(stderr, "Error sending response\n");
		*/
/* for getting the server side results */

		parse_the_msg(buf,sfd, peer_addr, peer_addr_len);
		fflush(stdout);
/* send EOF to signify the client that the transmission of data is complete */
		 if (sendto(sfd, "EOF", 4, 0,
                           (struct sockaddr *) &peer_addr,
                           peer_addr_len) != 4)
                   fprintf(stderr, "Error sending EOF\n");
/* end */

		
           }
       }

 
