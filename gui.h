#ifndef LOGIN_H
#define LOGIN_H
//改进点：错误响应，在搜索时添加book的类别，记录读者借书情况，改善UI
void create_login_window();
void create_main_window_reader();
void create_main_window_manager();

void on_login_button_clicked(GtkWidget *widget, gpointer data);
void on_register_button_clicked(GtkWidget *widget, gpointer data);

void show_info(const char* text);

void r_add_button_clicked(GtkWidget *widget, gpointer data);
void r_add_add_button_clicked(GtkWidget *widget, gpointer data);

void r_del_button_clicked(GtkWidget *widget, gpointer data);
void r_del_del_button_clicked(GtkWidget *widget, gpointer data);

void r_ch_button_clicked(GtkWidget *widget, gpointer data);
void r_ch_ch_button_clicked(GtkWidget *widget, gpointer data);

void r_search_id_button_clicked(GtkWidget *widget, gpointer data);
void r_s_search_id_button_clicked(GtkWidget *widget, gpointer data);

void r_search_name_button_clicked(GtkWidget *widget, gpointer data);
void r_s_search_name_button_clicked(GtkWidget *widget, gpointer data);

void r_list_button_clicked(GtkWidget *widget, gpointer data);

void b_add_button_clicked(GtkWidget *widget, gpointer data);
void b_add_add_button_clicked(GtkWidget *widget, gpointer data);

void b_del_button_clicked(GtkWidget *widget, gpointer data);
void b_del_del_button_clicked(GtkWidget *widget, gpointer data);

void b_ch_button_clicked(GtkWidget *widget, gpointer data);
void b_ch_ch_button_clicked(GtkWidget *widget, gpointer data);

void b_search_button_clicked(GtkWidget *widget, gpointer data);
void b_s_search_button_clicked(GtkWidget *widget, gpointer data);

void b_borrow_button_clicked(GtkWidget *widget, gpointer data);
void b_b_borrow_button_clicked(GtkWidget *widget, gpointer data);
void b_return_button_clicked(GtkWidget *widget, gpointer data);
void b_r_return_button_clicked(GtkWidget *widget, gpointer data);
void b_list_button_clicked(GtkWidget *widget, gpointer data);
#endif // LOGIN_H
