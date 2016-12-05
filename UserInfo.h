#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include <memory>

//define user role
#define ROLE_ADMINISTRATOR      0
#define ROLE_ENGINEER           1
#define ROLE_OPERATOR           2

//define actions
enum Action
{
    none,
    add,
    del,
    upd
};

class CUserInfo
{
public:
    CUserInfo();
    CUserInfo(int userid, QString username, QString password, int role, Action act);
    ~CUserInfo();

    int     getUserid();
    QString getUsername();
    QString getPassword();
    int     getRole();
    Action  getAction();

    void setUserid(int);
    void setUsername(QString);
    void setPassword(QString);
    void setRole(int);
    void setAction(Action);


private:
    int     m_iUserId;
    QString m_strUserName;
    QString m_strPassword;
    int     m_iRole;
    Action  m_eAction;
};


typedef std::auto_ptr<CUserInfo> ptrCUserInfo;

#endif // USERINFO_H
