// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
//��ı�׼�������� DLL �е������ļ��������������϶���� GAMEWORLD_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
//�κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ 
// GAMEWORLD_API ������Ϊ�ǴӴ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef GAMEWORLD_EXPORTS
#define GAMEWORLD_API __declspec(dllexport)
#else
#define GAMEWORLD_API __declspec(dllimport)
#endif

// �����Ǵ� GameWorld.dll ������
class GAMEWORLD_API CGameWorld 
{
public:
	CGameWorld(void);
	// TODO: �ڴ�������ķ�����
};

extern GAMEWORLD_API int nGameWorld;

GAMEWORLD_API int fnGameWorld(void);
