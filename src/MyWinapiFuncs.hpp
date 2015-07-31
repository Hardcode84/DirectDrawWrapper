#ifndef MYWINAPIFUNCS_HPP
#define MYWINAPIFUNCS_HPP

#include "common.hpp"

bool CreateHooks(logger* log);
void RemoveHooks(logger* log);

BOOL WINAPI MyGetCursorPos(LPPOINT lpPoint);
BOOL WINAPI MyClientToScreen(HWND hWnd,  LPPOINT lpPoint);

#endif // MYWINAPIFUNCS_HPP
