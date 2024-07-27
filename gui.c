#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "gui.h"

#include "book.h"
#include "user.h"

static GtkWidget *entries[5];
GtkWidget *login_window;

void create_login_window() {
    GtkWidget *grid;
    GtkWidget *name_label;
    GtkWidget *name_entry;
    GtkWidget *password_label;
    GtkWidget *password_entry;
    GtkWidget *login_button;
    GtkWidget *register_button;

    gtk_init(NULL, NULL);

    login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(login_window), "User Login");
    gtk_window_set_default_size(GTK_WINDOW(login_window), 500, 300);
    gtk_container_set_border_width(GTK_CONTAINER(login_window), 10);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(login_window), grid);

    name_label = gtk_label_new("Username:");
    name_entry = gtk_entry_new();
    password_label = gtk_label_new("Password:");
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    login_button = gtk_button_new_with_label("Login");
    register_button = gtk_button_new_with_label("Register");

    entries[0] = name_entry;
    entries[1] = password_entry;
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), entries);
    g_signal_connect(register_button, "clicked", G_CALLBACK(on_register_button_clicked), entries);
    g_signal_connect(login_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), login_button, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), register_button, 1, 2, 1, 1);

    gtk_widget_show_all(login_window);
}

void on_login_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;
    const char *name = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const char *password = gtk_entry_get_text(GTK_ENTRY(entries[1]));

    if (login(name, password) == 1) {
        gtk_widget_hide(login_window);
        create_main_window_reader();
        g_print("Login successful\n");
    }
    else if(login(name, password) == 2) {
        gtk_widget_hide(login_window);
        create_main_window_manager();
        g_print("Manager login successful\n");
    }
    else {
        g_print("Login failed\n");
        gtk_entry_set_text(GTK_ENTRY(entries[0]), "Failed to login");
        gtk_entry_set_text(GTK_ENTRY(entries[1]), "");
    }
}

void on_register_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;
    const char *name = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const char *password = gtk_entry_get_text(GTK_ENTRY(entries[1]));

    if (register_reader(name, password) == 0) {
        g_print("Registration successful\n");
        gtk_entry_set_text(GTK_ENTRY(entries[0]), "Registration successful");
        gtk_entry_set_text(GTK_ENTRY(entries[1]), "");
    } else {
        g_print("Registration failed\n");
        gtk_entry_set_text(GTK_ENTRY(entries[0]), "failed to register");
        gtk_entry_set_text(GTK_ENTRY(entries[1]), "");
    }
}

void create_main_window_reader() {
    // 创建主窗口
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *button_search, *button_list, *button_borrow, *button_return, *button_exit;
    GtkWidget *label_welcome;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Book Management System");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    button_search = gtk_button_new_with_label("Search Book");
    button_list = gtk_button_new_with_label("List Books");
    button_borrow = gtk_button_new_with_label("Borrow Book");
    button_return = gtk_button_new_with_label("Return Book");
    button_exit = gtk_button_new_with_label("Exit");
    label_welcome = gtk_label_new("Hello, reader");

    gtk_box_pack_start(GTK_BOX(vbox), label_welcome, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button_search, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button_list, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button_borrow, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button_return, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button_exit, FALSE, FALSE, 5);

    // 连接信号处理函数（例如按下按钮后触发相应的功能）
    g_signal_connect(button_list, "clicked", G_CALLBACK(b_list_button_clicked),NULL);
    g_signal_connect(button_search, "clicked",G_CALLBACK(b_search_button_clicked),NULL);
    g_signal_connect(button_borrow, "clicked",G_CALLBACK(b_borrow_button_clicked),NULL);
    g_signal_connect(button_return, "clicked",G_CALLBACK(b_return_button_clicked),NULL);
    g_signal_connect(button_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
}

void create_main_window_manager() {
    // 创建主窗口和侧边栏的代码
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button_search, *button_list, *button_borrow, *button_return, *button_exit, *button_ch, *button_add, *button_del;
    GtkWidget *button_r_list, *button_r_search_id, *button_r_search_name, *button_r_add, *button_r_del, *button_r_ch;
    GtkWidget *label_welcome, *label_book, *label_reader;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Book Management System");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    label_welcome = gtk_label_new("Hello, manager!");

    label_book = gtk_label_new("BOOK");
    button_search = gtk_button_new_with_label("Search Book");
    button_list = gtk_button_new_with_label("List Books");
    button_borrow = gtk_button_new_with_label("Borrow Book");
    button_return = gtk_button_new_with_label("Return Book");
    button_ch = gtk_button_new_with_label("Change Book");
    button_add = gtk_button_new_with_label("Add Book");
    button_del = gtk_button_new_with_label("Delete Book");

    label_reader = gtk_label_new("READER");
    button_r_add = gtk_button_new_with_label("Add Reader");
    button_r_del = gtk_button_new_with_label("Delete Reader");
    button_r_ch = gtk_button_new_with_label("Change Reader");
    button_r_list = gtk_button_new_with_label("List Readers");
    button_r_search_id = gtk_button_new_with_label("Search Reader with ID");
    button_r_search_name = gtk_button_new_with_label("Search Reader with Name");
    button_exit = gtk_button_new_with_label("Exit");

    // 连接信号处理函数（例如按下按钮后触发相应的功能）
    g_signal_connect(button_add, "clicked", G_CALLBACK(b_add_button_clicked),NULL);
    g_signal_connect(button_del, "clicked", G_CALLBACK(b_del_button_clicked),NULL);
    g_signal_connect(button_ch, "clicked", G_CALLBACK(b_ch_button_clicked),NULL);
    g_signal_connect(button_list, "clicked", G_CALLBACK(b_list_button_clicked),NULL);
    g_signal_connect(button_search, "clicked",G_CALLBACK(b_search_button_clicked),NULL);
    g_signal_connect(button_borrow, "clicked",G_CALLBACK(b_borrow_button_clicked),NULL);
    g_signal_connect(button_return, "clicked",G_CALLBACK(b_return_button_clicked),NULL);
    g_signal_connect(button_r_add, "clicked", G_CALLBACK(r_add_button_clicked),NULL);
    g_signal_connect(button_r_del, "clicked", G_CALLBACK(r_del_button_clicked),NULL);
    g_signal_connect(button_r_ch, "clicked", G_CALLBACK(r_ch_button_clicked),NULL);
    g_signal_connect(button_r_list, "clicked", G_CALLBACK(r_list_button_clicked),NULL);
    g_signal_connect(button_r_search_id, "clicked",G_CALLBACK(r_search_id_button_clicked),NULL);
    g_signal_connect(button_r_search_name, "clicked",G_CALLBACK(r_search_name_button_clicked),NULL);
    g_signal_connect(button_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_grid_attach(GTK_GRID(grid), label_welcome,0,0,2,1);

    gtk_grid_attach(GTK_GRID(grid),label_book,0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),button_add,0,2,1,1);
    gtk_grid_attach(GTK_GRID(grid),button_del,0,3,1,1);
    gtk_grid_attach(GTK_GRID(grid),button_ch,0,4,1,1);
    gtk_grid_attach(GTK_GRID(grid),button_borrow,0,5,1,1);
    gtk_grid_attach(GTK_GRID(grid),button_return,0,6,1,1);
    gtk_grid_attach(GTK_GRID(grid),button_list,0,7,1,1);
    gtk_grid_attach(GTK_GRID(grid),button_search,0,8,1,1);

    gtk_grid_attach(GTK_GRID(grid),label_reader,1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),button_r_add,1,2,1,1);
    gtk_grid_attach(GTK_GRID(grid),button_r_del,1,3,1,1);
    gtk_grid_attach(GTK_GRID(grid),button_r_ch,1,4,1,1);
    gtk_grid_attach(GTK_GRID(grid),button_r_search_id,1,5,1,1);
    gtk_grid_attach(GTK_GRID(grid),button_r_search_name,1,6,1,1);
    gtk_grid_attach(GTK_GRID(grid),button_r_list,1,7,1,1);

    gtk_grid_attach(GTK_GRID(grid),button_exit,0,9,2,1);

    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    create_login_window();
    gtk_main();

    return 0;
}

void show_info(const char* text) {
    GtkWidget *PopupWindow;
    GtkWidget *info_label;

    PopupWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL); // 使用GTK_WINDOW_TOPLEVEL而不是GTK_WINDOW_POPUP
    info_label = gtk_label_new(text);

    gtk_window_set_title(GTK_WINDOW(PopupWindow), "Popup Window");
    gtk_window_set_default_size(GTK_WINDOW(PopupWindow), 200, 100);
    gtk_container_add(GTK_CONTAINER(PopupWindow), info_label);
    gtk_widget_show_all(PopupWindow);
}

void b_list_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *scroll;
    GtkWidget *list_box;
    GtkWidget *label;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "List of Books");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 700);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);

    list_box = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scroll), list_box);

    label = gtk_label_new("ART");
    gtk_list_box_insert(GTK_LIST_BOX(list_box), label, -1);
    Book* books_1 = List(FILE_NAME_BOOK_ART);
    for (int i = 1; i < books_1[0].id; i++) {
        char info[256];
        if(books_1[i].status == AVAILABLE)
            snprintf(info, sizeof(info), "ID: %d, Title:%s, Author: %s, Availability:available", books_1[i].id, books_1[i].title,books_1[i].author);
        else
            snprintf(info, sizeof(info), "ID: %d, Title:%s, Author: %s, Availability:unavailable", books_1[i].id, books_1[i].title,books_1[i].author);
        label = gtk_label_new(info);
        gtk_widget_set_halign(label, GTK_ALIGN_START);
        gtk_list_box_insert(GTK_LIST_BOX(list_box), label, -1);
    }
    free(books_1);

    label = gtk_label_new("SCIENCE & ENGINEERING");
    gtk_list_box_insert(GTK_LIST_BOX(list_box), label, -1);
    Book* books_2 = List(FILE_NAME_BOOK_SCI);
    for (int i = 1; i < books_2[0].id; i++) {
        char info[256];
        if(books_2[i].status == AVAILABLE)
            snprintf(info, sizeof(info), "ID: %d, Title:%s, Author: %s, Availability:available", books_2[i].id, books_2[i].title,books_2[i].author);
        else
            snprintf(info, sizeof(info), "ID: %d, Title:%s, Author: %s, Availability:unavailable", books_2[i].id, books_2[i].title,books_2[i].author);
        label = gtk_label_new(info);
        gtk_widget_set_halign(label, GTK_ALIGN_START);
        gtk_list_box_insert(GTK_LIST_BOX(list_box), label, -1);
    }
    free(books_2);

    label = gtk_label_new("OTHER");
    gtk_list_box_insert(GTK_LIST_BOX(list_box), label, -1);
    Book* books_3 = List(FILE_NAME_BOOK_OTH);
    for (int i = 1; i < books_3[0].id; i++) {
        char info[256];
        if(books_3[i].status == AVAILABLE)
            snprintf(info, sizeof(info), "ID: %d, Title:%s, Author: %s, Availability:available", books_3[i].id, books_3[i].title,books_3[i].author);
        else
            snprintf(info, sizeof(info), "ID: %d, Title:%s, Author: %s, Availability:unavailable", books_3[i].id, books_3[i].title,books_3[i].author);
        label = gtk_label_new(info);
        gtk_widget_set_halign(label, GTK_ALIGN_START);
        gtk_list_box_insert(GTK_LIST_BOX(list_box), label, -1);
    }
    free(books_3);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);

    gtk_widget_show_all(window);
}

void b_add_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *title_label, *class_label, *author_label, *availibility_label, *tips_1, *tips_2;
    GtkWidget *title_entry, *class_entry, *author_entry, *availability_entry;
    GtkWidget *add_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "add book");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    title_label = gtk_label_new("Title:");
    title_entry = gtk_entry_new();
    author_label = gtk_label_new("Author");
    author_entry = gtk_entry_new();
    class_label = gtk_label_new("Class:");
    class_entry = gtk_entry_new();
    tips_1 = gtk_label_new("choose 0 for ART, 1 for Science and Engineering, 2 for others");
    availability_entry = gtk_entry_new();
    availibility_label = gtk_label_new("Availablility:");
    tips_2 = gtk_label_new("Type 0 for available, 1 for not");

    add_button = gtk_button_new_with_label("add");

    entries[0] = title_entry;
    entries[1] = author_entry;
    entries[2] = class_entry;
    entries[3] = availability_entry;
    g_signal_connect(add_button, "clicked", G_CALLBACK(b_add_add_button_clicked), entries);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL); // 更改为销毁窗口

    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), title_entry, 1, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), author_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), author_entry, 1, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), class_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), class_entry, 1, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), tips_1, 1, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), availibility_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), availability_entry, 1, 4, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), tips_2, 1, 5, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), add_button, 0, 6, 3, 1);

    gtk_widget_show_all(window);
}
void b_add_add_button_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = data;
    const char *title = gtk_entry_get_text(entries[0]);
    const char *author = gtk_entry_get_text(entries[1]);
    int class = atoi(gtk_entry_get_text(entries[2]));
    int status = atoi(gtk_entry_get_text(entries[3]));

    if(book_add(title, author, class, status) == 0) {
        show_info("successfully add book");
    }
    else {
        show_info("failed to add book");
    }

    // 销毁 add Book 窗口
    GtkWidget *parent_window = gtk_widget_get_toplevel(widget); // 获取父窗口
    gtk_widget_destroy(parent_window); // 销毁父窗口
}

void b_del_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *id_label, *class_label;
    GtkWidget *id_entry, *class_entry;
    GtkWidget *delete_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "delete book");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    id_label = gtk_label_new("ID:");
    id_entry = gtk_entry_new();
    class_entry = gtk_entry_new();
    class_label = gtk_label_new("CLASS:");
    delete_button = gtk_button_new_with_label("delete");

    entries[0] = id_entry;
    entries[1] = class_entry;
    g_signal_connect(delete_button, "clicked", G_CALLBACK(b_del_del_button_clicked), entries);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL); // 更改为销毁窗口

    gtk_grid_attach(GTK_GRID(grid), id_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), class_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), class_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), delete_button, 0, 2, 2, 1);

    gtk_widget_show_all(window);
}
void b_del_del_button_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = data;
    int id = atoi(gtk_entry_get_text(entries[0]));
    int status = atoi(gtk_entry_get_text(entries[1]));

    if(book_del(id,status) == 0) {
        show_info("successfully delete book");
    }
    else {
        show_info("failed to delete book");
    }

    // 销毁 add Book 窗口
    GtkWidget *parent_window = gtk_widget_get_toplevel(widget); // 获取父窗口
    gtk_widget_destroy(parent_window); // 销毁父窗口
}

void b_borrow_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *id_label, *class_label,*label;
    GtkWidget *borrow_button;
    GtkWidget *id_entry, *class_entry;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"borrow book");
    gtk_window_set_default_size(GTK_WINDOW(window),300,200);
    gtk_container_set_border_width(GTK_CONTAINER(window),10);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid),TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    label = gtk_label_new("Input the id and class of the book you want");
    class_label = gtk_label_new("CLASS:");
    id_label = gtk_label_new("ID:");
    id_entry = gtk_entry_new();
    class_entry = gtk_entry_new();
    borrow_button = gtk_button_new_with_label("borrow");

    entries[0] = id_entry;
    entries[1] = class_entry;
    g_signal_connect(borrow_button, "clicked", G_CALLBACK(b_b_borrow_button_clicked),entries);
    g_signal_connect(window,"destroy",G_CALLBACK(gtk_widget_destroy),NULL);

    gtk_grid_attach(GTK_GRID(grid),label,0,0,2,1);
    gtk_grid_attach(GTK_GRID(grid),id_label,0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),id_entry,1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),class_entry,1,2,1,1);
    gtk_grid_attach(GTK_GRID(grid),class_label,0,2,1,1);
    gtk_grid_attach(GTK_GRID(grid),borrow_button,0,3,2,1);

    gtk_widget_show_all(window);
}
void b_b_borrow_button_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = (GtkEntry **)data;
    int id = atoi(gtk_entry_get_text(entries[0]));
    int class = atoi(gtk_entry_get_text(entries[1]));
    int flag = book_borrow(id,class);
    if (flag == 0) {
        show_info("borrow book successfully");
    }
    else if (flag == 2){
        show_info("Book had beed borrowed");
    }
    else {
        show_info("Some error happened");
    }
    GtkWidget *parent_window = gtk_widget_get_toplevel(widget); // 获取父窗口
    gtk_widget_destroy(parent_window); // 销毁父窗口
}

void b_return_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *id_label, *class_label,*label;
    GtkWidget *return_button;
    GtkWidget *id_entry, *class_entry;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"return book");
    gtk_window_set_default_size(GTK_WINDOW(window),300,200);
    gtk_container_set_border_width(GTK_CONTAINER(window),10);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid),TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    label = gtk_label_new("Input the id and class of the book you want");
    class_label = gtk_label_new("CLASS:");
    id_label = gtk_label_new("ID:");
    id_entry = gtk_entry_new();
    class_entry = gtk_entry_new();
    return_button = gtk_button_new_with_label("return");

    entries[0] = id_entry;
    entries[1] = class_entry;
    g_signal_connect(return_button, "clicked", G_CALLBACK(b_r_return_button_clicked),entries);
    g_signal_connect(window,"destroy",G_CALLBACK(gtk_widget_destroy),NULL);

    gtk_grid_attach(GTK_GRID(grid),label,0,0,2,1);
    gtk_grid_attach(GTK_GRID(grid),id_label,0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),id_entry,1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),class_entry,1,2,1,1);
    gtk_grid_attach(GTK_GRID(grid),class_label,0,2,1,1);
    gtk_grid_attach(GTK_GRID(grid),return_button,0,3,2,1);

    gtk_widget_show_all(window);
}
void b_r_return_button_clicked(GtkWidget *widget, gpointer data){
    GtkEntry **entries = (GtkEntry **)data;
    int id = atoi(gtk_entry_get_text(entries[0]));
    int class = atoi(gtk_entry_get_text(entries[1]));
    int flag = book_return(id,class);
    if (flag == 0) {
        show_info("borrow book successfully");
    }
    else if (flag == 2){
        show_info("Book had beed borrowed");
    }
    else {
        show_info("failed to return book");
    }
    GtkWidget *parent_window = gtk_widget_get_toplevel(widget); // 获取父窗口
    gtk_widget_destroy(parent_window); // 销毁父窗口
}

void b_ch_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *id_label, *title_label, *class_label, *author_label, *availibility_label, *tips_1, *tips_2;
    GtkWidget *id_entry, *title_entry, *class_entry, *author_entry, *availability_entry;
    GtkWidget *change_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "change book");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    title_label = gtk_label_new("Title:");
    title_entry = gtk_entry_new();
    author_label = gtk_label_new("Author");
    author_entry = gtk_entry_new();
    id_label = gtk_label_new("ID:");
    id_entry = gtk_entry_new();
    class_label = gtk_label_new("Class:");
    class_entry = gtk_entry_new();
    tips_1 = gtk_label_new("choose 0 for ART, 1 for Science and Engineering, 2 for others");
    availability_entry = gtk_entry_new();
    availibility_label = gtk_label_new("Availablility:");
    tips_2 = gtk_label_new("Type 0 for available, 1 for not");

    change_button = gtk_button_new_with_label("change");

    entries[0] = title_entry;
    entries[1] = author_entry;
    entries[2] = class_entry;
    entries[3] = availability_entry;
    entries[4] = id_entry;
    g_signal_connect(change_button, "clicked", G_CALLBACK(b_ch_ch_button_clicked), entries);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL); // 更改为销毁窗口

    gtk_grid_attach(GTK_GRID(grid), id_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), title_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), author_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), author_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), class_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), class_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), tips_1, 0, 4, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), availibility_label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), availability_entry, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), tips_2, 0, 6, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), change_button, 0, 7, 2, 1);

    gtk_widget_show_all(window);
}
void b_ch_ch_button_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = data;
    const char *title = gtk_entry_get_text(entries[0]);
    const char *author = gtk_entry_get_text(entries[1]);
    int class = atoi(gtk_entry_get_text(entries[2]));
    int status = atoi(gtk_entry_get_text(entries[3]));
    int id = atoi(gtk_entry_get_text(entries[4]));

    if(book_change(id,title, author, class, status) == 0) {
        show_info("successfully change book");
    }
    else {
        show_info("failed to change book");
    }

    GtkWidget *parent_window = gtk_widget_get_toplevel(widget); // 获取父窗口
    gtk_widget_destroy(parent_window); // 销毁父窗口
}

void b_search_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *tip_label;
    GtkWidget *search_entry;
    GtkWidget *search_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "search book");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    search_entry = gtk_entry_new();
    search_button = gtk_button_new_with_label("search");
    tip_label = gtk_label_new("Input title or author");

    entries[0] = search_entry;
    g_signal_connect(search_button, "clicked", G_CALLBACK(b_s_search_button_clicked), entries);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL); // 更改为销毁窗口

    gtk_grid_attach(GTK_GRID(grid),tip_label,0,0,2,1);
    gtk_grid_attach(GTK_GRID(grid), search_entry, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), search_button, 0, 2, 2, 1);

    gtk_widget_show_all(window);
}
void b_s_search_button_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = data;
    const char* str = gtk_entry_get_text(entries[0]);
    Book *book = book_search(str);
    if(book[0].id == 0) {
        show_info("failed to search book");
        free(book);
    }
    else {
        GtkWidget *window;
        GtkWidget *vbox;
        GtkWidget *scroll;
        GtkWidget *list_box;
        GtkWidget *label;

        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "List of Books");
        gtk_container_set_border_width(GTK_CONTAINER(window), 10);
        gtk_window_set_default_size(GTK_WINDOW(window), 700, 300);

        vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        gtk_container_add(GTK_CONTAINER(window), vbox);

        scroll = gtk_scrolled_window_new(NULL, NULL);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);

        list_box = gtk_list_box_new();
        gtk_container_add(GTK_CONTAINER(scroll), list_box);

        label = gtk_label_new("Tips:for class, 0 means Art, 1 means Science&Engineering, 2 means others");
        gtk_list_box_insert(GTK_LIST_BOX(list_box), label, -1);

        g_print("reader number:%d\n",book[0].id);//测试
        for (int i = 1; i <= book[0].id; i++) {
            char info[256];
            if(book[i].status == AVAILABLE)
                snprintf(info, sizeof(info), "ID: %d, Class: %d, Title:%s, Author: %s, Availability:available", book[i].id, book[i].book_class,book[i].title,book[i].author);
            else
                snprintf(info, sizeof(info), "ID: %d, Class: %d, Title:%s, Author: %s, Availability:unavailable", book[i].id, book[i].book_class, book[i].title,book[i].author);
            label = gtk_label_new(info);
            gtk_widget_set_halign(label, GTK_ALIGN_START);
            gtk_list_box_insert(GTK_LIST_BOX(list_box), label, -1);
        }
        free(book);

        GtkWidget *parent_window = gtk_widget_get_toplevel(widget); // 获取父窗口
        gtk_widget_destroy(parent_window); // 销毁父窗口

        gtk_widget_show_all(window);
    }
    return;
}

void r_add_button_clicked(GtkWidget *widget, gpointer data){
        GtkWidget *window;
        GtkWidget *grid;
        GtkWidget *name_label;
        GtkWidget *name_entry;
        GtkWidget *password_label;
        GtkWidget *password_entry;
        GtkWidget *add_button;

        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "add reader");
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
        gtk_container_set_border_width(GTK_CONTAINER(window), 10);

        grid = gtk_grid_new();
        gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
        gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
        gtk_container_add(GTK_CONTAINER(window), grid);

        name_label = gtk_label_new("Username:");
        name_entry = gtk_entry_new();
        password_label = gtk_label_new("Password:");
        password_entry = gtk_entry_new();
        add_button = gtk_button_new_with_label("add");

        entries[0] = name_entry;
        entries[1] = password_entry;
        g_signal_connect(add_button, "clicked", G_CALLBACK(r_add_add_button_clicked), entries);
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL); // 更改为销毁窗口

        gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), password_label, 0, 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), add_button, 0, 2, 2, 1);

        gtk_widget_show_all(window);
    }

void r_add_add_button_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = data;
    const char *name = gtk_entry_get_text(entries[0]); // 修正为entries[0]获取用户名
    const char *password = gtk_entry_get_text(entries[1]);

    if(reader_add(name, password) == 0) {
        show_info("successfully add reader");
    }
    else {
        show_info("failed to add reader");
    }

    // 销毁 add reader 窗口
    GtkWidget *parent_window = gtk_widget_get_toplevel(widget); // 获取父窗口
    gtk_widget_destroy(parent_window); // 销毁父窗口
}

void r_del_button_clicked(GtkWidget *widget, gpointer data)  {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *name_label, *password_label, *id_label;
    GtkWidget *password_entry, *id_entry, *name_entry;
    GtkWidget *change_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "delete reader");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    id_label = gtk_label_new("ID:");
    id_entry = gtk_entry_new();
    name_label = gtk_label_new("Username:");
    name_entry = gtk_entry_new();
    password_label = gtk_label_new("Password:");
    password_entry = gtk_entry_new();
    change_button = gtk_button_new_with_label("delete");

    entries[0] = id_entry;
    entries[1] = name_entry;
    entries[2] = password_entry;
    g_signal_connect(change_button, "clicked", G_CALLBACK(r_del_del_button_clicked), entries);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL); // 更改为销毁窗口

    gtk_grid_attach(GTK_GRID(grid),id_label,0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid),id_entry,1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), change_button, 0, 3, 2, 1);

    gtk_widget_show_all(window);
}
void r_del_del_button_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = data;
    int id = atoi(gtk_entry_get_text(entries[0]));
    const char *name = gtk_entry_get_text(entries[1]);
    const char *password = gtk_entry_get_text(entries[2]);

    if(reader_del(id) == 0) {
        show_info("successfully delete reader");
    }
    else {
        show_info("failed to delete reader");
    }
    // 销毁 add reader 窗口
    GtkWidget *parent_window = gtk_widget_get_toplevel(widget); // 获取父窗口
    gtk_widget_destroy(parent_window); // 销毁父窗口
}

void r_ch_button_clicked(GtkWidget *widget, gpointer data)  {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *name_label, *password_label, *id_label, *tips_label;
    GtkWidget *password_entry, *id_entry, *name_entry;
    GtkWidget *change_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "change reader");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    id_label = gtk_label_new("ID:");
    id_entry = gtk_entry_new();
    name_label = gtk_label_new("Username:");
    name_entry = gtk_entry_new();
    password_label = gtk_label_new("Password:");
    password_entry = gtk_entry_new();
    change_button = gtk_button_new_with_label("change");
    tips_label = gtk_label_new("Input target ID, and then input new information");

    entries[0] = id_entry;
    entries[1] = name_entry;
    entries[2] = password_entry;
    g_signal_connect(change_button, "clicked", G_CALLBACK(r_ch_ch_button_clicked), entries);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL); // 更改为销毁窗口

    gtk_grid_attach(GTK_GRID(grid),id_label,0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid),id_entry,1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), change_button, 0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), tips_label, 0, 4, 2, 1);

    gtk_widget_show_all(window);
}
void r_ch_ch_button_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = data;
    int id = atoi(gtk_entry_get_text(entries[0]));
    const char *name = gtk_entry_get_text(entries[1]);
    const char *password = gtk_entry_get_text(entries[2]);

    if(reader_change(id, name, password) == 0) {
        show_info("successfully change reader");
    }
    else {
        show_info("failed to change reader");
    }

    // 销毁 add reader 窗口
    GtkWidget *parent_window = gtk_widget_get_toplevel(widget); // 获取父窗口
    gtk_widget_destroy(parent_window); // 销毁父窗口
}

void r_search_id_button_clicked(GtkWidget *widget, gpointer data)  {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *name_label;
    GtkWidget *id_entry;
    GtkWidget *s_name_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "search reader with id");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    name_label = gtk_label_new("ID:");
    id_entry = gtk_entry_new();
    s_name_button = gtk_button_new_with_label("search");

    entries[0] = id_entry;
    g_signal_connect(s_name_button, "clicked", G_CALLBACK(r_s_search_id_button_clicked), entries);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL); // 更改为销毁窗口

    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), s_name_button, 0, 2, 2, 1);

    gtk_widget_show_all(window);
}
void r_s_search_id_button_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = (GtkEntry **)data;
    int id = atoi(gtk_entry_get_text(entries[0]));
    Reader reader = reader_search_id(id);
    if(reader.id < 0) {
        show_info("There are no matching search results.");
    }
    else {
        GtkWidget *PopupWindow;
        GtkWidget *grid;
        GtkWidget *label_1_0;
        GtkWidget *label_1_1;
        GtkWidget *label_2_0;
        GtkWidget *label_2_1;
        GtkWidget *label_3_0;
        GtkWidget *label_3_1;

        PopupWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        char id[10];
        sprintf(id, "%d", reader.id);
        grid = gtk_grid_new();
        gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
        gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
        gtk_container_add(GTK_CONTAINER(PopupWindow), grid);

        gtk_window_set_title(GTK_WINDOW(PopupWindow), "Popup Window");
        gtk_window_set_default_size(GTK_WINDOW(PopupWindow), 300, 200);
        label_1_0 = gtk_label_new("ID:");
        label_1_1 = gtk_label_new(id);
        label_2_0 = gtk_label_new("name:");
        label_2_1 = gtk_label_new(reader.name);
        label_3_0 = gtk_label_new("password");
        label_3_1 = gtk_label_new(reader.password);

        gtk_grid_attach(GTK_GRID(grid), label_1_0, 0, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), label_1_1, 1, 0, 2, 1);
        gtk_grid_attach(GTK_GRID(grid),label_2_0, 0, 1,1, 1);
        gtk_grid_attach(GTK_GRID(grid), label_2_1, 1, 1, 2, 1);
        gtk_grid_attach(GTK_GRID(grid), label_3_0, 0, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), label_3_1, 1, 2, 2, 1);

        gtk_widget_show_all(PopupWindow);
    }

    // 销毁 add reader 窗口
    GtkWidget *parent_window = gtk_widget_get_toplevel(widget); // 获取父窗口
    gtk_widget_destroy(parent_window); // 销毁父窗口
}

void r_search_name_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *name_label;
    GtkWidget *name_entry;
    GtkWidget *s_name_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "search reader with name");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    name_label = gtk_label_new("Username:");
    name_entry = gtk_entry_new();
    s_name_button = gtk_button_new_with_label("search");

    entries[0] = name_entry;
    g_signal_connect(s_name_button, "clicked", G_CALLBACK(r_s_search_name_button_clicked), entries);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL); // 更改为销毁窗口

    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), s_name_button, 0, 2, 2, 1);

    gtk_widget_show_all(window);
}
void r_s_search_name_button_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = (GtkEntry **)data;
    const char *name = gtk_entry_get_text(entries[0]);
    Reader *reader = reader_search_name(name);
    if (reader == NULL) {
        show_info("No matching result.");
        return;
    }

    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *scroll;
    GtkWidget *list_box;
    GtkWidget *label;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "List of Readers");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);

    list_box = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scroll), list_box);

    g_print("reader number:%d\n",reader[0].id);//测试
    for (int i = 1; i < reader[0].id; i++) {
        char info[256];
        snprintf(info, sizeof(info), "ID: %d, Name: %s, Password: %s", reader[i].id, reader[i].name, reader[i].password);
        label = gtk_label_new(info);

        // 设置label左对齐
        gtk_widget_set_halign(label, GTK_ALIGN_START);

        gtk_list_box_insert(GTK_LIST_BOX(list_box), label, -1);
    }

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);

    gtk_widget_show_all(window);

    free(reader); // 记得释放动态分配的内存
    // 销毁 add reader 窗口
    GtkWidget *parent_window = gtk_widget_get_toplevel(widget); // 获取父窗口
    gtk_widget_destroy(parent_window); // 销毁父窗口
}

void r_list_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *scroll;
    GtkWidget *list_box;
    GtkWidget *label;
    int fd;
    Reader reader;
    ssize_t read_bytes;

    fd = open(FILE_NAME_READER, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open readers file");
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(widget),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Failed to open readers file");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "List of Readers");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 700);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);

    list_box = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scroll), list_box);

    while ((read_bytes = read(fd, &reader, sizeof(Reader))) == sizeof(Reader)) {
        char info[256];
        snprintf(info, sizeof(info), "ID: %d, Name: %s, Password: %s", reader.id, reader.name, reader.password);
        label = gtk_label_new(info);
        gtk_widget_set_halign(label, GTK_ALIGN_START);
        gtk_list_box_insert(GTK_LIST_BOX(list_box), label, -1);
    }

    if (read_bytes < 0) {
        perror("Error reading readers file");
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(widget),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error reading readers file");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }

    close(fd);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);

    gtk_widget_show_all(window);
}



