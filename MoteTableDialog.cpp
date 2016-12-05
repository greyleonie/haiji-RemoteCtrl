#include "MoteTableDialog.h"

CMoteTableDialog::CMoteTableDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setObjectName(tr("MoteTableDialog"));
    this->setWindowTitle(tr("现地站配置"));
    this->setModal(false);

    m_MoteTableModel = new QSqlTableModel(this);
    m_MoteTableModel->setTable("t_mote");
    m_MoteTableModel->setSort(0, Qt::AscendingOrder);
    m_MoteTableModel->select();
    m_MoteTableModel->removeColumn(0);
    m_MoteTableModel->removeColumn(6);
    m_MoteTableModel->setHeaderData(0, Qt::Horizontal, tr("名字"));
    m_MoteTableModel->setHeaderData(1, Qt::Horizontal, tr("Ip地址"));
    m_MoteTableModel->setHeaderData(2, Qt::Horizontal, tr("端口号"));
    m_MoteTableModel->setHeaderData(3, Qt::Horizontal, tr("编组号"));
    m_MoteTableModel->setHeaderData(4, Qt::Horizontal, tr("扫描间隔(秒)"));
    m_MoteTableModel->setHeaderData(5, Qt::Horizontal, tr("最大开度值"));
    //m_MoteTableModel->setHeaderData(6, Qt::Horizontal, tr("开度预设值"));

    m_MoteTableView = new QTableView(this);
    m_MoteTableView->setModel(m_MoteTableModel);

    m_MoteTableView->resize(640, 200);
    //m_MoteTableView->resizeColumnsToContents();
    //m_MoteTableView->resizeRowsToContents();
    //m_MoteTableView->show();

    //QHBoxLayout *layout = new QHBoxLayout();
    //layout->addWidget(m_MoteTableView);
    //this->setLayout(layout);
//    int i = m_MoteTableView->x();
//    int j = m_MoteTableView->y();
    this->setFixedSize(m_MoteTableView->width(), m_MoteTableView->height());
    //this->show();
}

CMoteTableDialog::~CMoteTableDialog()
{
    ;
}
