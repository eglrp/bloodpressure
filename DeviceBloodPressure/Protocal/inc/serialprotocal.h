#pragma once
#include "iprotocal.h"


//�豸
#include "idevinterfacetype.h"
#include "devfactory.h"


__BEGIN_NAMESPACE(Protocal)


class CSerialProtocal : public IProtocal
{
public:
	CSerialProtocal(void);
	~CSerialProtocal(void);

	int ParseProtocal(IN const char *pData, IN int nType, OUT char *&pProtocalData, OUT int &nLen);

protected:
	int ParseMessageProtocal(IN const char *pData, OUT char *pProtocalData);
	int ParseRecordsDataProtocal(IN const char *pData, OUT char *&pProtocalData, OUT int &nLen);
	int ParseRecordSumProtocal(IN const char *pData, OUT char *pProtocalData, OUT int &nCount);

protected:
	int SetData( IN const char *pData, \
				IN int nLen,	\
				IN const char nCmd,\
				IN const char nMode, \
				OUT char &nPacketLen,	\
				IN bool IsParam = false);			//д�豸����
	int GetData(OUT char **pData, IN int nLen);		//���豸����

	//�����豸����
protected:
	int InitDevice(IN const char *szDevContent);
	int OpenDevice();
	int CloseDevice();
	int WriteDivice(const char *pData, IN int nSize, IN int nTimeOut);
	int ReadDivice(char **pData, IN int &nSize, IN int nTimeOut);
	int FreeMemory(IN char **pData);

protected:
	//��Э���йص������
	int SetDataHand(OUT char &nPacketLen);	//����
	int SetDataClr(OUT char &nPacketLen);	//��������
	
	//��ȡ�û�ID������
	int GetDataUserId(OUT char *&pReadData, OUT char &nPacketLen);
	int GetDataUserName(OUT char *&pReadData, OUT char &nPacketLen);
	//��ȡ��¼����
	int GetDataRecordSum(OUT int &nSum, OUT char &nPacketLen);
	//��ȡָ����¼������
	int GetDataSpecifiedRecordId(IN short sRecordId, OUT char *&pReadData, OUT char &nPacketLen);
	
	
	//CRC����
	unsigned short Crc16cal(unsigned char p[], unsigned char num);

private:
	Dev::IDevInterfaceType *m_pSerialDev;	//�豸
};


__END_NAMESPACE(Parser)


