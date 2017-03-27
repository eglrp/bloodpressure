// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "libbpdevinterface.h"
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{

	char *pSerialDevContent = "{	\
							   \"TAG\":\"HingMed_ABP\", \
							  \"Cmd\":\"Serial\",	\
							  \"Param\":{	\
							  \"Com\":\"com9\",		\
							  \"Baud\":\"19200\",	\
							  \"Parity\":\"0\",	\
							  \"DataBits\":\"8\",		\
							  \"StopBits\":\"0\"		\
							  }	\
							  }";

	char *pUsbDevContent = "{	\
						    \"TAG\":\"ACF\", \
							  \"Cmd\":\"Usb\",	\
							  \"Param\":{	\
							  \"wVID\":\"1155\",		\
							  \"wPID\":\"22352\"	\
							  }	\
							  }";
	//0����ʼ���豸
	int nRet = BpInitDevice(pSerialDevContent);

	//1�����豸TODO:
	nRet = BpOpenDevice();

	//2��д�豸TODO:/*HingMed_ABP��ʾ�����豸*/
	char *pWriteData = "{\
					   \"TAG\":\"HingMed_ABP\", \
					   \"Cmd\":\"SET\", \
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

	char *pRrcd = "{\"TAG\":\"HingMed_ABP\",\
					\"Cmd\":\"RRCD\"\}";	//��ȡ�ܼ�¼��


	//��Ѷ�豸
	//1=�Զ������ݸ�ʽ
	char *pJson = "{ \
				  \"TAG\":\"ACF\", \
				  \"Cmd\":\"SET\", \
				  \"Param\": { \
				  \"ID\": \"16441\", \
				  \"Mode\": \"1\", \
				  \"TimeList\": [ \
				  { \
				  \"Hour\": \"8\", \
				  \"Min\":\"10\", \
				  \"Interval\": \"15\" \
				  }, \
				  { \
				  \"Hour\": \"7\", \
				  \"Min\":\"16\", \
				  \"Interval\": \"30\" \
				  } \
				  , \
				  { \
				  \"Hour\": \"0\", \
				  \"Min\":\"0\", \
				  \"Interval\": \"0\" \
				  } \
				  , \
				  { \
				  \"Hour\": \"0\", \
				  \"Min\":\"0\", \
				  \"Interval\": \"0\" \
				  } \
				  , \
				  { \
				  \"Hour\": \"0\", \
				  \"Min\":\"0\", \
				  \"Interval\": \"0\" \
				  } \
				  , \
				  { \
				  \"Hour\": \"0\", \
				  \"Min\":\"0\", \
				  \"Interval\": \"0\" \
				  } \
				  ] \
				  } \
				  }";

	//2=�Զ�������ݸ�ʽ
	char *pAutoJson = "{ \
					  \"TAG\":\"ACF\", \
					  \"Cmd\":\"SET\", \
					  \"Param\": { \
					  \"ID\": \"16441\", \
					  \"Mode\": \"2\" \
					  } \
					  }";

	//3=�ֶ�ʱ����
	char *pHandleJson = "{ \
						\"TAG\":\"ACF\", \
						\"Cmd\":\"SET\", \
						\"Param\": { \
						\"ID\": \"16441\", \
						\"Mode\": \"3\", \
						\"Day\": \"30\", \
						\"Night\": \"60\"  \
						} \
						}";

	char *pResultData = NULL;
	int nResultSize = 0;
	//���ò���ʱ��ģʽ
	nRet = BpWriteDevice(pWriteData, strlen(pWriteData), &pResultData, nResultSize);
	printf("pWriteData size = %d, pWriteData Data= %s", strlen(pWriteData),pWriteData);
	//������
	//nRet = BpWriteDevice(pRrcd, strlen(pRrcd), &pResultData, nResultSize);
	//printf("nResultSize = %d, Record Data= %s", nResultSize,pResultData);

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

