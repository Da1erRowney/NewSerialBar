#include "StdAfx.h"
#include ".\settings.h"
#include "log.h"
#include <math.h>

#include <shlobj.h>
#include <shlwapi.h>
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")

CSettings::CSettings()
{
	_iSrv = NULL;
	_iPort = NULL;
	_iManager = NULL;

	if (S_OK == CoInitialize(NULL))
	{
		_bComInit = TRUE;
	}
	_bForceClosed = FALSE;
	_baudeRate = 115200;
	_byteSize = 8;
	_parity = 0;
	_stopBits = 0;
	_timeout = 0;
	_waitRequest = 200;
	_waitByte = 100;
	_responseOff = 0;
	_responseOff = 0;
	_portNum = 1;
	
	_xmltool = new CXMLSettingsTool(this);

}


CSettings::~CSettings(void)
{
	if (_bComInit)
	{
		CoUninitialize();
	}

	delete _xmltool;
}

void CSettings::FromPort(IPort *iPort)
{			
	iPort->get_BaudeRate(&_baudeRate);
	iPort->get_ByteSize(&_byteSize);
	iPort->get_Parity(&_parity);
	iPort->get_StopBits(&_stopBits);
	iPort->get_TimeOut(&_timeout);
	iPort->get_WaitByte(&_waitByte);
	iPort->get_WaitRequest(&_waitRequest);
	iPort->get_PortNum(&_portNum);
	iPort->get_ResponseOff(&_responseOff);
	iPort->get_ResponseOn(&_responseOn);
	VARIANT_BOOL bForceClose = _bForceClosed;
	iPort->get_ForceClose(&bForceClose);

}

void CSettings::ToPort(IPort *iPort)
{
	iPort->put_BaudeRate(_baudeRate);
	iPort->put_ByteSize(_byteSize);
	iPort->put_Parity(_parity);
	iPort->put_StopBits(_stopBits);
	iPort->put_TimeOut(_timeout);
	iPort->put_WaitByte(_waitByte);
	iPort->put_WaitRequest(_waitRequest);
	iPort->put_PortNum(_portNum);
	iPort->put_ResponseOff(_responseOff);
	iPort->put_ResponseOn(_responseOn);
	VARIANT_BOOL bForceClose = _bForceClosed;
	iPort->put_ForceClose(bForceClose);
}


BOOL CSettings::Load(const char* fname,const BYTE portNum)
{
	return _xmltool->Load(fname,portNum);
}

BOOL CSettings::Save(const char* fname)
{
	return _xmltool->Save(fname);
}

//////////////////////////////////////////////////////////////////////////


BOOL CSettings::CXMLSettingsTool::Load(const char* fname,const BYTE portNum)
{
	BOOL ret = FALSE;
	CComPtr<IXMLDOMDocument> xmlDoc =  LoadXMLDoc(fname);

	if (xmlDoc)
	{
			if (SavedPortNode(xmlDoc,portNum))
			{
				_settings->_portNum = portNum;
				CComPtr<IXMLDOMNodeList>  paramNodes;
				
				char request[BUFSIZ];
				sprintf(request,"SerialSettings/port[@number = %d]/*",portNum);

				xmlDoc->selectNodes(CComBSTR(request),&paramNodes);
				IterateParams(paramNodes);
				ret = TRUE;
			}
	}
	
	return ret;
}


CComPtr<IXMLDOMNode> CSettings::CXMLSettingsTool::SavedPortNode(CComPtr<IXMLDOMDocument> xmlDoc,const BYTE portNum)
{
	CComPtr<IXMLDOMNodeList> portAttrs;
	CComPtr<IXMLDOMNodeList> portNodes;
	CComPtr<IXMLDOMNode> portAttr;
	CComPtr<IXMLDOMNode> ret = NULL;

	long cnt;
	
	xmlDoc->selectNodes(CComBSTR("SerialSettings/port/@number"),&portAttrs);
	xmlDoc->selectNodes(CComBSTR("SerialSettings/port"),&portNodes);
	
	portAttrs->get_length(&cnt);

	for (long i = 0; i < cnt ; i++)
	{	
		CComVariant portNumVar;
		
		portAttrs->get_item(i,&portAttr);
		portAttr->get_nodeValue(&portNumVar);

		if(portNum == GetPortNum(portNumVar))
		{
			portNodes->get_item(i,&ret);
			break;
		}
		portAttr.Release();
		portNumVar.Clear();
		
	}
	return ret;		
}


CComPtr<IXMLDOMDocument> CSettings::CXMLSettingsTool::LoadXMLDoc(const char *fname)
{
	CComPtr<IXMLDOMDocument> xmlDoc;
	CComPtr<IXMLDOMDocument> ret = NULL;
	VARIANT_BOOL bSucc;
	
	
	HRCALL(xmlDoc.CoCreateInstance(__uuidof(DOMDocument)),"Создание XMLDOMDocument. ");

	if (S_OK == xmlDoc->load(CComVariant(fname),&bSucc))
	{
		ret = xmlDoc;
	}
	
	return ret;
}


void CSettings::CXMLSettingsTool::IterateParams(CComPtr<IXMLDOMNodeList> paramNodes)
{
	CComPtr<IXMLDOMNode> paramNode;
	long paramsCnt = 0;

	paramNodes->get_length(&paramsCnt);
	for (long i = 0; i < paramsCnt; i++)
	{
		paramNodes->get_item(i,&paramNode);
		LoadParams(paramNode);
		paramNode.Release();
	}
}

BOOL CSettings::CXMLSettingsTool::LoadParams(CComPtr<IXMLDOMNode> paramNode)
{
	BOOL ret = FALSE;
	CComBSTR nodeName;
	CComBSTR nodeText;
	paramNode->get_nodeName(&nodeName);
	if (nodeName == A2BSTR("baude_rate"))
	{
		paramNode->get_text(&nodeText);
		GetParam(nodeText,&_settings->_baudeRate);
	}
	if (nodeName == A2BSTR("timeout"))
	{
		paramNode->get_text(&nodeText);
		GetParam(nodeText,&_settings->_timeout);
	}
	if (nodeName == A2BSTR("parity"))
	{
		paramNode->get_text(&nodeText);
		GetParam(nodeText,&_settings->_parity);
	}
	if (nodeName == A2BSTR("stop_bits"))
	{
		paramNode->get_text(&nodeText);
		GetParam(nodeText,&_settings->_stopBits);
	}
	if (nodeName == A2BSTR("byte_size"))
	{
		paramNode->get_text(&nodeText);
		GetParam(nodeText,&_settings->_byteSize);
	}
	if (nodeName == A2BSTR("wait_byte"))
	{
		paramNode->get_text(&nodeText);
		GetParam(nodeText,&_settings->_waitByte);
	}
	if (nodeName == A2BSTR("wait_request"))
	{
		paramNode->get_text(&nodeText);
		GetParam(nodeText,&_settings->_waitRequest);
	}
	if (nodeName == A2BSTR("response_on"))
	{
		paramNode->get_text(&nodeText);
		GetParam(nodeText,&_settings->_responseOn);
	}
	if (nodeName == A2BSTR("response_off"))
	{
		paramNode->get_text(&nodeText);
		GetParam(nodeText,&_settings->_responseOff);
	}
	if (nodeName == A2BSTR("force_closed"))
	{
		paramNode->get_text(&nodeText);
		GetParam(nodeText,&_settings->_bForceClosed);
	}
	return ret;
}

 template<class T> void CSettings::CXMLSettingsTool::GetParam(BSTR nodeText,T* value)
{
	USES_CONVERSION;
	char* buf = W2A(nodeText);

	switch(sizeof(*value))
	{
	case 4:
		sscanf(buf,"%ld",value);
		break;
	case 1:
		sscanf(buf,"%hc",value);
		*value -= '0';
		break;
	default:
		sscanf(buf,"%d",value);
		break;
	}
	
}

BYTE CSettings::CXMLSettingsTool::GetPortNum(const VARIANT varPortNum)
{	
	int ret;

	USES_CONVERSION;
	char* buf = W2A(varPortNum.bstrVal);
	sscanf(buf,"%d",&ret);
	
	return ret;
}

BOOL CSettings::CXMLSettingsTool::Save(const char *fname)
{
	BOOL ret = FALSE;
	CComPtr<IXMLDOMDocument> xmlDoc;
	CComPtr<IXMLDOMNode> newPortNode;
	CComPtr<IXMLDOMNode> oldPortNode;
	CComPtr<IXMLDOMNode> outOldPortNode;
	CComPtr<IXMLDOMElement> rootElement;

	xmlDoc = LoadXMLDoc(fname);	
    

	if (xmlDoc)
	{
		HRCALL(xmlDoc->get_documentElement(&rootElement),"Запрос root узла. ");
		newPortNode = CreatePortNode(xmlDoc);
		oldPortNode = SavedPortNode(xmlDoc,_settings->_portNum);

		if(oldPortNode)
		{
			HRCALL(rootElement->replaceChild(newPortNode,oldPortNode,&outOldPortNode),"Замена port узла. ");
		}
		else
		{
			HRCALL(rootElement->appendChild(newPortNode,&outOldPortNode),"Добавление port узла. ");
		}

		HRCALL(xmlDoc->putref_documentElement(rootElement),"Добавление root узла. ");
		HRCALL(xmlDoc->save(CComVariant(fname)),"Сохранение XMLDOMDocument. ");
		
	}
	else
	{
		SaveXMLDoc(fname,xmlDoc);
	}
		
	return ret;
}

void CSettings::CXMLSettingsTool::SaveXMLDoc(const char *fname,CComPtr<IXMLDOMDocument> xmlDoc)
{

	CComPtr<IXMLDOMElement> rootElement;
	CComPtr<IXMLDOMNode> outChild;

	HRCALL(xmlDoc.CoCreateInstance(__uuidof(DOMDocument)),"Создание XMLDOMDocument. ");
	HRCALL(xmlDoc->createElement(A2BSTR("SerialSettings"),&rootElement),"Создание root узла. ");
	HRCALL(rootElement->appendChild(CreatePortNode(xmlDoc),&outChild),"Добавление узлов port. ");		
	HRCALL(xmlDoc->putref_documentElement(rootElement),"Добавление root узла. ");
	HRCALL(xmlDoc->save(CComVariant(fname)),"Сохранение XMLDOMDocument. ");
}


CComPtr<IXMLDOMNode> CSettings::CXMLSettingsTool::CreatePortNode(CComPtr<IXMLDOMDocument> xmlDoc)
{
	CComPtr<IXMLDOMNode> portNode;
	CComPtr<IXMLDOMElement> portElement;
	CComPtr<IXMLDOMAttribute> portAttribute;
		
	xmlDoc->createNode(CComVariant(NODE_ELEMENT),A2BSTR("port"),A2BSTR(""),&portNode);
	xmlDoc->createAttribute(A2BSTR("number"),&portAttribute);
	portElement = portNode;
	portElement->setAttribute(A2BSTR("number"),CComVariant(_settings->_portNum));

	AddTextChild(xmlDoc,portNode,"baude_rate",_settings->_baudeRate);
	AddTextChild(xmlDoc,portNode,"timeout",_settings->_timeout);
	AddTextChild(xmlDoc,portNode,"parity",_settings->_parity);
	AddTextChild(xmlDoc,portNode,"stop_bits",_settings->_stopBits);
	AddTextChild(xmlDoc,portNode,"byte_size",_settings->_byteSize);
	AddTextChild(xmlDoc,portNode,"wait_byte",_settings->_waitByte);
	AddTextChild(xmlDoc,portNode,"wait_request",_settings->_waitRequest);
	AddTextChild(xmlDoc,portNode,"response_on",_settings->_responseOn);
	AddTextChild(xmlDoc,portNode,"response_off",_settings->_responseOff);
	AddTextChild(xmlDoc,portNode,"force_closed",_settings->_bForceClosed);
	
    return portNode;
}



template <class T> void	CSettings::CXMLSettingsTool::AddTextChild(CComPtr<IXMLDOMDocument> xmlDoc,CComPtr<IXMLDOMNode> parentNode,const char* name,T value)
{
	CString buf;
	CComPtr<IXMLDOMNode> childNode;
	CComPtr<IXMLDOMNode> outChild;

	buf.Format("%d",value);		
	HRCALL(xmlDoc->createNode(CComVariant(NODE_ELEMENT),A2BSTR(name),A2BSTR(""),&childNode),"Создание узла. ");
	HRCALL(childNode->put_text(A2BSTR((LPCSTR)buf)),"Установка текста узлу. ");
	HRCALL(parentNode->appendChild(childNode,&outChild),"Добавление узла param. ");
	outChild.Release();
}

CSettings::CXMLSettingsTool::CXMLSettingsTool(CSettings* settings)
{
	_settings = settings;
}

//  [7/25/2006] -----------------------------------------------------------------------------------------------


BYTE CSettings::_currentPortNum = 0;
IPort* CSettings::_iPort = NULL;
IManager* CSettings::_iManager = NULL;
SAFEARRAY* CSettings::_existPorts = NULL;
IServer* CSettings::_iSrv = NULL;



BOOL CSettings::InitCOM()
{
	return (S_OK == CoInitializeEx(NULL,COINIT_MULTITHREADED));				
}


void CSettings::UninitCom(BOOL bSuccInit)
{
	bSuccInit ? CoUninitialize() : FALSE;
}

void CSettings::ReleaseAll()
{
	_iSrv ? _iSrv->Release() : FALSE;
	_iManager ? _iManager->Release() : FALSE;
	_iManager  = NULL;
	_iPort = NULL;
	_iSrv = NULL;
}

ULONG CSettings::GetExistPorts(BYTE *retExistPorts)
{
	WriteLog("CSettings:GetExistPorts");
	CRITICAL_SECTION cs;
	InitializeCriticalSection(&cs);
	EnterCriticalSection(&cs);

	if (!retExistPorts)
	{
		AfxMessageBox("NULL Pointer retExistPorts in  CSettings:GetExistPorts!!");
	}

	BOOL succInit = InitCOM();
		InitInterfaces();
		
	USHORT portCnt;
	ULONG ret = 0;
	// Получим кол-во  максимально возможное число портов
	HRCALL(_iSrv->get_MaxPortCnt(&portCnt),"get_MaxPortCnt - FAILED");
	_existPorts = SafeArrayCreateVector(VT_UI1,0,portCnt);
	// Получим номера портов которые можно открыть 
	HRESULT hExist = _iSrv->get_ExistPorts(&_existPorts);

	int a = HRESULT_FACILITY(hExist);
	if (E_NOPORTS != hExist && FAILED(hExist) && 0x111 != HRESULT_FACILITY(hExist))
	{
		throw hExist;
	}

	if (_existPorts)
	{
		if (E_NOPORTS  == hExist)
		{
			ret = 0;
		}
		else
		{
			ret = _existPorts->rgsabound[0].cElements;

			for (ULONG i = 0; i < _existPorts->rgsabound[0].cElements ; i++ )
			{
				SafeArrayGetElement(_existPorts,(LONG*)&i,&_currentPortNum);
				if (!IsPortBroken(hExist,_currentPortNum))
				{
					retExistPorts[i] = _currentPortNum;
				}
				else
				{
					ret  = ret > 0 ? ret - 1 : ret;
				}
								
				
			}		
		}
		
	}
	UninitCom(succInit);
	LeaveCriticalSection(&cs);

	DeleteCriticalSection(&cs);
	return ret;
}

BOOL CSettings::IsPortBroken(HRESULT hExist,byte portNum)
{
	return (0x100 == HRESULT_FACILITY(hExist) && (HRESULT_CODE(hExist) & (int)pow(2,(double)portNum)) );
}

CString CSettings::GetDocumentsPath()
{
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, szPath)))
	{
		return CString(szPath);
	}
	return _T("");
}

BOOL CSettings::CreateDirectoryRecursive(LPCTSTR lpPath)
{
	TCHAR szDir[MAX_PATH];
	_tcscpy_s(szDir, MAX_PATH, lpPath);
	PathRemoveFileSpec(szDir);

	return (SHCreateDirectoryEx(NULL, szDir, NULL) == ERROR_SUCCESS);
}

void CSettings::SaveAll(const char* fname, CSettings* settings, BOOL bOnlyApply)
{
	WriteLog("CSettings:SaveAll");
	CRITICAL_SECTION cs;
	InitializeCriticalSection(&cs);
	EnterCriticalSection(&cs);
	BOOL bSuccInit = InitCOM();
	InitInterfaces();

	// Если имя файла не указано, используем путь к Документам
	CStringA strFilePath;

	CString strDocuments = CSettings().GetDocumentsPath();
	if (!strDocuments.IsEmpty())
	{
		CString strFullPath;
		strFullPath.Format(_T("%s\\БЭМН\\УниКон\\serial.xml"), strDocuments);
		strFilePath = CT2A(strFullPath);
	
		// Создаем директории, если их нет
		CSettings().CreateDirectoryRecursive(strFullPath);
	}
	else
	{
		strFilePath = "serial.xml"; // fallback
	}

	BYTE existPorts[MAX_PORT];
	ULONG existPortsCnt = 0;
	try
	{
		existPortsCnt = GetExistPorts(existPorts);
	}
	catch (HRESULT e)
	{
		throw e;
	}

	BYTE currentPortNum = 0;

	for (ULONG i = 0; i < existPortsCnt; i++)
	{
		currentPortNum = existPorts[i];

		if (SUCCEEDED(_iSrv->GetPort(currentPortNum, &_iPort)))
		{
			_iManager->SerializePort(currentPortNum);
			settings[currentPortNum].ToPort(_iPort);

			if (1 == currentPortNum)
			{
				WriteLog("CSettings: Set baude = %d", settings[currentPortNum]._baudeRate);
			}

			VARIANT_BOOL bOpened;
			_iPort->get_Opened(&bOpened);
			if (bOpened)
			{
				ULONG clientCnt = 0;
				_iPort->get_OpenCount(&clientCnt);
				_iPort->Stop();
				_iPort->SystemClose();
				for (ULONG i = 0; i < clientCnt; i++)
				{
					if (E_CONFIGURE == _iPort->Open())
					{
						_iManager->DeserializePort(currentPortNum);
						throw E_CONFIGURE;
					}
				}
				_iPort->Start();
			}
			else
			{
				if (E_CONFIGURE == _iPort->Open())
				{
					_iManager->DeserializePort(currentPortNum);
					throw E_CONFIGURE;
				}
				_iPort->SystemClose();
			}

			if (!bOnlyApply)
			{
				settings[currentPortNum].Save(strFilePath);
			}

			_iManager->SerializePort(currentPortNum);
			_iPort->Release();
		}
	}

	ReleaseAll();
	UninitCom(bSuccInit);
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
}



void CSettings::LoadAll(const char* fileName, CSettings** retSettings)
{
	WriteLog("CSettings:LoadAll");
	CRITICAL_SECTION cs;
	InitializeCriticalSection(&cs);
	EnterCriticalSection(&cs);

	BOOL bSuccInit = InitCOM();
	InitInterfaces();

	CSettings* settings = new CSettings[MAX_PORT];
	BYTE existPorts[MAX_PORT];
	ULONG existPortsCnt = 0;
	try
	{
		existPortsCnt = GetExistPorts(existPorts);
	}
	catch (HRESULT e)
	{
		throw e;
	}

	// Если имя файла не указано, используем путь к Документам
	CStringA strFilePath;
	if (fileName == NULL || strlen(fileName) == 0)
	{
		CString strDocuments = CSettings().GetDocumentsPath();
		if (!strDocuments.IsEmpty())
		{
			CString strFullPath;
			strFullPath.Format(_T("%s\\БЭМН\\УниКон\\serial.xml"), strDocuments);
			strFilePath = CT2A(strFullPath);
		}
		else
		{
			strFilePath = "serial.xml"; // fallback
		}
	}
	else
	{
		strFilePath = fileName;
	}

	BYTE currentPortNum = 0;

	for (ULONG i = 0; i < existPortsCnt; i++)
	{
		currentPortNum = existPorts[i];

		// Пробуем загрузить параметры из файла, в случае неудачи берем те настройки, что стоят по дефолту 
		// в обекте порта.
		if (!settings[currentPortNum].Load(strFilePath, currentPortNum))
		{
			// Проверяем, что _iSrv инициализирован
			if (_iSrv != nullptr)
			{
				_iSrv->GetPort(currentPortNum, &_iPort);
				settings[currentPortNum].FromPort(_iPort);
			}
			else
			{
				WriteLog("ERROR: _iSrv is nullptr in LoadAll!");
				// Обработка ошибки - можно использовать значения по умолчанию
			}
		}
	}

	*retSettings = settings;

	ReleaseAll();
	UninitCom(bSuccInit);
	WriteLog("CSettings:ReleaseAll");
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
}


void CSettings::InitInterfaces(void)
{
	HRESULT hRes = NULL;
	if (!_iSrv)
	{
		hRes = CoCreateInstance(__uuidof(CServer),NULL,CLSCTX_ALL,__uuidof(IServer),(void**)&_iSrv);
	}

	if (!_iManager)
	{
		HRCALL(_iSrv->QueryInterface(__uuidof(_iManager),(void**)&_iManager),"Получить IManager");
	}

}
