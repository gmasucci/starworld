struct dbMsgHeader
{
	size_t	size;	// ��Ϣ����
	size_t	uid;	// �û�id
	size_t	mid;	// ��Ϣ��
};

struct dbRegistUser	:	public dbMsgHeader
{
	_string username;
	_string password;
};