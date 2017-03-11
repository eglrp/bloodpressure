/*************************************************
  Copyright (C), 2015-2020, ���������ɷ����޹�˾
  @File name:	
  @Author:   	xianhui zhang
  @Date:     	2016-09-02
  @Description:  
*************************************************/
#ifndef LIBBPDEV_INTERFACE_H
#define LIBBPDEV_INTERFACE_H


#ifdef LIBBPDEVEXPORT                                                 
#define BPDEVEXPORT extern "C" __declspec(dllexport)
#else
#define BPDEVEXPORT extern "C" __declspec(dllimport)
#endif



#define INOUT                    
#define IN                       
#define OUT 


//////////////////////////////////////////////////////////////////////////
/// @function [InitDevice]
/// @brief [��USB HID�����豸]
///
/// @param [in] szDevContent	[�豸���ݣ�
///	USB�豸JSON��ʽ�� {wVID:"xxx",wPID:"xxx"}��
/// �����豸JSON��ʽ��{Com��"����"��nBaud��"������"��parity��"��ż����λ"��DataBit��"����λ"��StopBits:"ֹͣλ"}  
///                             ]
/// @param [in] nFormatType		[���ݸ�ʽ���ͣ�0��ʾjson��ʽ��1��ʾxml��ʽ��2��ʾprotobuf��ʽ]
/// @return						[int ����]
//////////////////////////////////////////////////////////////////////////
BPDEVEXPORT int BpInitDevice(IN const char *szDevContent);

//////////////////////////////////////////////////////////////////////////
/// @function [OpenDevice]
/// @brief [��USB HID�����豸]
///
/// @param [in] szDevContent	[�豸���ݣ�
///	USB�豸JSON��ʽ�� {wVID:"xxx",wPID:"xxx"}��
/// �����豸JSON��ʽ��{Com��"����"��nBaud��"������"��parity��"��ż����λ"��DataBit��"����λ"��StopBits:"ֹͣλ"}  
///                             ]
/// @param [in] nDevType		[�豸���ͣ�0��ʾ�����豸��1��ʾUSB�豸]
/// @return						[int ����]
//////////////////////////////////////////////////////////////////////////
BPDEVEXPORT int BpOpenDevice();

//////////////////////////////////////////////////////////////////////////
/// @function [CloseDevice]
/// @brief [�ر��豸]
///
/// @return				[int ����]
//////////////////////////////////////////////////////////////////////////
BPDEVEXPORT int BpCloseDevice();

/// @function [WriteDevice]
/// @brief [д���ݵ��豸]
///
/// @param [in] pData	[��������]
/// @param [in] nSize	[���ݳ���]
/// @param [OUT] pResultData	[���صĽ�����ݣ�����ֵ����Ϊ��]
/// @param [OUT] nResultSize	[���صĽ�����ݴ�С������ֵ����Ϊ0]
/// @return				[int ����]
//////////////////////////////////////////////////////////////////////////
BPDEVEXPORT int BpWriteDevice(IN const char *pData, IN int nSize, \
	OUT char **pResultData,  OUT int &nResultSize);

//////////////////////////////////////////////////////////////////////////
/// @function [ReadDevice]
/// @brief [���豸]
///
/// @param [OUT] pData	[��������]
/// @param [OUT] nSize	[���ݳ���]
/// @param [in] nTimeOut	[��ʱʱ��]
/// @return				[int ����]
//////////////////////////////////////////////////////////////////////////
BPDEVEXPORT int BpReadDevice(OUT char **pData, OUT int &nSize);


//////////////////////////////////////////////////////////////////////////
/// @function [FreeMemory]
/// @brief [���豸]
///
/// @param [in] pData	[��������]
/// @return				[int ����]
//////////////////////////////////////////////////////////////////////////
BPDEVEXPORT int BpFreeMemory(IN char **pData);









#endif





