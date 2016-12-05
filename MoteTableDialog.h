#ifndef MOTETABLEDIALOG_H
#define MOTETABLEDIALOG_H

#include <QSqlTableModel>
#include <QTableView>
#include <QDialog>
#include <QLayout>
//#include <QTextCodec>
//#include "ui_MoteTableDialog.h"
/*
namespace Ui
{
    class MoteTableDialog;
}
//*/
class CMoteTableDialog : public QDialog
{
    Q_OBJECT
public:
    CMoteTableDialog(QWidget *parent = 0);
    ~CMoteTableDialog();

private:
    //Ui::MoteTableDialog     *ui;
    //QDialog                 *m_MoteDialog;
    QSqlTableModel          *m_MoteTableModel;
    QTableView              *m_MoteTableView;
};

#endif // MOTETABLEDIALOG_H
