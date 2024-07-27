#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <gtk/gtk.h>

int get_top_readerID(const char* filename) {
    struct stat st;
    int id_top;
    if (stat(filename, &st) == 0) {
        long file_size = st.st_size;
        long struct_size = sizeof(Reader);
        id_top = file_size / struct_size;
        printf("Size of Reader struct: %lu bytes\n", struct_size);//测试
        printf("Size of file: %ld bytes\n", file_size);
        printf("Number of Reader structs in file: %ld\n", id_top);

    } else {
        id_top = 1;
        perror("stat");
    }
    return id_top;
}
int register_reader(const char* name, const char* password) {
    if((sizeof(name) == 0) || (sizeof(password) == 0)) {
        return 1;
    }//错误检查：防止录入非法账号
    int fd = open(FILE_NAME_READER, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("Failed to open database file");
        exit(1);
    }

    Reader reader;
    reader.id = lseek(fd, 0, SEEK_END) / sizeof(reader);
    g_print("%d\n",reader.id);//测试
    strncpy(reader.name, name, sizeof(reader.name) - 1);
    reader.name[sizeof(reader.name) - 1] = '\0';
    strncpy(reader.password, password, sizeof(reader.password) - 1);
    reader.password[sizeof(reader.password) - 1] = '\0';

    if (write(fd, &reader, sizeof(Reader)) == -1) {
        perror("Failed to write reader to database");
        close(fd);
        exit(1);
    }

    close(fd);
    return 0;
}

int login(const char* name, const char* password) {
    int fd = open(FILE_NAME_READER, O_RDWR);
    if (fd == -1) {
        perror("Failed to open database file");
        return 0;
    }
    int fd_1 = open(FILE_NAME_MANAGER, O_RDWR);
    if (fd_1 == -1) {
        perror("Failed to open database file");
        return 0;
    }

    Reader reader;
    ssize_t read_bytes;

    while ((read_bytes = read(fd_1, &reader, sizeof(Reader))) > 0) {
        if (strncmp(reader.name, name, USER_NAME_LEN) == 0 && strncmp(reader.password, password, USER_PASSWD_LEN) == 0) {
            close(fd_1);
            close(fd);
            return 2; // 登录成功
        }
    }
    close(fd_1);

    while ((read_bytes = read(fd, &reader, sizeof(Reader))) > 0) {
        if (strncmp(reader.name, name, USER_NAME_LEN) == 0 && strncmp(reader.password, password, USER_PASSWD_LEN) == 0) {
            close(fd);
            return 1; // 登录成功
        }
    }

    close(fd);
    return 0; // 登录失败
}//return 0返回错误处理

int reader_add(const char* name,const char *password) {
    if((sizeof(name) == 0) || (sizeof(password) == 0)) {
        return 1;
    }
    int fd = open(FILE_NAME_READER, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("Failed to open database file");
        return 1;
    }

    Reader reader;
    reader.id = (lseek(fd, 0, SEEK_END) / sizeof(Reader)) + 1;
    strncpy(reader.name, name, sizeof(reader.name) - 1);
    reader.name[sizeof(reader.name) - 1] = '\0';
    strncpy(reader.password, password, sizeof(reader.password) - 1);
    reader.password[sizeof(reader.password) - 1] = '\0';

    if (write(fd, &reader, sizeof(Reader)) == -1) {
        perror("Failed to write reader to dat");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}//return 0代表正常写入

Reader* reader_search_name(const char* name) {
    int count_s_reader = 0;

    Reader *reader = malloc(sizeof(Reader) * 1000); // 假设最多有1000个读者
    if (!reader) {
        g_print("Memory allocation failed\n");
        return NULL;
    }
    ssize_t read_bytes;
    count_s_reader = 1;

    int fd = open(FILE_NAME_READER, O_RDONLY);
    if (fd == -1) {
        g_print("Failed to open database file\n");
        free(reader);
        return NULL;
    }

    while ((read_bytes = read(fd, &reader[count_s_reader], sizeof(Reader))) == sizeof(Reader)) {
        if (strncmp(reader[count_s_reader].name, name, USER_NAME_LEN) == 0) {

            g_print("Search succeeded\n");
            count_s_reader++;
        }
    }

    close(fd);
    if (count_s_reader == 0) {
        free(reader);
        reader = NULL;
    }
    reader[0].id = count_s_reader;
    return reader;
}
int reader_del(int id) {
    int fd, temp_fd;
    Reader reader;
    int found = 0,temp_id = 0; // 用于更新readerID的临时变量
    // 打开要删除reader的文件
    fd = open(FILE_NAME_READER, O_RDWR, 0644);
    temp_fd = open("temp_reader.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        g_print("Failed to open database file");
        return 1;
    }
    while (read(fd, &reader, sizeof(Reader)) == sizeof(Reader)) {
        if (reader.id == id) {
            found = 1;
        } else {
            // 更新readerID并写入临时文件
            reader.id = ++temp_id;
            if (write(temp_fd, &reader, sizeof(Reader)) != sizeof(Reader)) {
                perror("write");
                close(fd);
                close(temp_fd);
                return 1; // 返回1表示失败
            }
        }
    }
    close(fd);
    close(temp_fd);
    // 删除旧文件，重命名临时文件
    if (remove(FILE_NAME_READER) == -1 || rename("temp_reader.dat", FILE_NAME_READER) == -1) {
        perror("remove/rename");
        return 1; // 返回1表示失败
    }
    if (found) {
        printf("Reader with ID %d deleted successfully.\n", id);
        return 0; // 返回0表示成功
    } else {
        printf("Reader with ID %d not found.\n", id);
        return 1; // 返回1表示失败
    }
}

int reader_change(int id, const char* name, const char* password) {
    int fd;
    Reader reader;
    int found = 0; // 用于更新readerID的临时变量
    // 打开要删除reader的文件
    fd = open(FILE_NAME_READER, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        g_print("Failed to open database file");
        return 1;
    }
    // 查找并更新图书信息
    while (read(fd, &reader, sizeof(Reader)) == sizeof(Reader)) {
        if (reader.id == id) {
            found = 1;
            // 将文件指针移回并写入更新后的数据
            lseek(fd, -sizeof(Reader), SEEK_CUR);
            // 更新图书信息
            strcpy(reader.name, name);
            strcpy(reader.password, password);


            if (write(fd, &reader, sizeof(Reader)) != sizeof(Reader)) {
                perror("write");
                close(fd);
                return 1;
            }
            printf("Reader with ID %d updated successfully.\n", id);
            break;
        }
    }

    close(fd);

    if (!found) {
        printf("Reader with ID %d not found.\n", id);
    }
    return 0;
}

Reader reader_search_id(int id) {
    Reader reader;
    reader.id = -1;
    ssize_t read_bytes;
    int buffer_size = sizeof(Reader);

    int fd = open(FILE_NAME_READER, O_RDONLY);
    if (fd == -1) {
        g_print("Failed to open database file");
        return reader;
    }
    while ((read_bytes = read(fd, &reader, sizeof(Reader))) == sizeof(Reader)) {
        if (read_bytes != sizeof(Reader)) {
            g_print("Read bytes size mismatch\n");
            close(fd);
            return reader;
        }
        if (id == reader.id) {
            close(fd);
            g_print("search successed:%d\n",reader.id);
            return reader; // 搜索成功
        }
    }
    reader.id = -1;
    close(fd);
    return reader;

}
