#ifndef __LICENSE_MANAGER_H__
#define __LICENSE_MANAGER_H__
#include <winsock2.h>
#include <windows.h>

class LicenseManager{
public:
	LicenseManager(void);
	~LicenseManager(void);

	void RunModal(void);
	int CheckLicense(void);
	BOOL GenLicense(const char* request_string, BYTE* activate_code, int card_count);
	inline BOOL IsLicensePresent(void)	{return m_bLicensePresent;}
	void CheckTrialLoop(void);

protected:
	void Initialize(HWND hWndDlg);
	void UpdateLicenseFile(HWND hWndDlg);
	void UpdateDlg(HWND hWndDlg);
	void RequestLicense(HWND hWndDlg);
	void ActivateLicense(HWND hWndDlg, BOOL bShowMessage = FALSE);
	BOOL UpdateRequestCode(HWND hWndDlg);
	static INT_PTR WINAPI DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam);

private:
	char			m_sLicensePath[2048];
	BOOL			m_bLicensePresent;
	DWORD			m_dwPrevTime;
	HMODULE			m_hModule;
	int GetLicenseInt(LPCTSTR sKeyName, int iDefault = 0);
	void SetLicenseInt(LPCTSTR sKeyName, int iData);
	void GetLicenseString(LPCTSTR sKeyName, LPTSTR sGet, DWORD dwSize);
	void SetLicenseString(LPCTSTR sKeyName, LPTSTR sSet);
	void GetRequsterFullName(HWND hWndDlg, LPTSTR sName);
};

extern LicenseManager	g_LicenseManager;

#endif//__LICENSE_MANAGER_H__
