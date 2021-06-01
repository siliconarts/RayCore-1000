#pragma once
#include "SMTPConnection2.h"

class CMail
{
	CMimeMessage	m_Message;
	CString			m_sDomainName;
public:
	CMail(void);
	virtual ~CMail(void);

	// sender's mail address and name
	void SetSender(LPCTSTR lpszAddress, LPCTSTR lpszName = NULL);
	// recipient's mail address and name
	void AddRecipient(LPCTSTR lpszAddress, LPCTSTR lpszName = NULL);
	// mail title
	void SetSubject(LPCTSTR lpszSubject);
	// add text of mail body
	void AddText(LPCTSTR lpszText);
	// add file
	void AddFile(LPCTSTR lpszFileName);
	// send email operation
	BOOL Send(void);
	// release this object : make sure to call this in the end of process
	void Release(void);
};
