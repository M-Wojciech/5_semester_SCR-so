#include <stdio.h>
#include <openssl/evp.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_DICTIONARY "./dictionaries/mini_dictionary.txt"

typedef struct {
    int id;
    char password_hash[33];
    char mail[35];
    char user_name[20];
    // char password[30];
} user_data;

user_data user_database[1000] = {{ -1, "", "", ""/*, "" */}};
int total_entry_count;

char dictionary_database[1000][30]; 
int dictionary_lines;


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


int main(int argc, char *argv[]) {

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

    for (total_entry_count = 0; fgets(line_buff, sizeof(line_buff), passwords) != NULL; total_entry_count++) {
        sscanf(line_buff, "%d\t%32s%34s%19[^\n]", &(user_database[total_entry_count].id), user_database[total_entry_count].password_hash, user_database[total_entry_count].mail, user_database[total_entry_count].user_name);
    }

    fclose(passwords);

    // reading passwords check
    // for (int i=0; i<total_entry_count; i++){
    //     printf("%d\t%s\t%s\t%s\n", user_database[i].id, user_database[i].password_hash, user_database[i].mail, user_database[i].user_name);
    // }

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

    for (dictionary_lines = 0; fgets(line_buff, sizeof(line_buff), dictionary) != NULL; dictionary_lines++) {
        sscanf(line_buff, "%29s", dictionary_database[dictionary_lines]);
    }

    // reading dictionary check
    // for (int i=0; i<dictionary_lines; i++){
    //     printf("%s\n", dictionary_database[i]);
    // }

    fclose(dictionary);

    char current_hash[33];
    char current_word[34];
    char number[3];
    char second_number[3];

    for (int i = 0; i < dictionary_lines; i++){
        for (int k = 1; k < 100; k++){
            snprintf(number, sizeof(number), "%d", k);

            strcpy(current_word, dictionary_database[i]);
            strcat(current_word, number);
            bytes2md5(current_word, strlen(current_word), current_hash);
            // printf("word: %s\thash: %s\n", current_word, current_hash);

            for (int j = 0; j < total_entry_count; j++){
                if ( !strcmp(current_hash, user_database[j].password_hash)){
                    printf("Password for %s is %s\n", user_database[j].mail, current_word);
                    break;
                }
            }

            strcpy(current_word, number);
            strcat(current_word, dictionary_database[i]);
            bytes2md5(current_word, strlen(current_word), current_hash);

            for (int j = 0; j < total_entry_count; j++){
                if ( !strcmp(current_hash, user_database[j].password_hash)){
                    printf("Password for %s is %s\n", user_database[j].mail, current_word);
                    break;
                }
            }

            for (int l = 1; l < 100; l++){
                snprintf(second_number, sizeof(number), "%d", l);
                strcpy(current_word, number);
                strcat(current_word, dictionary_database[i]);
                strcat(current_word, second_number);
                // printf("%s\n", current_word);
                bytes2md5(current_word, strlen(current_word), current_hash);

                for (int j = 0; j < total_entry_count; j++){
                    if ( !strcmp(current_hash, user_database[j].password_hash)){
                        printf("Password for %s is %s\n", user_database[j].mail, current_word);
                        break;
                    }
                }
            }
        }
    }
        for (int i = 0; i < dictionary_lines; i++){

        bytes2md5(dictionary_database[i], strlen(dictionary_database[i]), current_hash);

        for (int j = 0; j < total_entry_count; j++){
            if ( !strcmp(current_hash, user_database[j].password_hash)){
                printf("Password for %s is %s\n", user_database[j].mail, dictionary_database[i]);
                break;
            }
        }
    }

}