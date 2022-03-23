/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/


/**************************************************************************

    module: cosa_device_info_dml.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/05/2012    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "plugin_main_apis.h"
#include "cosa_device_info_apis.h"
#include "cosa_device_info_dml.h"
#include "cosa_device_info_internal.h"

/***********************************************************************

 APIs for Object:

    DeviceInfo.

    *  DeviceInfo_GetParamBoolValue
    *  DeviceInfo_GetParamStringValue
    *  DeviceInfo_SetParamBoolValue
    *  DeviceInfo_SetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            );

    description:

        This function is called to retrieve Boolean parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DeviceInfo_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDI;

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_FirmwareDownloadNow") == 0)
    {
	 if(pMyObject->Download_Control_Flag)
	 {
        	*pBool = FALSE;
	 }
        return TRUE;
    }

    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DeviceInfo_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pUlSize
            );

    description:

        This function is called to retrieve string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pUlSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
ULONG
DeviceInfo_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDI;
    char DL_Status[128]={0};
    char Protocol[16]={0};
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_FirmwareDownloadStatus") == 0)
    {
    	 if(pMyObject->Download_Control_Flag)
    	 {
	        /* collect value */
	        CosaDmlDIGetDLStatus((ANSC_HANDLE)pMyObject, DL_Status);
	        if ( _ansc_strlen(DL_Status) >= *pUlSize )
	        {
	            *pUlSize = _ansc_strlen(DL_Status);
	            return 1;
	        }
	        
	        AnscCopyString(pValue, DL_Status);
    	 }
        return 0;
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_FirmwareDownloadProtocol") == 0)
    {
    	 if(pMyObject->Download_Control_Flag)
    	 {
	        /* collect value */
	        CosaDmlDIGetProtocol( Protocol);
	        if ( _ansc_strlen(Protocol) >= *pUlSize )
	        {
	            *pUlSize = _ansc_strlen(Protocol);
	            return 1;
	        }
	        
	        AnscCopyString(pValue, Protocol);
    	 }
        return 0;
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_FirmwareDownloadURL") == 0)
    {
    	 if(pMyObject->Download_Control_Flag)
    	 {
                //sometimes syscfg db init not done during bootup call of CosaDmlDIInit.So recalling CosaDmlDIGetURL
                if(_ansc_strlen(pMyObject->DownloadURL) == 0)
                    CosaDmlDIGetURL((ANSC_HANDLE)pMyObject);
	        /* collect value */
	        if ( _ansc_strlen(pMyObject->DownloadURL) >= *pUlSize )
	        {
	            *pUlSize = _ansc_strlen(pMyObject->DownloadURL);
	            return 1;
	        }
	        
	        AnscCopyString(pValue, pMyObject->DownloadURL);
    	 }
        return 0;
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_FirmwareToDownload") == 0)
    {
    	 if(pMyObject->Download_Control_Flag)
    	 {
                //sometimes syscfg db init not done during bootup call of CosaDmlDIInit.So recalling CosaDmlDIGetURL
                if(_ansc_strlen(pMyObject->Firmware_To_Download) == 0)
                    CosaDmlDIGetImage((ANSC_HANDLE)pMyObject); 
	        /* collect value */
	        if ( _ansc_strlen(pMyObject->Firmware_To_Download) >= *pUlSize )
	        {
	            *pUlSize = _ansc_strlen(pMyObject->Firmware_To_Download);
	            return 1;
	        }
	        
	        AnscCopyString(pValue, pMyObject->Firmware_To_Download);
    	 }
        return 0;
    }
    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            );

    description:

        This function is called to set BOOL parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DeviceInfo_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDI;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_FirmwareDownloadNow") == 0)
    {
    	 if(pMyObject->Download_Control_Flag)
    	 {
    	 	if(bValue)
    	 	{
        		CosaDmlDIDownloadNow((ANSC_HANDLE)pMyObject);
    	 	}	
    	 }
	 else
    	 {
		CcspTraceError(("FW DL is not allowed for this image type \n"));
    	 }
        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DeviceInfo_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDI;

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "X_RDKCENTRAL-COM_FirmwareDownloadURL") == 0)
    {
    	 if(pMyObject->Download_Control_Flag)
    	 {
        	CosaDmlDISetURL((ANSC_HANDLE)pMyObject, pString);
    	 }
	 else
    	 {
		CcspTraceError(("FW DL is not allowed for this image type \n"));
    	 }
        return TRUE;
    }

    if (strcmp(ParamName, "X_RDKCENTRAL-COM_FirmwareToDownload") == 0)
    {
    	 if(pMyObject->Download_Control_Flag)
    	 {
        	CosaDmlDISetImage((ANSC_HANDLE)pMyObject, pString);
    	 }
	 else
    	 {
		CcspTraceError(("FW DL is not allowed for this image type \n"));
    	 }    	 
        return TRUE;
    }
    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
