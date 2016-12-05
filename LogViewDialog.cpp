#include "LogViewDialog.h"

CLogViewDialog::CLogViewDialog(QWidget *parent, CUserInfo *userInfo)
    : QDialog(parent)
{
    m_UserNameLabel = new QLabel(this);
    m_UserNameLabel->setText(tr("用户名:"));
    m_UserNameEdit = new QLineEdit(this);
    m_UserNameEdit->setFixedWidth(128);
    QRegExp regExp_username("[A-Za-z_][A-Za-z0-9_]{0,15}");
    m_UserNameEdit->setValidator(new QRegExpValidator(regExp_username, this));
    m_UserNameEdit->setText(userInfo->getUsername());

    m_StartDateLabel = new QLabel(this);
    m_StartDateLabel->setText(tr("开始日期:"));
    m_StartDateEdit = new QDateEdit(this);
    m_StartDateEdit->setDate(QDate::currentDate());

    m_EndDateLabel = new QLabel(this);
    m_EndDateLabel->setText(tr("结束日期:"));
    m_EndDateEdit = new QDateEdit(this);
    m_EndDateEdit->setDate(QDate::currentDate());

    m_QueryButton = new QPushButton(this);
    m_QueryButton->setText(tr("查询"));

    m_PrintButton = new QPushButton(this);
    m_PrintButton->setText(tr("打印"));

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addWidget(m_UserNameLabel);
    hlayout->addWidget(m_UserNameEdit);
    hlayout->addWidget(m_StartDateLabel);
    hlayout->addWidget(m_StartDateEdit);
    hlayout->addWidget(m_EndDateLabel);
    hlayout->addWidget(m_EndDateEdit);
    hlayout->addWidget(m_QueryButton);
    hlayout->addWidget(m_PrintButton);

    m_FunctionGroup = new QGroupBox(this);
    m_FunctionGroup->setTitle(tr("查询"));
    m_FunctionGroup->setLayout(hlayout);

    m_OperLogsModel = new QSqlTableModel(this);
    m_OperLogsView = new QTableView(this);
    m_OperLogsView->setModel(m_OperLogsModel);

    m_OperLogsModel->setTable("v_operlogs");
    m_OperLogsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_OperLogsModel->setHeaderData(0, Qt::Horizontal, tr("用户"));
    m_OperLogsModel->setHeaderData(1, Qt::Horizontal, tr("时间"));
    m_OperLogsModel->setHeaderData(2, Qt::Horizontal, tr("操作内容"));
    m_OperLogsModel->setSort(1, Qt::DescendingOrder);
    Query();

    QHBoxLayout *hlayout1 = new QHBoxLayout(this);
    hlayout1->addWidget(m_OperLogsView);

    m_ViewGroup = new QGroupBox(this);
    m_ViewGroup->setTitle(tr("操作记录列表"));
    m_ViewGroup->setLayout(hlayout1);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addWidget(m_FunctionGroup);
    vlayout->addWidget(m_ViewGroup);

    this->setWindowTitle(tr("操作记录"));
    this->setLayout(vlayout);
    this->showMaximized();

    connect(m_QueryButton, SIGNAL(clicked()), this, SLOT(Query()));
    connect(m_PrintButton, SIGNAL(clicked()), this, SLOT(Print()));
}

CLogViewDialog::~CLogViewDialog()
{
    ;
}

void CLogViewDialog::Query()
{
    QDate startDate = m_StartDateEdit->date();
    QDate endDate = m_EndDateEdit->date().addDays(1);
    QString sqlwhere = "opertime>='" + startDate.toString(Qt::ISODate) +
                        "' and opertime<='" + endDate.toString(Qt::ISODate) + "'";
    if(m_UserNameEdit->text() != "")
        sqlwhere += " and username='" + m_UserNameEdit->text() + "'";

    m_OperLogsModel->setFilter(sqlwhere);
    m_OperLogsModel->select();

    m_OperLogsView->resizeColumnsToContents();
}

void CLogViewDialog::Print()
{
    m_Printer = new QPrinter();
    QPrintDialog *printDialog = new QPrintDialog(m_Printer, this);
    if(!printDialog->exec())
        return;

    QDate startDate = m_StartDateEdit->date();
    QDate endDate = m_EndDateEdit->date().addDays(1);
    QString sqlStr = "SELECT * FROM v_operlogs WHERE opertime>='" + startDate.toString(Qt::ISODate) +
                       "' AND opertime<='" + endDate.toString(Qt::ISODate) + "'";
    if(m_UserNameEdit->text() != "")
        sqlStr += " AND username='" + m_UserNameEdit->text() + "'";

    QSqlQuery dbQuery;
    dbQuery.setForwardOnly(true);
    dbQuery.exec(sqlStr);

    QString html;
    html = "<table witdh=\"100%\" border=1 cellspacing=0>\n";
    html += "<tr style=\"font-weight:bold;background-color:lightgray;\">\n";
    html += "<td><center>" + tr("用户名") + "</td>\n";
    html += "<td><center>" + tr("操作时间") + "</td>\n";
    html += "<td><center>" + tr("操作内容") + "</td>\n";
    html += "</tr>\n";
    while(dbQuery.next())
    {
        html += "<tr>\n";
        html += "<td>" + dbQuery.value(0).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(1).toDateTime().toString(Qt::ISODate) + "</td>\n";
        html += "<td>" + dbQuery.value(2).toString() + "</td>\n";
        html += "</tr>\n";
    }
    html += "</table>\n";

    QTextDocument text;
    text.setHtml(html);
    text.print(m_Printer);
}
