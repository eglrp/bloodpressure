#include "StdAfx.h"
#include "SerialCom.h"



__BEGIN_NAMESPACE(Dev)



CSerialCom::CSerialCom(void) :
	m_hcom(INVALID_HANDLE_VALUE)
{
	m_hCommFree = CreateEvent(NULL,FALSE,TRUE,NULL);
	m_hCommRecv = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hCommSend = CreateEvent(NULL,FALSE,FALSE,NULL);
}


CSerialCom::~CSerialCom(void)
{
	ClosePort();
}

bool CSerialCom::InitPort(const char *szCom, \
					int nBaud, \
					const char nParity, \
					int nDataBits, \
					int nStopBits)
{
	/*if (!OpenPort(szCom))
	{
		printf("open serial failed!/n");
		return false;
	}*/

	if (m_hcom != INVALID_HANDLE_VALUE)
	{
		/*CloseHandle(m_hcom);
		m_hcom = INVALID_HANDLE_VALUE;*/
		return true;
	}

	//�첽���봮�� 
	m_hcom = CreateFile(szCom, // �������ƻ��豸·�� 
		GENERIC_READ | GENERIC_WRITE, // ��д��ʽ 
		0,    // ����ʽ����ռ 
		0,    // Ĭ�ϵİ�ȫ������ 
		OPEN_EXISTING, // ��һ�����ڵĴ��� 
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,    //�첽��ʽ��
		0);    // �������ģ���ļ� 

	//��մ��ڴ����־����¼��ǰͨ��״̬
	DWORD res;
	COMSTAT rst;
	ClearCommError(m_hcom,&res,&rst);

	// set serial port receive buffer andsend buffer size
	if (!SetupComm(m_hcom, 1024,1024)) 
	{
		printf("Set serialreceive buffer and send buffer failed!, error %d/n", GetLastError());
		return 0;
	}

	//���ڲ������� 
	DCB dcb;
	memset(&dcb, 0,sizeof(dcb));
	bool bRet = GetCommState(m_hcom, &dcb);
	dcb.BaudRate = nBaud;
	dcb.ByteSize = nDataBits;
	if (0 == nStopBits)
	{
		dcb.StopBits = ONESTOPBIT;	//1λֹͣλ
	}
	else if(1 == nStopBits)
	{
		dcb.StopBits = ONE5STOPBITS; //1.5λֹͣλ
	}
	else if(2 == nStopBits)
	{
		dcb.StopBits = TWOSTOPBITS; //2λֹͣλ
	}
	else
	{
		dcb.StopBits = ONESTOPBIT;	//1λֹͣλ
	}
		
	if (0 == nParity)
	{
		dcb.fParity = FALSE;	// ָ����żУ��ʹ�ܡ����˳�ԱΪ1����ֹ��żУ���� 
		dcb.fNull = FALSE;
		dcb.Parity = NOPARITY;	//NOPARITY ��У��
	}
	else
	{
		dcb.fParity = TRUE;	// ָ����żУ��ʹ�ܡ����˳�ԱΪ1��������żУ���� 
		dcb.fNull = FALSE;
		dcb.Parity = ODDPARITY;	//ODDPARITY ��У��
	}

	// set serial port device parameter
	if (!SetCommState(m_hcom, &dcb))
	{
		ClosePort();
		printf("Set serialport error:%d!!/n", GetLastError());
		return false;
	}

	if( !SetCommMask(m_hcom, EV_RXCHAR | EV_TXEMPTY))
	{// �����¼�: ���յ�һ���ַ�����������Ϊ��
		ClosePort();
		return false; 
	}

	 //��ʱ���� 
	COMMTIMEOUTS timeOuts;
	GetCommTimeouts(m_hcom, &timeOuts); 
	
	//�趨д��ʱ
	timeOuts.WriteTotalTimeoutMultiplier = 1;// д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms) 
	timeOuts.WriteTotalTimeoutConstant = 1000; // ������(�����)д��ʱʱ��: 100 ms 

	//�趨����ʱ
	timeOuts.ReadIntervalTimeout = 100; // �����ַ������ʱ���� 
	timeOuts.ReadTotalTimeoutMultiplier = 1;// ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms) 
	timeOuts.ReadTotalTimeoutConstant = 5000; // �������ܳ�ʱ���� 

	if(!SetCommTimeouts(m_hcom, &timeOuts)) //���ó�ʱ
	{
		ClosePort();
		return false; 
	}

	ClearCommBuff();
	
	return true;
}

bool CSerialCom::OpenPort(const char *szCom)
{
	 // open serial port
	if (m_hcom != INVALID_HANDLE_VALUE)
	{
		return true;
	}

	//�첽���봮�� 
	m_hcom = CreateFile(szCom, // �������ƻ��豸·�� 
		GENERIC_READ | GENERIC_WRITE, // ��д��ʽ 
		0,    // ����ʽ����ռ 
		0,    // Ĭ�ϵİ�ȫ������ 
		OPEN_EXISTING, // ��һ�����ڵĴ��� 
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,    //�첽��ʽ��
		0);    // �������ģ���ļ� 

	//ͬ��
	//m_hcom = CreateFile(szCom,GENERIC_READ | GENERIC_WRITE, 0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	//if (m_hcom == INVALID_HANDLE_VALUE)
	//{
	//	return false;	// �򿪴���ʧ�� 
	//}

	//Sleep(500);
	return true;
}


int CSerialCom::ReadFromPort(char **pBuff, IN int nSize, unsigned int timeout)
{
	//�첽��
	char lpInBuffer[1024] = "\0";
	DWORD dwBytesRead = 1024;
	COMSTAT ComStat;
	DWORD dwErrorFlags;

	OVERLAPPED osRead = {0};
	osRead.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if (osRead.hEvent == NULL)
		// Error creating overlapped event handle.
		return FALSE;

	ClearCommError(m_hcom, &dwErrorFlags, &ComStat);
	//dwBytesRead = min(dwBytesRead, (DWORD)ComStat.cbInQue);

	//dwBytesRead = min(dwBytesRead, (DWORD)ComStat.cbInQue);
	//if(!dwBytesRead) return FALSE;
	DWORD dwEvtMask, dwResult;
	//dwResult = WaitForSingleObject(osRead.hEvent,2000);

	DWORD dwRead = min(dwBytesRead, nSize);

	BOOL fWaitingOnRead = FALSE;
	if (!fWaitingOnRead)
	{
		BOOL bReadStatus;
		bReadStatus = ReadFile(m_hcom,lpInBuffer,nSize,&dwRead,&osRead);
		if(!bReadStatus)	//���ReadFile��������FALSE
		{
			if(GetLastError() != ERROR_IO_PENDING)
			{
				// Error in communications; report it.
				return false;
			}
			else	//GetLastError()��������ERROR_IO_PENDING,�����������ڽ��ж�����
			{
				fWaitingOnRead = TRUE;
				//WaitForSingleObject(osRead.hEvent,2000);
				////ʹ��WaitForSingleObject�����ȴ���ֱ����������ɻ���ʱ�Ѵﵽ2����
				////�����ڶ�����������Ϻ�m_osRead��hEvent�¼����Ϊ���ź�
				//PurgeComm(m_hcom,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
			}
		}
		else
		{
			// read completed immediately	
			*pBuff = new char[dwRead + 1];
			*(*pBuff + dwRead) = '\0';
			memcpy(*pBuff, lpInBuffer, dwRead);
			return dwRead;
		}

	}

BOOL fRes;
	if (fWaitingOnRead)
	{
#define READ_TIMEOUT      2000      // milliseconds
		DWORD dwRes = WaitForSingleObject(osRead.hEvent, READ_TIMEOUT);
		//ResetEvent(osReader.hEvent);
		switch(dwRes)
		{
			// Read completed.
		case WAIT_OBJECT_0:
			//if (!GetOverlappedResult(m_hcom, &osRead, &dwRead, FALSE))
			if (!GetOverlappedResult(m_hcom, &osRead, &dwRead, TRUE))
			{
				// Error in communications; report it.
				return false;
			}
			else
			{
				// Read completed successfully.									
				*pBuff = new char[dwRead + 1];
				*(*pBuff + dwRead) = '\0';
				memcpy(*pBuff, lpInBuffer, dwRead);
				return dwRead;
			}
			//  Reset flag so that another opertion can be issued.
			fWaitingOnRead = FALSE;
			break;
		case WAIT_TIMEOUT:
			// Operation isn't complete yet. fWaitingOnRead flag isn't
			// changed since I'll loop back around, and I don't want
			// to issue another read until the first one finishes.
			//
			// This is a good time to do some background work.
			fRes = CancelIo(m_hcom);
			break;  
		case WAIT_FAILED:
			fRes = FALSE;
			break;
		default:
			// Error in the WaitForSingleObject; abort.
			// This indicates a problem with the OVERLAPPED structure's
			// event handle.
			break;
		}
	}
	CloseHandle(osRead.hEvent);
	PurgeComm(m_hcom,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	return dwRead;
}

bool CSerialCom::WriteToPort(const void *pBuff, DWORD size, unsigned int timeout)
{
	//д�첽
	//char buffer[1024] = "\0";
	DWORD dwBytesWritten = 1024;
	DWORD dwErrorFlags;
	COMSTAT ComStat;

	OVERLAPPED osWrite = {0};
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osWrite.hEvent == NULL)
		// Error creating overlapped event handle.
		return FALSE;
	BOOL bWriteStat;


	dwBytesWritten = min(dwBytesWritten, size);

	bWriteStat = WriteFile(m_hcom,pBuff,dwBytesWritten, &dwBytesWritten,&osWrite);
	if(!bWriteStat)
	{
		if(GetLastError() != ERROR_IO_PENDING)
		{
			// WriteFile failed, but it isn't delayed. Report error and abort.
			bWriteStat = FALSE;
		}
		else
		{ 
			// Write is pending.
#define WRITE_TIMEOUT 1000 //write timeout
			DWORD dwRes = WaitForSingleObject(osWrite.hEvent, WRITE_TIMEOUT);
			//ResetEvent(osWrite.hEvent);
			switch(dwRes)
			{
				// Read completed.
			case WAIT_OBJECT_0:
				if (!GetOverlappedResult(m_hcom, &osWrite, &dwBytesWritten, TRUE))
					bWriteStat = FALSE;
				else
					// Write operation completed successfully.
					bWriteStat = TRUE;
				break;
			case WAIT_TIMEOUT:
				// This is a good time to do some background work.
				bWriteStat = CancelIo(m_hcom);
				break;  
			case WAIT_FAILED:
				bWriteStat = FALSE;
				break;
			default:
				// Error in the WaitForSingleObject; abort.
				// This indicates a problem with the OVERLAPPED structure's
				// event handle.
				break;
			}
		}
	}
	else
	{
		// WriteFile completed immediately.
		return bWriteStat;
	}
	return bWriteStat;

}

//UINT CommEventThread(LPVOID pParam)
//{
//	if( NULL == pParam)
//		return 0;
//	CSerialCom* pBC = (CSerialCom*)pParam;
//	// �����¼�
//	OVERLAPPED tOverLaped;
//	tOverLaped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
//	while(pBC->IsRunThread)
//	{
//		DWORD dwEvtMask, dwResult;
//		WaitCommEvent(pBC->hComm,&dwEvtMask, &tOverLaped);
//		dwResult = WaitForSingleObject(tOverLaped.hEvent,100);
//		if( WAIT_OBJECT_0 == dwResult)
//		{
//			if( dwEvtMask&EV_RXCHAR == EV_RXCHAR)
//			{
//				//printf("�յ����������ź�/n");
//				SetEvent(pBC->m_hCommRecv);
//			}
//			if( dwEvtMask&EV_TXEMPTY == EV_TXEMPTY)
//			{
//				//printf("��������ź�/n");
//				SetEvent(pBC->m_hCommSend);
//			}
//		}
//	}
//	return 0;
//}



// ������ 
// ����: pData - ���������ݻ�����ָ�� 
//      nLength - ������������ݳ��� 
// ����: ʵ�ʶ��������ݳ��� 
bool CSerialCom::ReadComm(void *pData, int nLength)
{
	DWORD res;
	res = WaitForSingleObject(m_hCommRecv,INFINITE);
	if( WAIT_OBJECT_0 != res)
		return false;
	res = WaitForSingleObject(m_hCommFree,INFINITE);
	if( WAIT_OBJECT_0 == res)
	{
		COMSTAT rst;
		do
		{
			ClearCommError(m_hcom,&res,&rst);
		}while( rst.cbInQue < nLength);

		DWORD factdata = 0;
		OVERLAPPED rOverLaped;
		rOverLaped.InternalHigh = 0;
		rOverLaped.Offset = 0;
		rOverLaped.OffsetHigh = 0;
		rOverLaped.hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
		if(ReadFile(m_hcom,pData,nLength,&factdata,&rOverLaped))
		{
			SetEvent(m_hCommFree);
			return true;
		}
		else
		{
			res = WaitForSingleObject(rOverLaped.hEvent,INFINITE);
			SetEvent(m_hCommFree);
			return true;
		}  
	}
	SetEvent(m_hCommFree);
	return false;
}

bool CSerialCom::WriteComm(void* pData, int nLength)
{
	Sleep(500);
	DWORD res;
	res = WaitForSingleObject(m_hCommFree,INFINITE);
	if( WAIT_OBJECT_0 == res)
	{
		//printf("д����!/n");
		COMSTAT rst;
		ClearCommError(m_hcom,&res,&rst);

		OVERLAPPED wOverLaped;
		wOverLaped.InternalHigh = 0;
		wOverLaped.Offset = 0;
		wOverLaped.OffsetHigh = 0;
		wOverLaped.hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
		DWORD factdata = 0;
		if(WriteFile(m_hcom,pData,nLength,&factdata,&wOverLaped))
		{
			SetEvent(m_hCommFree);
			return true;
		}
		else
		{
			res = WaitForSingleObject(wOverLaped.hEvent,INFINITE);
			if( WAIT_OBJECT_0 == res)
			{
				if(wOverLaped.InternalHigh + factdata >= nLength)
				{
					SetEvent(m_hCommFree);
					return true;
				}
			}
		}
	}
	SetEvent(m_hCommFree);
	return false;
}

void CSerialCom::ClearCommBuff()
{
	if( INVALID_HANDLE_VALUE != m_hcom) 
	{
		// clear serial port receive bufferand send buffer
		if(!PurgeComm(m_hcom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ))  // ����շ�����
		{
			printf("clear receivebuffer and send buffer failed!, error %d/n", GetLastError());
		}
	}
}

bool CSerialCom::ClosePort()
{
	if (INVALID_HANDLE_VALUE != m_hcom)
	{
		 SetCommMask(m_hcom, 0);  // �����¼� 
		 ClearCommBuff();
		 CloseHandle(m_hcom);
		 m_hcom = INVALID_HANDLE_VALUE;
	}
	
	return true;
}



__END_NAMESPACE(Dev)

