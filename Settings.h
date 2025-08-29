#pragma once
#import "../SerialServer/_SerialServer.tlb" raw_interfaces_only, no_namespace, named_guids
#include <atlcoll.h>
#include <atlsafe.h>
#include <shlobj.h>
#include <shlwapi.h>
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")

class CSettings
{
    class CXMLSettingsTool
    {
    public:
        CXMLSettingsTool(CSettings* settings);
        BOOL Save(const char* fname);
        BOOL Load(const char* fname, const BYTE portNum);
    private:
        CSettings* _settings;
    protected:
        CComPtr<IXMLDOMDocument> LoadXMLDoc(const char* fname);
        void SaveXMLDoc(const char* fname, CComPtr<IXMLDOMDocument> xmlDoc);
        template <class T> void AddTextChild(CComPtr<IXMLDOMDocument> xmlDoc, CComPtr<IXMLDOMNode> parentNode, const char* name, T value);
        template <class T> void GetParam(BSTR nodeText, T* value);
        BYTE GetPortNum(const VARIANT varPortNum);
        CComPtr<IXMLDOMNode> SavedPortNode(CComPtr<IXMLDOMDocument> xmlDoc, const BYTE portNum);
        BOOL LoadParams(CComPtr<IXMLDOMNode> paramNode);
        void IterateParams(CComPtr<IXMLDOMNodeList> paramNodes);
        CComPtr<IXMLDOMNode> CreatePortNode(CComPtr<IXMLDOMDocument> xmlDoc);
    };

public:
    CSettings();
    ~CSettings(void);

    void FromPort(IPort* iPort);
    void ToPort(IPort* iPort);
    BOOL Save(const char* fname);
    BOOL Load(const char* fname, const BYTE portNum);

    // Объявление методов без реализации
    CString GetDocumentsPath();
    BOOL CreateDirectoryRecursive(LPCTSTR lpPath);

public:
    BOOL _bForceClosed;
    BOOL _bComInit;
    DWORD _baudeRate;
    BYTE  _byteSize;
    BYTE  _parity;
    BYTE  _stopBits;
    DWORD _timeout;
    DWORD _waitByte;
    DWORD _waitRequest;
    BYTE _portNum;
    DWORD _responseOn;
    DWORD _responseOff;

private:
    CXMLSettingsTool* _xmltool;

    static IPort* _iPort;
    static IServer* _iSrv;
    static IManager* _iManager;
    static SAFEARRAY* _existPorts;
    static BYTE _currentPortNum;

    static BOOL InitCOM();
    static void UninitCom(BOOL bSuccInit);
    static void ReleaseAll();
    static void InitInterfaces(void);

public:
    static void LoadAll(const char* fileName, CSettings** retSettings);
    static void SaveAll(const char* fileName, CSettings* settings, BOOL bOnlyApply);
    static ULONG GetExistPorts(BYTE* retExistPorts);
    static BOOL IsPortBroken(HRESULT hExists, byte portNum);
};