// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "libbpdevinterface.h"
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{

	char *pSerialDevContent = "{	\
							  \"TAG\":\"Serial\",	\
							  \"Param\":{	\
							  \"Com\":\"com9\",		\
							  \"Baud\":\"19200\",	\
							  \"Parity\":\"0\",	\
							  \"DataBits\":\"8\",		\
							  \"StopBits\":\"0\"		\
							  }	\
							  }";
	//0����ʼ���豸
	int nRet = BpInitDevice(pSerialDevContent);

	//1�����豸TODO:
	nRet = BpOpenDevice();

	//2��д�豸TODO:/*HingMed_ABP��ʾ�����豸*/
	char *pWriteData = "{\
					   \"TAG\":\"HingMed_ABP\", \
					   \"Cmd\":\"MessureTime\", \
					   \"Param\":\{\
					   \"UserID\": \"112566\", \
					   \"UserName\": \"WSD\", \
					   \"Mode\": \"1\", \
					   \"DayStartHourTime\":\"8\",	\
					   \"DayStartMinTime\":\"30\",	\
					   \"DayInternal\":\"30\",	\
					   \"NightStartHourTime\":\"21\",	\
					   \"NightStartMinTime\":\"0\",	\
					   \"NightInternal\":\"30\"	\
					   }		\
					   }";

	char *pRrcd = "{\"TAG\":\"RRCD\"\}";	//��ȡ�ܼ�¼��

	char *pResultData = NULL;
	int nResultSize = 0;
	//���ò���ʱ��ģʽ
	nRet = BpWriteDevice(pWriteData, strlen(pWriteData), &pResultData, nResultSize);

	//������
	nRet = BpWriteDevice(pRrcd, strlen(pRrcd), &pResultData, nResultSize);

	//3�����豸TODO:
	char **pReadData = NULL;
	int nSize;
	nRet = BpReadDevice(pReadData, nSize);

	//4���ͷ��ڴ�TODO:
	nRet = BpFreeMemory(&pResultData);

	//5���ر��豸TODO:
	nRet = BpCloseDevice();


	getchar();
	return 0;
}

