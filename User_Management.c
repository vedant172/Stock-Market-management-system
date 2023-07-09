#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#define max 1000
#define size 500
pid_t pid[1000];
char username[max];

/*
int curr;
void Login(){
    char username[max];
    char Password[max];
    int true1 = 1;
    int tries = 0;
    int correct = 0;
    int i = 0;
    do{
        printf("Username: ");
        scanf("%s",username);
        printf("Password: ");
        scanf("%s",Password);
        FILE* file1 = fopen("UserNameAndPassword.txt","r");
        char temp1[max],temp2[max];
        while (!feof(file1))
        {
            fscanf(file1,"%s %s",temp1,temp2);
            if(strcmp(temp1,username)==0 && strcmp(temp2,Password) == 0){
                correct = 1;
            }
        }
        fclose(file1);
        if(correct == 1){
            int temp;
            if((temp = fork())==0){
                system("xterm -hold -e './client.out localhost 1500'");
            }
            else{
                pid[curr++] = temp;
                return;
            }
        }
        else{
            tries++;
            printf("NO. of attempts remain: %d\n",3-tries);
            if(tries >= 3){
                printf("You have crossed the limit of entering wrong password!\n");
                break;
            }
        }
    }while(tries<3);

}
*/

int check_user_name(char user_name[])
{
        char temp1[max],temp2[max];
        FILE* file1 = fopen("UserNameAndPassword.txt","r");

	while (!feof(file1))
        {
            fscanf(file1,"%s %s",temp1,temp2);
            if(strcmp(temp1,user_name) == 0){
                return 0;
               // break;
            }
        }
     return 1;

}

void set_pass(char pass[])
{
       char newLine[1000],buf[size];
	FILE* tmp = fopen("temp1.txt","w");
	FILE* fp= fopen("UserNameAndPassword.txt","r+");
       // fprintf(file1,"%s %s\n",username,pass);
         // sprintf(newLine,"%s %s\n",username,pass);
        //  fputs(newLine,file);

        while(fgets(buf,size,fp) != NULL)
       {


            fputs(buf,tmp);

      }

    fclose(fp);
    fclose(tmp);
    sprintf(newLine,"%s %s\n",username,pass);
    fputs(newLine,tmp);
    remove("UserNameAndPassword.txt");
    rename("temp1.txt","UserNameAndPassword.txt");


}

int check_pass(char pass[])
{
	char temp1[max],temp2[max];
        FILE* file1 = fopen("UserNameAndPassword.txt","r");

	while (!feof(file1))
        {
            fscanf(file1,"%s %s",temp1,temp2);
            if(strcmp(temp1,username) == 0)
            {
                if(strcmp(temp2,pass)==0)
                {
                 	 return 1;

                }
                else return 0;

            }
        }
     return 0;

}

/*
void signUp()
{
    //char username[max],pass[max],confPass[max];
    int a;

   int  check_user_name_key = check_user_name(username);
   if( check_user_name==0) return 0;

abc:
   // printf("Enter username without spaces: ");
   // scanf("%s",username);
    printf("\n");
    printf("Enter password: ");
    scanf("%s",pass);
    printf("\n");

    printf("Confirm Password: ");
    scanf("%s",confPass);
    printf("\n");

    if (strcmp(pass,confPass) == 0)
    {
        char temp1[max],temp2[max];
        FILE* file1 = fopen("UserNameAndPassword.txt","r");
        while (!feof(file1))
        {
            fscanf(file1,"%s %s",temp1,temp2);
            if(strcmp(temp1,username) == 0){
                printf("Username already exist!!\nTry logging in\n");
                break;
            }
        }
        fclose(file1);
        file1 = fopen("UserNameAndPassword.txt","a");
        fprintf(file1,"%s %s\n",username,pass);
        fclose(file1);
        printf("User created successfully\n");
        printf("Press 1 to login: ");
        scanf("%d",&a);
        if (a == 1)
        {
            Login();
        }
    }
    else
    {
        printf("Password doesn't match\n");
        printf("Press 0 to try again\n");
        scanf("%d",&a);
        if (a == 0)
        {
            goto abc;
        }
    }
}

void USER(int temp){
    //int temp;
    int status;
    system("cls");

   // while(1){
       // printf("1.For signUp\n2.For login\n3.To exit\n");
      //  printf("Enter option: ");
        scanf("%d",&temp);
        switch(temp){
            case 1:
                signUp();
                break;
            case 2:
                Login();
                break;
            case 3:
                goto abe;
                break;
        }
   // }
abe:
    for(int i = 0; i<curr; i++){
        kill(pid[i],SIGKILL);
    }
    return 0;
}
*/
