#include "UserInfo.h"

CUserInfo::CUserInfo()
{
    m_iUserId = 0;
    m_strUserName = "";
    m_strPassword = "";
    m_iRole = 0;
    m_eAction = none;
}

CUserInfo::CUserInfo(int userid, QString username, QString password, int role, Action act)
{
    m_iUserId = userid;
    m_strUserName = username;
    m_strPassword = password;
    m_iRole = role;
    m_eAction = act;
}

CUserInfo::~CUserInfo()
{

}

int CUserInfo::getUserid()
{
    return m_iUserId;
}

void CUserInfo::setUserid(int userid)
{
    m_iUserId = userid;
}

QString CUserInfo::getUsername()
{
    return m_strUserName;
}

void CUserInfo::setUsername(QString username)
{
    m_strUserName = username;
}

QString CUserInfo::getPassword()
{
    return m_strPassword;
}

void CUserInfo::setPassword(QString password)
{
    m_strPassword = password;
}

int CUserInfo::getRole()
{
    return m_iRole;
}

void CUserInfo::setRole(int role)
{
    m_iRole = role;
}

Action CUserInfo::getAction()
{
    return m_eAction;
}

void CUserInfo::setAction(Action act)
{
    m_eAction = act;
}



