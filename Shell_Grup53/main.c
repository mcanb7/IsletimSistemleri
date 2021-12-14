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
#define BUFFER_LEN 1024
static char cwd[1000]; //mevcut çalışma dizini
static int pidSirasi=0;
pid_t pid;
char line[80];      //komut satırı
char* argv[10];     //komut kelimeleri
int argc;           //kelime sayısı
char* token;        //komutu parçalarken kullanılıyor
int pidDizisi[5];   //proses id'lerin saklanacağı dizi
int spaceCount=0;   //boşluk sayısı


void print_prompt(void)
{
	srand(time(NULL));
	int random = rand() % 5 + 1; //1-5 aralığında rastgele bir sayı seçiyor
	switch(random){ //prompt'un yazı rengini, seçilen sayıya göre değiştiriyor
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
	
	getcwd(cwd, sizeof(cwd)); //mevcut çalışma dizinini buluyor.
	printf("%s/: sau > ", cwd ); //promptu yazdırıyor.
	printf("\033[0m"); //yazı rengini default değere döndürüyor.
	
}

int main() {
	
	while (1) {
		print_prompt(); //prompt yazdırılıyor.
		
		if (!fgets(line, BUFFER_LEN, stdin)) {  //komutu line değişkeni içerisine alıyoruz.
			break;                      // kullanıcı ctrl+D 'ye basarsa break çalışıyor.
		}
		
		size_t length = strlen(line); //komutun uzunluğunu buluyor
		
		if (line[length - 1] == '\n') //sonda '/n' karakteri varsa siliyoruz
			line[length - 1] = '\0';
			
			
		if (length-1 > 80) { //komutumuz, (son karakter hariç) 80 karakterden uzunsa hata döndürüyor.
			fprintf(stderr, "Hata: Komut max 80 karakter olmalıdır.\n");
			continue;
		}
			
		else if (line[length - 2] == ' '){ //komutun sonunda boşluk olma durumunu kontrol ediyor.
			fprintf(stderr, "Hata: Komutun sonunda boşluk olmamalıdır.\n");
			continue;
		}
			
		else if (line[0] == ' ') { //komutun başında boşluk olma durumunu kontrol ediyor.
			fprintf(stderr,"Hata: Komutun başında boşluk olmamalıdır.\n");
			continue;
		}

		else if (strstr(line, "  ") != NULL) { //komutta çift boşluk olma durumunu kontrol ediyor.
			fprintf(stderr,"Hata: Komutta çift boşluk olmamalıdır.\n");
			continue;
		}

		spaceCount=0;
		for(int num=0; num<=length; num++){ //boşluk sayısını hesaplar (boşluk konusundaki hatalı girişleri yukarıda kontrol ettik)
			 if(line[num]==' ')
       	 	 	spaceCount++;
   		} 
   		if(spaceCount>9){ //komuttaki boşluk sayısına göre 10 kelimeden fazla olma durumunu kontrol ediyor.
   			fprintf(stderr, "Hata: Komutta en fazla 10 kelime olmalıdır.\n");
			continue;
   		}		
		
		token = strtok(line, " "); 
		int sayac = 0;
		while (token != NULL) { //komut üzerinde parsing işlemi yapıyoruz ve her bölümü argv dizisine atıyoruz.
			argv[sayac] = token;
			token = strtok(NULL, " ");
			sayac++;
			
		}
		argv[sayac] = NULL;  //son değeri NULL yapar
		argc = sayac; 	 //komuttaki kelime sayısını uygun değişkene atar.
		
		if (strcmp(argv[0], "exit") == 0) { //exit komutunu gerçekleştirir.   
			fprintf(stderr,"exit\n");
			exit(0); 	//kabuk prosesi sonlandırır.
		}
		
		else if(strcmp(argv[0], "cd")  == 0){ //cd komutunu gerçekleştirir.
			chdir(argv[1]); //kullanıcının girdiği parametreye göre mevcut dizini değiştirir.
			getcwd(cwd, sizeof(cwd)); //mevcut dizini bulur.
			continue;
		}
		
		else if(strcmp(argv[0], "showpid")  == 0){ 	//showpid komutunu gerçekleştirir.
			for(int i=0; i<5; i++){                 //son 5 child pid'yi yazdırır.
				if(pidDizisi[i]!=0)		// 0 olmayan pid'leri yazdırır.
					printf("%d\n", pidDizisi[i]);
			}
			continue;
		}
	
		pid = fork();
		
		if (pid == 0){
			if (execvp(argv[0], argv) == -1){
				fprintf(stderr, "Hata: Komut icra edilemiyor.\n");
				kill(getpid(), SIGTERM);
			}
		}
		
		else if (pid < 0) //pid sıfırdan küçükse hata döndürür.
			fprintf(stderr, "Hata");
			
		else //zombi proses oluşmaması için child prosesin işleminin bitmesi bekleniyor.
			waitpid(pid,NULL,0);
			
			
		int temp;
		
		if(pidSirasi<5){ 	//pidDizisi dolana kadar pid'leri diziye atar.
				pidDizisi[pidSirasi]=pid;
				pidSirasi++;
		}
		
		//pidDizisi doluysa 0.indisteki pid'yi silecek şekilde değerleri kaydırır ve 4.indise yeni pid'yi yazar.
		else{  	
			for(int k=0; k<4; k++){
				temp=pidDizisi[k+1];
				pidDizisi[k]=temp;
			}
			pidDizisi[4]=pid;
		}
		for (int i = 0; i < argc; ++i)
			wait(NULL);
	}
}
