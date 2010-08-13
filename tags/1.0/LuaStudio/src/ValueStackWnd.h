#pragma once
#include "ListCtrlBar.h"

class ValueStackWnd : public ListCtrlBar
{
public:
	ValueStackWnd();

	virtual ~ValueStackWnd();

private:
	virtual void GetDispInfo(size_t item, int column, std::string& buffer);
	virtual void CreateColumns(CListCtrl& ctrl);
};
