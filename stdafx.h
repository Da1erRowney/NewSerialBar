#pragma once

// ������� ���������� ������ Windows �� ��������� ����� ����������
#define NTDDI_VERSION NTDDI_WIN10
#define _WIN32_WINNT 0x0A00  // Windows 10
#define WINVER 0x0A00        // Windows 10

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		
#endif

#ifndef _WIN32_IE			
#define _WIN32_IE 0x0A00	// Internet Explorer 10+
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	
#define _AFX_ALL_WARNINGS

// ����� �����: ��������� ������ Winsock ����� ���������� Windows.h
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_   // ������������� �������������� ��������� winsock.h

// �������� Windows.h ������ ��� ���������� ������ ���������� �������
#include <Windows.h>

// ���� �������� winsock2.h �� ����� ������ ����������, ������� ����� ������������ ������
#include <winsock2.h>
#include <ws2tcpip.h>

// ����� �������� SDKDDKVer.h
#include <SDKDDKVer.h>

// ����� MFC ���������
#include <afxwin.h>
#include <afxext.h>
#include <afxdtctl.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			
#endif

// ����� ��������� ��������� ���������
#include <stdio.h>
#include <tchar.h>

// ���� ���������������� ���������
#include "../SerialServer/server_error.h"

#define MAX_PORT 16
#define CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

// ��� Winsock2 ���������� ����� ���������� ����������
#pragma comment(lib, "ws2_32.lib")