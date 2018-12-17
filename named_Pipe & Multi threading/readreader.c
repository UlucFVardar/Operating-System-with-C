#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
//int str cevrim
char *intstr(int number) {
    int count=0;
    int numbertmp=number;
    while(numbertmp>0)
    {
        int digit= numbertmp%10;
        numbertmp/=10;
        count++;
    }
    char * value= (char * ) calloc (count,sizeof(unsigned char)  );
    numbertmp=number;
    count--;
    while(numbertmp>0)
    {
        int digit= numbertmp%10;
        numbertmp/=10;
        char x=(char) 48+digit;
        *(value+count)=x;
        count--;
    }

    return value;
}	

char* read_mess_from_pipe(){
	int fd1;
    char * myfifo = "/tmp/myfifo3";
    mkfifo(myfifo, 0666);
    fd1 = open(myfifo,O_RDONLY);

    char str1[80];
    read(fd1, str1, 80);
    return str1;
}

int read_from_file(int lineNumber){
   int num;
   FILE *fptr;
	if ((   fptr = fopen("./numbers.txt","r")) == NULL){
       printf("Error! opening file");
       exit(1);
   }   
   int i;
   for (i =0 ; i< lineNumber ; i++){
   		fscanf(fptr,"%d\n", &num);	
   }
   fclose(fptr);   
   return num; 

}
void send_number(int file_number){
	int fd1;
    char * myfifo = "/tmp/myfifo3";
    mkfifo(myfifo, 0666);

    fd1 = open(myfifo,O_WRONLY);
	char* str2 = intstr(file_number);
    write(fd1, str2, strlen(str2)+1);
    close(fd1);
}

int main(int argc, char *argv[])
{
	printf("Basladi\n");
	while (1){ // diger bir terminalde daima acık kalacak sanki bir server gibi gelen requestleri karşılayacak
		char* mess = read_mess_from_pipe();
		char Type_of_value = mess[0];   // Biliyorumki ilk indexte hangi tipte okuma yapmak istedigim var.
		int lineNumber = atoi((mess+1));// geri kalan indexlerde ise line numarası bulunuyor onuda int e cevirdim ettim.
		//printf("Gelen mesaj : Type: %c ---- lineNumber : %d\n",Type_of_value,lineNumber );
		int file_number = read_from_file(lineNumber);	
		//printf("filedaki veri : %d\n", file_number);
		printf("%d. %c = %d\n",lineNumber, Type_of_value,file_number );
		send_number(file_number); // geri gönderme
	}
    return 0;
}

