#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

typedef struct users{
    char* username;
    char* password;
    struct users* next;
}users;

typedef struct posts{
    char* text;
    int post_id;
    struct posts* next_post;
    users* writer;
}posts;

typedef struct likes{
    int post_id;
    char* username;
    struct likes* next_like;
}likes;

char* read_string(){
    /*this function reads inputs*/
    int i=0;
    char c;
   char* input=0;
    c = getchar();
    input = (char*)malloc((i+1));

    while(c!='\n' && c!=' ' && c!='\t'){
        input = (char*)realloc(input , i+2);
        input[i] = c;
        c = getchar();
        i++;
    }
    input[i] = '\0';
    return input;
}

int check_command(){
    /*this function detects the input command and return appropriate value*/
    char* command = read_string();
    if(!strcmp("signup", command)){
        return 1;
    }
    else if(!strcmp("login", command)){
        return 2;
    }
    else if(!strcmp("post", command)){
        return 3;
    }
    else if(!strcmp("like", command)){
        return 4;
    }
    else if(!strcmp("logout", command)){
        return 5;
    }
    else if(!strcmp("delete", command)){
        return 6;
    }
    else if(!strcmp("info", command)){
        return 7;
    }
    else if(!strcmp("find_user", command)){
        return 8;
    }
    else{
        return 0;
    }
}

int signup(users* head , users** current_user){
    /*return 0 if user has already signed up or logged in and return 1 when succesful*/
    users* curr = head;
    char* username = read_string();
    
    if((*current_user) != NULL){
        printf("a user already logged in\n");
        read_string();
        return 0;
    }
    while (curr->next != NULL) {
        if (!strcmp(curr->next->username , username)){ 
            read_string(); 
            printf("this user has already signed up\n");
            return 0;
            }
        curr = curr->next;
    }
    curr->next = (users*)malloc(sizeof(users));
    curr->next->next = NULL;
    curr->next->username = (char*)malloc(strlen(username) + 1);
    strcpy(curr->next->username , username);
    printf(" username in signup is %s\n ",curr->next->username );
    char* password = read_string();
    curr->next->password = (char*)malloc(strlen(password) + 1);
    strcpy(curr->next->password , password);
    printf(" password in signup is %s\n ",curr->next->password );
    printf("signup is succesful\n");
}

int login(users* head , users** current_user){
    /*return 0 if login unsuccesful and return 1 when succesful*/
    char* username = read_string();
    char* password = read_string();
    users* curr = head;
    if((*current_user) != NULL){
        printf("a user already logged in\n");
        return 0;
    }
    while(curr = curr->next){
        if(!strcmp(curr->username , username) && !strcmp(curr->password , password)){
            printf("login is succesful\n");
            *current_user = curr;
            return 1;
        }
    }
    printf("incorrect login information\n");
    return 0;
   
}

int post(posts* head , int* post_id , users** current_user){
    posts* curr = head;
    while (curr->next_post) {
        curr = curr->next_post;
    }
    /*get content of post by getchar*/
    int i=0;
    char c;
    char* post = (char*)malloc(sizeof(char));
    c = getchar();
    while(c!='\n'){
        post[i] = c;
        post = (char*)realloc(post , i+2);
        c = getchar();
        i++;
    }
    post[i] = NULL;

    if((*current_user) == NULL){
        printf("nobody logged in\n");
        return 0;
    }
    
    curr->next_post = (posts*)malloc(sizeof(posts));
    curr->next_post->next_post = NULL;
    curr->next_post->text = (char*)malloc(strlen(post) + 1);
    strcpy(curr->next_post->text , post);
    curr->next_post->post_id = *post_id;
    (*post_id)++;
    printf("%s %d\n", curr->next_post->text , curr->next_post->post_id);
    curr->next_post->writer = (*current_user);
    return 1;
}

int like(likes* like_head , posts* post_head , users** current_user){
    read_string();
    int i = getchar();
    i = i % 48;
    fflush(stdin);
    likes* curr_like = like_head;
    posts* curr_post = post_head;

    while(curr_post = curr_post->next_post){
        /*find post by post_id*/
        if(curr_post->post_id == i){
           
            while (curr_like->next_like) {
                curr_like=curr_like->next_like;
            }
            likes* like = (likes*)malloc(sizeof(likes));
            like->post_id = curr_post->post_id;
            like->username = (*current_user)->username;
            curr_like->next_like = like;
            printf("post succesfully liked\n");
            return 1;
        }
    }
    printf("post_id not found\n");
    return 0;
}

int num_of_likes(likes* head , int post_id){
    /*this function counts number of structs likes*/
    int num_of_likes = 0;
    likes* curr = head;
    while(curr = curr->next_like){
        if(curr->post_id == post_id){
            num_of_likes++;
        }
    }
    return num_of_likes;
}

int logout(users** current_user){
    /*return 0 if nobody logged in and return 1 if logout was succesful*/
    if((*current_user) == NULL){
        printf("nobody logged in\n");
        return 0;
    }
    else{
        (*current_user) = NULL;
        printf("logout succesful\n");
        return 1;
    }
}

int delete(posts* head , users** current_user){
    int i = getchar();
    i = i % 48;
    fflush(stdin);
    posts* curr = head;
    posts* prev = head;
    
    if(current_user == NULL){
        printf("nobody logged in\n");
        return 0;
    }
    while(curr = curr->next_post){
        /*check post_id and username for deleting the post*/
        if(curr->post_id == i && curr->writer->username == (*current_user)->username){
            printf("post succesfully deleted\n");
            prev->next_post = curr->next_post;
            free(curr->text);
            free(curr);
            return 1;
        }
        prev = curr;
    }
}

int info(posts* post_head , likes* like_head , users** current_user){
    /*if nobody loogged in*/
    if((*current_user) == NULL){
        printf("nobody logged in\n");
        return 0;
    }
    
    printf("username : %s\n", (*current_user)->username);
    printf("password : %s\n", (*current_user)->password);
    posts* curr = post_head;
    /*find posts of user*/
    while(curr = curr->next_post){
        if(curr->writer->username == (*current_user)->username){
            int i =num_of_likes(like_head , curr->post_id);
            printf("post : %s ", curr->text);
            printf("| post_id : %d ", curr->post_id);
            printf("| likes : %d\n", i);
        }
    }
    return 1;
}

int find_user(users* user_head , posts* post_head , likes* like_head){
    char* username = read_string();
    users* curr_user = user_head;
    posts* curr_post = post_head;
    
    /*find the user who searched*/
    while(curr_user = curr_user->next){
        if(!strcmp(curr_user->username , username)){
            printf("username : %s\n", username);
            /*find posts of user*/
            while(curr_post = curr_post->next_post){
                if(!strcmp(curr_post->writer->username , username)){
                    int i =num_of_likes(like_head , curr_post->post_id);
                    printf("post : %s ", curr_post->text);
                    printf("| post_id : %d ", curr_post->post_id);
                    printf("| likes : %d\n", i);
                }
            }
            return 1;
        }
    }
    /*if usrname does not exist*/
    printf("user not found\n");
    return 0;
}

int phase_2_accounts(users* head){
    FILE* accounts = fopen("accounts.txt" , "w");
    users* curr = head;
    while(curr = curr->next){
        char* username = curr->username;
        char* password = curr->password;
        fwrite(username , sizeof(char) , strlen(username) , accounts);
        fprintf(accounts , " ");
        fwrite(password , sizeof(char) , strlen(password) , accounts);
        fprintf(accounts , "\n");        
    }
    fclose(accounts);
    return 1;
}

int phase_2_posts(posts* head){
    FILE* post = fopen("post.txt" , "w");
    posts* curr = head;
    while(curr = curr->next_post){
        char* text = curr->text;
        char* author = curr->writer->username;
        fwrite(text , sizeof(char) , strlen(text) , post);
        fprintf(post , " ");
        fwrite(author , sizeof(char) , strlen(author) , post);
        fprintf(post , "\n");
    }
    fclose(post);
    return 1;
}

int main() {
    printf("welcome to ut_tuti\n");
    users* user_head = (users*)malloc(sizeof(users));
    user_head->next = NULL;
    posts* post_head = (posts*)malloc(sizeof(posts));
    post_head->next_post = NULL;
    likes* like_head = (likes*)malloc(sizeof(likes));
    like_head->next_like = NULL;
    int post_id = 0;
    users* current_user = NULL;
    int command = check_command();

    while(1){
        switch(command){
            case 1:{
                signup(user_head , &current_user);
                break;
            }
            case 2:{
                login(user_head , &current_user);
                break;
            }
            case 3:{
                post(post_head , &post_id , &current_user);
                break;
            }
            case 4:{
                like(like_head , post_head , &current_user);
                break;
            }
            case 5:{
                logout(&current_user);
                break;
            }
            case 6:{
                delete(post_head , &current_user);
                break;
            }
            case 7:{
                info(post_head , like_head , &current_user);
                break;
            }
            case 8:{
                find_user(user_head , post_head , like_head);
                break;
            }
            case 0:{
                printf("incorrect order\n");
                fflush(stdin);
                break;
            }
        }
        phase_2_accounts(user_head);
        phase_2_posts(post_head);
        command = check_command();
    }
}