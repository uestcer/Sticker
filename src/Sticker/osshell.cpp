#include "stdafx.h"
#include "osshell.h"


OSShell::OSShell()
{

}

OSShell::~OSShell()
{

}

bool OSShell::GetScreenSize( SIZE& size )
{
	size.cx = ::GetSystemMetrics(SM_CXSCREEN);
	size.cy = ::GetSystemMetrics(SM_CYSCREEN);
	if (size.cx == 0 || size.cy == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
