#pragma once
#include "typedef.h"

#define IN
#define OUT
#define MIN_LEN					32
#define MAX_LEN					512
#define SERIAL_PROTOCAL_LEN		256


__BEGIN_NAMESPACE(Format)



#pragma pack(push,1)

typedef enum E_DevType
{
	UNKNOW_TYPE		= -1,
	SERIAL_DEV_TYPE,			//�����豸
	USB_DEV_TYPE,				//USB�豸
	OPEN_DEVICE,				//���豸
	CLOSE_DEVICE,				//�ر��豸	
	SERIAL_MESSAGE_PROTOCAL,	//���ò���ʱ��ģʽ
	SERIAL_RECORD_SUM,			//���Ӽ�¼����
	SERIAL_SPECIFIED_RECORD,	//ȡ���ض�ĳһ����¼
	SERIAL_RECORD_DATA,			//��ȡ���м�¼����
	CLC_RECORD,					//�豸�ڼ�¼����
	USB_RRCD_PROTOCAL

}E_DevType_t;



/*Com��"����"��
nBaud��"������"��
parity��"��ż����λ"��
DataBits��"����λ"��
StopBits:"ֹͣλ"*/
typedef struct tag_SerialDev
{
	char szCom[MIN_LEN];
	int nBaud;
	char nParity;
	int nDataBits;
	int nStopBits;

}SerialDevData_t;

typedef struct tag_SerialProtocal
{
	char nType;
	int nLen;
	char nCmd;
	char nParam;
	short nCrc;
}SerialProtocal_t;


typedef struct tag_UsbDev
{
	int wPID;
	int wVID;

}UsbDevData_t;



typedef struct tag_MessureTime
{
	char szTag[MAX_LEN];
	char szCmd[MIN_LEN];
	char szUserId[MAX_LEN];
	char szUserName[MAX_LEN];
	int nMode;
	int nDayStartHour;
	int nDayStartMin;
	int nDayInternal;
	int nNightStartHour;
	int nNightStartMin;
	int nNigthInternal;

}MessureTime_t;

typedef struct tag_UserIdAndName
{
	char szUserId[MIN_LEN];
	char szUserName[MIN_LEN];
}UserIdAndName_t;


typedef struct tag_RecordData
{	
	short nRecordId;	//��¼��Ӧ��ID���
	short nSys;			//����ѹ
	short nDia;			//����ѹ
	short nRate;		//����
	char nYear;			//��
	char nMon;			//��
	char nDay;			//��
	char nHour;			//ʱ
	char nMin;			//��
	char nEc;			//
	char nAuto;

}RecordData_t, *pRecordData_t;


typedef struct tag_CmdKey
{
	char szTag[MAX_LEN];
	char szCmd[MIN_LEN];

}CmdKey_t;


#pragma pack(pop,1)


template<class T>
class CDataType
{
public:
	CDataType(void) {};
	~CDataType(void) {};

public:
	int ParseFormat(IN const char *pData, OUT T *&pFormatData)
	{
		E_Status_t nRet = UNKNOW_STATUS;
		const char *pBuf = pData;

		if (sizeof(SerialDevData_t) == sizeof(T))
		{
			int aa = 0;

		}
		else if (sizeof(UsbDevData_t) == sizeof(T))
		{

		}
		else{}
		return nRet;
	}

	int CreateFormat(IN const char *pData, OUT T *pFormatData)
	{

		return 0;
	}

	/*int ParseSerialDevFormat(IN const char *pData, OUT char *&pFormatData);
	int ParseUsbDevFormat(IN const char *pData, OUT char *&pFormatData);

	int CreateSerialDevFormat(IN const char *pData, OUT char *pFormatData);
	int CreateUsbDevFormat(IN const char *pData, OUT char *pFormatData);*/
	


};




__END_NAMESPACE(Format)


