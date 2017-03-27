#pragma once
#include "typedef.h"
#include "idevserver.h"

//��ʽ
#include "iformat.h"
#include "formatfactory.h"

//�豸
#include "idevinterfacetype.h"
#include "devfactory.h"

//����
#include "iparserinterface.h"
#include "parserfactory.h"

//Э��
#include "iprotocal.h"
#include "protocalfactory.h"

__BEGIN_NAMESPACE(DevServer)


class IDevServer;


class CDevServer : public IDevServer
{
public:
	static IDevServer *GetInstance();
	CDevServer(void);
	~CDevServer(void);	

public:
	int InitDevice(IN const char *szDevContent);
	int OpenDevice();
	int CloseDevice();
	int WriteDevice(IN const char *pData, IN int nSize, \
		OUT char **pResultData,  OUT int &nResultSize);
	int ReadDevice(OUT char **pData, OUT int &nSize);
	int FreeMemory(IN char **pData);


private:
	
	//Format::IFormat *m_pJsonFormat;	//��ʽ
	int m_nDevType;
	bool m_bDevInit;

	Parser::IParserInterface *m_pSerialParser;		//���ڽ���
	Protocal::IProtocal *m_pSerialProtocal;			//����Э�����

	Parser::IParserInterface *m_pUsbParser;			//USB����
	Protocal::IProtocal *m_pUsbProtocal;			//USBЭ�����

	static IDevServer *m_pgInstance;
};


__END_NAMESPACE(DevServer)

