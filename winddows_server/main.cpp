#include "table_mainwindow.h"
#include <QApplication>
#include"log_dialog.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    log_Dialog log;
    log.exec();
    if(log.isVisible()==false&&log.md5_isok==true)
    {
            table_MainWindow w;
            w.show();
            return a.exec();
    }
    else
    {
        return 0;
    }

}
