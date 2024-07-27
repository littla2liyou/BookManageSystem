// Created by one on 2024/7/14.
//book_search格式/输出，avalibale,排序,change_reader
//return的报错逻辑与

#ifndef BOOK_H
#define BOOK_H

#define FILE_NAME_BOOK_ART "book_art.dat"
#define FILE_NAME_BOOK_SCI "book_sci.dat"
#define FILE_NAME_BOOK_OTH "book_other.dat"
#define MAX_LEN_TITLE 70
#define MAX_LEN_AUTHOR 70
enum Book_status { UNAVAILABLE , AVAILABLE};
enum Book_class{ Arts, Science_and_Engineering, Other};
typedef struct {
    int id;
    char title[70];
    char author[30];
    enum Book_class book_class;
    enum Book_status status;
}Book;
int book_add(const char* title, const char* author, const enum Book_class book_class, enum Book_status book_status);//接收输入的名称、作者和类别，将信息存入对应类别的文件中##有更改参数，新增status
int book_del(int id, enum Book_class book_class);//接收输入id和类别，在对应类别的文件中删除
int book_change(int id,const char* title, const char* author, enum Book_class book_class,enum Book_status book_status);//接收输入的名称、作者和类别，将信息在对应类别的文件中更改
Book* book_search(const char* str);//输入搜索字段，传出搜索结果至缓冲区
//搜索失败
int book_borrow(int id, enum Book_class);//将对应书籍的status设为UNAVAILABLE
int book_return(int id, enum Book_class);//将对应书籍的status设为AVAILABLE
int compare_books(const void *a, const void *b);// 比较函数用于qsort排序
Book* List(const char *filename);//按分类逐个读取各类书籍的文件输出有序结果（按首字母排序）
int open_book_file(const char *filename, int flags, mode_t mode);
void close_book_file(int fd);
#endif //BOOK_H