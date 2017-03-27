#include "StdAfx.h"
#include "devserver.h"
#include "datatype.h"
#include "devtemplete.h"
#include "typedef.h"




__BEGIN_NAMESPACE(DevServer)



IDevServer *CDevServer::m_pgInstance = NULL;


CDevServer::CDevServer(void)
{
	m_nDevType = Format::UNKNOW_TYPE;
	m_bDevInit = false;

	m_pSerialParser = NULL;
	m_pSerialProtocal = NULL;

	m_pUsbParser = NULL;
	m_pUsbProtocal = NULL;
}


CDevServer::~CDevServer(void)
{
	DELARR(m_pSerialParser);
	DELARR(m_pSerialProtocal);	

	DELARR(m_pUsbParser);
	DELARR(m_pUsbProtocal);
}

IDevServer *CDevServer::GetInstance()
{
	if (NULL == m_pgInstance)
	{
		m_pgInstance = new CDevServer();
	}
	return m_pgInstance;
}

int CDevServer::InitDevice(IN const char *szDevContent)
{
//	if (m_bDevInit) return SUCCESS;	
//	m_bDevInit = true;

	DELARR(m_pSerialParser);
	DELARR(m_pSerialProtocal);	

	DELARR(m_pUsbParser);
	DELARR(m_pUsbProtocal);

	E_Status_t nRet = UNKNOW_STATUS;	

	//1������JSON��ʽ
	Format::CFormatFactory formatfactory;
	Format::IFormat * pJsonFormat = formatfactory.CreateFormat(Format::JSON_FORMAT_TYPE);
	CheckNullPtr(pJsonFormat);

	//2��������ʽ
	char *pFormatData = NULL;
	int nDevType = Format::UNKNOW_TYPE;
	 
	nRet = (E_Status_t)pJsonFormat->ParseFormat(szDevContent, nDevType, pFormatData);
	CheckStatus(nRet);

	if (Format::HingMed_ABP_DEV_INIT == nDevType)	//���������豸
	{
		//3��serial����
		/*	Parser::CParserFactory parserfactory;
		m_pSerialParser = parserfactory.CreateParser(Parser::SERIAL_DATA_TYPE);
		CheckNullPtr(m_pSerialParser);
		nRet = (E_Status_t)m_pSerialParser->Parse(szDevContent, nDevType, pFormatData);*/
	
		//4������Э��
		Protocal::CProtocalFactory protocalfactory;
		m_pSerialProtocal = protocalfactory.CreateProtocal(Protocal::SERIAL_DATA_TYPE);
		CheckNullPtr(m_pSerialProtocal);
		char *pProtocalData = NULL;
		int nCount;
		nRet = (E_Status_t)m_pSerialProtocal->ParseProtocal(pFormatData, nDevType, pProtocalData, nCount);

	}
	else if (Format::ACF_DEV_INIT == nDevType)	//��ѶUSB�豸
	{
		//4������Э��
		Protocal::CProtocalFactory protocalfactory;
		m_pUsbProtocal = protocalfactory.CreateProtocal(Protocal::USB_DATA_TYPE);
		CheckNullPtr(m_pUsbProtocal);
		char *pProtocalData = NULL;
		int nCount;
		nRet = (E_Status_t)m_pUsbProtocal->ParseProtocal(pFormatData, nDevType, pProtocalData, nCount);

	}

	m_nDevType = nDevType;

	DELPTR(pJsonFormat);
	DELPTR(pFormatData);
	return nRet;
}

int CDevServer::OpenDevice()
{
	E_Status_t nRet = UNKNOW_STATUS;

	if (Format::HingMed_ABP_DEV_INIT == m_nDevType)		//�����豸
	{
		int nDevType = Format::OPEN_DEVICE;		//���豸
		char *pProtocalData = NULL;
		int nCount;
		nRet = (E_Status_t)m_pSerialProtocal->ParseProtocal(NULL, nDevType, pProtocalData, nCount);
	}
	else if (Format::ACF_DEV_INIT == m_nDevType)	//USB�豸
	{
		int nDevType = Format::OPEN_DEVICE;		//���豸
		char *pProtocalData = NULL;
		int nCount;
		nRet = (E_Status_t)m_pUsbProtocal->ParseProtocal(NULL, nDevType, pProtocalData, nCount);

	}
	return nRet;
}


int CDevServer::WriteDevice(IN const char *pData, IN int nSize, \
	OUT char **pResultData,  OUT int &nResultSize)
{
	E_Status_t nRet = UNKNOW_STATUS;

	if (Format::HingMed_ABP_DEV_INIT == m_nDevType)	//�����豸
	{
		//1��������ʽ
		char *pFormatData = NULL;
		int nType = Format::UNKNOW_TYPE;

		//1������JSON��ʽ
		Format::CFormatFactory formatfactory;
		Format::IFormat * pJsonFormat = formatfactory.CreateFormat(Format::JSON_FORMAT_TYPE);
		CheckNullPtr(pJsonFormat);
		nRet = (E_Status_t)pJsonFormat->ParseFormat(pData, nType, pFormatData);
		CheckStatus(nRet);

		////2��serial����
		//CheckNullPtr(m_pSerialParser);
		//nRet = (E_Status_t)m_pSerialParser->Parse(pData, nType, pFormatData);
		//CheckStatus(nRet);
			
		//3������Э��
		char *pProtocalData = NULL;
		int nLen;
		nRet = (E_Status_t)m_pSerialProtocal->ParseProtocal(pFormatData, nType, pProtocalData, nLen);
		CheckStatus(nRet);

		//4������JSON��ʽ����
		nRet = (E_Status_t)pJsonFormat->CreateFormat(pProtocalData, nLen, nType, pResultData, nResultSize);
		CheckStatus(nRet);
		DELARR(pProtocalData);
		DELPTR(pJsonFormat);
	
	}
	else if (Format::ACF_DEV_INIT == m_nDevType)	//USB�豸
	{
		//1��������ʽ
		char *pFormatData = NULL;
		int nType = Format::UNKNOW_TYPE;

		//1������JSON��ʽ
		Format::CFormatFactory formatfactory;
		Format::IFormat * pJsonFormat = formatfactory.CreateFormat(Format::JSON_FORMAT_TYPE);
		CheckNullPtr(pJsonFormat);
		nRet = (E_Status_t)pJsonFormat->ParseFormat(pData, nType, pFormatData);
		CheckStatus(nRet);

		//3������Э��
		char *pProtocalData = NULL;
		int nLen;
		nRet = (E_Status_t)m_pUsbProtocal->ParseProtocal(pFormatData, nType, pProtocalData, nLen);
		CheckStatus(nRet);

		//4������JSON��ʽ����
		nRet = (E_Status_t)pJsonFormat->CreateFormat(pProtocalData, nLen, nType, pResultData, nResultSize);
		CheckStatus(nRet);
		DELARR(pProtocalData);
		DELPTR(pJsonFormat);

	}
	return nRet;
}

int CDevServer::ReadDevice(OUT char **pData, OUT int &nSize)
{
	E_Status_t nRet = UNKNOW_STATUS;
	if (Format::HingMed_ABP_DEV_INIT == m_nDevType)	//�����豸
	{	
		int nTimeOut = 10000;
		char *pReadData = NULL;
		//nRet = (E_Status_t)m_pSerialDev->ReadDevice(pData, nSize, nTimeOut);
		//3������Э��
		//char *pProtocalData = NULL;
		//nRet = (E_Status_t)m_pSerialProtocal->ParseProtocal(pFormatData, nType, pProtocalData);
	}
	else if (Format::ACF_DEV_INIT == m_nDevType)	//USB�豸
	{
		int nType = Format::HingMed_ABP_GET_RECORD_DATA;
		char *pProtocalData = NULL;
		int nCount;
		nRet = (E_Status_t)m_pSerialProtocal->ParseProtocal(*pData, nType, pProtocalData, nCount);
	}	
	return nRet;
}

int CDevServer::CloseDevice()
{
	E_Status_t nRet = UNKNOW_STATUS;

	if (Format::HingMed_ABP_DEV_INIT == m_nDevType)	//�����豸
	{
		//3������Э��
		int nType = Format::CLOSE_DEVICE;
		char *pProtocalData = NULL;
		int nCount;
		nRet = (E_Status_t)m_pSerialProtocal->ParseProtocal("", nType, pProtocalData, nCount);
	}
	else if (Format::ACF_DEV_INIT == m_nDevType)	//USB�豸
	{
		//3������Э��
		int nType = Format::CLOSE_DEVICE;
		char *pProtocalData = NULL;
		int nCount;
		nRet = (E_Status_t)m_pSerialProtocal->ParseProtocal("", nType, pProtocalData, nCount);
	}
	return nRet;
}

int CDevServer::FreeMemory(IN char **pData)
{
	E_Status_t nRet = SUCCESS;
	DELARR(*pData);
	return nRet;
}






__END_NAMESPACE(DevServer)

