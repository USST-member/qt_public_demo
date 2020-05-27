#ifndef TABLE_MAINWINDOW_H
#define TABLE_MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include<QTableWidget>
#include<QDialog>
#include<QLabel>
#include"searchwidget.h"
#include <QTimer>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlDriver>
#include<QtSql/QSqlRecord>
#include<QtSql/QSqlError>
#include<QUdpSocket>

namespace Ui {
class table_MainWindow;
}

class table_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit table_MainWindow(QWidget *parent = nullptr);
    ~table_MainWindow();
    void search_widgetshow(void);
    void addrowfun(void);
    void update_newrow(void);
    void get_timefun(void);
    void show_searchinfo(QString str,int index);
    void paintEvent ( QPaintEvent * event) ;
    bool init_sqlite(void);
    void convert_sqlite_table(void);
    void update_row_item(void);

    void udp_init(void);;


private:
    Ui::table_MainWindow *ui;
public:

    /*工具栏和菜单栏*/
    QMenu   *menu;
    QAction *search_row;
    QToolBar *toolbar;
    QTableWidget *table;

    static int RowCount;
    static bool update_flag;
    searchwidget *search_widget;
    QTimer *get_let_time;

    /*储存某一行的信息*/
    QString newrow[COLUMNCOUNT];

    /*数据库*/
    QSqlDatabase db;

    QUdpSocket *udpsocket;
    int udp_port;//udp端口参数
    bool udp_is_stated;
    QString address;
};

#endif // TABLE_MAINWINDOW_H
