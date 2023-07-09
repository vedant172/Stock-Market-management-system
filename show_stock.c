
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>


pthread_t threadID[2];
char key='N';
int childPID;
sem_t semaphore;

extern char stock_name[100];
extern int price;
extern int quan;

struct file_read
{
   char *file_name;
   int start_pos;
   int end_pos;
   char *stock_name;
};



void *reading(void *arg)
{


   struct file_read *temp = (struct file_read*)arg;
   FILE *file = fopen(temp->file_name,"r+");
    FILE *tmp = fopen(temp->file_name,"r+");

   fseek(file,temp->start_pos,SEEK_SET);
   int temp_price,temp_quan;
    char line[256];


   for(int i=temp->start_pos; i<=temp->end_pos; i++)
   {

      fscanf(file,"%s %d %d ",line,&temp_price,&temp_quan);


      if(strcmp(line,temp->stock_name)==0)
      {

         key = 'Y';

         price = temp_price;
         quan = temp_quan;


          sem_wait(&semaphore);
         kill(childPID,SIGUSR1);// add semaphore
         sem_post(&semaphore);
      }


   }
}

static void stop_thread()
{
   for(int i=0; i<2; i++)
   {
      pthread_cancel(threadID[i]);
   }
}

void SHOW()
{
	sem_init(&semaphore,0,1);

        if( signal( SIGUSR1,stop_thread) == SIG_ERR  )
        {
        	printf("Pærent: Unable to create handler for SIGUSR1\n");
        }

		FILE *stock_list_file = fopen("stock_list.txt","r");
		char line[256];
		int err;

		struct file_read temp_read[2];
		FILE *file1 = fopen("stock_list.txt","r");
    		fseek(file1,0L,SEEK_END);
	        int byte = ftell(file1) / 2;
	        int mod_byte = ftell(file1) % 2;

		for(int i=0; i<2; i++)
		{
		    temp_read[i].file_name  =  "stock_list.txt";
		    temp_read[i].stock_name = stock_name;
		    if(i==0)temp_read[i].start_pos =  0;
		    else temp_read[i].start_pos =  temp_read[i-1].end_pos + 1;

		     temp_read[i].end_pos =   temp_read[i].start_pos + byte;
		    if(mod_byte>0)
		    {
		        temp_read[i].end_pos++;
		        mod_byte--;
		    }

		  //  printf("start pos: %d  end_pos: %d\n",temp_read[i].start_pos,temp_read[i].end_pos);

		    pthread_create(&threadID[i],NULL,reading,(void*)&temp_read[i]);
		}

		for(int i=0; i<2; i++)
		{
		   pthread_join(threadID[i],NULL);
		}




}
