/*************************************************
  Copyright (C), 2015-2020, ���������ɷ����޹�˾
  @File name:	
  @Author:   	xianhui zhang
  @Date:     	2016-09-02
  @Description:  
*************************************************/
#ifndef ISDEVERVER_H
#define ISDEVERVER_H
#include "typedef.h"

#define IN
#define OUT

__BEGIN_NAMESPACE(DevServer)


class IDevServer
{
public:
	IDevServer(){};
	virtual ~IDevServer(){};


public:
	//////////////////////////////////////////////////////////////////////////
	/// @function [InitDevice]
	/// @brief [��ʼ���豸]
	///
	/// @param [in] szDevContent	[�豸����:
	///	HID USB�豸JSON��ʽ�� {"TAG":"Usb","Param":{wVID:"xxx",wPID:"xxx"}}��
	/// �����豸JSON��ʽ��{"TAG":"Serial","Param":{Com��"����"��nBaud��"������"��parity��"��ż����λ"��DataBit��"����λ"��StopBits:"ֹͣλ"}}  
	///                             ]
	/// @return						[int ����]
	//////////////////////////////////////////////////////////////////////////
	virtual int InitDevice(IN const char *szDevContent) = 0;

	//////////////////////////////////////////////////////////////////////////
	/// @function [OpenDevice]
	/// @brief [���豸]
	///
	/// @return						[int ����]
	//////////////////////////////////////////////////////////////////////////
	virtual int OpenDevice() = 0;

	//////////////////////////////////////////////////////////////////////////
	/// @function [CloseDevice]
	/// @brief [�ر��豸]
	///
	/// @return				[int ����]
	//////////////////////////////////////////////////////////////////////////
	virtual int CloseDevice() = 0;

	/// @function [WriteDevice]
	/// @brief [д���ݵ��豸]
	///
	/// @param [in] pData	[��������]
	/// @param [in] nSize	[���ݳ���]
	/// @param [OUT] pResultData	[���صĽ�����ݣ��޽������ֵΪ��]
	/// @param [OUT] nResultSize	[���صĽ�����ݴ�С���޽������ֵΪ0]
	/// @return				[int ����]
	//////////////////////////////////////////////////////////////////////////
	virtual int WriteDevice(IN const char *pData, IN int nSize, \
		OUT char **pResultData,  OUT int &nResultSize) = 0;

	//////////////////////////////////////////////////////////////////////////
	/// @function [ReadDevice]
	/// @brief [���豸]
	///
	/// @param [OUT] pData	[��������]
	/// @param [OUT] nSize	[���ݳ���]
	/// @param [in] nTimeOut	[��ʱʱ��]
	/// @return				[int ����]
	//////////////////////////////////////////////////////////////////////////
	virtual int ReadDevice(OUT char **pData, OUT int &nSize) = 0;


	//////////////////////////////////////////////////////////////////////////
	/// @function [FreeMemory]
	/// @brief [���豸]
	///
	/// @param [in] pData	[��������]
	/// @return				[int ����]
	//////////////////////////////////////////////////////////////////////////
	virtual int FreeMemory(IN char **pData) = 0;



};











__END_NAMESPACE(DevServer)


#endif



