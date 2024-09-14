#include <stdio.h>
#include<string.h>
int main()
{   
    char username[50];
    char entered_password[20];
    int count=0;

    printf("Enter username: ");
    scanf("%s", username);

    FILE *ptr = fopen("masterfile.txt","r");
    char m_username[50];
    char password_file_name[100]; 
    char pfile_name[100];
    char date[50];

    while(fscanf(ptr,"%s %s %s",m_username,date,password_file_name)!=EOF)
    {
        if(strcmp(username,m_username)==0)
        {
            printf("%s",password_file_name);
            strcpy(pfile_name,password_file_name);
            printf("Enter password: ");
            scanf("%s",entered_password);
            break;
        }
    }
    
    FILE *p=fopen(pfile_name,"r");
    char latest_password[20];
    fscanf(p,"%s",latest_password);
    printf("%s %s", latest_password, entered_password);
    if(strcmp(latest_password,entered_password)==0)
    {
        return 0;
    }
    else if(strcmp(latest_password,entered_password)!=0 && count <3)
    {
        printf("Wrong password! Enter password again:");
        scanf("%s",entered_password);
         if(strcmp(latest_password,entered_password)==0)
        {
            return 0;
        }
        count++;
    }

    if(count==3)
    {
        printf("Wrong password entered 3 times. Application exiting...");
        return 1;
    }
}

