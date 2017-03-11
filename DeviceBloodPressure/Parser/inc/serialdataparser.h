#pragma once
#include "iparserinterface.h"

#include "typedef.h"
#include "datatype.h"


#define IN
#define OUT



#define TAG							"TAG"
#define CMD							"Cmd"
#define PARAM						"Param"

#define DEVICE_TYPE					"DeviceType"
#define DEVICE_HINGMED_NAME			"HingMed_ABP"	//��������
#define DEVICE_ACF_NAME				"ACF"			//��Ѷ

#define DEVICE_COM					"Serial"		//�����豸����
#define DEVICE_USB					"USB"			//USB�豸����
#define DEVICE_WRITE				"Write"			//д����

#define READ_RECORD_SUM				"GetRecordSum"	//��ȡ��¼����
#define READ_RECORD_DATA			"RRCD"			//��ȡ��¼����


//���������ֶ���
#define SERIAL_NAME			"Com"
#define SERIAL_BAUD			"Baud"
#define SERIAL_PARITY		"Parity"
#define SERIAL_DATABITS		"DataBits"
#define SERIAL_STOPBITS		"StopBits"

//����ʱ���ַ�������
#define MESSAGE_USER_ID				"UserID"			//�û�ID
#define MESSAGE_USER_NAME			"UserName"			//�û�����
#define MESSAGE_MODE				"Mode"				//ģʽ��0��ʾ�̶�ģʽ��1��ʾ��׼ģʽ
#define MESSAGE_DAY_START_HOUR		"DayStartHourTime"	//���쿪ʼСʱ
#define MESSAGE_DAY_START_MIN		"DayStartMinTime"	//���쿪ʼ����
#define MESSAGE_DAY_INTERNAL		"DayInternal"		//������ʱ��
#define MESSAGE_NIGHT_START_HOUR	"NightStartHourTime"//ҹ��ʼСʱ
#define MESSAGE_NIGHT_START_MIN		"NightStartMinTime"	//ҹ��ʼ����
#define MESSAGE_NIGHT_INTERNAL		"NightInternal"		//ҹ����ʱ��




__BEGIN_NAMESPACE(Parser)



class SerialDataParser :
	public IParserInterface
{
public:
	SerialDataParser(void);
	~SerialDataParser(void);

public:
	int Parse(IN const char *pData, IN int nType, OUT char *&pParsedData);

	int ParseSerialDevFormat(IN const char *pData, OUT char *&pFormatData);
	int ParseMessureTime(IN const char *pData, OUT char *&pFormatData);
	int ParseRecordSum(IN const char *pData, OUT char *&pFormatData);


};


__END_NAMESPACE(Parser)
