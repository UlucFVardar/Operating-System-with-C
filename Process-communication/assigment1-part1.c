/* @author __Uluc Furkan Vardar__*/
/*Part 1 __ Shared Memory*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>



int main(  int argc, char *argv[] )
{
	if ( argc!=2 ){
		printf("please enter a value format > ./.* value\n");
		return 0;
	}	
	const int SIZE = 4096; // shared memory Size
	const char *name = "OS-A1.values"; //shared memory name
	int myValue=atoi(argv[1]);

	int shm_fd;
	void *ptr;
	
	// create a fork -child process and parent process
	pid_t process_id = fork();

	if(process_id==0){//child
		//shared memory created with 
	    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	    //size is setted
		ftruncate(shm_fd,SIZE);

		/* now map the shared memory segment in the address space of the process */
		ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
		if (ptr == MAP_FAILED) {
			printf("Map failed\n");
			return -1;
		}
		printf("Child process is writing...to shm\n");

		while(myValue!=1){	
			sprintf(ptr,"%d",myValue);
			printf("%d ",myValue);
			ptr += sizeof(int);
			myValue=function(myValue);
		}
		printf("%d \n\n",myValue);

		sprintf(ptr,"%d",myValue);
		exit(-1);
	}else{//parent
		wait(NULL);
		shm_fd = shm_open(name, O_RDONLY, 0666);
		if (shm_fd == -1) {
			printf("shared memory failed\n");
			exit(-1);
		}
		/* now map the shared memory segment in the address space of the process */
		ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
		if (ptr == MAP_FAILED) {
			printf("Map failed\n");
			exit(-1);
		}
		printf("Parent process is reading...from shm\n");

		int myStepValues=atoi(ptr);
		printf("%s ",ptr);
		while ( myStepValues!=1 ){
			ptr += sizeof(int);
			printf("%s ",ptr);
			myStepValues=atoi(ptr);
		}
		printf("\n\n ");
		if (shm_unlink(name) == -1) {
			printf("Error removing %s\n",name);
			exit(-1);
		}

	}
	return 0;




}
// implements the funciton that defined in the assigment
int function(int number){
	int test=number%2;
	if( test==0 ){//number is even 
		return number/2;
	}else{//number is odd
		return (number*3)+1;
	}
}