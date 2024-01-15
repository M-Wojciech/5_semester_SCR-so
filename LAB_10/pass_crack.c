#include <openssl/evp.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#define DEFAULT_DICTIONARY "./dictionaries/mini_dictionary.txt"

typedef struct {
    int id;
    char password_hash[33];
    char mail[35];
    char user_name[20];
    // char password[30];
} user_data;

void bytes2md5(const char *data, int len, char *md5buf) {
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	const EVP_MD *md = EVP_md5();
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len, i;
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, data, len);
	EVP_DigestFinal_ex(mdctx, md_value, &md_len);
	EVP_MD_CTX_free(mdctx);
	for (i = 0; i < md_len; i++) {
		snprintf(&(md5buf[i * 2]), 16 * 2, "%02x", md_value[i]);
	}
}

int hash_database_find(char *hash, user_data *database, int database_entrys){
    for (int i = 0; i < database_entrys; i++){
        if ( !strcmp(hash, database[i].password_hash)){
            return i;
        }
    }
    return -1;
}

void direct_hash(char *word, user_data *user_database, int user_database_lines){
    char current_hash[33];

    bytes2md5(word, strlen(word), current_hash);

    int user_index = hash_database_find (current_hash, user_database, user_database_lines);

    if (user_index >= 0){
        printf("Password for %s is %s\n", user_database[user_index].mail, word);
    }
}

// test 2 - prefixes of numbers 1 to 99
void prefixes(char *word, user_data *user_database, int user_database_lines){
    char current_hash[33];
    char current_word[34];
    char number[3];

    for (int i = 1; i < 100; i++){
        snprintf(number, sizeof(number), "%d", i);

        strcpy(current_word, word);
        strcat(current_word, number);
        bytes2md5(current_word, strlen(current_word), current_hash);


        int user_index = hash_database_find (current_hash, user_database, user_database_lines);

        if (user_index >= 0){
            printf("Password for %s is %s\n", user_database[user_index].mail, current_word);
        }
    }
}

// test 2 - postfixes of numbers 1 to 99
void postfixes(char *word, user_data *user_database, int user_database_lines){
    char current_hash[33];
    char current_word[34];
    char number[3];

    for (int i = 1; i < 100; i++){
        snprintf(number, sizeof(number), "%d", i);

        strcpy(current_word, number);
        strcat(current_word, word);
        bytes2md5(current_word, strlen(current_word), current_hash);

        int user_index = hash_database_find (current_hash, user_database, user_database_lines);
            
        if (user_index >= 0){
            printf("Password for %s is %s\n", user_database[user_index].mail, current_word);
        }
    }
}

// test 2 - pre and postfixes of numbers 1 to 99
void pre_and_postfixes(char *word, user_data *user_database, int user_database_lines){
    char current_hash[33];
    char current_word[34];
    char number[3];
    char second_number[3];

    for (int i = 1; i < 100; i++){
        snprintf(number, sizeof(number), "%d", i);

        for (int l = 1; l < 100; l++){
            snprintf(second_number, sizeof(number), "%d", l);
            strcpy(current_word, number);
            strcat(current_word, word);
            strcat(current_word, second_number);

            bytes2md5(current_word, strlen(current_word), current_hash);

            int user_index = hash_database_find (current_hash, user_database, user_database_lines);
                
            if (user_index >= 0){
                printf("Password for %s is %s\n", user_database[user_index].mail, current_word);
            }
        }
    }
}

void upper_case(char *word, user_data *database, int user_database_lines){
    char current_word[34];
    int length = strlen(word);
    current_word[length] = '\0';

    for (int i = 0; i < length; i++){
        current_word[i] = toupper(word[i]);
    }
    direct_hash(current_word, database, user_database_lines);
    prefixes(current_word, database, user_database_lines);
    postfixes(current_word, database, user_database_lines);
    pre_and_postfixes(current_word, database, user_database_lines);
}

void first_upper_case(char *word, user_data *database, int user_database_lines){
    char current_word[34];
    strcpy(current_word, word);
    current_word[0] = toupper(word[0]);

    prefixes(current_word, database, user_database_lines);
    postfixes(current_word, database, user_database_lines);
    pre_and_postfixes(current_word, database, user_database_lines);
}

int main(int argc, char *argv[]) {

    user_data user_database[1000] = {{ -1, "", "", ""/*, "" */}};
    int user_database_lines;
    int total_entry_count;

    if (argc < 2) {
        fprintf(stderr, "file of passwords to crack must be provided as (first) argument\n");
        exit(0);
    }

    FILE *passwords = fopen(argv[1], "r");
    if (passwords == NULL) {
        perror("openig passwords file failed");
        return 1;
    }

    char line_buff[120];

    for (user_database_lines = 0; fgets(line_buff, sizeof(line_buff), passwords) != NULL; user_database_lines++) {
        sscanf(line_buff, "%d\t%32s%34s%19[^\n]", &(user_database[user_database_lines].id), user_database[user_database_lines].password_hash, user_database[user_database_lines].mail, user_database[user_database_lines].user_name);
    }

    fclose(passwords);

    // reading passwords check
    // for (int i=0; i<user_database_lines; i++){
    //     printf("%d\t%s\t%s\t%s\n", user_database[i].id, user_database[i].password_hash, user_database[i].mail, user_database[i].user_name);
    // }

    printf("loading dictionary\n");

    FILE *dictionary;

    if (argc > 2) {
        dictionary = fopen(argv[2], "r");
    } else {
        dictionary = fopen(DEFAULT_DICTIONARY, "r");
    }

    if (dictionary == NULL) {
        perror("openig dictionary file failed");
        return 1;
    }

    char c;
    int dictionary_lines = 1;
    while ((c = getc(dictionary)) != EOF){
        if (c == '\n'){
            dictionary_lines++;
        }
    }
    fseek(dictionary, 0, SEEK_SET);
    
    char (*dictionary_database)[30] = calloc(dictionary_lines, sizeof(char[30]));

    for (int i = 0;  i < dictionary_lines; i++) {
        fgets(line_buff, sizeof(line_buff), passwords);
        sscanf(line_buff, "%29s", dictionary_database[i]);
    }

    fclose(dictionary);
    
    printf("dictionary loaded\n");

    // reading dictionary check
    // for (int i=0; i<dictionary_lines; i++){
    //     printf("%s\n", dictionary_database[i]);
    // }

    // test 1 - direct hashes
    char current_hash[33];

    for (int i = 0; i < dictionary_lines; i++){
        direct_hash(dictionary_database[i], user_database, user_database_lines);
    }

    // test 2 - pre/postfixes
    for (int i = 0; i < dictionary_lines; i++){
        prefixes(dictionary_database[i], user_database, user_database_lines);
        postfixes(dictionary_database[i], user_database, user_database_lines);
        pre_and_postfixes(dictionary_database[i], user_database, user_database_lines);
    }

    // test 3 - test 2 + first/all upper case
    for (int i = 0; i < dictionary_lines; i++){
        upper_case(dictionary_database[i], user_database, user_database_lines);
        first_upper_case(dictionary_database[i], user_database, user_database_lines);
    }

    free(dictionary_database);

    return 0;
}
