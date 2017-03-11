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
	/// @brief [打开USB HID类型设备]
	///
	/// @param [in] szDevContent	[设备内容，
	///	USB设备JSON格式： {wVID:"xxx",wPID:"xxx"}；
	/// 串口设备JSON格式：{Com："名称"，nBaud："波特率"，parity："奇偶检验位"，DataBit："数据位"，StopBits:"停止位"}  
	///                             ]
	/// @param [in] nDevType		[设备类型，0表示串口设备，1表示USB设备(未用)]
	/// @return						[int 类型]
	//////////////////////////////////////////////////////////////////////////
	virtual int InitDevice(IN const char *szDevContent) = 0;

	//////////////////////////////////////////////////////////////////////////
	/// @function [OpenDevice]
	/// @brief [打开USB HID类型设备]
	///
	/// @param [in] szDevContent	[设备内容，
	///	USB设备JSON格式： {wVID:"xxx",wPID:"xxx"}；
	/// 串口设备JSON格式：{Com："名称"，nBaud："波特率"，parity："奇偶检验位"，DataBit："数据位"，StopBits:"停止位"}  
	///                             ]
	/// @param [in] nDevType		[设备类型，0表示串口设备，1表示USB设备]
	/// @return						[int 类型]
	//////////////////////////////////////////////////////////////////////////
	virtual int OpenDevice() = 0;

	//////////////////////////////////////////////////////////////////////////
	/// @function [CloseDevice]
	/// @brief [关闭设备]
	///
	/// @return				[int 类型]
	//////////////////////////////////////////////////////////////////////////
	virtual int CloseDevice() = 0;

	//////////////////////////////////////////////////////////////////////////
	/// @function [WriteDevice]
	/// @brief [写数据到设备]
	///
	/// @param [in] pData	[数据内容]
	/// @param [in] nSize	[数据长度]
	/// @param [in] nTimeOut	[超时时间]
	/// @return				[int 类型]
	//////////////////////////////////////////////////////////////////////////
	virtual int WriteDevice(IN const char *pData, IN int nSize, IN int nTimeOut = 10000) = 0;

	//////////////////////////////////////////////////////////////////////////
	/// @function [ReadDevice]
	/// @brief [读设备]
	///
	/// @param [OUT] pData	[数据内容]
	/// @param [OUT] nSize	[数据长度]
	/// @param [in] nTimeOut	[超时时间]
	/// @return				[int 类型]
	//////////////////////////////////////////////////////////////////////////
	virtual int ReadDevice(OUT char **pData, IN int nSize, IN int nTimeOut = 10000) = 0;


	//////////////////////////////////////////////////////////////////////////
	/// @function [FreeMemory]
	/// @brief [读设备]
	///
	/// @param [in] pData	[数据内容]
	/// @return				[int 类型]
	//////////////////////////////////////////////////////////////////////////
	virtual int FreeMemory(IN char **pData) = 0;



};



__END_NAMESPACE(Dev)
