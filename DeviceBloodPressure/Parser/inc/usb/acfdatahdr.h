/*************************************************
  Copyright (C), 2015-2020, ���������ɷ����޹�˾
  @File name:	
  @Author:   	xianhui zhang
  @Date:     	2016-09-02
  @Description:  
*************************************************/
#ifndef ACF_DATA_HDR_H
#define ACF_DATA_HDR_H
#include <vector>
#include <iostream>

__BEGIN_NAMESPACE(Parser)


#pragma pack(push,1)
//1������ʱ������
typedef struct tag_AUTOTABLE
{
	int nHour;		//ʱ
	int nMin;		//��
	int nInterval;	//���
}AUTOTABLE, *pAUTOTABLE;
typedef std::vector<AUTOTABLE> AUTOTABLEVEC;

//2�����Զ�
typedef struct tag_AUTO
{
	int nDay;	//Ĭ�ϰ���15minһ��
	int nNight;	//Ĭ������30minһ��

}AUTO, *pAUTO;

//3�����ֶ�ʱ����,�Զ�����
typedef struct tag_HANDLEINTERVAL
{
	int nDay;	//Ĭ�ϰ���30minһ��
	int nNight;	//Ĭ������60minһ��

}HANDLEINTERVAL, *pHANDLEINTERVAL;

typedef struct tag_CURSYSTIME
{
	int nYear;
	int nMon;
	int nDay;
	int nHour;
	int nMin;
	int nSec;

}CURSYSTIME, *pCURSYSTIME;



typedef struct tag_SET
{
	char szCmd[MAX_LEN];
	int nID;	//��¼��ID
	int nMode;	//����ģʽ��0�����ֶ���1������ʱ��������2�����Զ�������15��������30���Ӳ�һ�Σ�3�����ֶ�ʱ����,�Զ����ԣ�4��������ѹ��ģʽ
	AUTOTABLE AutoTable[6];
	/*AUTOTABLEVEC AutoTableModeVec;		//1������ʱ������*/
	AUTO AutoMode;						//2�����Զ�
	HANDLEINTERVAL HandleIntervalMode;	//3�����ֶ�ʱ����,�Զ�����
	int nTime;  //��ʱʱ��
	CURSYSTIME CurSysTime;
	//	USBUINT nAddr;
	//	USBUINT nData;

}SET, *pSET;

//sdt cmd
typedef struct tag_SDT
{
	char szCmd[MAX_LEN];
	int nYear;
	int nMon;
	int nDay;

}SDT, *pSDT;

//st cmd
typedef struct tag_ST
{
	char szCmd[MAX_LEN];
	int nHour;
	int nMin;

}ST, *pST;

#pragma pack(pop,1)


__END_NAMESPACE(Parser)


#endif




