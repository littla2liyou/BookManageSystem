//
// Created by one on 2024/7/14.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "book.h"

int open_book_file(const char *filename, int flags, mode_t mode) {
    int fd = open(filename, flags, mode);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    return fd;
}
// 关闭文件
void close_book_file(int fd) {
    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}
// 添加图书到指定类别的文件中
int book_add(const char* title, const char* author, const enum Book_class book_class, const enum Book_status book_status) {
    int fd;
    Book book;
    struct stat st;
    int id;
    // 根据图书类别选择对应的文件
    switch (book_class) {
        case 0:
            fd = open(FILE_NAME_BOOK_ART, O_WRONLY | O_CREAT | O_APPEND , 0644);
        if (stat(FILE_NAME_BOOK_ART, &st) == 0) {
            long file_size = st.st_size;
            long struct_size = sizeof(Book);
            id = file_size / struct_size;
            book.id=++id;
        }
        break;
        case 1:
            fd = open(FILE_NAME_BOOK_SCI, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (stat(FILE_NAME_BOOK_SCI, &st) == 0) {
            long file_size = st.st_size;
            long struct_size = sizeof(Book);
            id = file_size / struct_size;
            book.id=++id;
        }
        break;
        case 2:
            fd = open(FILE_NAME_BOOK_OTH, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (stat(FILE_NAME_BOOK_OTH, &st) == 0) {
            long file_size = st.st_size;
            long struct_size = sizeof(Book);
            id = file_size / struct_size;
            book.id=++id;
        }
        break;
        default:
            printf("Invalid book class.\n");
        return 1; // 返回1表示失败
    }
    if(fd==-1)
    {
        perror("打开数据库文件失败");
        return 1;
    }
    strcpy(book.title,title);
    strcpy(book.author,author);
    book.book_class=book_class;
    book.status=book_status;
    int n_bytes_write=write(fd,&book,sizeof(Book));
    if(n_bytes_write==-1) {
        perror("写入数据库失败");
        return 1;
    }
    close(fd);
    return 0;
}
// 删除指定ID的图书信息
int book_del(const int id, enum Book_class book_class) {
    int fd, temp_fd;
    Book current_book;
    int found = 0,temp_id = 0; // 用于更新图书ID的临时变量

    // 打开要删除图书的文件
    switch (book_class) {
        case Arts:
            fd = open_book_file(FILE_NAME_BOOK_ART, O_RDWR, 0644);
            temp_fd = open_book_file("temp_art.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            break;
        case Science_and_Engineering:
            fd = open_book_file(FILE_NAME_BOOK_SCI, O_RDWR, 0644);
            temp_fd = open_book_file("temp_sci.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            break;
        case Other:
            fd = open_book_file(FILE_NAME_BOOK_OTH, O_RDWR, 0644);
            temp_fd = open_book_file("temp_oth.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            break;
        default:
            printf("Invalid book class.\n");
            return 1; // 返回1表示失败
    }

    while (read(fd, &current_book, sizeof(Book)) == sizeof(Book)) {
        if (current_book.id == id) {
            found = 1;
        } else {
            // 更新图书ID并写入临时文件
            current_book.id = ++temp_id;
            if (write(temp_fd, &current_book, sizeof(Book)) != sizeof(Book)) {
                perror("write");
                close_book_file(fd);
                close_book_file(temp_fd);
                return 1; // 返回1表示失败
            }
        }
    }

    close_book_file(fd);
    close_book_file(temp_fd);

    // 删除旧文件，重命名临时文件
    switch (book_class) {
        case Arts:
            if (remove(FILE_NAME_BOOK_ART) == -1 || rename("temp_art.dat", FILE_NAME_BOOK_ART) == -1) {
                perror("remove/rename");
                return 1; // 返回1表示失败
            }
            break;
        case Science_and_Engineering:
            if (remove(FILE_NAME_BOOK_SCI) == -1 || rename("temp_sci.dat", FILE_NAME_BOOK_SCI) == -1) {
                perror("remove/rename");
                return 1; // 返回1表示失败
            }
            break;
        case Other:
            if (remove(FILE_NAME_BOOK_OTH) == -1 || rename("temp_oth.dat", FILE_NAME_BOOK_OTH) == -1) {
                perror("remove/rename");
                return 1; // 返回1表示失败
            }
            break;
    }

    if (found) {
        printf("Book with ID %d deleted successfully.\n", id);
        return 0; // 返回0表示成功
    } else {
        printf("Book with ID %d not found.\n", id);
        return 1; // 返回1表示失败
    }
}
int book_change(int id, const char* title, const char* author,enum Book_class book_class, enum Book_status book_status) {
    int fd;
    Book current_book;
    int found = 0;

    // 打开要修改图书的文件
    switch (book_class) {
        case Arts:
            fd = open_book_file(FILE_NAME_BOOK_ART, O_RDWR, 0644);
        break;
        case Science_and_Engineering:
            fd = open_book_file(FILE_NAME_BOOK_SCI, O_RDWR, 0644);
        break;
        case Other:
            fd = open_book_file(FILE_NAME_BOOK_OTH, O_RDWR, 0644);
        break;
        default:
            printf("Invalid book class.\n");
        return 1;
    }

    // 查找并更新图书信息
    while (read(fd, &current_book, sizeof(Book)) == sizeof(Book)) {
        if (current_book.id == id) {
            found = 1;
            // 更新图书信息
            strncpy(current_book.title, title, sizeof(current_book.title)-1);
            strncpy(current_book.author, author, sizeof(current_book.author)-1);

            // 将文件指针移回并写入更新后的数据
            lseek(fd, -sizeof(Book), SEEK_CUR);
            if (write(fd, &current_book, sizeof(Book)) != sizeof(Book)) {
                perror("write");
                close_book_file(fd);
                return 1;
            }
            printf("Book with ID %d updated successfully.\n", id);
            break;
        }
    }

    close_book_file(fd);

    if (!found) {
        printf("Book with ID %d not found.\n", id);
    }
    return 0;
}

Book* book_search(const char* str) {
    int fd_art, fd_sci, fd_oth;
    Book *books = malloc(sizeof(Book) * 50);
    Book current_book;
    int found = 0;
    books[0].id = 0;

    // 打开艺术类书籍文件
    fd_art = open_book_file(FILE_NAME_BOOK_ART, O_RDONLY, 0644);
    while (read(fd_art, &current_book, sizeof(Book)) == sizeof(Book)) {
        if(strncmp(current_book.author,str,MAX_LEN_AUTHOR) == 0 || strncmp(current_book.title, str, MAX_LEN_TITLE) == 0) {
            books[found + 1].id = current_book.id;
            strncpy(books[found+1].title, current_book.title, MAX_LEN_TITLE);
            strncpy(books[found+1].author, current_book.author, MAX_LEN_AUTHOR);
            books[found+1].status = current_book.status;
            books[found+1].book_class = current_book.book_class;
            found++;
        }
    }
    close_book_file(fd_art);

    // 打开科学工程类书籍文件
    fd_sci = open_book_file(FILE_NAME_BOOK_SCI, O_RDONLY, 0644);
    while (read(fd_sci, &current_book, sizeof(Book)) == sizeof(Book)) {
        if(strncmp(current_book.author,str,MAX_LEN_AUTHOR) == 0 || 0 == strncmp(current_book.title, str, MAX_LEN_TITLE)) {
            books[found + 1].id = current_book.id;
            strncpy(books[found+1].title, current_book.title, MAX_LEN_TITLE);
            strncpy(books[found+1].author, current_book.author, MAX_LEN_AUTHOR);
            books[found+1].status = current_book.status;
            books[found+1].book_class = current_book.book_class;
            found++;
        }
    }
    close_book_file(fd_sci);

    // 打开其他类书籍文件
    fd_oth = open_book_file(FILE_NAME_BOOK_OTH, O_RDONLY, 0644);
    while (read(fd_oth, &current_book, sizeof(Book)) == sizeof(Book)) {
        if(strncmp(current_book.author,str,MAX_LEN_AUTHOR) == 0 || 0 ==strncmp(current_book.title, str, MAX_LEN_TITLE)) {
            books[found + 1].id = current_book.id;
            strncpy(books[found+1].title, current_book.title, MAX_LEN_TITLE);
            strncpy(books[found+1].author, current_book.author, MAX_LEN_AUTHOR);
            books[found+1].status = current_book.status;
            books[found+1].book_class = current_book.book_class;
            found++;
        }
    }
    close_book_file(fd_oth);

    if (found == 0) {
        return books;
    }
    books[0].id = found;
    return books;
}
int book_borrow(int id, enum Book_class class) {
    int fd_art, fd_sci, fd_oth;
    Book current_book;
    int found = 0;
    switch (class) {
        case Arts:
            // 打开艺术类书籍文件
    fd_art = open_book_file(FILE_NAME_BOOK_ART, O_RDWR, 0644);
    while (read(fd_art, &current_book, sizeof(Book)) == sizeof(Book)) {
        if (current_book.id == id) {
            found = 1;
            if (current_book.status == AVAILABLE) {
                current_book.status = UNAVAILABLE;
                lseek(fd_art, -sizeof(Book), SEEK_CUR);
                if (write(fd_art, &current_book, sizeof(Book)) != sizeof(Book)) {
                    perror("write");
                    close_book_file(fd_art);
                    return 1; // 返回1表示失败
                }
                printf("Book with ID %d borrowed successfully.\n", id);
                return 0;
            }
            else {
                printf("Book with ID %d is already borrowed.\n", id);
                return 2;
            }
            break;
        }
    }
    close_book_file(fd_art);

        break;
        case Science_and_Engineering:
             // 打开科学工程类书籍文件
    fd_sci = open_book_file(FILE_NAME_BOOK_SCI, O_RDWR, 0644);
    while (read(fd_sci, &current_book, sizeof(Book)) == sizeof(Book)) {
        if (current_book.id == id) {
            found = 1;
            if (current_book.status == AVAILABLE) {
                current_book.status = UNAVAILABLE;
                lseek(fd_sci, -sizeof(Book), SEEK_CUR);
                if (write(fd_sci, &current_book, sizeof(Book)) != sizeof(Book)) {
                    perror("write");
                    close_book_file(fd_sci);
                    return 1; // 返回1表示失败
                }
                printf("Book with ID %d borrowed successfully.\n", id);
            } else {
                printf("Book with ID %d is already borrowed.\n", id);
            }
            break;
        }
    }
    close_book_file(fd_sci);
        break;
        case Other:
             // 打开其他类书籍文件
    fd_oth = open_book_file(FILE_NAME_BOOK_OTH, O_RDWR, 0644);
    while (read(fd_oth, &current_book, sizeof(Book)) == sizeof(Book)) {
        if (current_book.id == id) {
            found = 1;
            if (current_book.status == AVAILABLE) {
                current_book.status = UNAVAILABLE;
                lseek(fd_oth, -sizeof(Book), SEEK_CUR);
                if (write(fd_oth, &current_book, sizeof(Book)) != sizeof(Book)) {
                    perror("write");
                    close_book_file(fd_oth);
                    return 1; // 返回1表示失败
                }
                printf("Book with ID %d borrowed successfully.\n", id);
            } else {
                printf("Book with ID %d is already borrowed.\n", id);
            }
            break;
        }
    }
    close_book_file(fd_oth);
        break;
        default:
            printf("Invalid book class.\n");
        return 1; // 返回1表示失败
    }
    if (!found) {
        printf("Book with ID %d not found.\n", id);
        return 1; // 返回1表示失败
    }

    return 0; // 返回0表示成功
}
int book_return(int id, enum Book_class class) {
    int fd_art, fd_sci, fd_oth;
    Book current_book;
    int found = 0;
    switch (class) {
        case Arts:
             // 打开艺术类书籍文件
    fd_art = open_book_file(FILE_NAME_BOOK_ART, O_RDWR, 0644);
    while (read(fd_art, &current_book, sizeof(Book)) == sizeof(Book)) {
        if (current_book.id == id) {
            found = 1;
            if (current_book.status == UNAVAILABLE) {
                current_book.status = AVAILABLE;
                lseek(fd_art, -sizeof(Book), SEEK_CUR);
                if (write(fd_art, &current_book, sizeof(Book)) != sizeof(Book)) {
                    perror("write");
                    close_book_file(fd_art);
                    return 1; // 返回1表示失败
                }
                printf("Book with ID %d returned successfully.\n", id);
            } else {
                printf("Book with ID %d is already available.\n", id);
            }
            break;
        }
    }
    close_book_file(fd_art);
        break;
        case Science_and_Engineering:
            // 打开科学工程类书籍文件
    fd_sci = open_book_file(FILE_NAME_BOOK_SCI, O_RDWR, 0644);
    while (read(fd_sci, &current_book, sizeof(Book)) == sizeof(Book)) {
        if (current_book.id == id) {
            found = 1;
            if (current_book.status == UNAVAILABLE) {
                current_book.status = AVAILABLE;
                lseek(fd_sci, -sizeof(Book), SEEK_CUR);
                if (write(fd_sci, &current_book, sizeof(Book)) != sizeof(Book)) {
                    perror("write");
                    close_book_file(fd_sci);
                    return 1; // 返回1表示失败
                }
                printf("Book with ID %d returned successfully.\n", id);
            } else {
                printf("Book with ID %d is already available.\n", id);
            }
            break;
        }
    }
    close_book_file(fd_sci);
        break;
        case Other:
             // 打开其他类书籍文件
    fd_oth = open_book_file(FILE_NAME_BOOK_OTH, O_RDWR, 0644);
    while (read(fd_oth, &current_book, sizeof(Book)) == sizeof(Book)) {
        if (current_book.id == id) {
            found = 1;
            if (current_book.status == UNAVAILABLE) {
                current_book.status = AVAILABLE;
                lseek(fd_oth, -sizeof(Book), SEEK_CUR);
                if (write(fd_oth, &current_book, sizeof(Book)) != sizeof(Book)) {
                    perror("write");
                    close_book_file(fd_oth);
                    return 1; // 返回1表示失败
                }
                printf("Book with ID %d returned successfully.\n", id);
            } else {
                printf("Book with ID %d is already available.\n", id);
            }
            break;
        }
    }
    close_book_file(fd_oth);
        break;
        default:
            printf("Invalid book class.\n");
        return 1; // 返回1表示失败
    }
    if (!found) {
        printf("Book with ID %d not found.\n", id);
        return 1; // 返回1表示失败
    }

    return 0; // 返回0表示成功
}
// 打开并排序输出书籍列表
Book* List(const char *filename) {
    Book *book = malloc(sizeof(Book) * 1000);
    book[0].id = 0;
    // 打开文件
    int num_books = 1;
    int fd = open(filename, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("Failed to open file");
        return book;
    }

    // 读取文件内容
    Book temp_book;
    while (read(fd, &temp_book, sizeof(Book)) > 0) {
        book[num_books] = temp_book;
        num_books++;
    }
    close(fd);

    // 按书名排序
    qsort(book+1, num_books, sizeof(Book), compare_books);
    book[0].id = num_books;
    return book;
}

int compare_books(const void *a, const void *b) {
    return strcmp(((Book *)a)->title, ((Book *)b)->title);
}