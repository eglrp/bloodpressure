#include "StdAfx.h"
#include "jsonformat.h"
#include "datatype.h"

#include "cJSON.h"



__BEGIN_NAMESPACE(Format)





int CJsonFormat::ParseFormat(IN const char *pData, OUT int &nType, OUT char *&pFormatData)
{
	E_Status_t nRet = SUCCESS;
	
	CheckNullPtr(pData);
	const char *pBuf = pData;

	cJSON *pRoot;
	if (pRoot = cJSON_Parse(pBuf), NULL == pRoot) return JSON_FORMAT_ERROR;
	//1��device type
	cJSON *pTagType = cJSON_GetObjectItem(pRoot, TAG);
	CheckNullPtr(pTagType);

	char szDeviceName[MIN_LEN] = "\0";
	sprintf_s(szDeviceName, sizeof(szDeviceName), pTagType->valuestring);

	if (0 == strcmp(szDeviceName, DEVICE_COM))	//parse serial device type
	{
		nType = SERIAL_DEV_TYPE;
	}
	else if (0 == strcmp(szDeviceName, DEVICE_USB))	//parse usb device type
	{ 		
		nType = USB_DEV_TYPE;	
		//TODO:δʵ��
	}
	else if (0 == strcmp(szDeviceName, DEVICE_HINGMED_NAME))	//�����豸
	{
		nType = SERIAL_MESSAGE_PROTOCAL;	
		//m_pSerialParser->Parse(pData, nType, pFormatData);
	}
	else if (0 == strcmp(szDeviceName, DEVICE_ACF_NAME))		//��Ѷ�豸
	{
	}
	else if (0 == strcmp(szDeviceName, READ_RECORD_SUM))	//��ȡ��¼����
	{
		nType = SERIAL_RECORD_SUM;
	}
	else if (0 == strcmp(szDeviceName, READ_RECORD_DATA))	//��ȡ��¼����
	{
		nType = SERIAL_RECORD_DATA;
	}
	else
	{
		nType = UNKNOW_TYPE;
	}	

	cJSON_Delete(pRoot);
	pRoot = NULL;

	return nRet;
}


int CJsonFormat::CreateFormat(IN const char *pData, IN int nLen, \
	IN int nType, OUT char **pFormatData,\
	OUT int &FormatLen)
{
	E_Status_t nRet = UNKNOW_STATUS;

	switch(nType)
	{
	case Format::SERIAL_DEV_TYPE:
		nRet = SUCCESS;
		break;
	case Format::USB_DEV_TYPE:
		nRet = SUCCESS;
		break;
	case Format::SERIAL_MESSAGE_PROTOCAL:
		nRet = SUCCESS;
		break;
	case Format::SERIAL_RECORD_SUM:
		nRet = SUCCESS;
		break;
	case Format::SERIAL_RECORD_DATA:
		nRet = (E_Status_t)CreateFromatRecordData(pData, nLen, pFormatData, FormatLen);
		break;
	case Format::USB_RRCD_PROTOCAL:
		nRet = SUCCESS;
		break;
	default:
		break;
	}

	return nRet;
}


int CJsonFormat::CreateFromatRecordData(IN const char *pData, IN int nLen,\
	OUT char **pFormatData,	OUT int &FormatLen)
{
	E_Status_t eRet = SUCCESS;
	
	CheckNullPtr(pData);
	const char *pBuf = pData;

	//1��ȡ�û����ƺ�ID
	int nUserDataLen = sizeof(Format::UserIdAndName_t);
	char *pUserIdAndNameData = new char[nUserDataLen + 1];
	memset(pUserIdAndNameData, 0, nUserDataLen + 1);
	memcpy(pUserIdAndNameData, pBuf, nUserDataLen);
	pBuf += nUserDataLen;

	Format::UserIdAndName_t *pUserIdAndName = (Format::UserIdAndName_t*)pUserIdAndNameData;
	
	cJSON *pRoot = cJSON_CreateObject();
	if (NULL == pRoot) return OUT_OF_MEMORY;
	cJSON_AddStringToObject(pRoot,"ID",pUserIdAndName->szUserId);	//�û�ID

	cJSON *pMeasureDataList = cJSON_CreateArray();
	if (NULL == pMeasureDataList) return OUT_OF_MEMORY;
	cJSON_AddItemToObject(pRoot, "MeasureData", pMeasureDataList);

	
	//2��ȡ����record��¼����
	int nRecordDataLen =  sizeof(Format::RecordData_t);
	int nCount = (nLen - nUserDataLen) / nRecordDataLen;
	Format::RecordData_t *pRecordData = (Format::RecordData_t*)pBuf;
	
	for (int i = 0; i < nCount; i++)
	{
		cJSON *pMeasureData = cJSON_CreateObject();
		if(NULL == pMeasureData) return OUT_OF_MEMORY;	
		char szBuf[128] = "\0";
		char szYear[16] = "\0";
		int year = pRecordData->nYear + 2000;
		sprintf_s(szYear, sizeof(szYear), "%04d", year);
		char szMon[16] = "\0";
		sprintf_s(szMon, sizeof(szMon), "%02d", pRecordData->nMon);
		char szDay[16] = "\0";
		sprintf_s(szDay, sizeof(szDay), "%02d", pRecordData->nDay);
		char szHour[16] = "\0";
		sprintf_s(szHour, sizeof(szHour), "%02d", pRecordData->nHour);
		char szMin[16] = "\0";
		sprintf_s(szMin, sizeof(szMin), "%02d", pRecordData->nMin);

		sprintf_s(szBuf, sizeof(szBuf),"%s%s%s%s%s",szYear,szMon,szDay,szHour,szMin);

		cJSON_AddStringToObject(pMeasureData,"Time", szBuf);

		memset(szBuf, 0 ,sizeof(szBuf));
		sprintf_s(szBuf, sizeof(szBuf), "%d", pRecordData->nSys);
		cJSON_AddStringToObject(pMeasureData,"SP", szBuf);

		memset(szBuf, 0 ,sizeof(szBuf));
		sprintf_s(szBuf, sizeof(szBuf), "%d", pRecordData->nDia);
		cJSON_AddStringToObject(pMeasureData,"DP",szBuf);

		memset(szBuf, 0 ,sizeof(szBuf));
		sprintf_s(szBuf, sizeof(szBuf), "%d", pRecordData->nRate);
		cJSON_AddStringToObject(pMeasureData,"HR",szBuf);
		cJSON_AddStringToObject(pMeasureData,"AverHR","");
		cJSON_AddStringToObject(pMeasureData,"Status","");
		cJSON_AddStringToObject(pMeasureData,"IsValid","");

		
		char nAuto = pRecordData->nAuto;
		//������ʽ
		if ((nAuto & 0x03) == 0x0)	//�Զ�����
		{
		}
		else if ((nAuto & 0x03) == 0x1)	//�ֶ�����
		{
		}
		else if ((nAuto & 0x03) == 0x2)	//�Զ��ز�
		{
		}

		//λλ��Ϣposition
		if ((nAuto & 0x30) == 0x0)	//ƽ��
		{
			cJSON_AddStringToObject(pMeasureData,"Position","ƽ��");
		}
		else if ((nAuto & 0x30) == 0x10)	//վ��
		{
			cJSON_AddStringToObject(pMeasureData,"Position","վ��");
		}
		else if ((nAuto & 0x30) == 0x20)	//��΢�˶�
		{
			cJSON_AddStringToObject(pMeasureData,"Position","��΢�˶�");
		}
		else if ((nAuto & 0x30) == 0x30)	//�����˶�
		{
			cJSON_AddStringToObject(pMeasureData,"Position","�����˶�");
		}
		else
		{
			cJSON_AddStringToObject(pMeasureData,"Position","");
		}
		cJSON_AddItemToArray(pMeasureDataList,pMeasureData);
		pRecordData += 1;
	}

	char *pJsonData = cJSON_Print(pRoot);
	FormatLen = strlen(pJsonData);
	*pFormatData = new char[FormatLen + 1];
	*(*pFormatData + FormatLen) = '\0';
	memcpy(*pFormatData, pJsonData, FormatLen);

	return eRet;
}


__END_NAMESPACE(Format)

