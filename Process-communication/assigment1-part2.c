/* @author __Uluc Furkan Vardar__ */
/*Part 2 __ Pipe Lining*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>




#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

//converts to letters to other case
char changeCase(char c){
	if('a'<c && c<'z')
		return c-32;
	else if('A'<c && c<'Z')
		return c+32;
	return c;


}
int main(int argc, char *argv[] )
{
	if ( argc!=2 || strlen(argv[1])>=BUFFER_SIZE ){
		printf("please enter a value (less than 25 letters without any space), format > ./.* string\n");
		return 0;
	}	
	int i;
	char write_msg[BUFFER_SIZE];
	char read_msg[BUFFER_SIZE];
	for( i =0 ; i<BUFFER_SIZE ; i++ )
		write_msg[i]='\0';			
	for( i =0 ; i<strlen(argv[1]) ; i++ )
		write_msg[i]=(char)argv[1][i];
	pid_t pid;
//----
	int fd_1[2];//only parent write
	int fd_2[2];//only child write
	/* create the pipe */
	if (pipe(fd_1) == -1) {
		fprintf(stderr,"Pipe 1 failed");
		return 1;
	}
	/* create the pipe */
	if (pipe(fd_2) == -1) {
		fprintf(stderr,"Pipe 2 failed");
		return 1;
	}
//----
	/* now fork a child process */
	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}
//----
	//let say Child process sends a message and parent make it revers and send again 
	if(pid==0){//child p
		
		//----child send message 
		/* close the unused end of the pipe */
		close(fd_2[READ_END]);
		/* write to the pipe */
		write(fd_2[WRITE_END], write_msg, strlen(write_msg)+1);  //1) Close it and see what happens 
		/* close the write end of the pipe */
		close(fd_2[WRITE_END]);		
		printf(">>>>Child sended message        :'%s'\n",write_msg );
		//----chiled read msg
		close(fd_1[WRITE_END]);
		/* read from the pipe */
		read(fd_1[READ_END], read_msg, BUFFER_SIZE);
		printf(">>>>Child read reversed massage :'%s'\n",read_msg);
		/* close the write end of the pipe */
		close(fd_1[READ_END]);

	}else{//parent p
		//----parent read message 
		/* close the unused end of the pipe */
		close(fd_2[WRITE_END]);
		/* read from the pipe */
		read(fd_2[READ_END], read_msg, BUFFER_SIZE);
		printf("^Parent read message.           :'%s'\n",read_msg);
		/* close the write end of the pipe */
		close(fd_2[READ_END]);
		//----revere it
		int size=strlen(read_msg);
		char new_msg[BUFFER_SIZE];
		int i;
		for( i =0 ; i<BUFFER_SIZE ; i++ )
			new_msg[i]='\0';			
		for( i =0 ; i<size ; i++ )
			new_msg[i]=changeCase(read_msg[i]);
		//----parent send reversed massage
		close(fd_1[READ_END]);
		/* write to the pipe */
		write(fd_1[WRITE_END], new_msg, strlen(new_msg)+1);  //1) Close it and see what happens 
		/* close the write end of the pipe */
		close(fd_1[WRITE_END]);		
		printf("^Parent sended reversed message :'%s'\n",new_msg );		
	}
}

