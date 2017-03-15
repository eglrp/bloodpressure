/*************************************************
  Copyright (C), 2015-2020, ���������ɷ����޹�˾
  @File name:	
  @Author:   	xianhui zhang
  @Date:     	2016-09-02
  @Description:  
*************************************************/
#ifndef LIBBPDEV_INTERFACE_H
#define LIBBPDEV_INTERFACE_H
#include "hdr.h"


#ifdef LIBBPDEVEXPORT                                                 
#define BPDEVEXPORT extern "C" __declspec(dllexport)
#else
#define BPDEVEXPORT extern "C" __declspec(dllimport)
#endif



#define INOUT                    
#define IN                       
#define OUT 




//////////////////////////////////////////////////////////////////////////
/// @function [BpInitDevice]
/// @brief [��ʼ���豸]
///
/// @param [in] szDevContent	[�豸����:
///	HID USB�豸JSON��ʽ�� {"TAG":"Usb","Param":{wVID:"xxx",wPID:"xxx"}}��
/// �����豸JSON��ʽ��{"TAG":"Serial","Param":{Com��"����"��nBaud��"������"��parity��"��ż����λ"��DataBit��"����λ"��StopBits:"ֹͣλ"}}  
///                             ]
/// @return						[int ����]
//////////////////////////////////////////////////////////////////////////
BPDEVEXPORT int BpInitDevice(IN const char *szDevContent);

//////////////////////////////////////////////////////////////////////////
/// @function [BpOpenDevice]
/// @brief [���豸]
///
/// @return						[int ����]
//////////////////////////////////////////////////////////////////////////
BPDEVEXPORT int BpOpenDevice();

//////////////////////////////////////////////////////////////////////////
/// @function [BpCloseDevice]
/// @brief [�ر��豸]
///
/// @return				[int ����]
//////////////////////////////////////////////////////////////////////////
BPDEVEXPORT int BpCloseDevice();

/// @function [BpWriteDevice]
/// @brief [д���ݵ��豸]
///
/// @param [in] pData	[��������]
/// @param [in] nSize	[���ݳ���]
/// @param [OUT] pResultData	[���صĽ�����ݣ��޽������ֵΪ��]
/// @param [OUT] nResultSize	[���صĽ�����ݴ�С���޽������ֵΪ0]
/// @return				[int ����]
//////////////////////////////////////////////////////////////////////////
BPDEVEXPORT int BpWriteDevice(IN const char *pData, IN int nSize, \
	OUT char **pResultData,  OUT int &nResultSize);

//////////////////////////////////////////////////////////////////////////
/// @function [BpReadDevice]
/// @brief [���豸]
///
/// @param [OUT] pData	[��������]
/// @param [OUT] nSize	[���ݳ���]
/// @return				[int ����]
//////////////////////////////////////////////////////////////////////////
BPDEVEXPORT int BpReadDevice(OUT char **pData, OUT int &nSize);


//////////////////////////////////////////////////////////////////////////
/// @function [BpFreeMemory]
/// @brief [���豸]
///
/// @param [in] pData	[��������]
/// @return				[int ����]
//////////////////////////////////////////////////////////////////////////
BPDEVEXPORT int BpFreeMemory(IN char **pData);




#endif



