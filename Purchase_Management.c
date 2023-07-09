/* Buying portal for customer */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include<pthread.h>
#include <sys/wait.h>
#include "common.h"
#define size 500
struct st{
    FILE* fp;
    int ln;
    char* newLine;
};

void* replaceLine(void *arg){
    struct st* a = (struct st*)arg;
    rewind(a->fp);
    FILE *tmp = fopen("tmp.txt","w");
    char buf[size];
    int cnt = 0;

    while(fgets(buf,size,a->fp) != NULL)
    {
        cnt++;
        if(cnt == a->ln){
            fputs(a->newLine,tmp);
        }
        else{
            fputs(buf,tmp);
        }
    }

    fclose(a->fp);
    fclose(tmp);
    remove("stock_list.txt");
    rename("tmp.txt","stock_list.txt");
}

void BUY(int temp){

    FILE* fp = NULL;
    char name[size],buf[size],newLine[size];
    int qty,Price;
    fp = fopen("stock_list.txt","r+");

    if(fp == NULL){
        printf("Service unavailable\n");
        return ;
    }

    //printf("Enter name of stock that you want to buy: ");
    //scanf("%s",name);
    strcpy(name,stock_name);
    int cnt = 0;
    int fl = 0;

    while(!feof(fp)){

        fscanf(fp,"%s %d %d",buf,&Price,&qty);
        cnt++;
        if(strcmp(buf,name) == 0)
        {
            fl = 1;
           // printf("Price of stock of %s is: %d and Quantity available is: %d\n",buf,price,qty);
          //  printf("Enter no. of stocks that you want to buy: ");
          //  scanf("%d",&temp);

                sprintf(newLine,"%s %d %d\n",buf,Price,qty-temp);
                struct st *a = (struct st*)malloc(sizeof(struct st));
                a->newLine = newLine;
                a->ln = cnt;
                a->fp = fp;
                pthread_t tid;
                pthread_create(&tid,NULL,replaceLine,(void*)a); // changing qty of stocks in file using thread
               // printf("Price of share(=%d) * No. of shares alloted(=%d) = Total amount to be paid(=%d)\n",Price,temp,price*temp);
               // printf("Transaction completed.\n");
                pthread_join(tid,NULL); // waiting for updating in file to complete
                break;
        }
    }

    if(!fl){
        printf("No such stock is available\n");
        fclose(fp);
    }
}

