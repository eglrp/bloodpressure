#pragma once
#include "iparserinterface.h"

#include "typedef.h"
#include "datatype.h"


#define IN
#define OUT


#ifdef WIN32
#define strncasecmp strnicmp
#else
/* Other systems have strncasecmp */
#endif



#define DEVICE_COM			"Serial"		//�����豸����


//���������ֶ���
#define SERIAL_NAME			"Com"
#define SERIAL_BAUD			"Baud"
#define SERIAL_PARITY		"Parity"
#define SERIAL_DATABITS		"DataBits"
#define SERIAL_STOPBITS		"StopBits"


//��������ʱ���ַ�������
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
	SerialDataParser(void){};
	~SerialDataParser(void){};

public:
	int Parse(IN const char *pData, IN int &nType, OUT char *&pParsedData);

protected:
	int ParseSerialDevFormat(IN const char *pData, OUT char *&pFormatData);
	int ParseMessureTimeMode(IN const char *pData, OUT char *&pFormatData);


};


__END_NAMESPACE(Parser)
