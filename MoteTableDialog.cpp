#include "MoteTableDialog.h"

CMoteTableDialog::CMoteTableDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setObjectName(tr("MoteTableDialog"));
    this->setWindowTitle(tr("�ֵ�վ����"));
    this->setModal(false);

    m_MoteTableModel = new QSqlTableModel(this);
    m_MoteTableModel->setTable("t_mote");
    m_MoteTableModel->setSort(0, Qt::AscendingOrder);
    m_MoteTableModel->select();
    m_MoteTableModel->removeColumn(0);
    m_MoteTableModel->removeColumn(6);
    m_MoteTableModel->setHeaderData(0, Qt::Horizontal, tr("����"));
    m_MoteTableModel->setHeaderData(1, Qt::Horizontal, tr("Ip��ַ"));
    m_MoteTableModel->setHeaderData(2, Qt::Horizontal, tr("�˿ں�"));
    m_MoteTableModel->setHeaderData(3, Qt::Horizontal, tr("�����"));
    m_MoteTableModel->setHeaderData(4, Qt::Horizontal, tr("ɨ����(��)"));
    m_MoteTableModel->setHeaderData(5, Qt::Horizontal, tr("��󿪶�ֵ"));
    //m_MoteTableModel->setHeaderData(6, Qt::Horizontal, tr("����Ԥ��ֵ"));

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
