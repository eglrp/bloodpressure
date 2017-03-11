#include "stdafx.h"
#include "devfactory.h"
#include "idevinterfacetype.h"




int _tmain(int argc, _TCHAR* argv[])
{
	Dev::CDevFactory devfactory;
	Dev::E_DevInterfaceType_t eDevType = Dev::DEV_SERIAL_TYPE;


	Dev::IDevInterfaceType *pSerialDev = devfactory.CreateDevInterface(eDevType);

	char *pSerialDevContent = "{	\
			\"DeviceType\":\"Serial\",	\
			\"Param\":{	\
			\"Com\":\"com1\",		\
			\"Baud\":\"115200\",	\
			\"Parity\":\"0\",	\
			\"DataBits\":\"8\",		\
			\"StopBits\":\"0\"		\
			}	\
}";
	int nSerialDevType = 0;

	char *pUsbDevContent = "{\"wVID\" : \"1155\", \"wPID\" : \"22352\"}";
	int nUsbDevType = 1;

	//0����ʼ���豸
	int nRet = pSerialDev->InitDevice(pSerialDevContent);
	//pSerialDev->InitDevice(pUsbDevContent, nUsbDevType);
	//1�����豸TODO:
//	nRet = pSerialDev->OpenDevice();
	



	//2��д�豸TODO:
	char *pWriteData = "xxx";
	nRet = pSerialDev->WriteDevice(pWriteData, strlen(pWriteData));

	//3�����豸TODO:
	char *pReadData = NULL;
	int nSize;
	nRet = pSerialDev->ReadDevice(&pReadData, nSize);

	//4���ͷ��ڴ�TODO:
	//nRet = pSerialDev->FreeMemory(pReadData);

	//5���ر��豸TODO:
	nRet = pSerialDev->CloseDevice();



	getchar();
	return 0;
}

