#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <time.h>

char line[80];  
char* argv[10];      
int argc;              
static int spaceCount = 0;
char* ciftBosluk = "  ";
static char cwd[1000];

void print_prompt(void)
{
	printf("\033[1;32m");
	getcwd(cwd, sizeof(cwd));
	printf("%s/: sau > ", cwd );
	printf("\033[0m");
	
}

int main() {

	while (1) {

		print_prompt();
		
		if (!fgets(line, 80, stdin)) { 
			break;                 
		}
		size_t length = strlen(line);
		if (line[length - 1] == '\n')
			line[length - 1] = '\0';
		
		//KOMUTUN SONUNDA BOSLUK OLMAMASINI YAP
		
		
		spaceCount = 0;
		char* token;                
		token = strtok(line, " ");
		int i = 0;
		while (token != NULL && spaceCount < 10) {

			argv[i] = token;
			token = strtok(NULL, " ");
			spaceCount++;
			i++;
			
		}
		if (strcmp(line, "exit") == 0) {         
			printf("exit\n");
			exit(0);
		} 
		
		argv[i] = NULL;                   
		argc = i; 
		            
		for (i = 0; i < argc; i++) {
			printf("%s\n", argv[i]);     
		}
		

	}
}
