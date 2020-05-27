#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include<QString>
#include<QLabel>
#include<QLineEdit>
/*列的数量*/
#define COLUMNCOUNT 4

class searchwidget : public QWidget
{
    Q_OBJECT
public:
    explicit searchwidget(QWidget *parent = nullptr);
    void set_arry_empty(void);

signals:

public slots:
public:
    QString searchrow[COLUMNCOUNT];
    QLabel *fir_lbl;
    QLabel *sec_lbl;
    QLabel *thre_lbl;
    QLabel *four_lbl;
//    QLabel *five_lbl;
//    QLabel *six_lbl;
//    QLabel *sev_lbl;

    QLineEdit *fir_let;
    QLineEdit *sec_let;
    QLineEdit *thre_let;
    QLineEdit *four_let;
//    QLineEdit *five_let;
//    QLineEdit *six_let;
//    QLineEdit *sev_let;

    static bool search_flag;
    static int empty_count;

};

#endif // SEARCHWIDGET_H
