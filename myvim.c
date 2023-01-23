#include <stdio.h>
#include <direct.h>
#include<unistd.h>
#define debug(x) printf("%s is: %d\n",#x,x)

int num_of_slash(char *address){
    int num = 0;
    for(int i = 0;i<strlen(address);i++){

        if(address[i]=='/'){num++;}
    }
    return num;
}

void createfile(char* add){
    for(int l = 0; l<num_of_slash(add);l++){
        int length = strlen(add);
        char* tmp = (char*)malloc(sizeof(char)*length);
        int ll = l+1;
        for(int i = 0; i < length; i++){
            if(add[i]=='/'){
                ll--;
            }
            if(ll == 0 && l !=0){tmp[i] = '\0'; break;}
            if(ll == 0 && l == 0){tmp[i] = '/'; tmp[i+1] = '\0';break;}
            tmp[i]=add[i];
        }
        if(strcmp(tmp,"root/")!= 0 && l == 0){
            printf("Invalid Address!\n");
            return;
        }
        if(l != 0)mkdir(tmp);
    }
    if(access(add, F_OK ) != -1){
        printf("The file have been created before!\n");
    }
    else{
        FILE* file = fopen(add,"a+");
        fclose(file);
    }

}

int main(){
    mkdir("root");
    int finish = 0;
    while(1){
        //too many input:
        if(finish == 1){
            char bb = getchar();
            while(bb != '\n'){
                bb = getchar();
            }
        }

        int num_of_word = 0;
        finish = 0;
        char** line = (char*)malloc(sizeof(char*)*20);
        //each loop gets one word
        while(1){
            int is_gume = 0;
            char *word = (char*)malloc(sizeof(char)*1000);
            char c = getchar();

            if(c == '\n'){break;}     if(c == ' '){continue;}   if(c=='"'){is_gume = 1;}

            int lng;
            if(is_gume == 0){
                if(c == '/'){c = getchar();}
                for(lng = 0; c!=' ' && c!= '\n';lng++){
                    word[lng] = c;
                    c = getchar();
                }
            }
            else{
                c = getchar();
                if(c == '/'){c = getchar();}
                for(lng = 0; c!='"' && c!= '\n';lng++){
                    word[lng] = c;
                    c = getchar();
                }
                if(c == '\n'){
                    printf("You didn't closed your address!!\n");
                    finish = 1;
                    break;
                }
            }

            word[lng] = '\0';
            *(line+num_of_word) = word;
            num_of_word++;
            //too many inputs:
            if(num_of_word >=20 && c!='\n'){
                printf("Too many inputs!\n");
                finish = 1;
                break;
            }
            if(c == '\n'){
                break;
            }
        }
        //too many inputs:
        if(finish){continue;}

        if(strcmp(*(line),"createfile") == 0 && strcmp(*(line +1 ),"-file") == 0&& num_of_word == 3){
            createfile(*(line+2));
        }


        else{
            printf("Invalid input!\n");
        }


    }
    return 0;
}
