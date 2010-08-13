#ifndef UIFACTORY_H
#define UIFACTORY_H

#define REGIST_CONTROL( TYPE, LABLE )\
	UILib::CUIRegister< TYPE > g_##__FILE__##LABLE( _T(#LABLE) )\

namespace UILib
{
	// UI������
	class XUI_Wnd;
	class CUIFactory
	{
	protected:
		CUIFactory(void){};
		~CUIFactory(void){};

	public:
		// ��������
		typedef XUI_Wnd* (*pfnCreator)(void);

		// ����ģʽ
		static CUIFactory& GetInstance();

		// ע��һ������
		void Regist( _lpctstr pszLable, pfnCreator pfunc )
		{
			CUIFactoryMap::const_iterator citer = m_FactoryMap.find( pszLable );
			if( citer != m_FactoryMap.end() )	return;

			m_FactoryMap[pszLable] = pfunc;
		}

		size_t GetXUI_WndCount()const
		{
			return m_FactoryMap.size();
		}

		XUI_Wnd* Creator( _lpctstr pszLable )
		{
			CUIFactoryMap::const_iterator citer = m_FactoryMap.find( pszLable );
			if( citer != m_FactoryMap.end() )
			{
				return citer->second();
			}
			return NULL;
		}

	private:
		typedef std::map< _string, pfnCreator >	CUIFactoryMap;
		CUIFactoryMap	m_FactoryMap;
	};

	template< typename UIType >
	class CUIRegister
	{
	public:
		explicit CUIRegister( _lpctstr pszLable )
		{
			CUIFactory::GetInstance().Regist( pszLable, CreateControl );
		}

		static XUI_Wnd* CreateControl()
		{
			return new UIType;
		}
	};
};
#endif // UIFACTORY_H
