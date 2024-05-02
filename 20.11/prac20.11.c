#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char **argv){
    int fd1[2];
    int fd2[2];
    pipe(fd1);
    pipe(fd2);
    FILE *f;
    f = fopen("test.txt", "w+");
    if (f == NULL){
        return -1;
    }
    else{
        pid_t process;
        process = fork();
        if (process < 0) return -1;
        else if (!process){ // son
            close(fd1[0]);
            close(fd2[1]);
            char str[80];
            int temp = 0;
            int n;
            printf("Enter the number\n");
            scanf("%d", &n);
            printf("Enter the string\n");
            getchar();
            fgets(str, 80, stdin);
            while (strcmp(str, "exit\n")){
                fputs(str, f);
                fflush(f);
                write(fd1[1], &n, sizeof(int));
                write(fd1[1], str, sizeof(str));
                //printf("check\n");
                read(fd2[0], &temp, sizeof(int));
                printf("Enter the number\n");
                scanf("%d", &n);
                printf("Enter the string\n");
                getchar();
                fgets(str, 80, stdin);
                //write(&f, str, 80);
                //write(fd1[1], &sym, sizeof(char));
            }
            //fclose(f);
           // printf("I am here\n");
            close(fd1[1]);
            close(fd2[0]);
            fclose(f);
            exit(0);
        }
        else{  // father
            close(fd1[1]);
            close(fd2[0]);
            int x;
            int temp = 0;
            char sstr[80];
            char sym;
            char scheck[80];
            read(fd1[0], &x, sizeof(int));
            read(fd1[0], sstr, sizeof(sstr));
            strcpy(scheck,sstr);
           // printf("str = %s", sstr);
           // printf("x = %d\n", x);
            int counter = 0;
            counter = strlen(sstr) - 1;
           // printf("counter = %d\n", counter);
            int i = 0;
            while (strcmp(sstr, "exit\n")){
                int m = counter/2;
                if (x>=counter){
                    while (sstr[i] != '\n'){
                        sstr[i] = '*';
                        i++;
                    } 
                    i = 0;
                    //printf("s1 = %s", sstr);
                }
                else{
                    //printf("s2 = %s", sstr);
                    //printf("m = %d\n", m);
                   // printf("x = %d\n", x);
                    int c = 0;
                    while (c < x){
                        if (m-i>=0 && c < x){
                            sstr[m-i] = '*';
                            c++;
                        }
                        if (m+i<counter && c < x && i!=0){
                            sstr[m+i] = '*';
                            c++;
                        }
                        i++;
                    }
                    i = 0;
                }
               // printf("sstr = %s", sstr);
                //printf("scheck = %s", scheck);
                if (strcmp(sstr, scheck)){
                    fputs(sstr, f);
                    fflush(f);
                }
                write(fd2[1], &temp, sizeof(int));
                strcpy(scheck,sstr);
                read(fd1[0], &x, sizeof(int));
                read(fd1[0], sstr, 80);
                counter = strlen(sstr) - 1;
            }
            close(fd1[0]);
            close(fd2[1]);
            fclose(f);
        }
    }
    return 0;
}