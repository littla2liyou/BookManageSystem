#ifndef USER_H
#define USER_H

#define FILE_NAME_READER "readers.db"
#define FILE_NAME_MANAGER "manager.db"
#define USER_NAME_LEN 32
#define USER_PASSWD_LEN 32


typedef struct {
    int id;
    char name[USER_NAME_LEN];
    char password[USER_PASSWD_LEN];
} Reader;

int get_top_readerID(const char* filename);
int register_reader(const char* name, const char* password);
int login(const char* name, const char* password);
int reader_del(int id);
int reader_add(const char* name,const char *password);
int reader_change(int id, const char* name, const char* password);
Reader reader_search_id(int id);
Reader* reader_search_name(const char* name);

#endif // USER_H
