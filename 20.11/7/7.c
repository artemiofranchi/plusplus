#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int recurs(int a, FILE *text){
	int p = a % 10;
	char c = (char)p+ '0'; // символьное представление числа
	a = a / 10;
	//printf("%d\n", a);
	if (a!=0){
		recurs(a,text);
	}
	fputc(c,text);
}

int main(int argc, char **argv){
	FILE *bin, *text;
	struct stat buff1;
	struct stat buff2;
	bin = fopen(argv[argc-2],"r+");
	text = fopen(argv[argc-1], "w");
	if (bin==NULL){
		printf("File opening error\n");
		return 0;
	}
	if (text==NULL){
		printf("File opening error\n");
		return 0;
	}
	stat(argv[argc-1], &buff1);
	stat(argv[argc-2], &buff2); // st_ino - это серийный номер файла, который отличает этот файл от всех других файлов на том же устройстве.
	if (buff1.st_ino == buff2.st_ino){ // указывают ли оба имени на один и тот же файл
		printf("ERROR. The names are pointing at the same file!");
		return 0;
	}
	int a, b, count = 0;
	while ((fread(&a, sizeof(int), 1, bin)) != 0){ // ищем количество элементов файла
		count++;
	}
	//printf("%d\n", count);
	for (int i = count*4; i > 0; i-=4){ // сортировка элементов файла
		for (int j = 0; j < i; j+=4){
			fseek(bin,j,SEEK_SET);
			fread(&a, sizeof(int), 1, bin);
			fread(&b, sizeof(int), 1, bin);
			if (b<a && (j+4)<i){
				fseek(bin,j,SEEK_SET);
				fwrite(&b,sizeof(int),1,bin);
				fseek(bin,j+4,SEEK_SET);
				fwrite(&a,sizeof(int),1,bin);
			}
		}
	}
	fseek(bin,0,SEEK_SET);
	while (fread(&a,sizeof(int),1,bin) != 0){ // заносим данные в текстовый файл
		if (a==0){
			fputc('0',text);
		}
		else{
			if (a<0){
				fputc('-',text);
				a = -a;
			}
			char c;
			recurs(a,text); //рекурсивно доходим до первого разряда в числе, печатаем, далее возврат ко второму, ...
            	}
            	fputc(' ', text);
	}
	fclose(bin);
	fclose(text);
	return 0;
}
				 
			

			
	
		
