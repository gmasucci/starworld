#pragma once
#include "singleton.h"
#include "hgefontmanager.h"
#include "hgefontex.h"

//struct hgeFontManager : public hgeFontManagerStub
//{
//	friend struct CreateStatic< hgeFontManager >; 
//};
//
//typedef SingletonHolder< hgeFontManager, CreateUsingNew > hgeFontManagerSingleton;

class CApplication
{
	friend struct CreateStatic< CApplication >;
private:
	CApplication(void);
	virtual ~CApplication(void);

public:
	bool Initialize();
	void Run();
	void UnInitialize();

	HGE* operator->(){ return m_hge; }
	HGE* getEngine(){ return m_hge; }

	hgeFontManagerStub& getFontManager(){ return m_FontManager; }

protected:
	bool UpdateLogic( float fDelta );
	void Render();

private:
	static
	bool RenderFunc();

	static
	bool FrameFunc();

private:

	HGE			*m_hge;
	static hgeFontManagerStub m_FontManager;
};

typedef SingletonHolder< CApplication, CreateStatic > Application;