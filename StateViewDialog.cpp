#include "StateViewDialog.h"

CStateViewDialog::CStateViewDialog(QWidget *parent, CUserInfo *userInfo)
{
    m_MoteNameLabel = new QLabel(this);
    m_MoteNameLabel->setText(tr("现地站名:"));
    m_MoteNameEdit = new QLineEdit(this);
    m_MoteNameEdit->setFixedWidth(128);

    m_StartTimeLabel = new QLabel(this);
    m_StartTimeLabel->setText(tr("开始时间:"));
    m_StartTimeEdit = new QDateTimeEdit(this);
    m_StartTimeEdit->setDateTime(QDateTime::currentDateTime());

    m_EndTimeLabel = new QLabel(this);
    m_EndTimeLabel->setText(tr("结束时间:"));
    m_EndTimeEdit = new QDateTimeEdit(this);
    m_EndTimeEdit->setDateTime(QDateTime::currentDateTime());

    m_QueryButton = new QPushButton(this);
    m_QueryButton->setText(tr("查询"));

    m_PrintButton = new QPushButton(this);
    m_PrintButton->setText(tr("打印"));

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addWidget(m_MoteNameLabel);
    hlayout->addWidget(m_MoteNameEdit);
    hlayout->addWidget(m_StartTimeLabel);
    hlayout->addWidget(m_StartTimeEdit);
    hlayout->addWidget(m_EndTimeLabel);
    hlayout->addWidget(m_EndTimeEdit);
    hlayout->addWidget(m_QueryButton);
    hlayout->addWidget(m_PrintButton);

    m_FunctionGroup = new QGroupBox(this);
    m_FunctionGroup->setTitle(tr("查询"));
    m_FunctionGroup->setLayout(hlayout);

    m_StateLogsModel = new QSqlTableModel(this);
    m_StateLogsView = new QTableView(this);
    m_StateLogsView->setModel(m_StateLogsModel);

    m_StateLogsModel->setTable("v_statelogs");
    m_StateLogsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_StateLogsModel->setHeaderData(0, Qt::Horizontal, tr("现地站"));
    m_StateLogsModel->setHeaderData(1, Qt::Horizontal, tr("时间"));
    m_StateLogsModel->setHeaderData(2, Qt::Horizontal, tr("闸门开度"));
    m_StateLogsModel->setHeaderData(3, Qt::Horizontal, tr("闸门开度预设"));
    m_StateLogsModel->setHeaderData(4, Qt::Horizontal, tr("闸门自动控制"));
    m_StateLogsModel->setHeaderData(5, Qt::Horizontal, tr("闸门远程控制"));
    m_StateLogsModel->setHeaderData(6, Qt::Horizontal, tr("闸门全开"));
    m_StateLogsModel->setHeaderData(7, Qt::Horizontal, tr("闸门全关"));
    m_StateLogsModel->setHeaderData(8, Qt::Horizontal, tr("闸门开启速度过低"));
    m_StateLogsModel->setHeaderData(9, Qt::Horizontal, tr("闸门开启速度过低停机"));
    m_StateLogsModel->setHeaderData(10, Qt::Horizontal, tr("闸门开启超时停机"));
    m_StateLogsModel->setHeaderData(11, Qt::Horizontal, tr("闸门下滑200米"));
    m_StateLogsModel->setHeaderData(12, Qt::Horizontal, tr("闸门下滑300米"));
    m_StateLogsModel->setHeaderData(13, Qt::Horizontal, tr("1#泵自动控制"));
    m_StateLogsModel->setHeaderData(14, Qt::Horizontal, tr("1#泵电机故障"));
    m_StateLogsModel->setHeaderData(15, Qt::Horizontal, tr("1#泵主路合闸"));
    m_StateLogsModel->setHeaderData(16, Qt::Horizontal, tr("1#泵电机运行"));
    m_StateLogsModel->setHeaderData(17, Qt::Horizontal, tr("1#泵启动失败"));
    m_StateLogsModel->setHeaderData(18, Qt::Horizontal, tr("2#泵自动控制"));
    m_StateLogsModel->setHeaderData(19, Qt::Horizontal, tr("2#泵电机故障"));
    m_StateLogsModel->setHeaderData(20, Qt::Horizontal, tr("2#泵主路合闸"));
    m_StateLogsModel->setHeaderData(21, Qt::Horizontal, tr("2#泵电机运行"));
    m_StateLogsModel->setHeaderData(22, Qt::Horizontal, tr("2#泵启动失败"));
    m_StateLogsModel->setHeaderData(23, Qt::Horizontal, tr("系统压力"));
    m_StateLogsModel->setHeaderData(24, Qt::Horizontal, tr("压力过高"));
    m_StateLogsModel->setHeaderData(25, Qt::Horizontal, tr("压力过低"));
    m_StateLogsModel->setHeaderData(26, Qt::Horizontal, tr("有杆腔高压软管破裂"));
    m_StateLogsModel->setHeaderData(27, Qt::Horizontal, tr("有杆腔超压"));
    m_StateLogsModel->setHeaderData(28, Qt::Horizontal, tr("压力变送器故障"));
    m_StateLogsModel->setHeaderData(29, Qt::Horizontal, tr("油缸行程"));
    m_StateLogsModel->setHeaderData(30, Qt::Horizontal, tr("滤油器堵塞"));
    m_StateLogsModel->setHeaderData(31, Qt::Horizontal, tr("油位过高"));
    m_StateLogsModel->setHeaderData(32, Qt::Horizontal, tr("油位过低"));
    m_StateLogsModel->setHeaderData(33, Qt::Horizontal, tr("油温过高"));
    m_StateLogsModel->setHeaderData(34, Qt::Horizontal, tr("油温过低"));
    m_StateLogsModel->setHeaderData(35, Qt::Horizontal, tr("状态寄存器1"));
    m_StateLogsModel->setHeaderData(36, Qt::Horizontal, tr("状态寄存器2"));
    m_StateLogsModel->setHeaderData(37, Qt::Horizontal, tr("左有杆腔高压软管破裂"));
    m_StateLogsModel->setHeaderData(38, Qt::Horizontal, tr("右有杆腔高压软管破裂"));
    m_StateLogsModel->setHeaderData(39, Qt::Horizontal, tr("油箱油温变送器故障"));
    m_StateLogsModel->setHeaderData(40, Qt::Horizontal, tr("无杆腔压力过高"));
    m_StateLogsModel->setHeaderData(41, Qt::Horizontal, tr("1#控制电源正常"));
    m_StateLogsModel->setHeaderData(42, Qt::Horizontal, tr("2#控制电源正常"));
    m_StateLogsModel->setHeaderData(43, Qt::Horizontal, tr("电磁阀电源正常"));
    m_StateLogsModel->setHeaderData(44, Qt::Horizontal, tr("油箱加热器投入"));
    m_StateLogsModel->setHeaderData(45, Qt::Horizontal, tr("闸门同步偏差过大"));
    m_StateLogsModel->setHeaderData(46, Qt::Horizontal, tr("闸门开启"));
    m_StateLogsModel->setHeaderData(47, Qt::Horizontal, tr("闸门关闭"));
    m_StateLogsModel->setHeaderData(48, Qt::Horizontal, tr("油箱油温"));
    m_StateLogsModel->setHeaderData(49, Qt::Horizontal, tr("闸门下滑量"));
    m_StateLogsModel->setHeaderData(50, Qt::Horizontal, tr("闸门偏差"));
    m_StateLogsModel->setHeaderData(51, Qt::Horizontal, tr("闸门左油缸行程"));
    m_StateLogsModel->setHeaderData(52, Qt::Horizontal, tr("闸门右油缸行程"));
    m_StateLogsModel->setHeaderData(53, Qt::Horizontal, tr("状态寄存器3"));
    m_StateLogsModel->setSort(1, Qt::DescendingOrder);
    Query();

    QHBoxLayout *hlayout1 = new QHBoxLayout(this);
    hlayout1->addWidget(m_StateLogsView);

    m_ViewGroup = new QGroupBox(this);
    m_ViewGroup->setTitle(tr("状态记录列表"));
    m_ViewGroup->setLayout(hlayout1);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addWidget(m_FunctionGroup);
    vlayout->addWidget(m_ViewGroup);

    this->setWindowTitle(tr("状态记录"));
    this->setLayout(vlayout);
    this->showMaximized();

    connect(m_QueryButton, SIGNAL(clicked()), this, SLOT(Query()));
    connect(m_PrintButton, SIGNAL(clicked()), this, SLOT(Print()));
}

CStateViewDialog::~CStateViewDialog()
{
    ;
}

void CStateViewDialog::Query()
{
    QDateTime startTime = m_StartTimeEdit->dateTime();
    QDateTime endTime = m_EndTimeEdit->dateTime().addDays(1);
    QString sqlwhere = "happen_time>='" + startTime.toString(Qt::ISODate) +
                        "' and happen_time<='" + endTime.toString(Qt::ISODate) + "'";
    if(m_MoteNameEdit->text() != "")
        sqlwhere += " and name='" + m_MoteNameEdit->text() + "'";

    m_StateLogsModel->setFilter(sqlwhere);
    m_StateLogsModel->select();

    m_StateLogsView->resizeColumnsToContents();
}

void CStateViewDialog::Print()
{
    m_Printer = new QPrinter();
    QPrintDialog *printDialog = new QPrintDialog(m_Printer, this);
    if(!printDialog->exec())
        return;

    QDateTime startTime = m_StartTimeEdit->dateTime();
    QDateTime endTime = m_EndTimeEdit->dateTime().addDays(1);
    QString sqlStr = "SELECT * FROM v_statelogs WHERE happen_time>='" + startTime.toString(Qt::ISODate) +
                       "' AND happen_time<='" + endTime.toString(Qt::ISODate) + "'";
    if(m_MoteNameEdit->text() != "")
        sqlStr += " AND name='" + m_MoteNameEdit->text() + "'";

    QSqlQuery dbQuery;
    dbQuery.setForwardOnly(true);
    dbQuery.exec(sqlStr);

    QString html;
    html = "<table witdh=\"100%\" border=1 cellspacing=0>\n";
    html += "<tr style=\"font-weight:bold;background-color:lightgray;\">\n";
    html += "<td><center>" + tr("现地站") + "</td>\n";
    html += "<td><center>" + tr("时间") + "</td>\n";
    html += "<td><center>" + tr("闸门开度") + "</td>\n";
    html += "<td><center>" + tr("闸门开度预设") + "</td>\n";
    html += "<td><center>" + tr("闸门自动控制") + "</td>\n";
    html += "<td><center>" + tr("闸门远程控制") + "</td>\n";
    html += "<td><center>" + tr("闸门全开") + "</td>\n";
    html += "<td><center>" + tr("闸门全关") + "</td>\n";
    html += "<td><center>" + tr("闸门开启速度过低") + "</td>\n";
    html += "<td><center>" + tr("闸门开启速度过低停机") + "</td>\n";
    html += "<td><center>" + tr("闸门开启超时停机") + "</td>\n";
    html += "<td><center>" + tr("闸门下滑200米") + "</td>\n";
    html += "<td><center>" + tr("闸门下滑300米") + "</td>\n";
    html += "<td><center>" + tr("1#泵自动控制") + "</td>\n";
    html += "<td><center>" + tr("1#泵电机故障") + "</td>\n";
    html += "<td><center>" + tr("1#泵主路合闸") + "</td>\n";
    html += "<td><center>" + tr("1#泵电机运行") + "</td>\n";
    html += "<td><center>" + tr("1#泵启动失败") + "</td>\n";
    html += "<td><center>" + tr("2#泵自动控制") + "</td>\n";
    html += "<td><center>" + tr("2#泵电机故障") + "</td>\n";
    html += "<td><center>" + tr("2#泵主路合闸") + "</td>\n";
    html += "<td><center>" + tr("2#泵电机运行") + "</td>\n";
    html += "<td><center>" + tr("2#泵启动失败") + "</td>\n";
    html += "<td><center>" + tr("系统压力") + "</td>\n";
    html += "<td><center>" + tr("压力过高") + "</td>\n";
    html += "<td><center>" + tr("压力过低") + "</td>\n";
    html += "<td><center>" + tr("有杆腔高压软管破裂") + "</td>\n";
    html += "<td><center>" + tr("有杆腔超压") + "</td>\n";
    html += "<td><center>" + tr("压力变送器故障") + "</td>\n";
    html += "<td><center>" + tr("油缸行程") + "</td>\n";
    html += "<td><center>" + tr("滤油器堵塞") + "</td>\n";
    html += "<td><center>" + tr("油位过高") + "</td>\n";
    html += "<td><center>" + tr("油位过低") + "</td>\n";
    html += "<td><center>" + tr("油温过高") + "</td>\n";
    html += "<td><center>" + tr("油温过低") + "</td>\n";
    html += "<td><center>" + tr("状态寄存器1") + "</td>\n";
    html += "<td><center>" + tr("状态寄存器2") + "</td>\n";
    html += "<td><center>" + tr("左有杆腔高压软管破裂") + "</td>\n";      ///2014
    html += "<td><center>" + tr("右有杆腔高压软管破裂") + "</td>\n";
    html += "<td><center>" + tr("油箱油温变送器故障") + "</td>\n";
    html += "<td><center>" + tr("无杆腔压力过高") + "</td>\n";
    html += "<td><center>" + tr("1#控制电源正常") + "</td>\n";
    html += "<td><center>" + tr("2#控制电源正常") + "</td>\n";
    html += "<td><center>" + tr("电磁阀电源正常") + "</td>\n";
    html += "<td><center>" + tr("油箱加热器投入") + "</td>\n";
    html += "<td><center>" + tr("闸门同步偏差过大") + "</td>\n";
    html += "<td><center>" + tr("闸门开启") + "</td>\n";
    html += "<td><center>" + tr("闸门关闭") + "</td>\n";
    html += "<td><center>" + tr("油箱油温") + "</td>\n";
    html += "<td><center>" + tr("闸门下滑量") + "</td>\n";
    html += "<td><center>" + tr("闸门偏差") + "</td>\n";
    html += "<td><center>" + tr("闸门左油缸行程") + "</td>\n";
    html += "<td><center>" + tr("闸门右油缸行程") + "</td>\n";
    html += "<td><center>" + tr("状态寄存器3") + "</td>\n";
    html += "</tr>\n";

    while(dbQuery.next())
    {
        html += "<tr>\n";
        html += "<td>" + dbQuery.value(0).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(1).toDateTime().toString(Qt::ISODate) + "</td>\n";
        html += "<td>" + dbQuery.value(2).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(3).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(4).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(5).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(6).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(7).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(8).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(9).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(10).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(11).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(12).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(13).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(14).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(15).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(16).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(17).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(18).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(19).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(20).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(21).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(22).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(23).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(24).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(25).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(26).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(27).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(28).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(29).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(30).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(31).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(32).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(33).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(34).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(35).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(36).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(37).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(38).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(39).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(40).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(41).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(42).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(43).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(44).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(45).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(46).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(47).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(48).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(49).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(50).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(51).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(52).toString() + "</td>\n";
        html += "<td>" + dbQuery.value(53).toString() + "</td>\n";
        html += "</tr>\n";
    }
    html += "</table>\n";

    QTextDocument text;
    text.setHtml(html);
    text.print(m_Printer);
}
