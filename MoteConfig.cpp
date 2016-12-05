#include "MoteConfig.h"


CMoteConfig::CMoteConfig(QWidget *parent, CMote *mote) :
    QWidget(parent)
{
    m_NameLabel = new QLabel(this);
    m_NameLabel->setText(tr("现地站名:"));

    m_NameEdit = new QLineEdit(this);

    m_GroupLabel = new QLabel(this);
    m_GroupLabel->setText(tr("编组:"));

    m_GroupCombo = new QComboBox(this);
    m_GroupCombo->addItem(tr("未编组"));
    m_GroupCombo->addItem(tr("第一组"));
    m_GroupCombo->addItem(tr("第二组"));
    m_GroupCombo->addItem(tr("第三组"));
    m_GroupCombo->addItem(tr("第四组"));
    m_GroupCombo->addItem(tr("第五组"));

    m_MaxOpenValLabel = new QLabel(this);
    m_MaxOpenValLabel->setText(tr("最大开度值:"));

    m_MaxOpenValEdit = new QLineEdit(this);
    QRegExp regExp_number_10("[1-9][0-9]{0,9}");
    m_MaxOpenValEdit->setValidator(new QRegExpValidator(regExp_number_10, this));

    m_IpAddressLabel = new QLabel(this);
    m_IpAddressLabel->setText(tr("IP地址:"));

    m_IpAddressEdit = new QLineEdit(this);
    m_IpAddressEdit->setInputMask("000.000.000.000");

    m_PortLabel = new QLabel(this);
    m_PortLabel->setText(tr("端口号:"));

    m_PortEdit = new QLineEdit(this);
    QRegExp regExp_number_5("[1-9][0-9]{0,4}");
    m_PortEdit->setValidator(new QRegExpValidator(regExp_number_5, this));

    m_ScanIntervalLabel = new QLabel(this);
    m_ScanIntervalLabel->setText(tr("扫描间隔(秒):"));

    m_ScanIntervalEdit = new QLineEdit(this);
    m_ScanIntervalEdit->setValidator(new QRegExpValidator(regExp_number_10, this));

    m_TypeLabel = new QLabel(this);
    m_TypeLabel->setText(tr("现地站类型:"));

    m_TypeCombo = new QComboBox(this);
    m_TypeCombo->addItem(tr("表孔"));
    m_TypeCombo->addItem(tr("中孔"));

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(m_NameLabel, 0, 0, 1, 1);
    mainLayout->addWidget(m_NameEdit, 0, 1, 1, 1);
    mainLayout->addWidget(m_TypeLabel, 0, 2, 1, 1);
    mainLayout->addWidget(m_TypeCombo, 0, 3, 1, 1);
    mainLayout->addWidget(m_GroupLabel, 0, 4, 1, 1);
    mainLayout->addWidget(m_GroupCombo, 0, 5, 1, 1);
    mainLayout->addWidget(m_MaxOpenValLabel, 0, 6, 1, 1);
    mainLayout->addWidget(m_MaxOpenValEdit, 0, 7, 1, 1);
    mainLayout->addWidget(m_IpAddressLabel, 1, 0, 1, 1);
    mainLayout->addWidget(m_IpAddressEdit, 1, 1, 1, 1);
    mainLayout->addWidget(m_PortLabel, 1, 2, 1, 1);
    mainLayout->addWidget(m_PortEdit, 1, 3, 1, 1);
    mainLayout->addWidget(m_ScanIntervalLabel, 1, 4, 1, 1);
    mainLayout->addWidget(m_ScanIntervalEdit, 1, 5, 1, 1);

    this->setLayout(mainLayout);



    setName(mote->getName());
    setType(mote->getType());
    setIpAddr(mote->getIpAddr());
    setPort(mote->getPort());
    setGroupNum(mote->getGroupNum());
    setMaxOpenVal(mote->getMaxOpenVal());
    setScanInterval(mote->getScanInterval() / 1000);
    setMote(mote);
    setAction(CMoteConfig::NONE);

    connect(m_NameEdit, SIGNAL(textEdited(QString)), this, SLOT(updName()));
    connect(m_NameEdit, SIGNAL(textEdited(QString)), this, SLOT(EnSave()));
    connect(m_TypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updType()));
    connect(m_TypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(EnSave()));
    connect(m_MaxOpenValEdit, SIGNAL(textEdited(QString)), this, SLOT(updMaxOpenVal()));
    connect(m_MaxOpenValEdit, SIGNAL(textEdited(QString)), this, SLOT(EnSave()));
    connect(m_IpAddressEdit, SIGNAL(textEdited(QString)), this, SLOT(updIpAddr()));
    connect(m_IpAddressEdit, SIGNAL(textEdited(QString)), this, SLOT(EnSave()));
    connect(m_PortEdit, SIGNAL(textEdited(QString)), this, SLOT(updPort()));
    connect(m_PortEdit, SIGNAL(textEdited(QString)), this, SLOT(EnSave()));
    connect(m_GroupCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updGroupNum()));
    connect(m_GroupCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(EnSave()));
    connect(m_ScanIntervalEdit, SIGNAL(textEdited(QString)), this, SLOT(updScanInterval()));
    connect(m_ScanIntervalEdit, SIGNAL(textEdited(QString)), this, SLOT(EnSave()));

/*
    ui = new Ui::MoteConfig;
    ui->setupUi(this);

    //m_ScanIntervalList << "1s" << "5s" << "10s" << "30s" << "1min" << "5min" << "10min";
    //ui->comboBox_interval->addItems(m_ScanIntervalList);
    QRegExp regExp_number_10("[1-9][0-9]{0,9}");
    ui->lineEdit_open->setValidator(new QRegExpValidator(regExp_number_10, this));
    ui->lineEdit_interval->setValidator(new QRegExpValidator(regExp_number_10, this));

    QRegExp regExp_number_5("[1-9][0-9]{0,4}");
    ui->lineEdit_port->setValidator(new QRegExpValidator(regExp_number_5, this));

    m_GroupList << tr("未编组") << tr("第一组") << tr("第二组") << tr("第三组") << tr("第四组") << tr("第五组");
    ui->comboBox_group->addItems(m_GroupList);

    setName(mote->getName());
    setIpAddr(mote->getIpAddr());
    setPort(mote->getPort());
    setGroupNum(mote->getGroupNum());
    setMaxOpenVal(mote->getMaxOpenVal());
    setScanInterval(mote->getScanInterval() / 1000);
    setMote(mote);
    setAction(CMoteConfig::NONE);


    connect(ui->lineEdit_name, SIGNAL(textEdited(QString)), this, SLOT(updName()));
    connect(ui->lineEdit_name, SIGNAL(textEdited(QString)), this, SLOT(EnSave()));
    connect(ui->lineEdit_open, SIGNAL(textEdited(QString)), this, SLOT(updMaxOpenVal()));
    connect(ui->lineEdit_open, SIGNAL(textEdited(QString)), this, SLOT(EnSave()));
    connect(ui->lineEdit_ip, SIGNAL(textEdited(QString)), this, SLOT(updIpAddr()));
    connect(ui->lineEdit_ip, SIGNAL(textEdited(QString)), this, SLOT(EnSave()));
    connect(ui->lineEdit_port, SIGNAL(textEdited(QString)), this, SLOT(updPort()));
    connect(ui->lineEdit_port, SIGNAL(textEdited(QString)), this, SLOT(EnSave()));
    connect(ui->comboBox_group, SIGNAL(currentIndexChanged(int)), this, SLOT(updGroupNum()));
    connect(ui->comboBox_group, SIGNAL(currentIndexChanged(int)), this, SLOT(EnSave()));
    connect(ui->lineEdit_interval, SIGNAL(textEdited(QString)), this, SLOT(updScanInterval()));
    connect(ui->lineEdit_interval, SIGNAL(textEdited(QString)), this, SLOT(EnSave()));
    //*/
}

CMoteConfig::~CMoteConfig()
{
    ;
}

void CMoteConfig::setName(QString name)
{
//    ui->lineEdit_name->setText(name);
    m_NameEdit->setText(name);
}

QString CMoteConfig::getName(void)
{
//    return ui->lineEdit_name->text();
    return m_NameEdit->text();
}

void CMoteConfig::setType(int type)
{
    m_TypeCombo->setCurrentIndex(type);
}

int CMoteConfig::getType(void)
{
    return m_TypeCombo->currentIndex();
}

void CMoteConfig::setIpAddr(QString ipAddr)
{
//    ui->lineEdit_ip->setText(ipAddr);
    m_IpAddressEdit->setText(ipAddr);
}

QString CMoteConfig::getIpAddr(void)
{
//    return ui->lineEdit_ip->text();
    return m_IpAddressEdit->text();
}

void CMoteConfig::setPort(int port)
{
//    ui->lineEdit_port->setText(QString::number(port, 10));
    m_PortEdit->setText(QString::number(port, 10));
}

int CMoteConfig::getPort(void)
{
//    return ui->lineEdit_port->text().toInt();
    return m_PortEdit->text().toInt();
}

void CMoteConfig::setGroupNum(int group)
{
//    ui->comboBox_group->setCurrentIndex(group);
    m_GroupCombo->setCurrentIndex(group);
}

int CMoteConfig::getGroupNum(void)
{
//    return ui->comboBox_group->currentIndex();
    return m_GroupCombo->currentIndex();
}

void CMoteConfig::setMaxOpenVal(int open)
{
//    ui->lineEdit_open->setText(QString::number(open, 10));
    m_MaxOpenValEdit->setText(QString::number(open, 10));
}

int CMoteConfig::getMaxOpenVal(void)
{
//    return ui->lineEdit_open->text().toInt();
    return m_MaxOpenValEdit->text().toInt();
}

void CMoteConfig::setScanInterval(int intervalInSec)
{
//    ui->lineEdit_interval->setText(QString::number(intervalInSec));
    m_ScanIntervalEdit->setText(QString::number(intervalInSec));
}

int CMoteConfig::getScanInterval(void)
{
//    return ui->lineEdit_interval->text().toInt();
    return m_ScanIntervalEdit->text().toInt();
}

void CMoteConfig::setMote(CMote *mote)
{
    m_Mote = mote;
}

CMote *CMoteConfig::getMote(void)
{
    return m_Mote;
}

void CMoteConfig::EnSave(void)
{
    if(m_Action != NEW)
        m_Action = UPDATE;
    emit enableSave();
}

void CMoteConfig::updName(void)
{
    m_Mote->setName(getName());
    emit updateName(getName());
}

void CMoteConfig::updType(void)
{
    m_Mote->setType(getType());
}

void CMoteConfig::updIpAddr(void)
{
    m_Mote->setIpAddr(getIpAddr());
}

void CMoteConfig::updPort(void)
{
    m_Mote->setPort(getPort());
}

void CMoteConfig::updGroupNum(void)
{
    m_Mote->setGroupNum(getGroupNum());
}

void CMoteConfig::updMaxOpenVal(void)
{
    m_Mote->setMaxOpenVal(getMaxOpenVal());
}

void CMoteConfig::updScanInterval(void)
{
    m_Mote->setScanInterval(1000 * getScanInterval());
}

void CMoteConfig::setAction(int action)
{
    m_Action = action;
}

int CMoteConfig::getAction()
{
    return m_Action;
}


