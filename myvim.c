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

int invalid_add(char* str){
    for(int i = 0; i <strlen(str); i++){
        if(str[i] == '<' ||
           str[i] == '>' ||
           str[i] == ':' ||
           str[i] == '"' ||
           str[i] == '\\' ||
           str[i] == '|' ||
           str[i] == '?' ||
           str[i] == '*' ||
           str[i] < 32){
            return 0;
           }
    }
    return 1;
}

void make_dir(char* add){
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
        if(l != 0) mkdir(tmp);
    }
}

void createfile(char* add){
    if(invalid_add(add) == 0){printf("Invalid Address!\n");return;}

    make_dir(add);

    if(access(add, F_OK ) != -1){
        printf("The file have been created before!\n");
    }
    else{
        FILE* file = fopen(add,"a+");
        fclose(file);
    }

}

void make_pos(char* pos, int* l,int* c){
    int after = 0,before = 1;
    for(int i = 0; i< strlen(pos);i++){
        if(pos[i] == ':'){
            before = 0;
            after = 1;
            continue;
        }
        if(before){
        *l *= 10;
        *l += pos[i] - '0';
        }
        if(after){
            *c *= 10;
            *c += pos[i] - '0';
        }
    }
}

void enter_and_gume(int length,char* text,int* tmp){
    for(int i = 0; i<length; i++){
        tmp[i] = 0;
        if(i>=1 && (text[i] == '"' || text[i] == 'n') && text[i-1] == '\\'){
            if(text[i-2] == '\\'){
                text[i-2] = '\0';
                tmp[i-1] = 1;
                continue;
            }
            if(text[i-2]!= '\\' || i==1){
                text[i-1] = '\0';
                tmp[i] = 1;
                if(text[i] == 'n') text[i]='\n';
            }
        }
    }
    tmp[0] = 1;
}

void my_insert(char *add,char *text,char* pos){
    int line = 0, ch = 0;
    make_pos(pos,&line,&ch);

    int length = strlen(text);
    int* tmp = (int*)malloc(sizeof(int)*length);
    enter_and_gume(length,text,tmp);

    if(access(add, F_OK ) == -1){
        printf("The file haven't been created before!\n");
    }
    else{
        int is_done = 0;
        FILE* file = fopen(add,"a+");        FILE* file_tmp = fopen("tmp.txt","w+");
        int ln = 1,chr = -1;

        char c = getc(file);
        while(c != EOF){
            chr ++;
            if((ln == line && chr == ch)||(c == '\n' && is_done == 0 && ln == line)){
                is_done = 1;
                for(int j = 0; j<length;j++){
                    if(tmp[j]==1){
                        fprintf(file_tmp,text + j);
                    }
                }
            }
            putc(c,file_tmp);
            if(c == '\n'){
                ln ++;
                chr = -1;
            }
            c = getc(file);
        }
        if(is_done == 0){
            is_done = 1;
            for(int j = 0; j<length;j++){
                if(tmp[j]==1){
                    fprintf(file_tmp,text + j);
                }
            }
        }
        fclose(file);
        fclose(file_tmp);
        file = fopen(add,"w+");
        file_tmp = fopen("tmp.txt","r+");
        c = getc(file_tmp);
        while(c != EOF){
            putc(c,file);
            c = getc(file_tmp);
        }
        fclose(file);
        fclose(file_tmp);
    }
}

void my_cat(char* add){
    if(access(add, F_OK ) == -1){
        printf("The file haven't been created before!\n");
    }
    else{
        FILE* file = fopen(add,"r+");
        char c = getc(file);
        while(c != EOF){
            printf("%c",c);
            c = getc(file);
        }
        fclose(file);
        printf("\n");
    }



}

int make_num(char* num){
    int my_num = 0;
    for(int i = 0;i<strlen(num);i++){
        my_num *= 10;
        my_num += num[i] - '0';
    }
    return my_num;
}

void my_remove(char *add,char *pos,char* size,char mode){
    int line = 0, ch = 0;
    make_pos(pos,&line,&ch);
    int sz = make_num(size);
    if(access(add, F_OK ) == -1){
        printf("The file haven't been created before!\n");
    }
    else{
        int counter = 0,ln = 1,chr = -1,start=0,end=0;
        FILE* file = fopen(add,"a+");        FILE* file_tmp = fopen("tmp.txt","w+");
        char c = getc(file);
        while(c != EOF){
            counter++;chr ++;
            if(ln == line && chr == ch){
                start = counter;
            }
            putc(c,file_tmp);
            if(c == '\n'){
                ln ++;
                chr = -1;
            }
            c = getc(file);
        }
        fclose(file);
        fclose(file_tmp);
        if(mode == 'f') end = start + sz -1;
        if(mode == 'b'){
            start = start -1;
            end = start - sz +1;
        }
        if(start <= 0){
            printf("invalid position!\n");
            return;
        }
        if(end < 1 || end > counter ){
            printf("Invalid size!\n");
            return;
        }
        if(start > end){
            int t = start;
            start = end;
            end = t;
        }
        file = fopen(add,"w+");
        file_tmp = fopen("tmp.txt","r+");
        c = getc(file_tmp);
        counter = 0;
        while(c != EOF){
            counter ++;
            if(counter <start || counter > end){
                putc(c,file);
            }
            c = getc(file_tmp);
        }
        fclose(file);
        fclose(file_tmp);
    }
}

void my_copy(char *add,char *pos,char* size,char mode){
    int line = 0, ch = 0;
    make_pos(pos,&line,&ch);
    int sz = make_num(size);
    if(access(add, F_OK ) == -1){
        printf("The file haven't been created before!\n");
    }
    else{
        int counter = 0,ln = 1,chr = -1,start = 0,end=0;
        FILE* file = fopen(add,"a+");
        char c = getc(file);
        while(c != EOF){
            counter++;chr ++;
            if(ln == line && chr == ch){
                start = counter;
            }
            if(c == '\n'){
                ln ++;
                chr = -1;
            }
            c = getc(file);
        }
        fclose(file);
        if(mode == 'f') end = start + sz -1;
        if(mode == 'b'){
            start = start -1;
            end = start - sz +1;
        }
        if(start <= 0){
            printf("invalid position!\n");
            return;
        }
        if(end < 1 || end > counter ){
            printf("Invalid size!\n");
            return;
        }
        if(start > end){
            int t = start;
            start = end;
            end = t;
        }
        file = fopen(add,"r+");
        FILE *file_clip = fopen("clipboard.txt","w+");
        c = getc(file);
        counter = 0;
        while(c != EOF){
            counter ++;
            if(counter >= start && counter <= end){
                putc(c,file_clip);
            }
            c = getc(file);
        }
        fclose(file);
        fclose(file_clip);
    }
}

void my_paste(char *add,char* pos){
    int line = 0, ch = 0;
    make_pos(pos,&line,&ch);

    if(access(add, F_OK ) == -1){
        printf("The file haven't been created before!\n");
    }
    else{
        FILE* file = fopen(add,"a+");        FILE* file_tmp = fopen("tmp.txt","w+");    FILE* file_clip = fopen("clipboard.txt","r+");
        int ln = 1,chr = -1;
        int is_done = 0;
        char c = getc(file);
        while(c != EOF){
            chr ++;
            if((ln == line && chr == ch)||(c == '\n' && is_done == 0 && ln == line)){
                is_done = 1;
                char cc = getc(file_clip);
                while(cc!=EOF){
                    putc(cc,file_tmp);
                    cc = getc(file_clip);
                }

            }
            putc(c,file_tmp);
            if(c == '\n'){
                ln ++;
                chr = -1;
            }
            c = getc(file);
        }
        if(is_done == 0){
            is_done = 1;
            char cc = getc(file_clip);
                while(cc!=EOF){
                    putc(cc,file_tmp);
                    cc = getc(file_clip);
                }
        }
        fclose(file);
        fclose(file_tmp);
        fclose(file_clip);
        file = fopen(add,"w+");
        file_tmp = fopen("tmp.txt","r+");
        c = getc(file_tmp);
        while(c != EOF){
            putc(c,file);
            c = getc(file_tmp);
        }
        fclose(file);
        fclose(file_tmp);
    }
}

int main(){
    mkdir("root");
    int finish = 0;
    while(1){
        //too many input:
        if(finish){
            char bb = getchar();
            while(bb != '\n'){
                bb = getchar();
            }
        }

        int num_of_word = 0;    finish = 0;
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
                for(lng = 0; !(c=='"' && word[lng-1] !='\\') && c!= '\n';lng++){
                    word[lng] = c;
                    c = getchar();
                }
                if(c == '\n'){
                    printf("You didn't closed your address!!\n");
                    finish = 1;
                    break;
                }
            }
            if(finish){break;}
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

        else if(num_of_word == 7 && strcmp(*(line),"insertstr") == 0 && strcmp(*(line+1),"-file") == 0 && strcmp(*(line+3),"-str") == 0 && strcmp(*(line+5),"-pos") == 0){
            my_insert(*(line+2), *(line + 4),*(line+6));
        }

        else if(num_of_word == 3 && strcmp(*(line),"cat") == 0 && strcmp(*(line+1),"-file") == 0 ){
            my_cat(*(line+2));
        }

        else if(num_of_word == 8 && strcmp(*(line),"removestr") == 0 && strcmp(*(line+1),"-file") == 0 && strcmp(*(line+3),"-pos") == 0
                && strcmp(*(line+5),"-size") == 0 && (strcmp(*(line+7),"-b") == 0 || strcmp(*(line+7),"-f") == 0 )){
            my_remove(*(line+2), *(line + 4),*(line+6),*(*(line+7)+1));
        }

        else if(num_of_word == 8 && strcmp(*(line),"copystr") == 0 && strcmp(*(line+1),"-file") == 0 && strcmp(*(line+3),"-pos") == 0
                && strcmp(*(line+5),"-size") == 0 && (strcmp(*(line+7),"-b") == 0 || strcmp(*(line+7),"-f") == 0 )){
            my_copy(*(line+2), *(line + 4),*(line+6),*(*(line+7)+1));
        }

        else if(num_of_word == 8 && strcmp(*(line),"cutstr") == 0 && strcmp(*(line+1),"-file") == 0 && strcmp(*(line+3),"-pos") == 0
                && strcmp(*(line+5),"-size") == 0 && (strcmp(*(line+7),"-b") == 0 || strcmp(*(line+7),"-f") == 0 )){
            my_copy(*(line+2), *(line + 4),*(line+6),*(*(line+7)+1));
            my_remove(*(line+2), *(line + 4),*(line+6),*(*(line+7)+1));
        }

        else if(num_of_word == 5 && strcmp(*(line),"pastestr") == 0 && strcmp(*(line+1),"-file") == 0 && strcmp(*(line+3),"-pos") == 0){
            my_paste(*(line+2), *(line + 4));
        }

        else{
            printf("Invalid input!\n");
        }


    }
    return 0;
}
