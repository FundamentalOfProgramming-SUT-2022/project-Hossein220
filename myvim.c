#include <stdio.h>
#include <direct.h>
#include<unistd.h>
#define debug(x) printf("%s is: %d\n",#x,x)

void file_copy(char* add1,char*add2){
    make_dir(add2);
    FILE* file1 = fopen(add1,"r+");
    FILE* file2 = fopen(add2,"w+");
    char c = getc(file1);
    while(c!=EOF){
        putc(c,file2);
        c = getc(file1);
    }
    fclose(file1);
    fclose(file2);
}

int num_of_char(char *address,char c){
    int num = 0;
    for(int i = 0;i<strlen(address);i++){
        if(address[i]==c){num++;}
    }
    return num;
}

int invalid_char(char* str){
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
    for(int l = 1; l<num_of_char(add,'/');l++){
        int length = strlen(add);
        char* tmp = (char*)malloc(sizeof(char)*length);
        int ll = l+1;
        for(int i = 0; i < length; i++){
            if(add[i]=='/'){
                ll--;
            }
            if(ll == 0){tmp[i] = '\0'; break;}
            tmp[i]=add[i];
        }
        mkdir(tmp);
    }
}

int check_add(char* add){
    char*tmp = (char*)malloc(sizeof(char)*(strlen(add)+10));
    int n = num_of_char(add,'/');
    int nn = 0,i;
    for(i = 0;nn != n;i++){
        char c = add[i];
        tmp[i] = add[i];
        if(c == '/') nn++;
    }
    tmp[i] = 'a';tmp[i+1] = '.';tmp[i+2] = 't';tmp[i+3] = 'x';tmp[i+4] = 't';tmp[i+5] = '\0';
    int was_valid = 0;
    if(access(tmp, F_OK) == 0) was_valid = 1;
    FILE* file = fopen(tmp,"a+");
    fclose(file);
    int a = 1 + (access(tmp, F_OK));
    if(was_valid == 0) remove(tmp);
    return a;
}

void createfile(char* add){
    if(invalid_char(add) == 0){printf("Error: Using special characters!\n");return;}

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

char* enter_and_gume_and_star(int length,char* text,char c){
    for(int i = 0; i<length; i++){
        if(i>=1 && text[i] == c && text[i-1] == '\\'){
            if(text[i-2] == '\\'){
                text[i-2] = '\0';
                continue;
            }
            if(text[i-2]!= '\\' || i==1){
                text[i-1] = '\0';
                if(text[i] == 'n') text[i]='\n';
            }
        }
    }
    char *tmp = (char*)malloc(sizeof(char)*length);
    int counter = 0;
    for(int i = 0; i<length; i++){
        if(*(text+i) == '\0')
            continue;
        *(tmp+counter) = *(text+i);
        counter ++;
    }
    tmp[counter] = '\0';
    return tmp;
}

void insert2(char *add,char *text,int line,int ch){
        text = enter_and_gume_and_star(strlen(text),text,'n');
        text = enter_and_gume_and_star(strlen(text),text,'*');
        text = enter_and_gume_and_star(strlen(text),text,'"');
        int is_done = 0;
        FILE* file = fopen(add,"a+");        FILE* file_tmp = fopen("tmp.txt","w+");
        int ln = 1,chr = -1;

        char c = getc(file);
        while(c != EOF){
            chr ++;
            if(ln == line && chr == ch){
                is_done = 1;
                fprintf(file_tmp,text);
            }
            putc(c,file_tmp);
            if(c == '\n'){
                ln ++;
                chr = -1;
            }
            c = getc(file);
        }
        if(is_done == 0){
            if(ln == line && chr == ch -1){
                fprintf(file_tmp,text);
            }
            else
                printf("Invalid position!\n");
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

void my_insert(char *add,char *text,int line,int ch){
    if(access(add, F_OK ) == -1){
        if(check_add(add) == 1)
            printf("The file haven't been created before!\n");
        else
            printf("Incorrect Address!\n");
        return;
    }
    else{
        file_copy(add,add+1);
        insert2(add,text,line,ch);
    }
}

void my_cat(char* add){
    if(access(add, F_OK ) == -1){
        if(check_add(add) == 1)
            printf("The file haven't been created before!\n");
        else
            printf("Incorrect Address!\n");
        return;
    }
    else{
        file_copy(add,add+1);
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

void remove2(char *add,int line,int ch,int sz,char mode){
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

void my_remove(char *add,int line,int ch,int sz,char mode){
    if(access(add, F_OK ) == -1){
        if(check_add(add) == 1)
            printf("The file haven't been created before!\n");
        else
            printf("Incorrect Address!\n");
        return;
    }
    else{
        file_copy(add,add+1);
        remove2(add,line,ch,sz,mode);
    }
}

void my_copy(char *add,char *pos,char* size,char mode){
    int line = 0, ch = 0;
    make_pos(pos,&line,&ch);
    int sz = make_num(size);
    if(access(add, F_OK ) == -1){
        if(check_add(add) == 1)
            printf("The file haven't been created before!\n");
        else
            printf("Incorrect Address!\n");
        return;
    }
    else{
        file_copy(add,add+1);
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
        if(check_add(add) == 1)
            printf("The file haven't been created before!\n");
        else
            printf("Incorrect Address!\n");
        return;
    }
    else{
        file_copy(add,add+1);
        FILE* file = fopen(add,"a+");        FILE* file_tmp = fopen("tmp.txt","w+");    FILE* file_clip = fopen("clipboard.txt","r+");
        int ln = 1,chr = -1;
        int is_done = 0;
        char c = getc(file);
        while(c != EOF){
            chr ++;
            if(ln == line && chr == ch){
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
            printf("Invalid position!\n");
            return;
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

int my_cmp(char* a,char* b,int m){
    int l1 = strlen(a) - 1;
    int l2 = strlen(b);
    if(m == 1){
        for(int i = l1; i>0;i--){
            l2--;
            if(l2<0 || a[i] != b[l2]){
                return 0;
            }
        }
        return 1;
    }
    else if(m==2){
        for(int i = 0; i<l1 ; i++){
            if(i>=l2 || a[i] != b[i]){
                return 0;
            }
        }
        return 1;
    }
    else if(m==3){
        return(!strcmp(a,b));
    }
}

char** to_words(char*text,int*x){
    int w=0,ch=0,so = 0,sn = 0;
    char **words = (char*)malloc(sizeof(char*)*10000);
    text = enter_and_gume_and_star(strlen(text),text,'n');
    char c = text[0];
    *(words + w) = (char*)malloc(sizeof(char)*1000);
    for(int i = 0;c!= '\0';i++){
        so = sn;
        if(c == ' '|| c == '\n'){
            sn = 0;
        }
        else{
            sn = 1;
        }
        if(sn != so && i!=0){
            *(*(words +w)+ch) = '\0';
            w++;
            *(words + w) = (char*)malloc(sizeof(char)*1000);
            ch = 0;
        }
        *(*(words +w)+ch) = text[i];
        ch++;
        c = text[i+1];
    }
    *(*(words +w)+ch) = '\0';
    *x = w + 1;
    return words;
}

int fnd(char* text,char* text2){
    int n1,n2;
    char** words1 = to_words(text,&n1);
    char** words2 = to_words(text2,&n2);
    if(n1!=n2){
        return 0;
    }
    for(int i = 0; i<n1;i++){
        int m;
        char * mytext = *(words1+i);
        if(*mytext == '*'){
            m = 1;
        }
        else if(*(mytext+strlen(mytext)-1) == '*' && *(mytext+strlen(mytext)-2) != '\\'){
            m = 2;
        }
        else m = 3;
        mytext = enter_and_gume_and_star(strlen(mytext),mytext,'n');
        mytext = enter_and_gume_and_star(strlen(mytext),mytext,'"');
        mytext = enter_and_gume_and_star(strlen(mytext),mytext,'*');
        if(!my_cmp(mytext,*(words2+i),m)){
            return 0;
        }
    }
    return 1;
}

int chars_of_file(char *a){
    int counter = 0;
    FILE* f = fopen(a,"r+");
    char c = getc(f);
    while(c!=EOF){
        counter++;
        c = getc(f);
    }
    fclose(f);
    return counter;
}

void my_find(char* text,char* add,int mode,int num){
    if(access(add, F_OK ) == -1){
        if(check_add(add) == 1)
            printf("The file haven't been created before!\n");
        else
            printf("Incorrect Address!\n");
        return;
    }
    else{
        file_copy(add,add+1);
        text = enter_and_gume_and_star(strlen(text),text,'n');
        int n = num_of_char(text,' ') + num_of_char(text,'\n');
        char *tt = (char*)calloc(sizeof(char),10000);
        int* word = (int*)malloc(sizeof(int)*10000);
        int* chars = (int*)malloc(sizeof(int)*10000);
        int count = 0,my_count = 1,word_done = -1,sz = chars_of_file(add);
        char cc = ' ';
        int num_char = 1,num_word = 0;
        while(1){
            num_char = 1;num_word = 0;
            int nn = 0,counter = 0;
            FILE* file = fopen(add,"r+");
            char c = getc(file);
            while(c!=EOF){
                if((c == ' ' || c == '\n') && cc != ' ' && cc != '\n') num_word++;
                if(num_char >= my_count && nn <= n){
                    tt[counter] = c;
                    counter++;
                    if(c == ' ' || c == '\n') nn++;
                }
                if(nn>n) break;
                cc = c;
                c = getc(file);
                if(nn==n && num_char >= my_count && (c== ' '|| c=='\n' || c == EOF) && word_done != num_word){
                    tt[counter] = '\0';
                    if(fnd(text,tt)){
                        word_done = num_word ;
                        word[count] = num_word + 1 - n;
                        chars[count] = num_char - strlen(tt) +1;
                        count++;
                        break;
                    }
                }
                if(c!=EOF) num_char++;
            }
            if(c == EOF && cc != ' ' && cc != '\n'){
                num_word++;
            }
            fclose(file);
            if(nn<n || my_count > sz){break;}
            my_count++;
        }
        if(mode == 1){
            if(count == 0) printf("-1\n");
            else printf("%d\n",chars[0]);
        }
        else if(mode == 2){
            printf("%d\n",count);
        }
        else if(mode == 3){
            if(num>count) printf("-1\n");
            else printf("%d\n",chars[num-1]);
        }
        else if(mode == 4){
            if(count == 0) printf("-1\n");
            else printf("%d\n",word[0]);
        }
        else if(mode == 5){
            if(count == 0) printf("-1\n");
            else{
                for(int i = 0;i<count;i++){
                    printf("%d ",chars[i]);
                }
                printf("\n");
            }
        }
        else if(mode == 6){
            if(count == 0) printf("-1\n");
            else{
                for(int i = 0;i<count;i++){
                    printf("%d ",word[i]);
                }
                printf("\n");
            }
        }
        else if(mode == 7){
            if(num>count) printf("-1\n");
            else printf("%d\n",word[num-1]);
        }
    }
}

void my_replace(char* add,char* text1,char* text2,int mode,int num){
    if(access(add, F_OK ) == -1){
        if(check_add(add) == 1)
            printf("The file haven't been created before!\n");
        else
            printf("Incorrect Address!\n");
        return;
    }
    else{
        file_copy(add,add+1);
        text1 = enter_and_gume_and_star(strlen(text1),text1,'n');
        char* origin_text2 = (char*)malloc(sizeof(char)*strlen(text2));
        for(int i = 0;i<=strlen(text2);i++){
                origin_text2[i] = text2[i];
        }
        text2 = enter_and_gume_and_star(strlen(text2),text2,'n');
        text2 = enter_and_gume_and_star(strlen(text2),text2,'*');
        text2 = enter_and_gume_and_star(strlen(text2),text2,'"');
        int l2 = strlen(text2);
        int n = num_of_char(text1,' ') + num_of_char(text1,'\n');
        char *tt = (char*)calloc(sizeof(char),10000);
        int count = 0,my_count = 1,num_char = 1,num_word = 0,word_done = -1;
        char cc = ' ';
        int sz = chars_of_file(add);
        while(1){
            for(int i = 0;i<=strlen(origin_text2);i++){
                text2[i] = origin_text2[i];
            }
            num_char = 1;num_word = 0;
            int nn = 0,counter = 0,chch = 0,ll = 1;
            FILE* file = fopen(add,"r+");
            char c = getc(file);
            while(c!=EOF){
                chch++;
                if((c == ' ' || c == '\n') && cc != ' ' && cc != '\n') num_word++;
                if(num_char >= my_count && nn <= n){
                    tt[counter] = c;
                    counter++;
                    if(c == ' ' || c == '\n') nn++;
                }
                if(nn>n) break;
                cc = c;
                c = getc(file);
                if(nn==n && num_char >= my_count && (c== ' '|| c=='\n' || c == EOF) &&(word_done != num_word || mode == 2)){
                    tt[counter] = '\0';
                    if(fnd(text1,tt)){
                        word_done = num_word;
                        if(mode == 2 || mode == 1 && count+1 == num){
                            remove2(add,ll,chch-strlen(tt),strlen(tt),'f');
                            insert2(add,text2,ll,chch-strlen(tt));
                            sz = chars_of_file(add);
                            my_count += l2-1;
                        }
                        count++;
                        break;
                    }
                }
                if(c!=EOF) num_char++;
                if(cc == '\n'){
                    ll++;
                    chch = 0;
                }
            }
            if(c == EOF && cc != ' ' && cc != '\n'){
                num_word++;
            }
            fclose(file);
            if(nn<n || my_count > sz){break;}
            my_count++;

        }
        if(count == 0) printf("No such expression!\n");
    }
}

int grep_help(char* text,char* pat){
    char* pat2 = (char*)malloc(sizeof(char)*(strlen(pat)+1));
    for(int i = 0;i<strlen(pat);i++){
        pat2[i] = pat[i];
    }
    pat2[strlen(pat)] = '\0';
    pat2 = enter_and_gume_and_star(strlen(pat),pat,'n');
    pat2 = enter_and_gume_and_star(strlen(pat),pat,'"');
    pat2 = enter_and_gume_and_star(strlen(pat),pat,'*');
    int l = strlen(text);
    for(int i = 0; i<l;i++){
        char* tmp = (char*)malloc(sizeof(char)*(l+1));
        for(int j = 0; j<l-i;j++){
            tmp[j] = text[j+i];
            tmp[j+1] = '\0' ;
            if(strcmp(tmp,pat2) == 0) return 1;
        }
    }
    return 0;
}

void my_grep(char mode, char* pat,char** files,int n){
    int * vaild = (int*)malloc(sizeof(int)*n);
    int cn = 0;
    for(int k = 0; k<n; k++){
        vaild[k] = 0;
    }
    for(int i = 0; i<n; i++){
        char* add = *(files + i);
        if(access(add, F_OK ) == -1){
            if(check_add(add) == 1)
                printf("file%d: The file haven't been created before!\n",i+1);
            else
                printf("Address%d: Incorrect Address!\n",i+1);
        }
        else{
            file_copy(add,add+1);
            FILE* ff =fopen(add,"r+");
            char* tt = (char*)malloc(sizeof(char)*10000);
            int counter = 0;
            char c = getc(ff);
            while(c!=EOF){
                tt[counter] = c;
                counter ++;
                if(c == '\n'){
                    tt[counter-1] = '\0';
                    if(grep_help(tt,pat) == 1){
                        cn ++;
                        if(mode == 'o'){
                            printf("%s : %s\n",add,tt);
                        }
                        if(mode == 'i'){
                            vaild[i] = 1;
                        }
                    }
                    counter = 0;
                }
                c = getc(ff);
            }
            tt[counter] = '\0';
            if(grep_help(tt,pat) == 1){
                        cn ++;
                        if(mode == 'o'){
                            printf("%s : %s\n",add,tt);
                        }
                        if(mode == 'i'){
                            vaild[i] = 1;
                        }
                    }
            fclose(ff);
        }
    }
    if(mode == 'i'){
        for(int k = 0; k<n; k++){
            if(vaild[k] == 1) printf("%s\n",files[k]);
        }
    }
    if(mode == 'c'){
        printf("%d\n",cn);
    }
}

void my_undo(char* add){
    if(access(add, F_OK ) == -1 && access(add+1, F_OK ) == -1){
        if(check_add(add) == 1)
            printf("The file haven't been created before!\n");
        else
            printf("Incorrect Address!\n");
        return;
    }
    if(access(add, F_OK ) == -1){
        file_copy(add+1,add);
        remove(add+1);
        return;
    }
    if(access(add+1, F_OK ) == -1){
        file_copy(add,add+1);
        remove(add);
        return;
    }
    file_copy(add,"tmp.txt");
    file_copy(add+1,add);
    file_copy("tmp.txt",add+1);
}

void put_enter_and_space(char*add){
    file_copy(add,"tmp.txt");
    FILE* file = fopen(add,"w+");
    FILE* tmp = fopen("tmp.txt","r+");
        char c = getc(tmp);
        char cc = ' ',ccc = ' ';
        int num_space = 0;
        while(c!=EOF){
            if(cc == '\n' && c == ' '){
                c = getc(tmp);
                continue;
            }
            if(c == ' '){
                num_space ++;
            }
            if(c != ' ' && num_space > 0){
                if(c == '{'){
                    putc(' ',file);
                }
                else
                for(int i = 0;i<num_space;i++){
                    putc(' ',file);
                }
                num_space = 0;
            }
            if(cc != ' ' && cc!= '\n' && c == '{'){
                putc(' ',file);
            }
            if(cc == '{' && c != '\n'){
                putc('\n',file);
            }
            if(cc != '\n' && c == '}'){
                putc('\n',file);
            }
            else if(cc == '}' && c != '\n'){
                putc('\n',file);
            }
            if(ccc == '{' && cc == '\n' && c == '}'){
                putc('\n',file);
            }
            if(c != ' ') putc(c,file);
            ccc = cc;
            cc = c;
            c = getc(tmp);
        }
    fclose(file);
    fclose(tmp);
}

void put_tab(char*add){
    file_copy(add,"tmp.txt");
    FILE* file = fopen(add,"w+");
    FILE* tmp = fopen("tmp.txt","r+");
        char c = getc(tmp);
        char cc = ' ';
        int nn = 0;
        while(c!=EOF){
            if(c == '}') nn--;
            if(cc == '\n'){
                if(c == ' '){
                    c = getc(tmp);
                    continue;
                }
                for(int j = 0;j<4*nn;j++){
                    putc(' ',file);
                }
            }
            if(c == '{') nn++;
            putc(c,file);
            cc = c;
            c = getc(tmp);
        }
    fclose(file);
    fclose(tmp);
}

my_auto_indent(char*add){
    if(access(add, F_OK ) == -1){
        if(check_add(add) == 1)
            printf("The file haven't been created before!\n");
        else
            printf("Incorrect Address!\n");
        return;
    }
    else{
        file_copy(add,add+1);
        put_enter_and_space(add);
        put_tab(add);
    }
}

int main(){
    mkdir("root");
    mkdir("oot");
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
            int lne = 0, ch = 0;
            make_pos(*(line+6),&lne,&ch);
            my_insert(*(line+2), *(line + 4),lne,ch);
        }

        else if(num_of_word == 3 && strcmp(*(line),"cat") == 0 && strcmp(*(line+1),"-file") == 0 ){
            my_cat(*(line+2));
        }

        else if(num_of_word == 8 && strcmp(*(line),"removestr") == 0 && strcmp(*(line+1),"-file") == 0 && strcmp(*(line+3),"-pos") == 0
                && strcmp(*(line+5),"-size") == 0 && (strcmp(*(line+7),"-b") == 0 || strcmp(*(line+7),"-f") == 0 )){
            int lne = 0, ch = 0;
            make_pos(*(line+4),&lne,&ch);
            int sz = make_num(*(line+6));
            my_remove(*(line+2), lne,ch,sz,*(*(line+7)+1));
        }

        else if(num_of_word == 8 && strcmp(*(line),"copystr") == 0 && strcmp(*(line+1),"-file") == 0 && strcmp(*(line+3),"-pos") == 0
                && strcmp(*(line+5),"-size") == 0 && (strcmp(*(line+7),"-b") == 0 || strcmp(*(line+7),"-f") == 0 )){
            my_copy(*(line+2), *(line + 4),*(line+6),*(*(line+7)+1));
        }

        else if(num_of_word == 8 && strcmp(*(line),"cutstr") == 0 && strcmp(*(line+1),"-file") == 0 && strcmp(*(line+3),"-pos") == 0
                && strcmp(*(line+5),"-size") == 0 && (strcmp(*(line+7),"-b") == 0 || strcmp(*(line+7),"-f") == 0 )){
            int lne = 0, ch = 0;
            make_pos(*(line+4),&lne,&ch);
            int sz = make_num(*(line+6));
            my_copy(*(line+2), *(line + 4),*(line+6),*(*(line+7)+1));
            my_remove(*(line+2), lne,ch,sz,*(*(line+7)+1));
        }

        else if(num_of_word == 5 && strcmp(*(line),"pastestr") == 0 && strcmp(*(line+1),"-file") == 0 && strcmp(*(line+3),"-pos") == 0){
            my_paste(*(line+2), *(line + 4));
        }

        else if(strcmp(*(line),"find") == 0 && strcmp(*(line+1),"-str") == 0 && strcmp(*(line+3),"-file") == 0){
            if(num_of_word == 5) my_find(*(line+2), *(line + 4),1,0);

            else if(num_of_word == 6 && strcmp(*(line+5),"-count") == 0)  my_find(*(line+2), *(line + 4),2,0);
            else if(num_of_word == 7 && strcmp(*(line+5),"-at") == 0) my_find(*(line+2), *(line + 4),3,make_num(*(line+6)));
            else if(num_of_word == 6 && strcmp(*(line+5),"-byword") == 0) my_find(*(line+2), *(line + 4),4,0);
            else if(num_of_word == 6 && strcmp(*(line+5),"-all") == 0) my_find(*(line+2), *(line + 4),5,0);

            else if(num_of_word == 7 && strcmp(*(line+5),"-all") == 0 && strcmp(*(line+6),"-byword") == 0) my_find(*(line+2), *(line + 4),6,0);
            else if(num_of_word == 7 && strcmp(*(line+5),"-byword") == 0 && strcmp(*(line+6),"-all") == 0) my_find(*(line+2), *(line + 4),6,0);
            else if(num_of_word == 8 && strcmp(*(line+5),"-byword") == 0 && strcmp(*(line+6),"-at") == 0) my_find(*(line+2), *(line + 4),7,make_num(*(line + 7)));
            else if(num_of_word == 8 && strcmp(*(line+5),"-at") == 0 && strcmp(*(line+7),"-byword") == 0) my_find(*(line+2), *(line + 4),7,make_num(*(line+6)));
            else printf("Invalid options!\n");
        }

        else if(strcmp(*(line),"replace") == 0 && strcmp(*(line+1),"-str1") == 0 && strcmp(*(line+3),"-str2") == 0 && strcmp(*(line+5),"-file") == 0){
            if(num_of_word == 7) my_replace(*(line+6), *(line + 2), *(line + 4),1,1);
            else if(num_of_word == 8 && strcmp(*(line+7),"-all") == 0) my_replace(*(line+6), *(line + 2), *(line + 4),2,0);
            else if(num_of_word == 9 && strcmp(*(line+7),"-at") == 0 ) my_replace(*(line+6), *(line + 2), *(line + 4),1,make_num(*(line+8)));
            else printf("Invalid options!\n");
        }

        else if(strcmp(*(line),"grep") == 0){
            if(strcmp(*(line+1),"-str") == 0 && strcmp(*(line+3),"-files") == 0) my_grep('o', *(line + 2), line + 4,num_of_word - 4);
            else if((strcmp(*(line+1),"-c") == 0 || strcmp(*(line+1),"-i") == 0)&& strcmp(*(line+2),"-str") == 0 && strcmp(*(line+4),"-files") == 0) my_grep(*(*(line+1)+1), *(line + 3),line + 5,num_of_word - 5);
        }

        else if(strcmp(*(line),"undo") == 0 && strcmp(*(line +1 ),"-file") == 0 && num_of_word == 3){
            my_undo(*(line+2));
        }

        else if(strcmp(*(line),"auto-indent") == 0 && strcmp(*(line +1 ),"-file") == 0 && num_of_word == 3){
            my_auto_indent(*(line+2));
        }

        else{
            printf("Invalid input!\n");
        }


    }
    return 0;
}
