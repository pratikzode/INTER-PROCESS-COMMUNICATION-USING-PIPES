#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
int main()
{
	pid_t process_id;
	int pipe1[2],pipe2[2], count, i=0;
	char filename1[20]="\0",filename[20]="\0", buffer[1000]="\0" ,buffer2[1000]="\0" ;
	FILE *fileptr;
	pipe(pipe1);
	pipe(pipe2);
	
	process_id=fork();
	
	if(process_id<0)
	{
		printf("Error in fork System Call\n");
	}
	
	else if(process_id==0)
	{
			//child process
			sleep(10);
			printf("Control goes to Child Process\n");
			close(pipe1[1]);			
			count=read(pipe1[0], filename1, sizeof(filename1));
			close(pipe1[0]);		
			printf("Filename is: %s\n", filename1);
			fileptr=fopen(filename1, "r");
			buffer[0]=fgetc(fileptr);	
			while(!feof(fileptr))
			{
				buffer[++i]=fgetc(fileptr);
			}
			buffer[strlen(buffer)-1]= '\0';	
			fclose(fileptr);			
			printf("Copying data from file....\n");
			
			close(pipe2[0]);			
			write(pipe2[1], buffer, sizeof(buffer));
			close(pipe2[1]);			
			printf("All data Copied.\n");			
	}
	else
	{
			//parent process
			printf("Control goes to parent process.\n");
			printf("Enter the file name:\n");
			scanf("%s", filename);

			if(!(fopen(filename,"r")))
			{
				printf("File opening error.\n");
				return 0;
			}		
			
			close(pipe1[0]);			
			write(pipe1[1], filename, sizeof(filename));
			close(pipe1[1]);			
			
			sleep(10);
			printf("Control comes to parent process again.\n");
			
			close(pipe2[1]);
			read(pipe2[0], buffer2, sizeof(buffer2));
			close(pipe2[0]);

			printf("File Data:\n%s", buffer2);
	}
	return 0;
}
