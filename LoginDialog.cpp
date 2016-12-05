#include "LoginDialog.h"

CLoginDialog::CLoginDialog(QWidget *parent) :
    QDialog(parent)
{
    m_UsernameLabel = new QLabel(this);
    m_UsernameLabel->setText(tr("用户名:"));

    QRegExp regExp_username("[A-Za-z_][A-Za-z0-9_]{0,15}");
    m_UsernameEdit = new QLineEdit(this);
    m_UsernameEdit->setValidator(new QRegExpValidator(regExp_username, this));

    m_PasswordLabel = new QLabel(this);
    m_PasswordLabel->setText(tr("密码:"));

    QRegExp regExp_password("[^\'\"\t\r\f\n\a\v]{1,16}");
    m_PasswordEdit = new QLineEdit(this);
    m_PasswordEdit->setValidator(new QRegExpValidator(regExp_password, this));
    m_PasswordEdit->setEchoMode(QLineEdit::Password);

    m_LoginButton = new QPushButton(this);
    m_LoginButton->setText(tr("登录"));
    m_LoginButton->setEnabled(false);

    m_QuitButton = new QPushButton(this);
    m_QuitButton->setText(tr("退出"));

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->addWidget(m_UsernameLabel, 0, 0);
    gridLayout->addWidget(m_UsernameEdit, 0, 1);
    gridLayout->addWidget(m_PasswordLabel, 1, 0);
    gridLayout->addWidget(m_PasswordEdit, 1, 1);

    m_LoginGroup = new QGroupBox(this);
    m_LoginGroup->setTitle(tr("用户登录"));
    m_LoginGroup->setLayout(gridLayout);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(m_LoginGroup, 0, 0, 1, 3);
    mainLayout->addWidget(m_LoginButton, 1, 1, 1, 1);
    mainLayout->addWidget(m_QuitButton, 1, 2, 1, 1);

    this->setLayout(mainLayout);
    this->setWindowTitle(tr("三里坪闸门集控系统"));
    this->setStyleSheet("font-size:14px");

    connect(m_UsernameEdit, SIGNAL(textEdited(QString)), this, SLOT(UserNameChanged()));
    connect(m_LoginButton, SIGNAL(clicked()), this, SLOT(CheckLogin()));
    connect(m_QuitButton, SIGNAL(clicked()), this, SLOT(close()));
}

CLoginDialog::~CLoginDialog()
{
    delete m_pUserInfo;
}

void CLoginDialog::UserNameChanged()
{
    m_LoginButton->setEnabled(true);
}

void CLoginDialog::CheckLogin()
{
    CLogs log;
    QSqlQuery dbQuery;
    dbQuery.setForwardOnly(true);
    dbQuery.exec("SELECT userid, username, password, role FROM t_user WHERE username='"
                 + m_UsernameEdit->text()
                 + "' and password='"
                 + m_PasswordEdit->text()
                 + "'");
    if (dbQuery.next())
    {
        m_pUserInfo = new CUserInfo(dbQuery.value(0).toInt(),
                                   dbQuery.value(1).toString(),
                                   dbQuery.value(2).toString(),
                                   dbQuery.value(3).toInt(),
                                   none);

        log.set("当前用户：" + m_pUserInfo->getUsername());
        this->accept();
    }
    else
    {
        log.set("用户" + m_UsernameEdit->text() + "验证失败");
        QMessageBox::warning(this, tr("警告"), tr("用户名或密码不正确！"), QMessageBox::Yes);
        m_UsernameEdit->clear();
        m_PasswordEdit->clear();
        m_UsernameEdit->setFocus();
        m_LoginButton->setEnabled(false);
    }
}

CUserInfo *CLoginDialog::getUserInfo()
{
    return m_pUserInfo;
}

