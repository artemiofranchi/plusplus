if (leng[n] != leng[n+1]){
            rep[n] = 1;
            if (same != 0){
                rep[n] += same;
            }
            ndelta = same;
            
            if (s != 0 && s>=leng[n] && s>=leng[n+1]){
                same += 1;
            }
            else{
                if (c == 1){
                    int ind = 1;
                    fseek(f,mas[n]+s,SEEK_SET);
                    str1 = fgets(cst1,sizeof(cst1),f);
                    fseek(f,mas[n+1]+s,SEEK_SET);
                    str2 = fgets(cst2,sizeof(cst2),f);
                    // printf("str1 = %s", str1);
                    // printf("str2 = %s", str2);
                    while (str2!=NULL){
                        if (strcmp(str1,str2) == 0){
                            rep[n]+=1;
                            fseek(f,mas[n+1+ind]+s,SEEK_SET);
                            str2 = fgets(cst2,sizeof(cst2),f);
                            ind++;
                        }
    	                else break;
                    }
                    ind = 1;
                    fseek(f, mas[n], SEEK_SET);
                }
                same = 0;
            }
            printf("same = %d\n", same);
            if (d == 1 && rep[n]>1 && u == 0){
                if (c==1) printf("\t%d ", rep[n]);
                fseek(f,mas[n-ndelta],SEEK_SET);
                for (long j = 0; j<leng[n-ndelta]; j++){
                    sym = fgetc(f);
                    if(sym!=EOF){
                        printf("%c", sym);
                    }
                }
                fseek(f,mas[n],SEEK_SET);
            }
            else if (d == 0 && u == 0){
                if (c==1) printf("\t%d ", rep[n]);
                if (s!=0){
                    if (same == 0){
                        fseek(f,mas[n-ndelta],SEEK_SET);
                        for (long j = 0; j<leng[n-ndelta]; j++){
                            sym = fgetc(f);
                            if(sym!=EOF){
                                printf("%c", sym);
                            }
                        }
                        fseek(f,mas[n-ndelta],SEEK_SET);
                    }
                }
                fseek(f,mas[n-ndelta],SEEK_SET);
                for (long j = 0; j<leng[n-ndelta]; j++){
                    sym = fgetc(f);
                    if(sym!=EOF){
                        printf("%c", sym);
                    }
                }
                fseek(f,mas[n-ndelta],SEEK_SET);
            }
            else if (d == 0 && u == 1 && rep[n] == 1){
                if (c==1) printf("\t%d ", rep[n]);
                for (long j = 0; j<leng[n]; j++){
                    sym = fgetc(f);
                    if(sym!=EOF){
                        printf("%c", sym);
                    }
                }
            }

        }