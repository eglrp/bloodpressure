#pragma once
#include "typedef.h"

#define IN
#define OUT


#define MIN_LEN					32
#define MAX_LEN					512
#define SERIAL_PROTOCAL_LEN		256




#define TAG					"TAG"
#define CMD					"Cmd"
#define PARAM				"Param"

#define MEASURE_MODE		"SET"			//���ò���ģʽ
#define HELP_COMMAND		"HELP"			//��������
#define RESET_COMMAND		"RESET"			//��λ����
#define SDT_COMMAND			"SDT"			//����ʱ������
#define ST_COMMAND			"ST"			//������������
#define RDA_COMMAND			"RDA"			//��ģת������
#define RDF_COMMAND			"RDF"
#define RRCD_COMMAND		"RRCD"			//��ȡ��������
#define VER_COMMAND			"VEC"
#define CLR_COMMAND			"CLR"			//�����������




#define DEVICE_HINGMED_NAME			"HingMed_ABP"	//������������
#define DEVICE_ACF_NAME				"ACF"			//��Ѷ��������


#define READ_RECORD_SUM				"GetRecordSum"	//��ȡ��¼����
#define READ_RECORD_DATA			"RRCD"			//��ȡ��¼����








__BEGIN_NAMESPACE(Format)



#pragma pack(push,1)


typedef struct 
{
	const char *szName;
	int id;

}str_int_entry, *pstr_int_entry;



typedef enum E_Dev_Type
{
	UNKNOW_DEV_TYPE		= -1,
	HingMed_ABP_DEVICE,			//�����豸
	ACF_DEVICE					//��Ѷ�豸
}Dev_Type_t;



typedef enum E_DataType
{
	UNKNOW_TYPE		= -1,	
	HingMed_ABP_DEV_INIT,		//���������豸
	HingMed_ABP_MEASURE_MODE,	//������������ʱ��ģʽ	
	HingMed_ABP_GET_RECORD_DATA,			//��ȡ���м�¼����
	HingMed_ABP_CLC_RECORD_DATA,			//�豸�ڼ�¼����
	ACF_DEV_INIT,				//��ѶUSB�豸	
	ACF_MEASURE_MODE,			//���ð�Ѷ����ʱ��ģʽ
	ACF_GET_RECORD_DATA,			//��ȡ���м�¼����
	ACF_CLC_RECORD_DATA,			//�豸�ڼ�¼����
	OPEN_DEVICE,				//���豸
	CLOSE_DEVICE				//�ر��豸	

}E_DataType_t;



typedef struct tag_Head_Param
{
	char szTag[MAX_LEN];
	char szCmd[MIN_LEN];

}HeadParam_t, *pHeadParam_t;


/*Com��"����"��
nBaud��"������"��
parity��"��ż����λ"��
DataBits��"����λ"��
StopBits:"ֹͣλ"*/
typedef struct tag_SerialDev
{
	HeadParam_t Head;
	char szCom[MIN_LEN];
	int nBaud;
	char nParity;
	int nDataBits;
	int nStopBits;

}SerialDevData_t;


//HID Usb����
typedef struct tag_UsbDev
{
	HeadParam_t Head;
	int wPID;
	int wVID;

}UsbDevData_t;


typedef struct tag_MessureTime
{
	HeadParam_t Head;
	char szUserId[MAX_LEN];
	char szUserName[MAX_LEN];
	int nMode;
	int nDayStartHour;
	int nDayStartMin;
	int nDayInternal;
	int nNightStartHour;
	int nNightStartMin;
	int nNigthInternal;

}MessureTime_t, *pMessureTime_t;

typedef struct tag_UserIdAndName
{
	char szUserId[MIN_LEN];
	char szUserName[MIN_LEN];

}UserIdAndName_t, *pUserIdAndName_t;


typedef struct tag_RecordData
{	
	short nRecordId;	//��¼��Ӧ��ID���
	unsigned short nSys;			//����ѹ
	unsigned short nDia;			//����ѹ
	unsigned short nRate;		//����
	char nYear;			//��
	char nMon;			//��
	char nDay;			//��
	char nHour;			//ʱ
	char nMin;			//��
	char nEc;			//
	char nAuto;

}RecordData_t, *pRecordData_t;


typedef struct {
	int nId;
	short nSys;			//����ѹ
	short nDia;			//����ѹ
	short nRate;		//����
	char nYear;			//��
	char nMon;			//��
	char nDay;			//��
	char nHour;			//ʱ
	char nMin;			//��
	char nSec;			//��

}acf_record_data_t;


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


