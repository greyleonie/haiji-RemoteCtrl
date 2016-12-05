#include "UserDialog.h"


CUserDialog::CUserDialog(QWidget *parent) :
    QDialog(parent)
{
    ui = new Ui::Dialog_user;
    ui->setupUi(this);

    QRegExp regExp_username("[A-Za-z_][A-Za-z0-9_]{0,15}");
    ui->lineEdit_username->setValidator(new QRegExpValidator(regExp_username, this));

    QRegExp regExp_password("[^\'\"\t\r\f\n\a\v]{1,16}");
    ui->lineEdit_password->setValidator(new QRegExpValidator(regExp_password, this));
    ui->lineEdit_confirm->setValidator(new QRegExpValidator(regExp_password, this));

    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
    ui->lineEdit_confirm->clear();

    QSqlQuery dbQuery;
    dbQuery.setForwardOnly(true);
    dbQuery.exec("SELECT MAX(userid) FROM t_user;");
    if(dbQuery.next())
        m_iMaxUserid = dbQuery.value(0).toInt();
    dbQuery.exec("SELECT userid, username, password, role FROM t_user;");
    while(dbQuery.next())
    {
        CUserInfo userinfo(dbQuery.value(0).toInt(),
                           dbQuery.value(1).toString(),
                           dbQuery.value(2).toString(),
                           dbQuery.value(3).toInt(),
                           none);
        m_liUserList.append(userinfo);
        ui->listWidget->addItem(dbQuery.value(1).toString());
    }
    ui->listWidget->setFocus();
    ui->listWidget->setCurrentRow(0);
    CUserInfo userinfo = m_liUserList.at(ui->listWidget->currentRow());
    ui->lineEdit_username->setText(userinfo.getUsername());
    ui->lineEdit_password->setText(userinfo.getPassword());
    ui->lineEdit_confirm->setText(userinfo.getPassword());
    ui->comboBox_actor->setCurrentIndex(userinfo.getRole());


    connect(ui->pushButton_close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(AddUser()));
    connect(ui->pushButton_del, SIGNAL(clicked()), this, SLOT(DelUser()));
    connect(ui->pushButton_save, SIGNAL(clicked()), this, SLOT(Save()));
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelectItem(QListWidgetItem*)));
    connect(ui->lineEdit_username, SIGNAL(textEdited(QString)), this, SLOT(EditUsername()));
    connect(ui->lineEdit_password, SIGNAL(textEdited(QString)), this, SLOT(EditPassword()));
    connect(ui->comboBox_actor, SIGNAL(currentIndexChanged(int)), this, SLOT(SelectRole(int)));
    connect(ui->lineEdit_confirm, SIGNAL(editingFinished()), this, SLOT(ConfirmPassword()));
}

CUserDialog::~CUserDialog()
{
    delete ui;
}

void CUserDialog::AddUser()
{
    m_iMaxUserid++;
    CUserInfo userinfo(m_iMaxUserid, "NewUser", "", ROLE_OPERATOR, add);
    m_liUserList.append(userinfo);

    ui->listWidget->addItem(userinfo.getUsername());
    ui->listWidget->setCurrentRow(ui->listWidget->count() - 1);
    ui->lineEdit_username->setText(userinfo.getUsername());
    ui->lineEdit_password->setText(userinfo.getPassword());
    ui->lineEdit_confirm->setText(userinfo.getPassword());
    ui->comboBox_actor->setCurrentIndex(userinfo.getRole());
}

void CUserDialog::DelUser()
{
    CUserInfo userinfo = m_liUserList.at(ui->listWidget->currentRow());
    userinfo.setAction(del);
    m_liUserList.replace(ui->listWidget->currentRow(), userinfo);

    ui->listWidget->currentItem()->setHidden(true);
    ui->listWidget->setCurrentRow(0);
    userinfo = m_liUserList.at(ui->listWidget->currentRow());
    ui->lineEdit_username->setText(userinfo.getUsername());
    ui->lineEdit_password->setText(userinfo.getPassword());
    ui->lineEdit_confirm->setText(userinfo.getPassword());
    ui->comboBox_actor->setCurrentIndex(userinfo.getRole());
}

void CUserDialog::Save()
{
    QSqlQuery dbQuery;
    dbQuery.setForwardOnly(true);
    CLogs log;

    for (int i = 0; i < m_liUserList.size(); ++i)
    {
        CUserInfo userinfo = m_liUserList.at(i);
        switch(userinfo.getAction())
        {
            case none:
                break;

            case add:
                dbQuery.exec("INSERT INTO t_user(userid, username, password, role) VALUES ("
                             + QString::number(userinfo.getUserid(), 10) + ", '"
                             + userinfo.getUsername() + "', '"
                             + userinfo.getPassword() + "', "
                             + QString::number(userinfo.getRole(), 10)
                             + ");"
                             );
                userinfo.setAction(none);
                m_liUserList.replace(i, userinfo);
                log.set("添加用户：" + userinfo.getUsername());
                break;

            case del:
                dbQuery.exec("DELETE FROM t_user WHERE userid="
                             + QString::number(userinfo.getUserid(), 10)
                             + ";"
                            );
                userinfo.setAction(none);
                m_liUserList.replace(i, userinfo);
                log.set("删除用户：" + userinfo.getUsername());
                break;

            case upd:
                dbQuery.exec("UPDATE t_user SET username='"
                             + userinfo.getUsername() + "', password='"
                             + userinfo.getPassword() + "', role="
                             + QString::number(userinfo.getRole(), 10) + " WHERE userid="
                             + QString::number(userinfo.getUserid(), 10)
                             + ";"
                            );
                userinfo.setAction(none);
                m_liUserList.replace(i, userinfo);
                log.set("更新用户：" + userinfo.getUsername());
                break;

            default:
                break;
        }
    }
}

void CUserDialog::SelectItem(QListWidgetItem *item)
{
    ui->listWidget->setCurrentItem(item);
    CUserInfo userinfo = m_liUserList.at(ui->listWidget->currentRow());
    ui->lineEdit_username->setText(userinfo.getUsername());
    ui->lineEdit_password->setText(userinfo.getPassword());
    ui->lineEdit_confirm->setText(userinfo.getPassword());
    ui->comboBox_actor->setCurrentIndex(userinfo.getRole());
}

void CUserDialog::EditUsername()
{
    CUserInfo userinfo = m_liUserList.at(ui->listWidget->currentRow());
    userinfo.setUsername(ui->lineEdit_username->text());
    if (userinfo.getAction() != add)
        userinfo.setAction(upd);
    m_liUserList.replace(ui->listWidget->currentRow(), userinfo);

    QListWidgetItem *item = ui->listWidget->currentItem();
    item->setText(ui->lineEdit_username->text());
}

void CUserDialog::EditPassword()
{
    CUserInfo userinfo = m_liUserList.at(ui->listWidget->currentRow());
    userinfo.setPassword(ui->lineEdit_password->text());
    if (userinfo.getAction() != add)
        userinfo.setAction(upd);
    m_liUserList.replace(ui->listWidget->currentRow(), userinfo);
}

void CUserDialog::SelectRole(int role)
{
    CUserInfo userinfo = m_liUserList.at(ui->listWidget->currentRow());
    userinfo.setRole(role);
    if (userinfo.getAction() != add)
        userinfo.setAction(upd);
    m_liUserList.replace(ui->listWidget->currentRow(), userinfo);
}

void CUserDialog::ConfirmPassword()
{
    if (ui->lineEdit_password->text() != ui->lineEdit_confirm->text())
    {
        QMessageBox::warning(this, tr("Warning"), tr("Confirm password failed."), QMessageBox::Yes);
        ui->lineEdit_confirm->setFocus();
    }
}

