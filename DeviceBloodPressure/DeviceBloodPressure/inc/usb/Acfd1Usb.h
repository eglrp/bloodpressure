#pragma once
#include "Usb.h"
#include "cJSON.h"
#include <assert.h>
#include <vector>


#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>  
#include <log4cplus/configurator.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
//#include <conio.h>
#include <log4cplus/helpers/sleep.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;
using namespace log4cplus::helpers; 

#include <iostream>





#ifdef WIN32
#define strncasecmp strnicmp
#endif


/* Used to represent different types of tokens in input. */
typedef enum
{
	TT_IDENTIFIER, 
	TT_WHITESPACE, 
	TT_CRLF, 
	TT_CTRL,
	TT_SEPARATOR,
	TT_QUOTEDSTRING,
} token_type_t;

/*! status of parsing */
typedef enum 
{
	/*! msg was parsed successfully. */
	PARSE_SUCCESS = 0,
	/*! need more data to continue. */
	PARSE_INCOMPLETE,
	/*! for responses that don't have length specified. */
	PARSE_INCOMPLETE_ENTITY,
	/*! parse failed; check status code for details. */
	PARSE_FAILURE,
	/*! done partial. */
	PARSE_OK,
	/*! token not matched. */
	PARSE_NO_MATCH,
	/*! private. */
	PARSE_CONTINUE_1
} parse_status_t;


typedef enum E_DATA_TYPE
{
	DATE_HIGH = 0,
	DATE_LOW,
	HEART_RATE,
	BLOOD_PRESSURE_HIGH,
	BLOOD_PRESSURE_LOW,
	ID_HIGH,
	ID_LOW,
	RESERVE

};


typedef struct tag_MEASUREDATA
{
	USBUINT nID;			//���˼�¼��
	USBDWORD nTime;			//����ʱ��
	USBUINT HR;				//����
	USBUINT DP;				//����ѹ
	USBUINT SP;				//����ѹ
	USBUINT	nReserve;		//����8λ

}MEASUREDATA, *pMEASUREDATA;



//set cmd
//typedef struct tag_SET
//{
//	USBCHAR szCmd[USB_LEN];
//	USBUINT nAddr;
//	USBUINT nData;
//
//}SET, *pSET;


//1������ʱ������
typedef struct tag_AUTOTABLE
{
	USBUINT nHour;		//ʱ
	USBUINT nMin;		//��
	USBUINT nInterval;	//���
}AUTOTABLE, *pAUTOTABLE;
typedef std::vector<AUTOTABLE> AUTOTABLEVEC;


//2�����Զ�
typedef struct tag_AUTO
{
	USBUINT nDay;	//Ĭ�ϰ���15minһ��
	USBUINT nNight;	//Ĭ������30minһ��

}AUTO, *pAUTO;


//3�����ֶ�ʱ����,�Զ�����
typedef struct tag_HANDLEINTERVAL
{
	USBUINT nDay;	//Ĭ�ϰ���30minһ��
	USBUINT nNight;	//Ĭ������60minһ��

}HANDLEINTERVAL, *pHANDLEINTERVAL;


typedef struct tag_CURSYSTIME
{
	USBUINT nYear;
	USBUINT nMon;
	USBUINT nDay;
	USBUINT nHour;
	USBUINT nMin;
	USBUINT nSec;

}CURSYSTIME, *pCURSYSTIME;

typedef struct tag_SET
{
	USBCHAR szCmd[USB_LEN];
	USBUINT nID;	//��¼��ID
	USBUINT nMode;	//����ģʽ��0�����ֶ���1������ʱ��������2�����Զ�������15��������30���Ӳ�һ�Σ�3�����ֶ�ʱ����,�Զ����ԣ�4��������ѹ��ģʽ
	AUTOTABLEVEC AutoTableModeVec;		//1������ʱ������
	AUTO AutoMode;						//2�����Զ�
	HANDLEINTERVAL HandleIntervalMode;	//3�����ֶ�ʱ����,�Զ�����
	USBUINT nTime;  //��ʱʱ��
	CURSYSTIME CurSysTime;
//	USBUINT nAddr;
//	USBUINT nData;

}SET, *pSET;



//sdt cmd
typedef struct tag_SDT
{
	USBCHAR szCmd[USB_LEN];
	USBUINT nYear;
	USBUINT nMon;
	USBUINT nDay;

}SDT, *pSDT;

//st cmd
typedef struct tag_ST
{
	USBCHAR szCmd[USB_LEN];
	USBUINT nHour;
	USBUINT nMin;

}ST, *pST;



class CAcfd1Usb
{
public:
	static CAcfd1Usb *GetInstance();	
	~CAcfd1Usb(void);

	USBRES FindUsbDevice(OUT USBCHAR **pData);
	USBRES OpenUsb(IN USBWORD wVID = 1155, IN USBWORD wPID = 22352);	
	USBRES CloseUsb();
	USBRES WriteData(IN const USBCHAR *pData, IN USBUINT nSize);
	USBRES ReadData(OUT USBCHAR **pData, OUT USBUINT &nSize);
	USBRES ReadDataRef(OUT USBCHAR *&pData, OUT USBUINT &nSize);
	USBRES FreeMemory(IN USBCHAR *&pData);
	

protected:
	void InitLog();
	USBRES ParseJsonCmd(IN const USBCHAR *pData, OUT USBCHAR *pCmd);
	USBRES HandleJsonCmd(IN const USBCHAR *pData, IN const USBCHAR *pCmd);
	USBRES HandleSetCmd(IN const USBCHAR *pData, IN const USBCHAR *pCmd);

	USBRES ParseJsonSetCmd(IN const USBCHAR *pData, OUT SET &pSet);
	USBRES ParseJsonSdtCmd(IN const USBCHAR *pData, OUT SDT *&pSdt);
	USBRES ParseJsonStCmd(IN const USBCHAR *pData, OUT ST *&pSt);

	USBRES ParseJsonData(IN const USBCHAR *pData, IN USBUINT nSize);

	
protected:
	USBRES SendSetCommand(std::string szCmd);	//send set cmd	
	USBRES SendReadCommand(std::string szCmd);
	std::string GetCurrentCommad();

	std::string Int2Hex(int value, int digit);

	USBRES Help();			//? cmd
	USBRES ReadRSet();		//rset cmd
	USBRES ReadRecord();	//rrcd cmd
	USBRES ClearRecord();	//clr rrcd
		
	USBRES Set(USBINT address, USBINT value);
	USBRES SetId(USBINT id);
	USBRES SetDate(USBUINT nYear, USBUINT nMon, USBUINT nDay);		//sdt
	USBRES SetTime(USBUINT nHour, USBUINT nMin);		//st
	USBRES SetEmpty(int address);

	USBRES SetAuto();	
	USBRES SetManual(USBINT intervalDay, USBINT intervalNight);
	USBRES SetAutoTable();
	USBRES setAutoTableEntry(int index, int hour, int minute, int interval);
	USBRES SetAutoTableEmpty(int index);


	USBRES ParseCmd(OUT USBCHAR *pCmd, OUT USBUINT &nCmdLen, IN const USBCHAR *cursor);
	USBRES ParseSetCmdData(OUT SET &pSetData, IN const USBCHAR *cursor);
	USBRES WriteCmd(IN const USBCHAR *cmd, IN const USBCHAR *cursor);

	USBRES ParseRxBuf(OUT membuffer *pdata, IN const USBCHAR *prxBuf, OUT USBINT nSize);
	USBRES ParseData(OUT membuffer *pdata, IN const USBCHAR *cmd, IN const USBCHAR *cursor, OUT USBINT nSize);
	
	USBRES ParseRrcdCmdData(OUT membuffer *pdata, IN const USBCHAR *cursor, OUT USBINT nSize);
	USBRES ParseRsetCmdData(OUT membuffer *pdata, IN const USBCHAR *cursor, OUT USBINT nSize);
	
	USBRES GetTokenString(USBCHAR *&pBuf, USBINT &nSize, const USBCHAR *prxBuf);

	USBRES GenRrcdCmdDataJson(OUT membuffer *pdata, IN cJSON *pMeasureDataList, IN USBUINT *pRrcdData, IN USBUINT nSize);
	USBRES GenRsetCmdDataJson(OUT membuffer *pdata, IN cJSON *pTimeList, IN USBUINT *pRsetData, IN USBUINT nSize);


	uint64_t HexToASCII(const char* szIn);
	int8_t HexToInt(char c);

	parse_status_t ScannerGetToken(INOUT const USBCHAR *scanner, OUT memptr *token, OUT token_type_t *tok_type);

	inline int IsSeparatorChar(IN int c)
	{
		return strchr(" \t()<>@,;:\\\"/[]?={}", c) != 0;
	}

	inline int IsIdentifierChar(IN int c)
	{
		return c >= 32 && c <= 126 && !IsSeparatorChar(c);
	}

	inline int IsControlChar(IN int c)
	{
		return (c >= 0 && c <= 31) || c == 127;
	}

	inline int IsQdtextChar(IN int c)
	{
		assert( c != '"' );
		return
			(c >= 32 && c != 127) ||
			c < 0 ||
			c == TOKCHAR_CR ||
			c == TOKCHAR_LF ||
			c == '\t';
	}


private:
	explicit CAcfd1Usb(void);
	static CAcfd1Usb *m_gpAcfd1Usb;
	CUsb m_Usb;
	CRITICAL_SECTION m_csMutex;
	

	Logger m_Logger;
};

