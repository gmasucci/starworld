#ifndef _TRANSACTIME_H
#define _TRANSACTIME_H

#define TRANS_BUF_SIZE	4096;
struct transaction
{
	size_t	size;	// ��Ϣ����
	size_t	uid;	// �û�id
	_byte	mid;	// ��Ϣ��
	_byte	pid;	// ��ˮ�ߺ�

	char	msg[1];	// ��Ϣ����
};

enum
{
	db_user_regist,
	db_user_logon,
};

enum
{
	lg_logon_failed,
	lg_logon_success,
};

struct dbRegistUser
{
	char username[128];
	char password[128];
};

struct dbUserLogon
{
	char username[128];
	char password[128];
};

#endif // _TRANSACTIME_H