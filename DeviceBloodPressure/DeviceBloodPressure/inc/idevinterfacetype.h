#pragma once


#define IN
#define OUT

#include "typedef.h"



__BEGIN_NAMESPACE(Dev)




class IDevInterfaceType
{
public:
	IDevInterfaceType(void) {};
	virtual ~IDevInterfaceType(void) = 0 {};

public:
	//////////////////////////////////////////////////////////////////////////
	/// @function [OpenDevice]
	/// @brief [��USB HID�����豸]
	///
	/// @param [in] szDevContent	[�豸���ݣ�
	///	USB�豸JSON��ʽ�� {wVID:"xxx",wPID:"xxx"}��
	/// �����豸JSON��ʽ��{Com��"����"��nBaud��"������"��parity��"��ż����λ"��DataBit��"����λ"��StopBits:"ֹͣλ"}  
	///                             ]
	/// @param [in] nDevType		[�豸���ͣ�0��ʾ�����豸��1��ʾUSB�豸(δ��)]
	/// @return						[int ����]
	//////////////////////////////////////////////////////////////////////////
	virtual int InitDevice(IN const char *szDevContent) = 0;

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
	virtual int OpenDevice() = 0;

	//////////////////////////////////////////////////////////////////////////
	/// @function [CloseDevice]
	/// @brief [�ر��豸]
	///
	/// @return				[int ����]
	//////////////////////////////////////////////////////////////////////////
	virtual int CloseDevice() = 0;

	//////////////////////////////////////////////////////////////////////////
	/// @function [WriteDevice]
	/// @brief [д���ݵ��豸]
	///
	/// @param [in] pData	[��������]
	/// @param [in] nSize	[���ݳ���]
	/// @param [in] nTimeOut	[��ʱʱ��]
	/// @return				[int ����]
	//////////////////////////////////////////////////////////////////////////
	virtual int WriteDevice(IN const char *pData, IN int nSize, IN int nTimeOut = 10000) = 0;

	//////////////////////////////////////////////////////////////////////////
	/// @function [ReadDevice]
	/// @brief [���豸]
	///
	/// @param [OUT] pData	[��������]
	/// @param [OUT] nSize	[���ݳ���]
	/// @param [in] nTimeOut	[��ʱʱ��]
	/// @return				[int ����]
	//////////////////////////////////////////////////////////////////////////
	virtual int ReadDevice(OUT char **pData, IN int nSize, IN int nTimeOut = 10000) = 0;


	//////////////////////////////////////////////////////////////////////////
	/// @function [FreeMemory]
	/// @brief [���豸]
	///
	/// @param [in] pData	[��������]
	/// @return				[int ����]
	//////////////////////////////////////////////////////////////////////////
	virtual int FreeMemory(IN char **pData) = 0;



};



__END_NAMESPACE(Dev)
