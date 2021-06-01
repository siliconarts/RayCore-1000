#include "LicenseManager.h"
#include "NetworkAdapter.h"
#include <CommCtrl.h>
#include "resource.h"
#include "SEED_KISA.h"
#include "Mail.h"
#include <stdarg.h>

using namespace std;

#pragma comment(linker, \
	"\"/manifestdependency:type='Win32' "\
	"name='Microsoft.Windows.Common-Controls' "\
	"version='6.0.0.0' "\
	"processorArchitecture='*' "\
	"publicKeyToken='6595b64144ccf1df' "\
	"language='*'\"")
#pragma comment(lib, "ComCtl32.lib")
#pragma comment(lib, "Winmm.lib")

typedef enum{
	RECIPIENT_SILICONARTS,
	RECIPIENT_MRA,
}RECIPIENT;

static BYTE g_pUserKey[16] = {	0x45, 0x33, 0x15, 0xA3, 0xB4, 0x89, 0x3A, 0x7B, 		// User secret key
								0xDE, 0xAD, 0xC4, 0xC8, 0xAC, 0xEE, 0xCF, 0x0B};

static BYTE g_sActiveKey[16] = { 'L', 'i', 'c', 'e', 'n', 's', 'e', 'a',
								 'c', 't', 'i', 'v', 'a', 't', 'e', '\0'};

LicenseManager	g_LicenseManager;

static void scatprint(char* str, const char* format, ...){
	char line[1024];
	va_list	ap;
	va_start(ap, format);
	vsprintf(line, format, ap);
	va_end(ap);
	strcat(str, line);
}

static void strtrim(char* str, char ch){
	char* pStr;
	while((pStr = strchr(str, ch))){
		strcpy(pStr, pStr+1);
	}
}

static BOOL CheckEmailAddress(char* str){
	if(!*str) return FALSE;
	int at_pos = -1;
	for(int i=0;str[i];i++){
		char ch = str[i];
		if(ch >= '0' && ch <= '9') continue;
		if(ch >= 'a' && ch <= 'z') continue;
		if(ch >= 'A' && ch <= 'Z') continue;
		if(i!=0){
			if(at_pos == -1 && ch == '@'){
				at_pos	= i;
				continue;
			}
			if(ch == '_' || ch == '-' || ch == '.') continue;
		}
		return FALSE;
	}
	if(at_pos == -1) return FALSE;
	if(at_pos == strlen(str)-1) return FALSE;
	return TRUE;
}

LicenseManager::LicenseManager(void){
	InitCommonControls();
	{	// retreive license path
		TCHAR module_path[2048];
		TCHAR* pFilePart;
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (PCTSTR)scatprint, &m_hModule);
		GetModuleFileName(m_hModule, module_path, 2047);
		GetFullPathName(module_path, 2047, m_sLicensePath, &pFilePart);
		*pFilePart	= NULL;
		strcat(m_sLicensePath, "raycore_license.ini");
	}
	m_bLicensePresent	= FALSE;
}

LicenseManager::~LicenseManager(void){
}

static LPCTSTR	__sAppName			= "LICENSE";
static LPCTSTR	__sFirstName		= "FIRST_NAME";
static LPCTSTR	__sLastName			= "LAST_NAME";
static LPCTSTR	__sCompanyName		= "COMPANY";
static LPCTSTR	__sAddress1			= "ADDRESS1";
static LPCTSTR	__sAddress2			= "ADDRESS2";
static LPCTSTR	__sCity				= "CITY";
static LPCTSTR	__sState			= "STATE";
static LPCTSTR	__sZipCode			= "ZIPCODE";
static LPCTSTR	__sCountry			= "COUNTRY";
static LPCTSTR	__sPhone			= "PHONE";
static LPCTSTR	__sEmail			= "EMAIL";
static LPCTSTR	__sRegNumber		= "REG_NUMBER";
static LPCTSTR	__sSubmitterID		= "SUBMITTER_ID";
static LPCTSTR	__sKeyEthernet		= "ETHERNET";
static LPCTSTR	__sKeyActivateCode	= "ACTIVATE_CODE";
static LPCTSTR	__sDistribution		= "DISTRIBUTION";

int LicenseManager::GetLicenseInt(LPCTSTR sKeyName, int iDefault){
	return GetPrivateProfileInt(__sAppName, sKeyName, iDefault, m_sLicensePath);
}

void LicenseManager::SetLicenseInt(LPCTSTR sKeyName, int iData){
	CString str;
	str.Format("%d", iData);
	WritePrivateProfileString(__sAppName, sKeyName, str, m_sLicensePath);
}

void LicenseManager::GetLicenseString(LPCTSTR sKeyName, LPTSTR sGet, DWORD dwSize){
	GetPrivateProfileString(__sAppName, sKeyName, "", sGet, dwSize, m_sLicensePath);
}

void LicenseManager::SetLicenseString(LPCTSTR sKeyName, LPTSTR sSet){
	strtrim(sSet, '?');
	strtrim(sSet, '=');
	strtrim(sSet, '\"');
	strtrim(sSet, '&');
	WritePrivateProfileString(__sAppName, sKeyName, sSet, m_sLicensePath);
}

BOOL LicenseManager::GenLicense(const char* request_string, BYTE* activate_code, int card_count){
	BYTE	request_code[16];

	printf("Requested code : %s\n", request_string);
	{	// get request code
		if(strlen(request_string) != 32) return FALSE;
		for(int i=0;i<16;i++){
			TCHAR	sCode[3];
			DWORD	dwCode;
			sCode[0]	= request_string[i*2];
			sCode[1]	= request_string[i*2+1];
			sCode[2]	= NULL;
			sscanf(sCode, "%02X", &dwCode);
			request_code[i] = (BYTE)dwCode;
		}
	}

	{
		DWORD RoundKey[32];

		{
			memcpy(activate_code, g_sActiveKey, sizeof(BYTE)*16);
			activate_code[15] = card_count;

			SeedRoundKey(RoundKey, request_code);
			SeedEncrypt(activate_code, RoundKey);
			SeedRoundKey(RoundKey, g_pUserKey);
			SeedEncrypt(activate_code, RoundKey);
		}
	}

	return TRUE;
}

void LicenseManager::Initialize(HWND hWndDlg){
	HWND hWnd;
	{	// update fields
		TCHAR sName[1024];
		GetLicenseString(__sFirstName, sName, 1023);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_FIRST_NAME, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sName));
		GetLicenseString(__sLastName, sName, 1023);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_LAST_NAME, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sName));
		GetLicenseString(__sCompanyName, sName, 1023);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_COMPANY_NAME, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sName));
		GetLicenseString(__sAddress1, sName, 1023);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_ADDRESS_1, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sName));
		GetLicenseString(__sAddress2, sName, 1023);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_ADDRESS_2, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sName));
		GetLicenseString(__sCity, sName, 1023);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_CITY, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sName));
		GetLicenseString(__sState, sName, 1023);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_STATE, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sName));
		GetLicenseString(__sZipCode, sName, 1023);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_ZIPCODE, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sName));
		GetLicenseString(__sCountry, sName, 1023);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_COUNTRY, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sName));
		GetLicenseString(__sPhone, sName, 1023);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_PHONE, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sName));
		GetLicenseString(__sEmail, sName, 1023);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_EMAIL, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sName));
		GetLicenseString(__sRegNumber, sName, 1023);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_REG_NUMBER, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sName));
		GetLicenseString(__sSubmitterID, sName, 1023);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_SUBMITTER_ID, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sName));
	}
	{	// update activate code
		TCHAR sActivateKey[1024];
		GetLicenseString(__sKeyActivateCode, sActivateKey, 1023);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_LICENSE_KEY, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sActivateKey));
	}

	hWnd	= GetDlgItem(hWndDlg, IDC_COMBO_REQUEST_COUNT);
	{
		for(int i=0;i<8;i++){
			char name[1024];
			sprintf(name, "%d license activation", i+1);
			SendMessage(hWnd, CB_INSERTSTRING, i, reinterpret_cast<LPARAM>(name));
			SendMessage(hWnd, CB_SETITEMDATA, i, (LPARAM)(i+1));
		}
		SendMessage(hWnd, CB_SETCURSEL, 0, 0);
	}

	hWnd	= GetDlgItem(hWndDlg, IDC_COMBO_REQUEST_RECIPIENT);
	{
		const char* sRecipient_Siliconarts	= "Siliconarts, Inc.";
		const char* sRecipient_MRA			= "MRA Digital, LLC";

		SendMessage(hWnd, CB_INSERTSTRING, 0, reinterpret_cast<LPARAM>(sRecipient_Siliconarts));
		SendMessage(hWnd, CB_SETITEMDATA, 0, RECIPIENT_SILICONARTS);
		SendMessage(hWnd, CB_INSERTSTRING, 1, reinterpret_cast<LPARAM>(sRecipient_MRA));
		SendMessage(hWnd, CB_SETITEMDATA, 1, RECIPIENT_MRA);
		SendMessage(hWnd, CB_SETCURSEL, GetLicenseInt(__sDistribution), 0);
	}

	hWnd	= GetDlgItem(hWndDlg, IDC_COMBO_ADAPTER_LIST);
	{	// ethernet Adapter combo listbox
		int index = 0;

		{
			PIP_ADAPTER_INFO	pAdapter	= g_NetworkAdapter.GetAdapters();
			while(pAdapter){
				char name[1024];
				if(	(pAdapter->AddressLength == 6) &&
					(pAdapter->Type == MIB_IF_TYPE_ETHERNET) &&
					!strstr(pAdapter->Description, "Virtual") &&
					!strstr(pAdapter->Description, "VMWare"))
				{
					sprintf(name, "%s [%02X%02X%02X%02X%02X%02X]",
						pAdapter->Description,
						pAdapter->Address[0],
						pAdapter->Address[1],
						pAdapter->Address[2],
						pAdapter->Address[3],
						pAdapter->Address[4],
						pAdapter->Address[5]);
					SendMessage(hWnd, CB_INSERTSTRING, index, reinterpret_cast<LPARAM>(name));
					SendMessage(hWnd, CB_SETITEMDATA, index, reinterpret_cast<LPARAM>(pAdapter));
					index++;
				}
				pAdapter	= pAdapter->Next;
			}
		}

		if(!index){
			SendMessage(hWnd, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(("No network adaptor!")));
			SendMessage(hWnd, CB_SETITEMDATA, index, NULL);
		}
		SendMessage(hWnd, CB_SETCURSEL, 0, 0);

		if(index)
		{	// check ethernet
			TCHAR	sEthernet[256], sCmp[256];
			GetLicenseString(__sKeyEthernet, sEthernet, 255);
			for(int i=0;i<index;i++){
				PIP_ADAPTER_INFO	pAdapter	= (PIP_ADAPTER_INFO)SendMessage(hWnd, CB_GETITEMDATA, i, 0);
				if(!pAdapter) continue;
				sprintf(sCmp, "%02X%02X%02X%02X%02X%02X",
					pAdapter->Address[0],
					pAdapter->Address[1],
					pAdapter->Address[2],
					pAdapter->Address[3],
					pAdapter->Address[4],
					pAdapter->Address[5]);
				if(!strcmp(sEthernet, sCmp)){
					SendMessage(hWnd, CB_SETCURSEL, i, 0);
					break;
				}
			}
		}
	}
	UpdateDlg(hWndDlg);
	ActivateLicense(hWndDlg);
}

void LicenseManager::UpdateLicenseFile(HWND hWndDlg){
	{	// save request id
		TCHAR sName[1024];
		SendDlgItemMessage(hWndDlg, IDC_EDIT_FIRST_NAME, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
		SetLicenseString(__sFirstName, sName);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_LAST_NAME, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
		SetLicenseString(__sLastName, sName);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_COMPANY_NAME, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
		SetLicenseString(__sCompanyName, sName);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_ADDRESS_1, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
		SetLicenseString(__sAddress1, sName);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_ADDRESS_2, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
		SetLicenseString(__sAddress2, sName);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_CITY, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
		SetLicenseString(__sCity, sName);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_STATE, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
		SetLicenseString(__sState, sName);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_ZIPCODE, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
		SetLicenseString(__sZipCode, sName);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_COUNTRY, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
		SetLicenseString(__sCountry, sName);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_PHONE, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
		SetLicenseString(__sPhone, sName);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_EMAIL, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
		SetLicenseString(__sEmail, sName);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_REG_NUMBER, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
		SetLicenseString(__sRegNumber, sName);
		SendDlgItemMessage(hWndDlg, IDC_EDIT_SUBMITTER_ID, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
		SetLicenseString(__sSubmitterID, sName);
	}
	{	// save ethernet address
		TCHAR	sEthernet[256];
		PIP_ADAPTER_INFO	pAdapter;
		pAdapter	= (PIP_ADAPTER_INFO) SendDlgItemMessage(hWndDlg, IDC_COMBO_ADAPTER_LIST, CB_GETITEMDATA, SendDlgItemMessage(hWndDlg, IDC_COMBO_ADAPTER_LIST, CB_GETCURSEL, 0, 0), 0);

		if(pAdapter)
			sprintf(sEthernet, "%02X%02X%02X%02X%02X%02X",
			pAdapter->Address[0],
			pAdapter->Address[1],
			pAdapter->Address[2],
			pAdapter->Address[3],
			pAdapter->Address[4],
			pAdapter->Address[5]);
		else
			sEthernet[0]	= NULL;

		SetLicenseString(__sKeyEthernet, sEthernet);
	}
	{	// save distribution
		SetLicenseInt(__sDistribution, SendDlgItemMessage(hWndDlg, IDC_COMBO_REQUEST_RECIPIENT, CB_GETCURSEL, 0, 0));
	}
	{	// save activate key code
		TCHAR sActivateKey[1024];
		SendDlgItemMessage(hWndDlg, IDC_EDIT_LICENSE_KEY, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sActivateKey));
		SetLicenseString(__sKeyActivateCode, sActivateKey);
	}
}

int LicenseManager::CheckLicense(void){
	BYTE				request_key[16];
	BYTE				activate_key[16];

	m_dwPrevTime		= timeGetTime();
	m_bLicensePresent	= FALSE;
	{	// get request key
		TCHAR				request_name[1024];
		int					request_name_len;
		memset(request_key, 0, sizeof(BYTE)*16);
		GetLicenseString(__sFirstName, request_name, 1023);
		request_name_len	= strlen(request_name);
		if(!request_name[0]) return 0;
		request_name[request_name_len]	= ' ';
		request_name_len++;
		GetLicenseString(__sLastName, &request_name[request_name_len], 1023);
		request_name_len	= strlen(request_name);

		{
			TCHAR sEthernet[256];
			GetLicenseString(__sKeyEthernet, sEthernet, 255);
			if(!sEthernet[0]) return 0;
			for(int i=0;i<6;i++){
				TCHAR	sCode[3];
				DWORD	dwCode;
				sCode[0]	= sEthernet[i*2];
				sCode[1]	= sEthernet[i*2+1];
				sCode[2]	= NULL;
				sscanf(sCode, "%02X", &dwCode);
				request_key[i] = (BYTE)dwCode;
			}
			// check exist ethernet network
			{
				PIP_ADAPTER_INFO	pAdapter	= g_NetworkAdapter.GetAdapters();
				while(pAdapter){
					if(	(pAdapter->AddressLength == 6) &&
						(pAdapter->Type == MIB_IF_TYPE_ETHERNET) &&
						!strstr(pAdapter->Description, "Virtual") &&
						!strstr(pAdapter->Description, "VMWare"))
					{
						if(	pAdapter->Address[0] == request_key[0] &&
							pAdapter->Address[1] == request_key[1] &&
							pAdapter->Address[2] == request_key[2] &&
							pAdapter->Address[3] == request_key[3] &&
							pAdapter->Address[4] == request_key[4] &&
							pAdapter->Address[5] == request_key[5])
								goto ETHERNET_CHECKED;
					}
					pAdapter	= pAdapter->Next;
				}
				return 0;
			}
ETHERNET_CHECKED:
			for(int i=0;i<request_name_len;i++)
				request_key[(i+6) % 16] += request_name[i];
		}
	}
	{	// get activate key
		TCHAR				activate_code[1024];
		GetLicenseString(__sKeyActivateCode, activate_code, 1023);
		if(strlen(activate_code) != 32) return 0;
		for(int i=0;i<16;i++){
			TCHAR	sCode[3];
			DWORD	dwCode;
			sCode[0]	= activate_code[i*2];
			sCode[1]	= activate_code[i*2+1];
			sCode[2]	= NULL;
			sscanf(sCode, "%02X", &dwCode);
			activate_key[i] = (BYTE)dwCode;
		}
	}
	
	{
		DWORD RoundKey[32];
		SeedRoundKey(RoundKey, g_pUserKey);
		SeedEncrypt(request_key, RoundKey);

		SeedDecrypt(activate_key, RoundKey);
		SeedRoundKey(RoundKey, request_key);
		SeedDecrypt(activate_key, RoundKey);

		for(int i=0;i<15;i++)
			if(activate_key[i] != g_sActiveKey[i]) return 0;

		m_bLicensePresent	= TRUE;

		return (int)activate_key[15];
	}

	return 0;
}

void LicenseManager::GetRequsterFullName(HWND hWndDlg, LPTSTR sName){
	SendMessage(GetDlgItem(hWndDlg, IDC_EDIT_FIRST_NAME), WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
	{
		char lastname[1024];
		SendMessage(GetDlgItem(hWndDlg, IDC_EDIT_LAST_NAME), WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(lastname));
		if(lastname[0]){
			strcat(sName, " ");
			strcat(sName, lastname);
		}
	}
}

BOOL LicenseManager::UpdateRequestCode(HWND hWndDlg){
	char				request_name[1024];
	int					request_name_len;
	BYTE				request_key[16];
	PIP_ADAPTER_INFO	pAdapter;
	memset(request_key, 0, sizeof(BYTE)*16);
	GetRequsterFullName(hWndDlg, request_name);
	request_name_len	= strlen(request_name);

	if(!request_name[0]) return FALSE;
	
	if(!SendDlgItemMessage(hWndDlg, IDC_COMBO_ADAPTER_LIST, CB_GETCOUNT, 0, 0)) return FALSE;
	pAdapter	= (PIP_ADAPTER_INFO) SendDlgItemMessage(hWndDlg, IDC_COMBO_ADAPTER_LIST, CB_GETITEMDATA, SendDlgItemMessage(hWndDlg, IDC_COMBO_ADAPTER_LIST, CB_GETCURSEL, 0, 0), 0);
	if(!pAdapter) return FALSE;

	for(int i=0;i<6;i++)
		request_key[i] = pAdapter->Address[i];
	
	for(int i=0;i<request_name_len;i++)
		request_key[(i+6) % 16] += request_name[i];

	{
		DWORD RoundKey[32];
		SeedRoundKey(RoundKey, g_pUserKey);
		SeedEncrypt(request_key, RoundKey);
	}
	{
		char sReqKey[256];
		char sByteCode[8];
		sReqKey[0] = NULL;

		for(int i=0;i<16;i++){
			sprintf(sByteCode, "%02X", request_key[i]);
			strcat(sReqKey, sByteCode);
		}

		SendDlgItemMessage(hWndDlg, IDC_EDIT_SERIAL_NUMBER, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sReqKey));
	}

	return TRUE;
}

void LicenseManager::UpdateDlg(HWND hWndDlg){
	char name[1024];
	GetRequsterFullName(hWndDlg, name);
	EnableWindow(GetDlgItem(hWndDlg, IDC_REQUEST_LICENSE), name[0] != NULL);
	if(name[0]){
		PIP_ADAPTER_INFO	pAdapter	= (PIP_ADAPTER_INFO) SendDlgItemMessage(hWndDlg, IDC_COMBO_ADAPTER_LIST, CB_GETITEMDATA, SendDlgItemMessage(hWndDlg, IDC_COMBO_ADAPTER_LIST, CB_GETCURSEL, 0, 0), 0);
		if(!pAdapter){
			SendMessage(GetDlgItem(hWndDlg, IDC_EDIT_SERIAL_NUMBER), WM_SETTEXT, 0, reinterpret_cast<LPARAM>("Can't find any ethernet adaptor!!"));
		}else{
			UpdateRequestCode(hWndDlg);
		}
		EnableWindow(GetDlgItem(hWndDlg, IDC_REQUEST_LICENSE), pAdapter != NULL);
	}else{
		SendMessage(GetDlgItem(hWndDlg, IDC_EDIT_SERIAL_NUMBER), WM_SETTEXT, 0, reinterpret_cast<LPARAM>("First! Fill the requestor's name."));
		EnableWindow(GetDlgItem(hWndDlg, IDC_REQUEST_LICENSE), FALSE);
	}
}

#include "curl/curl.h"

void LicenseManager::RequestLicense(HWND hWndDlg){
	char name[1024];
	BOOL	bReqeustDone			= FALSE;
	int		license_owned			= 0;
	int		license_request_count	= (int)SendDlgItemMessage(hWndDlg, IDC_COMBO_REQUEST_COUNT, CB_GETITEMDATA, SendDlgItemMessage(hWndDlg, IDC_COMBO_REQUEST_COUNT, CB_GETCURSEL, 0, 0), 0);
	UpdateLicenseFile(hWndDlg);
	GetRequsterFullName(hWndDlg, name);
	if(!name[0]) return;
	{
		PIP_ADAPTER_INFO	pAdapter	= (PIP_ADAPTER_INFO) SendDlgItemMessage(hWndDlg, IDC_COMBO_ADAPTER_LIST, CB_GETITEMDATA, SendDlgItemMessage(hWndDlg, IDC_COMBO_ADAPTER_LIST, CB_GETCURSEL, 0, 0), 0);
		if(!pAdapter){
			MessageBox(hWndDlg, "Can't find any ethernet adaptor!!\nIt's ethernet based license management system.\nYour request is canceled.", "Warning", MB_ICONEXCLAMATION|MB_OK);
			return;
		}else{
			if(!UpdateRequestCode(hWndDlg)) return;
			license_owned	= CheckLicense();
		}
		switch((RECIPIENT) SendDlgItemMessage(hWndDlg, IDC_COMBO_REQUEST_RECIPIENT, CB_GETITEMDATA, SendDlgItemMessage(hWndDlg, IDC_COMBO_REQUEST_RECIPIENT, CB_GETCURSEL, 0, 0), 0)){
		case RECIPIENT_SILICONARTS:
			{	// request to Siliconarts
				TCHAR sName[1024];
				CMail*	pMail	= new CMail;

				pMail->AddRecipient("support@siliconarts.com", "siliconarts");
				pMail->SetSubject("RayCore license request.");
				
				pMail->SetSender("siliconarts.com@gmail.com", "client");
				
				{
					TCHAR sMsg[1024];
#define SetMailField(format, name)	GetLicenseString(name, sName, 1023); if(!sName[0]) goto MAIL_ERROR_OCCURED; sprintf(sMsg, "%s = %s\r\n",format, sName); pMail->AddText(sMsg);


					SendDlgItemMessage(hWndDlg, IDC_EDIT_SERIAL_NUMBER, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
					sprintf(sMsg, "reqCode=%s\r\n",sName);
					pMail->AddText(sMsg);

					sprintf(sMsg, "macAddress=%s [%02X:%02X:%02X:%02X:%02X:%02X]\r\n",
						pAdapter->Description,
						pAdapter->Address[0],
						pAdapter->Address[1],
						pAdapter->Address[2],
						pAdapter->Address[3],
						pAdapter->Address[4],
						pAdapter->Address[5]);
					pMail->AddText(sMsg);

					{	// current license count
						if(!license_owned) sprintf(sName, "none (%d license request.)", license_request_count);
						else sprintf(sName, "%d card license is owned. (%d license request.)", license_owned, license_request_count);
						sprintf(sMsg, "alreadyPurchased=%s\r\n", sName);
						pMail->AddText(sMsg);
					}
					{	// current license
						if(!license_owned) strcpy(sName, "none");
						else GetLicenseString(__sKeyActivateCode, sName, 1023);
						sprintf(sMsg, "licenseOwned=%s\r\n", sName);
						pMail->AddText(sMsg);
					}

					SetMailField("submitterID", __sSubmitterID);

					SetMailField("email", __sEmail);
					if(!CheckEmailAddress(sName)){	// 정상 이메일주소 확인
						MessageBox(hWndDlg, "Incorrect e-mail format.", "Warning", MB_OK|MB_ICONQUESTION);
						goto MAIL_PROCESS_DONE;
					}
					SetMailField("regNum", __sRegNumber);
					SetMailField("phone", __sPhone);
					SetMailField("country", __sCountry);
					SetMailField("zipcode", __sZipCode);
					SetMailField("state", __sState);
					SetMailField("city", __sCity);
					SetMailField("addressLine2", __sAddress2);
					SetMailField("addressLine1", __sAddress1);
					SetMailField("companyName", __sCompanyName);
					SetMailField("lastName", __sLastName);
					SetMailField("firstName", __sFirstName);
				}

				pMail->Send();

				bReqeustDone	= TRUE;
				goto MAIL_PROCESS_DONE;
MAIL_ERROR_OCCURED:
				MessageBox(hWndDlg, "All fields must be filled!", "Warning", MB_OK|MB_ICONQUESTION);
MAIL_PROCESS_DONE:
				pMail->Release();
			}
			break;
		case RECIPIENT_MRA:
			{	// request to MRA
				curl_global_init(CURL_GLOBAL_ALL);
				{
					CURL* pCtx	= curl_easy_init();
					char fields[4096];
					fields[0]	= '\0';

					if(!pCtx){
						MessageBox(hWndDlg, "Can't initialize cURL interface", "Warning", MB_OK);
						return;
					}
					curl_easy_setopt(pCtx, CURLOPT_URL, "http://milappmarket.com/sdk-reg-form-n5mztvfhem");
					curl_easy_setopt(pCtx, CURLOPT_NOPROGRESS, TRUE);
					curl_easy_setopt(pCtx, CURLOPT_WRITEHEADER, stderr);	// 헤더는 표준 에러로 출력함.
					curl_easy_setopt(pCtx, CURLOPT_WRITEDATA, stdout);		// body 데이터는 표준 출력함.

					{
						TCHAR sName[1024];

#define SetField(format, name)	GetLicenseString(name, sName, 1023); if(!sName[0]) goto ERROR_OCCURED; scatprint(fields, "&" format "=%s", sName);

						scatprint(fields, "_wpcf7=1141");
						scatprint(fields, "&_wpnonce=cd15cd1b84");

						SetField("firstName", __sFirstName);
						SetField("lastName", __sLastName);
						SetField("companyName", __sCompanyName);
						SetField("addressLine1", __sAddress1);
						SetField("addressLine2", __sAddress2);
						SetField("city", __sCity);
						SetField("state", __sState);
						SetField("zipcode", __sZipCode);
						SetField("country", __sCountry);
						SetField("phone", __sPhone);
						SetField("regNum", __sRegNumber);
						SetField("email", __sEmail);
						if(!CheckEmailAddress(sName)){	// 정상 이메일주소 확인
							MessageBox(hWndDlg, "Incorrect e-mail format.", "Warning", MB_OK|MB_ICONQUESTION);
							goto PROCESS_DONE;
						}
						SetField("submitterID", __sSubmitterID);
						{	// current license
							if(!license_owned) strcpy(sName, "none");
							else GetLicenseString(__sKeyActivateCode, sName, 1023);
							scatprint(fields, "&licenseOwned=%s", sName);
						}
						{	// current license count
							if(!license_owned) sprintf(sName, "none (%d license request.)", license_request_count);
							else sprintf(sName, "%d card license is owned. (%d license request.)", license_owned, license_request_count);
							scatprint(fields, "&alreadyPurchased=%s", sName);
						}

						scatprint(fields, "&macAddress=%s [%02X:%02X:%02X:%02X:%02X:%02X]",
							pAdapter->Description,
							pAdapter->Address[0],
							pAdapter->Address[1],
							pAdapter->Address[2],
							pAdapter->Address[3],
							pAdapter->Address[4],
							pAdapter->Address[5]);
						SendDlgItemMessage(hWndDlg, IDC_EDIT_SERIAL_NUMBER, WM_GETTEXT, 1023, reinterpret_cast<LPARAM>(sName));
						scatprint(fields, "&reqCode=%s",sName);

						curl_easy_setopt(pCtx, CURLOPT_POSTFIELDS, fields);
					}
					if(curl_easy_perform(pCtx)){
						// error occurred!
						MessageBox(hWndDlg, "License request failed!", "Error", MB_OK|MB_ICONERROR);
					}else{
						// done!
						MessageBox(hWndDlg, "The license request has been sent.", "Information", MB_OK|MB_ICONEXCLAMATION);
					}
					bReqeustDone	= TRUE;
					goto PROCESS_DONE;
ERROR_OCCURED:
					MessageBox(hWndDlg, "All fields must be filled!", "Warning", MB_OK|MB_ICONQUESTION);
PROCESS_DONE:
					curl_easy_cleanup(pCtx);
				}
				curl_global_cleanup();
			}
			break;
		}
	}
	if(bReqeustDone) MessageBox(hWndDlg, "License request is done.\nWe'll send a reply, check your E-mail soon.", "License Request", MB_OK);
}

void LicenseManager::ActivateLicense(HWND hWndDlg, BOOL bShowMessage){
	char name[1024];
	GetRequsterFullName(hWndDlg, name);
	UpdateLicenseFile(hWndDlg);
	{
		int iActivateCount	= CheckLicense();
		char sMsg[128];
		if(iActivateCount)
			sprintf(sMsg, "Activated %d", iActivateCount);
		else
			strcpy(sMsg, "Not Activated");

		SendDlgItemMessage(hWndDlg, IDC_LICENSE_STATUS, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(sMsg));

		MessageBox(hWndDlg,
			iActivateCount ?
			"Activate license successfully" :
			"Invalid license activation code!",
			"Activate License", MB_OK);
	}
}

void LicenseManager::CheckTrialLoop(void){
	if(IsLicensePresent()) return;
	DWORD dwCurTime = timeGetTime();

	if((dwCurTime - m_dwPrevTime) > 1000*30){
		RunModal();
	}
}

INT_PTR WINAPI LicenseManager::DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam){
	switch(Msg)
	{
	case WM_INITDIALOG:
		g_LicenseManager.Initialize(hWndDlg);
		return TRUE;
	case WM_COMMAND:
		switch(HIWORD(wParam)){
		case EN_CHANGE:
			{
				int CtrlID	= (int)LOWORD(wParam);
				if(	(CtrlID == IDC_EDIT_FIRST_NAME) ||
					(CtrlID == IDC_EDIT_LAST_NAME)){
					g_LicenseManager.UpdateDlg(hWndDlg);
					return TRUE;
				}
			}
			break;
		case CBN_SELCHANGE:
			{
				int CtrlID	= (int)LOWORD(wParam);
				if(CtrlID == IDC_COMBO_ADAPTER_LIST){
					g_LicenseManager.UpdateDlg(hWndDlg);
					return TRUE;
				}
			}
			break;
		default:
			break;
		}
		switch(wParam){
		case IDC_REQUEST_LICENSE:
			g_LicenseManager.RequestLicense(hWndDlg);
			return TRUE;
		case IDC_BTN_ACTIVE:
			g_LicenseManager.ActivateLicense(hWndDlg, TRUE);
			return TRUE;
		case IDCANCEL:
		case IDOK:
			g_LicenseManager.UpdateLicenseFile(hWndDlg);
			EndDialog(hWndDlg, 0);
			return TRUE;
		}
		break;
	}

	return FALSE;
}


HWND GetConsoleHwnd(void)
{
#define MY_BUFSIZE 1024 // Buffer size for console window titles.
	HWND hwndFound;         // This is what is returned to the caller.
	char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
	// WindowTitle.
	char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
	// WindowTitle.

	// Fetch current window title.

	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

	// Format a "unique" NewWindowTitle.

	wsprintf(pszNewWindowTitle,"%d/%d",
		GetTickCount(),
		GetCurrentProcessId());

	// Change current window title.

	SetConsoleTitle(pszNewWindowTitle);

	// Ensure window title has been updated.

	Sleep(40);

	// Look for NewWindowTitle.

	hwndFound=FindWindow(NULL, pszNewWindowTitle);

	// Restore original window title.

	SetConsoleTitle(pszOldWindowTitle);

	return(hwndFound);
}

void LicenseManager::RunModal(void){
	DialogBox(m_hModule, MAKEINTRESOURCE(IDD_LICENSING_DIALOG), NULL, DlgProc);
	if(!CheckLicense()){
		MessageBox(NULL, "This SDK will be run as a trial version.\nEvery 30 seconds, you will see this license check window.\nAnd you can utilize only one RayCore Card.\nTry on it. And register!", "Information", MB_OK|MB_ICONEXCLAMATION);
		CheckLicense();
	}
}
