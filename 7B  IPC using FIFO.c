PROCESS 1:
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#define pipe1 "pipe1"
#define pipe2 "pipe2"

int main()
{
	char str[100]="\0", str1[100]="\0";
	int i=0, fd;
	printf("Enter Data:\n");
	str[i]=getchar();
	while(str[i]!='#')
	{
		i++;
		str[i]=getchar();
	}
	str[strlen(str)-1]= '\0';
	printf("\nData entered successfully!!\n\nData in File: %s", str);
	printf("\nTransferring Data..\n");
	
	mkfifo(pipe1, 0666);
	fd=open(pipe1,O_WRONLY);
	write(fd, str, sizeof(str));
	close(fd);
	
	printf("\nData Transferred\n");
	printf("\nReceiving Data..\n");
	fd=open(pipe2, O_RDONLY);
	read(fd, str1 , sizeof(str1));
	close(fd);
	printf("\nData Received!!\n");
	printf("Data of File: %s", str1);
	return 0;
}
PROCESS 2:
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#define pipe1 "pipe1"
#define pipe2 "pipe2"

int main()
{
	char str[100]="\0", fName[20], ch[100]="\0";
	int fd, i, wordCount=0, lineCount=0, charCount=0;
	FILE *fp;
	printf("\nReceiving Data..");
	fd=open(pipe1, O_RDONLY);
	read(fd, str, sizeof(str));
	close(fd);
	
	printf("\nData Received: \n%s", str);
	printf("\nCounting Character, Words and Lines..\n");
	for(i=0;i<strlen(str); i++)
	{
		if(str[i]==' ' || str[i]=='\n')
			wordCount++;
	
		if(str[i]=='\n')
			lineCount++;
	
		if(str[i]!='\n' && str[i]!=' ')
			charCount++;
	}
	
	printf("\nEnter the File Name(with extension): ");
	scanf("%s", fName);
	//printf("\nNumber of:\nCharacters: %d\nWords: %d\nLines: %d\n",charCount, wordCount, lineCount);
	printf("\nWriting Data to File..\n");
	fp=fopen(fName, "w");
	fprintf(fp,"Charater Count: %d\nWord Count: %d\nLine Count: %d\n", charCount, wordCount,lineCount);
	fclose(fp);
	
	printf("\nRetrieving Data From File..\n");
	fp=fopen(fName, "r");
	i=0;
	ch[0]=fgetc(fp);	
	while(!feof(fp))
	{
		i++;
		ch[i]=fgetc(fp);
	}
	//ch[i]='\0';
	ch[strlen(ch)-1]= '\0';	
	fclose(fp);
	//printf("Retrieved Data: %s", ch);
	printf("\nTransferring Data..\n");
	
	mkfifo(pipe2, 0666);
	fd=open(pipe2, O_WRONLY);
	write(fd, ch, sizeof(ch));
	close(fd);
	
	printf("\nData Transferred!!\n");	
	return 0;
}
