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

#include "ansc_platform.h"
#include "cosa_apis_epon_plugin.h"
#include "ccsp_trace.h"
#include "dpoe_hal.h"
//#include "pace_serialization_data.h"
#include "stdio.h"

#define MAX_DPOE_DATA_ENTRY  32
#define REMOVE_EXTRA_CRLF

static ULONG i = 0;
static dpoe_onu_packet_buffer_capabilities_t    Capabilities;
static BOOL bOnu_packet_buffer_capabilities_fetched = FALSE;
static dpoe_device_sys_descr_info_t             DeviceSysDescrInfo;
static BOOL bDevice_sys_descr_info_fetched = FALSE;
static dpoe_manufacturer_t                      ManufacturerInfo;
static BOOL bManufacturer_fetched = FALSE;
static dpoe_epon_chip_info_t                    EponChipInfo;
static BOOL bEpon_chip_info_fetched = FALSE;
static dpoe_firmware_info_t                     FirmwareInfo;
static BOOL bFirmware_info_fetched = FALSE;
static dpoe_link_traffic_stats_t    onuLinkTrafficStats[MAX_DPOE_DATA_ENTRY];
static dpoe_link_mac_address_t    linkStaticMacTable[MAX_DPOE_DATA_ENTRY];
static dpoe_link_mac_address_t    linkDynamicMacTable[MAX_DPOE_DATA_ENTRY];
static dpoe_link_oam_frame_rate_t    linkOamFrameRate[MAX_DPOE_DATA_ENTRY];
static dpoe_link_forwarding_state_t linkForwardingState[MAX_DPOE_DATA_ENTRY];

#define INFO  0
#define WARNING  1
#define ERROR 2

#ifdef FEATURE_SUPPORT_RDKLOG
#define EPONAGENTLOG(x, ...) { if((x)==(INFO)){CcspTraceInfo((__VA_ARGS__));}else if((x)==(WARNING)){CcspTraceWarning((__VA_ARGS__));}else if((x)==(ERROR)){CcspTraceError((__VA_ARGS__));} }
#else
#define EPONAGENTLOG(x, ...) {fprintf(stderr, "EponAgentLog<%s:%d> ", __FUNCTION__, __LINE__);fprintf(stderr, __VA_ARGS__);}
#endif

#define SUCCESS 0

#define FAILURE -1

#define DPOE_MAC_ADDRESS_LENGTH 6

#if 1
// local functions to be replaced by Broadcom's
int dpoe_DynamicMacTableGetEntryCount( USHORT *pNumEntries ) {*pNumEntries = 1; return RETURN_OK;};
int dpoe_StaticMacTableGetEntryCount( USHORT *pNumEntries ) {*pNumEntries = 5; return RETURN_OK;};
#endif

//  This function should be called by scp_main to prefetch all the static DPoE parameters.
//  This will make the dmcli operation a lot quicker.
BOOL
DPoE_Init_Parms( void )
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)

    if ( dpoe_getFirmwareInfo( &FirmwareInfo ) == RETURN_OK )
    {
        bFirmware_info_fetched = TRUE;
    }

    if ( dpoe_getEponChipInfo( &EponChipInfo ) == RETURN_OK )
    {
        bEpon_chip_info_fetched = TRUE;
    }

    if ( dpoe_getManufacturerInfo(&ManufacturerInfo) == RETURN_OK )
    {
        bManufacturer_fetched = TRUE;
    }

    if ( dpoe_getDeviceSysDescrInfo(&DeviceSysDescrInfo) == RETURN_OK )
    {
        bDevice_sys_descr_info_fetched = TRUE;
    }

    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return TRUE;
}

BOOL
DPoE_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    BOOL status = TRUE;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "NumberOfNetworkPorts", TRUE) )
    {
        if( dpoe_getNumberOfNetworkPorts(puLong) == FAILURE)
        {
            EPONAGENTLOG(ERROR, "dpoe_getNumberOfNetworkPorts returned with FAILURE\n")
            status = FALSE;
        }
        else
            EPONAGENTLOG(INFO, "dpoe_getNumberOfNetworkPorts returned with SUCCESS\n")
    }
    else if( AnscEqualString(ParamName, "NumberOfS1Interfaces", TRUE) )
    {
        if( dpoe_getNumberOfS1Interfaces(puLong) == FAILURE)
        {
            EPONAGENTLOG(ERROR, "dpoe_getNumberOfS1Interfaces returned with FAILURE\n")
            status = FALSE;
        }
        else
            EPONAGENTLOG(INFO, "dpoe_getNumberOfS1Interfaces returned with SUCCESS\n")
    }
    else if( AnscEqualString(ParamName, "DynamicMacAddressAgeLimit", TRUE) )
    {
        USHORT value;
        if( dpoe_getDynamicMacAddressAgeLimit(&value) == FAILURE)
        {
            EPONAGENTLOG(ERROR, "dpoe_getDynamicMacAddressAgeLimit returned with FAILURE\n")
            status = FALSE;
        }
        else
        {
            *puLong = (ULONG)value;
            EPONAGENTLOG(INFO, "dpoe_getDynamicMacAddressAgeLimit returned with SUCCESS\n")
        }
    }
    else if( AnscEqualString(ParamName, "DynamicMacLearningTableSize", TRUE) )
    {
        USHORT value;
        if( dpoe_getDynamicMacLearningTableSize(&value) == FAILURE)
        {
            EPONAGENTLOG(ERROR, "dpoe_getDynamicMacLearningTableSize returned with FAILURE\n")
            status = FALSE;
        }
        else
        {
            *puLong = (ULONG)value;
            EPONAGENTLOG(INFO, "dpoe_getDynamicMacLearningTableSize returned with SUCCESS\n")
        }
    }
    else if( AnscEqualString(ParamName, "MacLearningAggregateLimit", TRUE) )
    {
        USHORT value;
        if( dpoe_getMacLearningAggregateLimit(&value) == FAILURE)
        {
            EPONAGENTLOG(ERROR, "dpoe_getMacLearningAggregateLimit returned with FAILURE\n")
            status = FALSE;
        }
        else
        {
            *puLong = (ULONG)value;
            EPONAGENTLOG(INFO, "dpoe_getMacLearningAggregateLimit returned with SUCCESS\n")
        }
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter=%s\n", __FUNCTION__, __LINE__, ParamName));
        status = FALSE;
    }


    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return status;
}

BOOL
DPoE_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)

    AnscTraceWarning(("<%s:%d> Unsupported parameter=%s\n", __FUNCTION__, __LINE__, ParamName));
    EPONAGENTLOG(INFO, "ParamName is: %s\n", ParamName)

    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return FALSE;
}


BOOL
DPoE_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    AnscTraceWarning(("<%s:%d> Unsupported parameter=%s\n", __FUNCTION__, __LINE__, ParamName));
    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return FALSE;
}


BOOL
DPoE_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    BOOL status = TRUE;
    dpoe_mac_address_t tDpoe_Mac;
    char macAddress[18] = {0};

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Mac_address", TRUE) )
    {
        if( dpoe_getOnuId(&tDpoe_Mac) == FAILURE)
        {
            EPONAGENTLOG(ERROR, "dpoe_getOnuId returned with FAILURE\n")
            status = FALSE;
        }
        else
        {
            EPONAGENTLOG(INFO, "dpoe_getOnuId returned with SUCCESS\n")

            sprintf(macAddress, "%02x:%02x:%02x:%02x:%02x:%02x",tDpoe_Mac.macAddress[0], tDpoe_Mac.macAddress[1],
                tDpoe_Mac.macAddress[2], tDpoe_Mac.macAddress[3], tDpoe_Mac.macAddress[4],tDpoe_Mac.macAddress[5]);
            strcpy( pValue, macAddress);
            *pUlSize = strlen(pValue);

            EPONAGENTLOG(INFO, "tDpoe_Mac.macAddress=%u macAddress=%s pValue=%s *pUlSize=%d \n", tDpoe_Mac.macAddress, macAddress, pValue, *pUlSize)
        }
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter=%s\n", __FUNCTION__, __LINE__, ParamName));
        status = FALSE;
    }

    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return ( (status == TRUE) ? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE );
}


BOOL
DPoE_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    BOOL status = FALSE;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "ResetOnu", TRUE) )
    {
        status = TRUE;
    }
    else if( AnscEqualString(ParamName, "ClearOnuLinkStatistics", TRUE) )
    {
        status = TRUE;
    }
    else
    {
        EPONAGENTLOG(WARNING, "<%s:%d>Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName)
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return status;
}
BOOL
DPoE_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    BOOL status = TRUE;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "ResetOnu", TRUE) )
    {
        if ( dpoe_setResetOnu() == FAILURE)
        {
            EPONAGENTLOG(ERROR, "dpoe_setResetOnu returned with FAILURE\n")
            status = FALSE;
        }
        else
            EPONAGENTLOG(INFO, "dpoe_setResetOnu returned with SUCCESS\n")
    }
    else if( AnscEqualString(ParamName, "ClearOnuLinkStatistics", TRUE) )
    {
        if ( dpoe_setClearOnuLinkStatistics() == FAILURE)
        {
            EPONAGENTLOG(ERROR, "dpoe_setClearOnuLinkStatistics returned with FAILURE\n")
            status = FALSE;
        }
        else
        {
            bOnu_packet_buffer_capabilities_fetched = FALSE;
            bFirmware_info_fetched = FALSE;
            EPONAGENTLOG(INFO, "dpoe_setClearOnuLinkStatistics returned with SUCCESS\n")
        }
    }
    else
    {
        EPONAGENTLOG(WARNING, "<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName)
        status = FALSE;
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return status;
}

BOOL
DPoE_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        INT*                        iValue
    )
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return FALSE;
}

BOOL
DPoE_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        INT                         iValue
    )
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        DPoE_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
DPoE_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return 0;
}

BOOL
DPoE_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return TRUE;
}

ULONG
DPoE_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return 0;
}

BOOL DPoE_FirmwareInfo_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
)
{
    BOOL status = RETURN_OK;
    *pValue = (char)0;
    *pUlSize = 0L;
    if ( !bFirmware_info_fetched )
    {
        status = dpoe_getFirmwareInfo( &FirmwareInfo );
        bFirmware_info_fetched = TRUE;
    }

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "bootVersion", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            char buffer[64] = {0};
            sprintf(buffer, "%X", FirmwareInfo.info_bootVersion);
            *pUlSize = strlen( buffer );
            strncpy( pValue, buffer, *pUlSize );
        }
    }
    else if( AnscEqualString(ParamName, "appVersion", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            char buffer[64] = {0};
            sprintf(buffer, "%X", FirmwareInfo.info_appVersion);
            *pUlSize = strlen( buffer );
            strncpy( pValue, buffer, *pUlSize );
        }
    }
    else if( AnscEqualString(ParamName, "bootCrc32", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            char buffer[64] = {0};
            sprintf(buffer, "%X", FirmwareInfo.info_bootCrc32);
            *pUlSize = strlen( buffer );
            strncpy( pValue, buffer, *pUlSize );
        }
    }
    else if( AnscEqualString(ParamName, "appCrc32", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            char buffer[64] = {0};
            sprintf(buffer, "%X", FirmwareInfo.info_appCrc32);
            *pUlSize = strlen( buffer );
            strncpy( pValue, buffer, *pUlSize );
        }
    }
    else
    {
        AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
        status = FALSE;
    }

    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return ( (status == RETURN_OK) ? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE );
}

BOOL DPoE_ChipInfo_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
)
{
    BOOL status = RETURN_OK;
    *pValue = (char)0;
    *pUlSize = 0L;
    if ( !bEpon_chip_info_fetched )
    {
        status = dpoe_getEponChipInfo( &EponChipInfo );
        bEpon_chip_info_fetched = TRUE;
    }

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "chipModel", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            char buffer[64] = {0};
            sprintf(buffer, "%X", EponChipInfo.info_ChipModel);
            *pUlSize = strlen( buffer );
            strncpy( pValue, buffer, *pUlSize );
        }
    }
    else if( AnscEqualString(ParamName, "chipVersion", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            char buffer[64] = {0};
            sprintf(buffer, "%X", EponChipInfo.info_ChipVersion);
            *pUlSize = strlen( buffer );
            strncpy( pValue, buffer, *pUlSize );
        }
    }    
    else if( AnscEqualString(ParamName, "jedecId", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            char buffer[64] = {0};
            sprintf(buffer, "%X", EponChipInfo.info_JedecId);
            *pUlSize = strlen( buffer );
            strncpy( pValue, buffer, *pUlSize );
        }
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
        status = FALSE;
    }

    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return ( (status == RETURN_OK) ? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE );
}

BOOL DPoE_ManufacturerInfo_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
)
{
    BOOL status = RETURN_OK;
    *pValue = (char)0;
    *pUlSize = 0L;
    char HwRev[64] = {0};
    char Model[64] = {0};
    char ManufactureDate[64] = {0};

    if ( !bManufacturer_fetched )
    {
        status = dpoe_getManufacturerInfo(&ManufacturerInfo);
        bManufacturer_fetched = TRUE;
    }

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

#if 1    // should be replaced with get_serialization_info
    {
        int ret = RETURN_OK;
        FILE *fp;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        char lineStr[255];

        line = lineStr;
        fp = fopen("/nvram/serialization.txt", "r");
        if (fp == NULL)
        {
    	    fp = fopen("/tmp/data/serialization.txt", "r");
        }

        if (fp != NULL)
        {
            while((read = getline(&line, &len, fp)) != -1)
            {
                char *str = line;
    	        char *found, *remaining;
    	        char *del = "=";

    	        found = strtok_r(str, del, &remaining);
    	        if (found != NULL)
	        {
    	            int tmpLen = strlen(remaining);

    	            if (tmpLen > 128)
                    {
                        tmpLen = 128;
                    }

                    if (tmpLen > 0)
                    {
                        tmpLen --;
                    }

                    if (strcmp(found, "HW_REV") == 0)
                    {
                        strncpy(HwRev, remaining, tmpLen);
                    }
                    else if (strcmp(found, "MODEL") == 0)
                    {
                        strncpy(Model, remaining, tmpLen);
                    }
                    else if (strcmp(found, "MANUFACTURE_DATE") == 0)
                    {
                        strncpy(ManufactureDate, remaining, tmpLen);
                    }
                }
            }
            fclose(fp);
        }
    }
#endif

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "info", TRUE) )
    {
        char buffer[256] = {0};
        int iLen = 0;
//        status = get_serialization_info( ENUM_MODEL, buffer );
        strcpy( buffer, Model );
        if ( status == RETURN_OK )
        {
            iLen = strlen( buffer );
            buffer[iLen] = ' ';
//            status = get_serialization_info( ENUM_HW_REV, buffer+iLen+1 );
            strcpy( buffer+iLen+1, HwRev);
        }

        if ( status == RETURN_OK )
        {
            *pUlSize = strlen( buffer );
            strncpy( pValue, buffer, *pUlSize );
        }
    }
    else if( AnscEqualString(ParamName, "organizationName", TRUE) )
    {
        status = dpoe_getDeviceSysDescrInfo(&DeviceSysDescrInfo);
        if ( status == RETURN_OK )
        {
            char *p = (char *)&DeviceSysDescrInfo.info_VendorName;
            *pUlSize = strlen( p );

#if defined( REMOVE_EXTRA_CRLF )
            int j = *pUlSize;

            while (j)
            {
                if ( (*p == 0x0d) || (*p == 0x0a) )
                {
                    *pUlSize = (p - (char *)&DeviceSysDescrInfo.info_VendorName);
                    break;
                }
                p++;
                j--;
            }
#endif
            strncpy( pValue, (char *)&DeviceSysDescrInfo.info_VendorName, *pUlSize );
        }
    }
    else if( AnscEqualString(ParamName, "manufacturerDate", TRUE) )
    {
        char buffer[256] = {0};

//        status = get_serialization_info( ENUM_MANUFACTURE_DATE, buffer );
        strcpy( buffer, ManufactureDate );
        if ( status == RETURN_OK )
        {
            *pUlSize = strlen( buffer );
            strncpy( pValue, buffer, *pUlSize );
        }
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
        status = FALSE;
    }

    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return ( (status == RETURN_OK) ? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE );
}

BOOL DPoE_DeviceSysDescrInfo_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
)
{
    BOOL status = RETURN_OK;
    *pValue = (char)0;
    *pUlSize = 0L;
    if ( !bDevice_sys_descr_info_fetched )
    {
        status = dpoe_getDeviceSysDescrInfo(&DeviceSysDescrInfo);
        bDevice_sys_descr_info_fetched = TRUE;
    }

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    //check the parameter name and return the corresponding value
    if( AnscEqualString(ParamName, "vendorName", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            char *p = (char *)&DeviceSysDescrInfo.info_VendorName;
            *pUlSize = strlen( p );

#if defined( REMOVE_EXTRA_CRLF )
            int j = *pUlSize;

            while (j)
            {
                if ( (*p == 0x0d) || (*p == 0x0a) )
                {
                    *pUlSize = (p - (char *)&DeviceSysDescrInfo.info_VendorName);
                    break;
                }
                p++;
                j--;
            }
#endif
            strncpy( pValue, (char *)&DeviceSysDescrInfo.info_VendorName, *pUlSize );
        }
    }
    else if( AnscEqualString(ParamName, "modelNumber", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            char *p = (char *)&DeviceSysDescrInfo.info_ModelNumber;
            *pUlSize = strlen( p );

#if defined( REMOVE_EXTRA_CRLF )
            int j = *pUlSize;

            while (j)
            {
                if ( (*p == 0x0d) || (*p == 0x0a) )
                {
                    *pUlSize = (p - (char *)&DeviceSysDescrInfo.info_ModelNumber);
                    break;
                }
                p++;
                j--;
            }
#endif
            strncpy( pValue, (char *)&DeviceSysDescrInfo.info_ModelNumber, *pUlSize );
        }
    }
    else if( AnscEqualString(ParamName, "hardwareVersion", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            char *p = (char *)&DeviceSysDescrInfo.info_HardwareVersion;
            *pUlSize = strlen( p );

#if defined( REMOVE_EXTRA_CRLF )
            int j = *pUlSize;

            while (j)
            {
                if ( (*p == 0x0d) || (*p == 0x0a) )
                {
                    *pUlSize = (p - (char *)&DeviceSysDescrInfo.info_HardwareVersion);
                    break;
                }
                p++;
                j--;
            }
#endif
            strncpy( pValue, (char *)&DeviceSysDescrInfo.info_HardwareVersion, *pUlSize );
        }
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
        status = FALSE;
    }

    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return ( (status == RETURN_OK) ? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE );
}

BOOL DPoE_OnuPacketBufferCapabilities_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
)
{
    BOOL status = RETURN_OK;
    *puLong = 0L;

    if ( !bOnu_packet_buffer_capabilities_fetched )
    {
        status = dpoe_getOnuPacketBufferCapabilities( &Capabilities );
        bOnu_packet_buffer_capabilities_fetched = TRUE;
    }

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "upstreamQueues", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = Capabilities.capabilities_UpstreamQueues;
        }
    }
    else if( AnscEqualString(ParamName, "upQueuesMaxPerLink", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = Capabilities.capabilities_UpQueuesMaxPerLink;
        }
    }
    else if( AnscEqualString(ParamName, "upQueueIncrement", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = Capabilities.capabilities_UpQueueIncrement;
        }
    }
    else if( AnscEqualString(ParamName, "downstreamQueues", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = Capabilities.capabilities_DownstreamQueues;
        }
    }
    else if( AnscEqualString(ParamName, "dnQueuesMaxPerPort", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = Capabilities.capabilities_DnQueuesMaxPerPort;
        }
    }
    else if( AnscEqualString(ParamName, "dnQueueIncrement", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = Capabilities.capabilities_DnQueueIncrement;
        }
    }
    else if( AnscEqualString(ParamName, "totalPacketBuffer", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = Capabilities.capabilities_TotalPacketBuffer;
        }
    }
    else if( AnscEqualString(ParamName, "dnPacketBuffer", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = Capabilities.capabilities_DnPacketBuffer;
        }
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
        status = FALSE;
    }

    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return ( (status == RETURN_OK) ? TRUE : FALSE );
}

ULONG DPoE_LlidForwardingState_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
)
{
    int j;
    USHORT uEntryCount;
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: LlidForwardingState_GetEntryCount\n")

    if ( dpoe_LlidForwardingStateGetEntryCount( &uEntryCount ) != RETURN_OK )
    {
        uEntryCount = 0;
    }

    if ( uEntryCount > MAX_DPOE_DATA_ENTRY )
    {
        uEntryCount = MAX_DPOE_DATA_ENTRY;
    }

    for ( j = 0; j < uEntryCount; j++ )
    {
        linkForwardingState[j].link_Id = j+1;
    }

    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return (ULONG)uEntryCount;
}

ANSC_HANDLE DPoE_LlidForwardingState_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    ANSC_HANDLE *pHandle = (ANSC_HANDLE)NULL;
    USHORT uEntryCount;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: LlidForwardingState_GetEntry %d\n", nIndex);
    
    *pInsNumber = nIndex + 1;
    if ( dpoe_LlidForwardingStateGetEntryCount( &uEntryCount ) != RETURN_OK )
    {
        uEntryCount = 0;
    }

    if ( uEntryCount > MAX_DPOE_DATA_ENTRY )
    {
        uEntryCount = MAX_DPOE_DATA_ENTRY;
    }

    if ( nIndex < uEntryCount )
    {
        dpoe_getLlidForwardingState( &linkForwardingState[nIndex], 1);
        if ( linkForwardingState[nIndex].link_ForwardingState )
        {
            pHandle = (ANSC_HANDLE)&linkForwardingState[nIndex];
        }
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return pHandle;
}

BOOL DPoE_LlidForwardingState_GetParamBoolValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    BOOL*                       pBool
)
{
    BOOL status = TRUE;
    *pBool = FALSE;
    dpoe_link_forwarding_state_t *pLink_forwarding_state = (dpoe_link_forwarding_state_t *)hInsContext;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "forwardingState", TRUE) )
    {
        if ( pLink_forwarding_state )
        {
//            dpoe_getLlidForwardingState( pLink_forwarding_state, 1);
            *pBool = pLink_forwarding_state->link_ForwardingState;
        }
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
        status = FALSE;
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return status;
}

BOOL DPoE_LlidForwardingState_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
)
{
    BOOL status = TRUE;
    *puLong = 0L;
    dpoe_link_forwarding_state_t *pLink_forwarding_state = (dpoe_link_forwarding_state_t *)hInsContext;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "link", TRUE) )
    {
        if ( pLink_forwarding_state )
        {
            dpoe_getLlidForwardingState( pLink_forwarding_state, 1);
            *puLong = pLink_forwarding_state->link_Id;
        }
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
        status = FALSE;
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return status;
}

ULONG DPoE_OamFrameRate_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
)
{
    int j;
    USHORT uEntryCount;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: OamFrameRate_GetEntryCount\n")

    if ( dpoe_OamFrameRateGetEntryCount( &uEntryCount ) != RETURN_OK )
    {
        uEntryCount = 0;
    }

    if ( uEntryCount > MAX_DPOE_DATA_ENTRY )
    {
        uEntryCount = MAX_DPOE_DATA_ENTRY;
    }

    for ( j = 0; j < uEntryCount; j++ )
    {
        linkOamFrameRate[j].link_Id = j+1;
    }

    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return (ULONG)uEntryCount;
}

ANSC_HANDLE DPoE_OamFrameRate_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    ANSC_HANDLE *pHandle = (ANSC_HANDLE)NULL;
    USHORT uEntryCount;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: DPoE_OamFrameRate_GetEntry %d\n", nIndex)

    *pInsNumber = nIndex + 1;

    if ( dpoe_OamFrameRateGetEntryCount( &uEntryCount ) != RETURN_OK )
    {
        uEntryCount = 0;
    }

    if ( uEntryCount > MAX_DPOE_DATA_ENTRY )
    {
        uEntryCount = MAX_DPOE_DATA_ENTRY;
    }

    if ( nIndex < uEntryCount )
    {
        dpoe_getLlidForwardingState( &linkForwardingState[nIndex], 1);
        if ( linkForwardingState[nIndex].link_ForwardingState )
        {
            pHandle = (ANSC_HANDLE)&linkOamFrameRate[nIndex];
        }
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return pHandle;
}

BOOL DPoE_OamFrameRate_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
)
{
    dpoe_link_oam_frame_rate_t    *pLinkOamFrameRate = (dpoe_link_oam_frame_rate_t *)hInsContext;
    BOOL status = RETURN_OK;
    *puLong = 0L;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "link", TRUE) )
    {
        if ( pLinkOamFrameRate )
        {
            dpoe_getOamFrameRate(pLinkOamFrameRate, 1);
            *puLong = pLinkOamFrameRate->link_Id;
        }
    }
    else if( AnscEqualString(ParamName, "maxRate", TRUE) )
    {
        if ( pLinkOamFrameRate )
        {
            dpoe_getOamFrameRate(pLinkOamFrameRate, 1);
            *puLong = pLinkOamFrameRate->link_MaxRate;
        }
    }
    else if( AnscEqualString(ParamName, "minRate", TRUE) )
    {
        if ( pLinkOamFrameRate )
        {
//            dpoe_getOamFrameRate(pLinkOamFrameRate, 1);
            *puLong = pLinkOamFrameRate->link_MinRate;
        }
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
        status = FALSE;
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return ( (status == RETURN_OK) ? TRUE : FALSE );
}

ULONG DPoE_DynamicMacTable_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
)
{
    int j;
    ULONG uEntryCount;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: DynamicMacTable_GetEntryCount\n")

// from Rui's email
//    if ( dpoe_DynamicMacTableGetEntryCount( &uEntryCount ) != RETURN_OK )
    if ( dpoe_getNumberOfS1Interfaces( &uEntryCount ) == RETURN_ERR )
    {
        uEntryCount = 0;
    }

    if ( uEntryCount > MAX_DPOE_DATA_ENTRY )
    {
        uEntryCount = MAX_DPOE_DATA_ENTRY;
    }

    for ( j = 0; j < uEntryCount; j++ )
    {
        linkDynamicMacTable[j].numEntries = j+1;
        linkDynamicMacTable[j].pMacAddress = NULL;
    }

    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return uEntryCount;
}

ANSC_HANDLE DPoE_DynamicMacTable_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    ANSC_HANDLE *pHandle = (ANSC_HANDLE)NULL;
    ULONG uEntryCount;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: DynamicMacTable_GetEntry %d\n", nIndex)

    *pInsNumber = nIndex + 1;

// from Rui's email
//    if ( dpoe_DynamicMacTableGetEntryCount( &uEntryCount ) != RETURN_OK )
    if ( dpoe_getNumberOfS1Interfaces( &uEntryCount ) == RETURN_ERR )
    {
        uEntryCount = 0;
    }

    if ( uEntryCount > MAX_DPOE_DATA_ENTRY )
    {
        uEntryCount = MAX_DPOE_DATA_ENTRY;
    }

    if ( nIndex < uEntryCount )
    {  
        pHandle = (ANSC_HANDLE)&linkDynamicMacTable[nIndex];
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return pHandle;
}

BOOL DPoE_DynamicMacTable_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
)
{
    dpoe_link_mac_address_t    *pLinkDynamicMacTable = (dpoe_link_mac_address_t *)hInsContext;
    BOOL status = ANSC_STATUS_SUCCESS;
    *pValue = (char)0;
    *pUlSize = 0L;
    dpoe_mac_address_t tDpoe_Mac;
    char macAddress[19] = {0};

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "macAddress", TRUE) )
    {
        if ( pLinkDynamicMacTable )
        {
            int i;
            USHORT  linkId = pLinkDynamicMacTable->numEntries;
            char *p = pValue;

            dpoe_getDynamicMacTable(pLinkDynamicMacTable, linkId);
            if ( pLinkDynamicMacTable->pMacAddress )
            {
                dpoe_mac_address_t *p2 = pLinkDynamicMacTable->pMacAddress;
                for ( i = 0; i < pLinkDynamicMacTable->numEntries; i++ )
                {
                    tDpoe_Mac = *p2++;
                    sprintf(p, "%02x:%02x:%02x:%02x:%02x:%02x ",tDpoe_Mac.macAddress[0], tDpoe_Mac.macAddress[1],
                        tDpoe_Mac.macAddress[2], tDpoe_Mac.macAddress[3], tDpoe_Mac.macAddress[4],tDpoe_Mac.macAddress[5]);
                    p += 18;
                }
                free( pLinkDynamicMacTable->pMacAddress );
                pLinkDynamicMacTable->pMacAddress = NULL;
            }
            *pUlSize = strlen(pValue);
            pLinkDynamicMacTable->numEntries = linkId;
        }
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
        status = ANSC_STATUS_FAILURE;
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return status;
}

BOOL DPoE_DynamicMacTable_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
)
{
    dpoe_link_mac_address_t    *pLinkDynamicMacTable = (dpoe_link_mac_address_t *)hInsContext;
    BOOL status = TRUE;
    *puLong = 0L;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "link", TRUE) )
    {
        if ( pLinkDynamicMacTable )
        {
            *puLong = pLinkDynamicMacTable->numEntries;
        }
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
        status = FALSE;
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return status;
}

ULONG DPoE_StaticMacTable_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
)
{
    int j;
    ULONG uEntryCount;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: StaticMacTable_GetEntryCount \n")

// from Rui's email
//    if ( dpoe_StaticMacTableGetEntryCount( &uEntryCount ) != RETURN_OK )
    if ( dpoe_getNumberOfS1Interfaces( &uEntryCount ) == RETURN_ERR )
    {
        uEntryCount = 0;
    }

    if ( uEntryCount > MAX_DPOE_DATA_ENTRY )
    {
        uEntryCount = MAX_DPOE_DATA_ENTRY;
    }

    for ( j = 0; j < uEntryCount; j++ )
    {
        linkStaticMacTable[j].numEntries = j+1;
        linkStaticMacTable[j].pMacAddress = NULL;
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return uEntryCount;
}

ANSC_HANDLE DPoE_StaticMacTable_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    ANSC_HANDLE *pHandle = (ANSC_HANDLE)NULL;
    ULONG uEntryCount;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: StaticMacTable_GetEntry %d\n", nIndex)

    *pInsNumber = nIndex + 1;

// from Rui's email
//    if ( dpoe_StaticMacTableGetEntryCount( &uEntryCount ) != RETURN_OK )
    if ( dpoe_getNumberOfS1Interfaces( &uEntryCount ) == RETURN_ERR )
    {
        uEntryCount = 0;
    }

    if ( uEntryCount > MAX_DPOE_DATA_ENTRY )
    {
        uEntryCount = MAX_DPOE_DATA_ENTRY;
    }

    if ( nIndex < uEntryCount )
    {
        pHandle = (ANSC_HANDLE)&linkStaticMacTable[nIndex];
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return pHandle;
}

BOOL DPoE_StaticMacTable_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
)
{
    dpoe_link_mac_address_t    *pLinkStaticMacTable = (dpoe_link_mac_address_t *)hInsContext;
    BOOL status = TRUE;
    *puLong = 1;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "link", TRUE) )
    {
        if ( pLinkStaticMacTable )
        {
            *puLong = pLinkStaticMacTable->numEntries;
        }
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
        status = FALSE;
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return status;
}

BOOL DPoE_StaticMacTable_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
)
{
    dpoe_link_mac_address_t    *pLinkStaticMacTable = (dpoe_link_mac_address_t *)hInsContext;
    BOOL status = ANSC_STATUS_SUCCESS;
    dpoe_mac_address_t    tDpoe_Mac;
    char macAddress[18] = {0};
    *pValue = (char)0;
    *pUlSize = 0L;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "macAddress", TRUE) )
    {
        if ( pLinkStaticMacTable )
        {
            int i;
            USHORT  linkId = pLinkStaticMacTable->numEntries;
            char *p = pValue;

            dpoe_getStaticMacTable(pLinkStaticMacTable, linkId);
            if ( pLinkStaticMacTable->pMacAddress )
            {
                dpoe_mac_address_t *p2 = pLinkStaticMacTable->pMacAddress;
                for ( i = 0; i < pLinkStaticMacTable->numEntries; i++ )
                {
                    tDpoe_Mac = *p2++;
                    sprintf(p, "%02x:%02x:%02x:%02x:%02x:%02x ",tDpoe_Mac.macAddress[0], tDpoe_Mac.macAddress[1],

                    tDpoe_Mac.macAddress[2], tDpoe_Mac.macAddress[3], tDpoe_Mac.macAddress[4],tDpoe_Mac.macAddress[5]);
                    p += 18;
                }
                free( pLinkStaticMacTable->pMacAddress );
                pLinkStaticMacTable->pMacAddress = NULL;
            }
            *pUlSize = strlen(pValue);
            pLinkStaticMacTable->numEntries = linkId;
        }
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
        status = ANSC_STATUS_FAILURE;
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return status;
}

ULONG DPoE_OnuLinkStatistics_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
)
{
    int j;
    USHORT uEntryCount;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: OnuLinkStatistics_GetEntryCount\n")

    if ( dpoe_OnuLinkStatisticsGetEntryCount(&uEntryCount) != RETURN_OK )
    {
        uEntryCount = 0;
    }

    if ( uEntryCount > MAX_DPOE_DATA_ENTRY )
    {
      uEntryCount = MAX_DPOE_DATA_ENTRY;
    }

    for ( j = 0; j < uEntryCount; j++ )
    {
        onuLinkTrafficStats[j].link_Id = j+1;
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return (ULONG)uEntryCount;
}

ANSC_HANDLE DPoE_OnuLinkStatistics_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    ANSC_HANDLE *pHandle = (ANSC_HANDLE)NULL;
    USHORT uEntryCount;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: OnuLinkStatistics_GetEntry %d\n", nIndex)

    if ( dpoe_OnuLinkStatisticsGetEntryCount( &uEntryCount ) != RETURN_OK )
    {
        uEntryCount = 0;
    }

    if ( uEntryCount > MAX_DPOE_DATA_ENTRY )
    {
        uEntryCount = MAX_DPOE_DATA_ENTRY;
    }

    *pInsNumber = nIndex + 1;
    if ( nIndex < uEntryCount )
    {
        dpoe_getLlidForwardingState( &linkForwardingState[nIndex], 1);
        if ( linkForwardingState[nIndex].link_ForwardingState )
        {
            pHandle = (ANSC_HANDLE)&onuLinkTrafficStats[nIndex];
        }
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return pHandle;
}

BOOL DPoE_OnuLinkStatistics_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
)
{
    dpoe_link_traffic_stats_t    *pOnuLinkTrafficStats = (dpoe_link_traffic_stats_t *)hInsContext;
    BOOL status = TRUE;
    ULONGLONG uLongLong = 0L;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "rxUnicastFrames", TRUE) )
    {
        dpoe_getOnuLinkStatistics(pOnuLinkTrafficStats, 1);
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_RxUnicastFrames;
    }
    else if( AnscEqualString(ParamName, "txUnicastFrames", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_TxUnicastFrames;
    }
    else if( AnscEqualString(ParamName, "rxFrameTooShort", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_RxFrameTooShort;
    }
    else if( AnscEqualString(ParamName, "rxFrame64", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_RxFrame64;
    }
    else if( AnscEqualString(ParamName, "rxFrame65_127", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_RxFrame65_127;
    }
    else if( AnscEqualString(ParamName, "rxFrame128_255", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_RxFrame128_255;
    }
    else if( AnscEqualString(ParamName, "rxFrame256_511", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_RxFrame256_511;
    }
    else if( AnscEqualString(ParamName, "rxFrame512_1023", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_RxFrame512_1023;
    }
    else if( AnscEqualString(ParamName, "rxFrame1024_1518", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_RxFrame1024_1518;
    }
    else if( AnscEqualString(ParamName, "rxFrame1519_Plus", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_RxFrame1519_Plus;
    }
    else if( AnscEqualString(ParamName, "txFrame64", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_TxFrame64;
    }
    else if( AnscEqualString(ParamName, "txFrame65_127", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_TxFrame65_127;
    }
    else if( AnscEqualString(ParamName, "txFrame128_255", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_TxFrame128_255;
    }
    else if( AnscEqualString(ParamName, "txFrame256_511", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_TxFrame256_511;
    }
    else if( AnscEqualString(ParamName, "txFrame512_1023", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_TxFrame512_1023;
    }
    else if( AnscEqualString(ParamName, "txFrame_1024_1518", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_TxFrame_1024_1518;
    }
    else if( AnscEqualString(ParamName, "txFrame_1519_Plus", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_TxFrame_1519_Plus;
    }
    else if( AnscEqualString(ParamName, "framesDropped", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_FramesDropped;
    }
    else if( AnscEqualString(ParamName, "bytesDropped", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_BytesDropped;
    }
    else if( AnscEqualString(ParamName, "opticalMonVcc", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_OpticalMonVcc;
    }
    else if( AnscEqualString(ParamName, "opticalMonTxBiasCurrent", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_OpticalMonTxBiasCurrent;
    }
    else if( AnscEqualString(ParamName, "opticalMonTxPower", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_OpticalMonTxPower;
    }
    else if( AnscEqualString(ParamName, "opticalMonRxPower", TRUE) )
    {
        uLongLong = pOnuLinkTrafficStats->link_TrafficStats.port_OpticalMonRxPower;
    }
    else
    {
        AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
        status = FALSE;
    }

    *puLong = (ULONG)uLongLong;

    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return status;
}

