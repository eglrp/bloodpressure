#include "StdAfx.h"
#include "jsonformat.h"
#include "datatype.h"

#include "cJSON.h"

#include <iostream>



__BEGIN_NAMESPACE(Format)




int CJsonFormat::ParseFormat(IN const char *pData, OUT int &nType, OUT char *&pFormatData)
{
	E_Status_t eRet = SUCCESS;
	
	CheckNullPtr(pData);
	const char *pBuf = pData;

	cJSON *pRoot;
	if (pRoot = cJSON_Parse(pBuf), NULL == pRoot) return JSON_FORMAT_ERROR;
	//1��Tag type
	cJSON *pTagType = cJSON_GetObjectItem(pRoot, TAG);
	CheckNullPtr(pTagType);

	Format::HeadParam_t headparam = { 0 };
	sprintf_s(headparam.szTag, sizeof(headparam.szTag), pTagType->valuestring);

	//cmd
	cJSON *pCmd = cJSON_GetObjectItem(pRoot, CMD);
	sprintf_s(headparam.szCmd, sizeof(headparam.szCmd), pCmd->valuestring);

	cJSON_Delete(pRoot);
	pRoot = NULL;
	
	if (0 == strncasecmp(headparam.szTag, DEVICE_HINGMED_NAME, strlen(headparam.szTag)))//�����豸
	{
		//3������
		Parser::CParserFactory parserfactory;
		Parser::IParserInterface *pSerialParser = parserfactory.CreateParser(Parser::SERIAL_DATA_TYPE);
		CheckNullPtr(pSerialParser);		
		eRet = (E_Status_t)pSerialParser->Parse(pData, nType, pFormatData);
		DELPTR(pSerialParser);
	}
	else if (0 == strncasecmp(headparam.szTag, DEVICE_ACF_NAME, strlen(headparam.szTag)))	//��Ѷ�豸
	{ 		
		//3������
		Parser::CParserFactory parserfactory;
		Parser::IParserInterface *pUsbParser = parserfactory.CreateParser(Parser::USB_DATA_TYPE);
		CheckNullPtr(pUsbParser);
		eRet = (E_Status_t)pUsbParser->Parse(pData, nType, pFormatData);
		DELPTR(pUsbParser);
	}
	else
	{
		nType = UNKNOW_TYPE;
	}
	return eRet;
}


int CJsonFormat::CreateFormat(IN const char *pData, IN int nLen, \
	IN int nType, OUT char **pFormatData,\
	OUT int &FormatLen)
{
	E_Status_t nRet = UNKNOW_STATUS;

	switch(nType)
	{
	case Format::HingMed_ABP_DEV_INIT:
		nRet = SUCCESS;
		break;
	case Format::ACF_DEV_INIT:
		nRet = SUCCESS;
		break;
	case Format::HingMed_ABP_MEASURE_MODE:
		nRet = SUCCESS;
		break;
	case Format::HingMed_ABP_GET_RECORD_DATA:
		nRet = (E_Status_t)CreateFromatRecordData(pData, nLen, pFormatData, FormatLen);
		break;
	case Format::ACF_GET_RECORD_DATA:
		nRet = (E_Status_t)CreateACFRecordData(pData, nLen, pFormatData, FormatLen);
		break;
	default:
		break;
	}

	return nRet;
}

int CJsonFormat::CreateACFRecordData(IN const char *pData, IN int nLen,\
	OUT char **pFormatData,	OUT int &FormatLen)
{
	E_Status_t eRet = SUCCESS;
	CheckNullPtr(pData);
	const char *pBuf = pData;

	//2��ȡ����record��¼����
	int nRecordDataLen =  sizeof(Format::acf_record_data_t);
	int nCount = nLen/ nRecordDataLen;
	Format::acf_record_data_t *pRecordData = (Format::acf_record_data_t *)pBuf;

	cJSON *pRoot = cJSON_CreateObject();
	if (NULL == pRoot) return OUT_OF_MEMORY;

	char szId[128] = { 0 };
	sprintf_s(szId, sizeof(szId), "%d", pRecordData->nId);
	cJSON_AddStringToObject(pRoot,"ID",szId);	//�û�ID

	cJSON *pMeasureDataList = cJSON_CreateArray();
	if (NULL == pMeasureDataList) return OUT_OF_MEMORY;
	cJSON_AddItemToObject(pRoot, "MeasureData", pMeasureDataList);

	for (int i = 0; i < nCount; i++)
	{
		cJSON *pMeasureData = cJSON_CreateObject();
		if(NULL == pMeasureData) return OUT_OF_MEMORY;	

		char szBuf[128] = { 0 };
		int year,mon,day,hour,min,sec;
		year = pRecordData->nYear;
		mon = pRecordData->nMon;
		day = pRecordData->nDay;
		hour = pRecordData->nHour;
		min = pRecordData->nMin;
		sec = pRecordData->nSec;
		sprintf_s(szBuf, sizeof(szBuf),"%4d%2d%2d%2d%2d%2d",year,mon,day,hour,min,sec);

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
		cJSON_AddStringToObject(pMeasureData,"Position","");

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

