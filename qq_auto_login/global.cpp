#include "stdafx.h"
#include "global.h"
#include <winable.h>
#include <fstream>
//#include <Psapi.h>
//#pragma comment(lib, "Psapi.lib")

#include <TLHELP32.H>
void SendAscii(wchar_t data, BOOL shift)
{
	INPUT input[2];
	memset(input, 0, 2 * sizeof(INPUT));

	if (shift)
	{
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = VK_SHIFT;
		SendInput(1, input, sizeof(INPUT));
	}

	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = data;

	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = data;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(2, input, sizeof(INPUT));

	if (shift)
	{
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = VK_SHIFT;
		input[0].ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, input, sizeof(INPUT));  
	}
}


void SendUnicode(wchar_t data)
{
	INPUT input[2];
	memset(input, 0, 2 * sizeof(INPUT));

	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0;
	input[0].ki.wScan = data;
	input[0].ki.dwFlags = 0x4;//KEYEVENTF_UNICODE;

	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0;
	input[1].ki.wScan = data;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP | 0x4;//KEYEVENTF_UNICODE;

	SendInput(2, input, sizeof(INPUT));
}

//Ϊ����ʹ�ã��������������װ��ǰ����������
void SendKeys(CString msg)
{
	short vk;
	BOOL shift;

	USES_CONVERSION;
	wchar_t* data = T2W(msg.GetBuffer(0));
	int len = wcslen(data);

	for(int i=0;i<len;i++)
	{
		if (data[i]>=0 && data[i]<256) //ascii�ַ�
		{
			vk = VkKeyScanW(data[i]);

			if (vk == -1)
			{
				SendUnicode(data[i]);
			}
			else
			{
				if (vk < 0)
				{
					vk = ~vk + 0x1;
				}

				shift = vk >> 8 & 0x1;

				if (GetKeyState(VK_CAPITAL) & 0x1)
				{
					if (data[i]>='a' && data[i]<='z' || data[i]>='A' && data[i]<='Z')
					{
						shift = !shift;
					}
				}

				SendAscii(vk & 0xFF, shift);
			}
		}
		else //unicode�ַ�
		{
			SendUnicode(data[i]);
		}
	}
}

void PressShiftAndTab()
{
	::keybd_event(VK_SHIFT, 0, 0, 0);
	::keybd_event(VK_TAB, 0, 0, 0);
	::keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
}


void PressKey(char key)
{
	::keybd_event(key, 0, 0, 0);
	::keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
}

void ClearPwd()
{
	::keybd_event(VK_HOME, 0, 0, 0);
	for (int i=0; i<16; i++)
	{
		::keybd_event(VK_DELETE, 0, 0, 0);
		::keybd_event(VK_DELETE, 0, KEYEVENTF_KEYUP, 0);
	}
}


void InputCode(CString strCode)
{
	for (int i=0; i<strCode.GetLength(); i++)
	{
		::keybd_event(strCode.GetAt(i), 0, 0, 0);
		::keybd_event(strCode.GetAt(i), 0, KEYEVENTF_KEYUP, 0);
	}	
}


void ReadQQAccount(CString strPath, vector_type &vQQ  )
{
	FILE *file = fopen(strPath, "r");
	if (NULL == file)
	{
		return ;
	}

	ifstream fin(strPath);  
	string   line;  
	while( getline(fin,line) )
	{    		
		int nIndex = line.find('|');
		if (-1 != nIndex)
		{
			string code = line.substr(0, nIndex);
			string pwd  = line.substr(nIndex+1);
			if (code.length() > 0 && pwd.length() > 0)
			{
				vQQ.push_back(spair__(code, pwd));
			}			
		}
	}
 
}

bool AotoLoginQQ(int nType, CString strPath, CString strCode, CString strPwd)
{
	if (1 == nType)
	{
		return	AutoLoginQQ2013(strPath, strCode, strPwd);
	}
	
	return	AutoLoginQQ2012(strPath, strCode, strPwd);
}

bool AutoLoginQQ2012(CString strPath, CString strCode, CString strPwd)
{
	if (strPath == "" )
	{
		return false;
	}

	ShellExecute(NULL, "open", strPath, NULL, NULL, true);
	Sleep(2000);

	HWND hWnd = FindWindow(NULL, "QQ2012");
	if (hWnd != NULL )
	{
		HWND hPassword = ::FindWindowEx(hWnd, 0, "Edit", NULL);
		if (NULL == hPassword)
		{
			return false;
		}

		::SetForegroundWindow(hPassword);	//����ǰ��
		PressShiftAndTab();					//��shift+tab��ʹ����������ص��ʺ������
		Sleep(50);
		SendKeys(strCode);					//�����ʺ�
		Sleep(50);
		PressKey(VK_TAB);					//��סTAB���ý��㶨λ�������
		::SetForegroundWindow(hPassword);	//����ǰ��
		Sleep(50);
		ClearPwd();							//������������
		SendKeys(strPwd);					//��������
		PressKey(VK_RETURN);				//�س�
		Sleep(3000);						//Ĭ�ϵ�¼һ����3����ٵ�¼����һ��
		return true;
	}

	AfxMessageBox("QQ2012δ����");
	return false;
}

bool AutoLoginQQ2013(CString strPath, CString strCode, CString strPwd)
{
	if (strPath == "" )
	{
		return false;
	}

	ShellExecute(NULL, "open", strPath, NULL, NULL, true);
	Sleep(3000);

	HWND hWnd = FindWindow("TXGFLayerMask", NULL);
	if (NULL != hWnd)
	{
		hWnd = FindWindowEx(hWnd, NULL, "TXGuiFoundation", NULL);
		if (NULL != hWnd)
		{
			hWnd = FindWindowEx(hWnd, NULL, "Edit", NULL);
			if (NULL == hWnd)
			{
				return false;
			}
		}
	}

	if (NULL == hWnd)
	{
		AfxMessageBox("QQ2013δ����");
		return false;
	}

	::SetForegroundWindow(hWnd);	//����ǰ��
	for (int i=0; i<12; i++)
	{
		PressKey(VK_TAB);			//qq2013���ε���shift+tab�л�����������򣬹�ʹ������12��TAB�������л����ʺ������
		Sleep(100);
	}
	
	Sleep(50);
	SendKeys(strCode);					//�����ʺ�
	Sleep(50);
	PressKey(VK_TAB);					//��סTAB���ý��㶨λ�������
	Sleep(50);
	ClearPwd();							//������������
	SendKeys(strPwd);					//��������
	Sleep(100);
	PressKey(VK_RETURN);				//�س�
	return true;
}


void GetCurrDir(int nLength, char *lpBuffer)
{
	::GetModuleFileName(NULL, lpBuffer, nLength);
	CString strPath(lpBuffer);
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	memset(lpBuffer, 0, nLength);
	strcpy(lpBuffer, strPath);
}


void Fun_TerminateProcess(CString strProcessName)
{
	try
	{         
		HANDLE hSnapshot = NULL;

		//���ĳһʱ��ϵͳ�Ľ��̡��ѣ�heap����ģ�飨module�����̵߳Ŀ�����Ϣ
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot != NULL)
		{
			PROCESSENTRY32 processListStr;

			processListStr.dwSize = sizeof(PROCESSENTRY32);

			BOOL bReturnValue;

			bReturnValue = Process32First(hSnapshot, &processListStr);

			CString strExeFile = processListStr.szExeFile;

			DWORD dwProcessID = processListStr.th32ProcessID;

			int iThreadCnt = processListStr.cntThreads;

			while (bReturnValue)
			{    
				if (strExeFile.CompareNoCase(strProcessName) == 0)
				{                    
					//�Ȼ�øý��̾����ͨ�����̱�ʶ
					HANDLE hProcessHandle;                    
					hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessID);

					//��������IDָ���Ļ�ô��ڵ�һ�����̵ľ��
					if (hProcessHandle)
					{
						TerminateProcess(hProcessHandle, 0);
					}

					CloseHandle (hProcessHandle);

					//break;//�Ƿ�ȫ����ɱ����, ֻɱ��һ��
				} 

				//���ϵͳ������������һ�����̵���Ϣ
				bReturnValue = Process32Next(hSnapshot,&processListStr);

				iThreadCnt   = processListStr.cntThreads;            
				dwProcessID  = processListStr.th32ProcessID;            
				strExeFile   = processListStr.szExeFile;        
			}        
			CloseHandle(hSnapshot);
		}
	}
	catch (...)
	{
	}
}