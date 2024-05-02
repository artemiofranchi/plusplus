#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int rread(FILE *f){
	char c = 'a';
    	int i = 0;
    	while (c != EOF) {
       	    c = fgetc(f);
            while ((c != EOF) && (c != '\n')) c = fgetc(f);
            i++;
    	}
    	fseek(f, 0, SEEK_SET);
    	return i;
}


void myuniq(int argc, char **argv, char *name, ...){
    FILE *f;
    int c = 0,d = 0,u = 0,s = 0;
    f = fopen(argv[1], "r+");
    if (argc > 2){
        for (int i = 2; i<argc; i++){
            if (s == 1){
                s = atoi(argv[argc-1]);
                break;
            }
            if (argv[i][1] == 'c'){
                c = 1;
            }
            if (argv[i][1] == 'd'){
                d = 1;
            }
            if (argv[i][1] == 'u'){
                u = 1;
            }
            if (argv[i][1] == 's'){
                s = 1;
            }
        }
    }
    //else if (argc == 2){
    //    printf("No options\n");
    //}
    int count = rread(f);
	long mas[count];
	int leng[count];
	int len = 0;
	int i = 0;
    char sym = 'a';
    char sym1 = 'a';
    char sym2 = 'a';
	while (sym!=EOF){
		mas[i] = ftell(f);
		len = 0;
		sym = fgetc(f);
		while (sym!=EOF && sym!='\n'){
			len++;
			sym = fgetc(f);
		}
        len++;
		leng[i] = len;
		i++;
	}
    int max = 0;
    for (int k = 0; k<count-1; k++){
        if (leng[k]>max){
            max = leng[k];
        }
    }
    char cst[max], cst1[max], cst2[max];
    char *str1, *str2;
    int rep[count];
    int n = 0;
    int check = 0;
    int same = 0;
    int ndelta = 0;
    //printf("s = %d\n", s);
    //printf("count = %d\n", count);
    while ((n+1) != count){
		fseek(f, mas[n], SEEK_SET);
        rep[n] = 1;
        ndelta = same;
        //printf("len1 =%d, len2 = %d, ndelta = %d\n", leng[n], leng[n+1], ndelta);
        if ((n+1) != count - 1){
            //printf("s = %d, leng[n] - 1 = %d, leng[n+1] - 1 = %d\n", s, leng[n]-1, leng[n+1]-1);
            if (s>=(leng[n]-1) && s>=(leng[n+1]-1) && same == 0){
                same = 1;
                //printf("same = %d\n", same);
            }
            else if (s>=(leng[n]-1) && s>=(leng[n+1]-1) && same != 0){
                same += 1;
                //printf("same = %d\n", same);
            }
            else if (s<leng[n]-1 && s<leng[n+1]-1){
                int myc = 0;
                for (long j = s; j<leng[n]; j++){
                    fseek(f, mas[n]+j, SEEK_SET);
                    sym1 = fgetc(f);
                    fseek(f, mas[n+1]+j, SEEK_SET);
                    sym2 = fgetc(f);
                    if (sym1!=sym2){
                        same = 0;
                        break;
                    }
                    myc+=1;
                }
                if (myc == leng[n]-s){
                    same += 1;
                }
            }
            else if (s<leng[n]-1 || s<leng[n+1]-1){
                same = 0;
            }
            //printf("same = %d\n", same);
            if (s>=max-1){
                rep[n] = count - 1;
            }
            else if (s>=(leng[n]-1) && s>=(leng[n+1]-1)){
                fseek(f,mas[n+1]+s,SEEK_SET);
                if (fgets(cst,sizeof(cst),f) == NULL){
                    rep[n] += 1;
                }
                else{
                    int ind = 0;
                    fseek(f,mas[n+ind]+s,SEEK_SET);
                    while (s>=leng[n]-1 && s>=leng[n+1+ind]-1 && (n+1+ind)<count-1){
                        rep[n] += 1;
                        ind++;
                    }
                }
                fseek(f, mas[n], SEEK_SET);
            }
            else{
                int indd = 1;
                fseek(f,mas[n]+s,SEEK_SET);
                str1 = fgets(cst1,sizeof(cst1),f);
                fseek(f,mas[n+1]+s,SEEK_SET);
                str2 = fgets(cst2,sizeof(cst2),f);
                while (str2!=NULL){
                    if (strcmp(str1,str2) == 0){
                        rep[n]+=1;
                        fseek(f,mas[n+1+indd]+s,SEEK_SET);
                        str2 = fgets(cst2,sizeof(cst2),f);
                        indd++;
                    }
    	            else break;
                }
                //else rep[n]+=1;
                fseek(f, mas[n], SEEK_SET);
            }
        }
        //printf("rep[%d] = %d\n", n, rep[n]);
        if (same == 0 && ndelta == 0){
            if (d == 0 && u == 0){
                if (c==1) printf("%7d ", rep[n]);
                fseek(f,mas[n],SEEK_SET);
                for (long j = 0; j<leng[n]; j++){
                    sym = fgetc(f);
                    if(sym!=EOF){
                        printf("%c", sym);
                    }
                }
                fseek(f,mas[n],SEEK_SET);
            }
            else if (d == 0 && u == 1 && rep[n] == 1){
                if (c==1) printf("%7d ", rep[n]);
                fseek(f,mas[n],SEEK_SET);
                for (long j = 0; j<leng[n]; j++){
                    sym = fgetc(f);
                    if(sym!=EOF){
                        printf("%c", sym);
                    }
                }
                fseek(f,mas[n],SEEK_SET);
            }
        }
        if (same == 1 && ndelta == 0){
            if (d == 1 && u == 0 && rep[n] > 1){
                if (c==1) printf("%7d ", rep[n]);
                fseek(f,mas[n-ndelta],SEEK_SET);
                for (long j = 0; j<leng[n-ndelta]; j++){
                    sym = fgetc(f);
                    if(sym!=EOF){
                        printf("%c", sym);
                    }
                }
            }
            else if (d == 0 && u == 0){
                if (c==1) printf("%7d ", rep[n]);
                fseek(f,mas[n],SEEK_SET);
                for (long j = 0; j<leng[n]; j++){
                    sym = fgetc(f);
                    if(sym!=EOF){
                        printf("%c", sym);
                    }
                }
            }
        }
        n++;
    }
   // printf("%d %d %d %d\n", c,d,u,s);
    fclose(f);
}


int main(int argc, char **argv){
    if (argc == 2){
        myuniq(argc, argv, argv[1]);
    }
    if (argc == 3){
        myuniq(argc, argv, argv[1],argv[2]);
    }
    if (argc == 4){
        myuniq(argc, argv, argv[1],argv[2], argv[3]);
    }
    if (argc == 5){
        myuniq(argc, argv, argv[1],argv[2],argv[3],argv[4]);
    }
    if (argc == 6){
        myuniq(argc, argv, argv[1],argv[2],argv[3],argv[4], argv[5]);
    }
    if (argc == 7){
        myuniq(argc, argv, argv[1],argv[2],argv[3],argv[4],argv[5],argv[6]);
    }
    if (argc == 8){
        myuniq(argc, argv, argv[1],argv[2],argv[3],argv[4],argv[5],argv[6], argv[7]);
    }
    return 0;
}