#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> 
#include <pwd.h>

pid_t pid;
char line[80];  //get command line
char* argv[10];        //user command
int argc;               //arg count
int spaceCount = 0;
char* token;                  //split command into separate strings
static char cwd[1000];
int pidDizisi[5];
int pidSirasi=0;
void print_prompt(void)
{
	srand(time(NULL));
	int random = rand() % 5 + 1;
	switch(random){
		case 1:
			printf("\033[1;32m");
			break;
		case 2:
			printf("\033[0;33m");
			break;
		case 3:
			printf("\033[0;34m");
			break;
		case 4:
			printf("\033[0;35m");
			break;
		case 5:
			printf("\033[0;36m");
			break;
	}
	
	getcwd(cwd, sizeof(cwd));
	printf("%s/: sau > ", cwd );
	printf("\033[0m");
	
}

int main() {

	while (1) {
		print_prompt();
		
		if (!fgets(line, 80, stdin)) {  //get command and put it in line
			break;                      //if user hits CTRL+D break
		}
		
		size_t length = strlen(line);
		
		if (line[length - 1] == '\n')
			line[length - 1] = '\0';
			
		if (line[length - 2] == ' '){
			printf("Hata: Komutun sonunda boşluk olmamalıdır.\n");
			continue;
		}
			
		if (line[0] == ' ') {
			printf("Hata: Komutun başında boşluk olmamalıdır.\n");
			continue;
		}

		if (strstr(line, "  ") != NULL) {
			printf("Hata: Komutta çift boşluk olmamalıdır.\n");
			continue;
		}
		
		spaceCount = 0;
		token = strtok(line, " ");
		int i = 0;
		while (token != NULL && spaceCount < 10) {

			argv[i] = token;
			if (strlen(token) > 80) {
				fprintf(stderr,"Hata: Komut max 80 karakter olmalıdır.\n");
				break;
			}
			token = strtok(NULL, " ");
			spaceCount++;
			i++;
			
		}
		argv[i] = NULL; //set last value to NULL for execvp
		argc = i; 	//get arg count
		
		
		if (strcmp(argv[0], "exit") == 0) {            //check if command is exit
			fprintf(stderr,"exit\n");
			exit(0);
		}
		
		else if(strcmp(argv[0], "cd")  == 0){ //dosyalar arasinda gezinme
			chdir(argv[1]);
			getcwd(cwd, sizeof(cwd));
			continue;
		}
		
		else if(strcmp(argv[0], "showpid")  == 0){ //dosyalar arasinda gezinme
			for(int i=0; i<5; i++){
				printf("%d\n", pidDizisi[i]);
			}
			continue;
		}
}