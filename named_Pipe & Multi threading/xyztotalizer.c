#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>


int linenum ;
int readcount ; // ana programa pametre oalk gelecek ve 3 ün katı olacak 
int temp_sum  ;
pthread_mutex_t Read_File;


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

// Basit kullanım --> int value = send_and_take_value(5,"Y");
int send_and_take_value(int line_number, char* value){
    //-- named pipı hazırlama 
    int npipefd;
    char * myfifo = "/tmp/myfifo3";
    mkfifo(myfifo, 0666);
    //------------------------------------------------
    //## PIPE A KOYDUĞUMUZ MESAJIN FORMATI "VALUETYPE-LINENUMBER"   
    //##                                   EX:  "Y3"   
    //------------------------------------------------


    //--- ınput verisi birleştirilmiş strig haline geliyor 
    char *mess = (char*)calloc(10, sizeof(char));    
    char *line_number_str = (char*)calloc(10, sizeof(char));    
    line_number_str = intstr(line_number);
    strcat(mess, value);
    strcat(mess, line_number_str); // mess in icinde X3 gibi degerler bulunuyor 
    //------------------------------------------------

    //-- Pıpe uzerinden veriyi gönderme
    npipefd = open(myfifo, O_WRONLY);
    write(npipefd, mess, sizeof(mess));
    close(npipefd);   
    //------------------------------------------------     

    
    //--geri alma
    char *file_number=(char*)calloc(10, sizeof(char));
    //i kare degerini pipedan oku
    npipefd = open(myfifo, O_RDONLY);
    read(npipefd, file_number, sizeof(file_number));
    close(npipefd);
    
    return atoi(file_number);
 }


void thread_run(char *type){
    pthread_mutex_lock(&Read_File);    // kilitle ki diğer threadler araya giremesin 
    int value = send_and_take_value(linenum +1,type);  // yazmıs oldugumuz fonksiyonla diger koddan veriyi alalım
    printf("%s = %d\n", type, value ); //gelen veri
    linenum = linenum +1; // global degiskenlerin updatei
    temp_sum += value ;
    pthread_mutex_unlock(&Read_File);     // kilidi bırak kiritik alan bitti
    pthread_exit(0); 
}


int main(int argc,char *argv[])
{
    srand(time(0));

    // x + y + z 
    pthread_t thread_X;    
    pthread_t thread_Y;    
    pthread_t thread_Z;    



    readcount = atoi(argv[1]);  // kac iterasyon yapacağını görmek icin gerekli olan sey parametre olarak geliyor
    // ve ınteger olarak cevrilerek kullanılıyor
    linenum =0 ;
    temp_sum = 0;

    if (pthread_mutex_init(&Read_File, NULL) != 0) // mutexın oluşturulması
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    int i;
    for (i =0 ; i< (readcount / 3) ; i++ ){  // oyun kac defa oynanacak ise o kadar defa 3 thread uyandırılıyor ve sonuc hesaplanıyor 
        pthread_create(&thread_X,NULL,(void *)&thread_run,"X");
        pthread_create(&thread_Y,NULL,(void *)&thread_run,"Y");
        pthread_create(&thread_Z,NULL,(void *)&thread_run,"Z");

        pthread_join(thread_Z,NULL);
        pthread_join(thread_X,NULL);
        pthread_join(thread_Y,NULL);
        printf(" x + y + z = %d\n-----------------------\n\n", temp_sum);
        temp_sum = 0;
    }


    return 0;
  }

