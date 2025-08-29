#pragma once

// Сначала определяем версии Windows ДО включения любых заголовков
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

// ОЧЕНЬ ВАЖНО: Отключаем старый Winsock перед включением Windows.h
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_   // Предотвращаем автоматическое включение winsock.h

// Включаем Windows.h ПЕРВЫМ для правильной работы мониторных функций
#include <Windows.h>

// Явно включаем winsock2.h ДО любых других заголовков, которые могут использовать сокеты
#include <winsock2.h>
#include <ws2tcpip.h>

// Затем включаем SDKDDKVer.h
#include <SDKDDKVer.h>

// Затем MFC заголовки
#include <afxwin.h>
#include <afxext.h>
#include <afxdtctl.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			
#endif

// Затем остальные системные заголовки
#include <stdio.h>
#include <tchar.h>

// Ваши пользовательские заголовки
#include "../SerialServer/server_error.h"

#define MAX_PORT 16
#define CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

// Для Winsock2 необходимо также подключить библиотеку
#pragma comment(lib, "ws2_32.lib")