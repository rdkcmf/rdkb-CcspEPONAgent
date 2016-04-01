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

static dpoe_link_traffic_stats_t    onuLinkTrafficStats[128];

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
        if( dpoe_getDynamicMacAddressAgeLimit(puLong) == FAILURE)
        {
            EPONAGENTLOG(ERROR, "dpoe_getDynamicMacAddressAgeLimit returned with FAILURE\n")
            status = FALSE;
        }
        else
            EPONAGENTLOG(INFO, "dpoe_getDynamicMacAddressAgeLimit returned with SUCCESS\n")
    }
    else if( AnscEqualString(ParamName, "DynamicMacLearningTableSize", TRUE) )
    {
        if( dpoe_getDynamicMacLearningTableSize(puLong) == FAILURE)
        {
            EPONAGENTLOG(ERROR, "dpoe_getDynamicMacLearningTableSize returned with FAILURE\n")
            status = FALSE;
        }
        else
            EPONAGENTLOG(INFO, "dpoe_getDynamicMacLearningTableSize returned with SUCCESS\n")
    }
    else if( AnscEqualString(ParamName, "MacLearningAggregateLimit", TRUE) )
    {
        if( dpoe_getMacLearningAggregateLimit(puLong) == FAILURE)
        {
            EPONAGENTLOG(ERROR, "dpoe_getMacLearningAggregateLimit returned with FAILURE\n")
            status = FALSE;
        }
        else
            EPONAGENTLOG(INFO, "dpoe_getMacLearningAggregateLimit returned with SUCCESS\n")
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
        return dpoe_setClearOnuLinkStatistics();
        if ( dpoe_setClearOnuLinkStatistics() == FAILURE)
        {
            EPONAGENTLOG(ERROR, "dpoe_setClearOnuLinkStatistics returned with FAILURE\n")
            status = FALSE;
        }
        else
            EPONAGENTLOG(INFO, "dpoe_setClearOnuLinkStatistics returned with SUCCESS\n")
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
    return FALSE;
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
    *pValue = NULL;
    *pUlSize = 0L;

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
    else
    {
        AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
        status = FALSE;
    }

    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return ( (status == RETURN_OK) ? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE );
}

BOOL DPoE_FirmwareInfo_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      pLong
)
{
    BOOL status = RETURN_OK;
    *pLong = 0L;
    if ( !bFirmware_info_fetched )
    {
        status = dpoe_getFirmwareInfo( &FirmwareInfo );
        bFirmware_info_fetched = TRUE;
    }

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "bootCrc32", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *pLong = FirmwareInfo.info_bootCrc32;
        }
    }
    else if( AnscEqualString(ParamName, "appCrc32", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *pLong = FirmwareInfo.info_appCrc32;
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

BOOL DPoE_ChipInfo_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
)
{
    BOOL status = RETURN_OK;
    *pValue = NULL;
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
            sprintf(buffer, "%u", EponChipInfo.info_ChipModel);
            *pUlSize = strlen( buffer );
            strncpy( pValue, buffer, *pUlSize );
        }
    }
    else if( AnscEqualString(ParamName, "chipVersion", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            char buffer[64] = {0};
            sprintf(buffer, "%u", EponChipInfo.info_ChipVersion);
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

BOOL DPoE_ChipInfo_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      pLong
)
{
    BOOL status = RETURN_OK;
    *pLong = 0L;
    if ( !bEpon_chip_info_fetched )
    {
        status = dpoe_getEponChipInfo( &EponChipInfo );
        bEpon_chip_info_fetched = TRUE;
    }

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "jedecId", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *pLong = EponChipInfo.info_JedecId;
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

BOOL DPoE_ManufacturerInfo_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
)
{
    BOOL status = RETURN_OK;
    *pValue = NULL;
    *pUlSize = 0L;
    if ( !bManufacturer_fetched )
    {
        status = dpoe_getManufacturerInfo(&ManufacturerInfo);
        bManufacturer_fetched = TRUE;
    }

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "info", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *pUlSize = strlen( ManufacturerInfo.manufacturer_Info );
            strncpy( pValue, ManufacturerInfo.manufacturer_Info, *pUlSize );
        }
    }
    else if( AnscEqualString(ParamName, "organizationName", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *pUlSize = strlen( ManufacturerInfo.manufacturer_OrganizationName );
            strncpy( pValue, ManufacturerInfo.manufacturer_OrganizationName, *pUlSize );
        }
    }
    else if( AnscEqualString(ParamName, "manufacturerDate", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            char buffer[64] = {0};
            char * pMonth[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
            unsigned char mon = ManufacturerInfo.manufacturer_Date.manufacturer_DateMonth;
            if ( mon > 11 )
            {
                mon = 0;
            }
            sprintf(buffer, "%s-%d-%d", pMonth[mon], ManufacturerInfo.manufacturer_Date.manufacturer_DateMonth, ManufacturerInfo.manufacturer_Date.manufacturer_DateDay, ManufacturerInfo.manufacturer_Date.manufacturer_DateYear);
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
    *pValue = NULL;
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
            *pUlSize = strlen( (char *)&DeviceSysDescrInfo.info_VendorName );
            strncpy( pValue, (char *)&DeviceSysDescrInfo.info_VendorName, *pUlSize );
        }
    }
    else if( AnscEqualString(ParamName, "modelNumber", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *pUlSize = strlen( (char *)&DeviceSysDescrInfo.info_ModelNumber );
            strncpy( pValue, (char *)&DeviceSysDescrInfo.info_ModelNumber, *pUlSize );
        }
    }
    else if( AnscEqualString(ParamName, "hardwareVersion", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *pUlSize = strlen( (char *)&DeviceSysDescrInfo.info_HardwareVersion );
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
//            bOnu_packet_buffer_capabilities_fetched = FALSE;   // last entry, reset for next fetch
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
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    AnscTraceWarning(("<%s:%d> Unsupported parameter\n", __FUNCTION__, __LINE__));
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return 1;
}

ANSC_HANDLE DPoE_LlidForwardingState_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    AnscTraceWarning(("<%s:%d> Unsupported parameter nIndex=%d\n", __FUNCTION__, __LINE__, nIndex));
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return NULL;
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

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    AnscTraceWarning(("    ***Unsupported parameter '%s'\n", ParamName));

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "forwardingState", TRUE) )
    {

    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
        status = FALSE;
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return status;
}

BOOL DPoE_LlidForwardingState_GetParamIntValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    INT*                        iValue
)
{
    dpoe_link_forwarding_state_t linkForwardingState[128];
    BOOL status = TRUE;
    *iValue = 0;
    int j;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "forwardingState", TRUE) )
    {
        for ( j = 0; j < 128; j++ )
        {
            linkForwardingState[j].link_Id = j;
            linkForwardingState[j].link_ForwardingState = FALSE;
        }
        status = dpoe_getLlidForwardingState( &linkForwardingState[0], i+1 );
        if ( (status == RETURN_OK) )
        {
           *iValue = linkForwardingState[i].link_ForwardingState ? 1 : 0;
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

ULONG DPoE_OamFrameRate_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
)
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    AnscTraceWarning(("<%s:%d> Unsupported parameter\n", __FUNCTION__, __LINE__));
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return 2;
}

ANSC_HANDLE DPoE_OamFrameRate_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    AnscTraceWarning(("<%s:%d> Unsupported parameter nIndex=%d\n", __FUNCTION__, __LINE__, nIndex));
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return NULL;
}

BOOL DPoE_OamFrameRate_GetParamIntValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    INT*                        iValue
)
{
    dpoe_link_oam_frame_rate_t    linkOamFrameRate[128];
    BOOL status = TRUE;
    *iValue = 0;
    int j;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    for ( j = 0; j < 128; j++ )
    {
        linkOamFrameRate[j].link_Id = j;
    }
    status = dpoe_getOamFrameRate(&linkOamFrameRate[0], i+1);

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "maxRate", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *iValue = linkOamFrameRate[i].link_MaxRate;
        }
    }
    else if( AnscEqualString(ParamName, "minRate", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *iValue = linkOamFrameRate[i].link_MinRate;
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
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    AnscTraceWarning(("<%s:%d> Unsupported parameter\n", __FUNCTION__, __LINE__));
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return 3;
}

ANSC_HANDLE DPoE_DynamicMacTable_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    AnscTraceWarning(("<%s:%d> Unsupported parameter nIndex=%d\n", __FUNCTION__, __LINE__, nIndex));
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return NULL;
}

BOOL DPoE_DynamicMacTable_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
)
{
    dpoe_link_mac_address_t    linkDynamicMacTable[128];
    BOOL status = RETURN_OK;
    *pValue = NULL;
    *pUlSize = 0L;
    dpoe_link_mac_address_t tDpoe_Mac;
    char macAddress[18] = {0};
    int j;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    for ( j = 0; j < 128; j++ )
    {
        linkDynamicMacTable[j].link = j;
    }
    status = dpoe_getDynamicMacTable(&linkDynamicMacTable[0], i+1);
    tDpoe_Mac = linkDynamicMacTable[i];
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "macAddress", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            sprintf(macAddress, "%02x:%02x:%02x:%02x:%02x:%02x",tDpoe_Mac.macAddress[0], tDpoe_Mac.macAddress[1],
                tDpoe_Mac.macAddress[2], tDpoe_Mac.macAddress[3], tDpoe_Mac.macAddress[4],tDpoe_Mac.macAddress[5]);
            strcpy( pValue, macAddress);
            *pUlSize = strlen(pValue);
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

BOOL DPoE_DynamicMacTable_GetParamIntValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    INT*                        iValue
)
{
    BOOL status = TRUE;
    *iValue = 0;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "link", TRUE) )
    {
        *iValue = 1;
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return status;
}

ULONG DPoE_StaticMacTable_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
)
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    AnscTraceWarning(("<%s:%d> Unsupported parameter\n", __FUNCTION__, __LINE__));
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return 4;
}

ANSC_HANDLE DPoE_StaticMacTable_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    AnscTraceWarning(("<%s:%d> Unsupported parameter nIndex=%d\n", __FUNCTION__, __LINE__, nIndex));
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return NULL;
}

BOOL DPoE_StaticMacTable_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
)
{
    BOOL status = TRUE;
    *puLong = 1;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "link", TRUE) )
    {

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
    dpoe_link_mac_address_t    linkStaticMacTable[128];
    BOOL status = RETURN_OK;
    dpoe_link_mac_address_t    tDpoe_Mac;
    char macAddress[18] = {0};
    *pValue = NULL;
    *pUlSize = 0L;
    int j;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    for ( j = 0; j < 128; j++ )
    {
        linkStaticMacTable[j].link = j;
    }
    status = dpoe_getStaticMacTable(&linkStaticMacTable[0], i+1);
    tDpoe_Mac = linkStaticMacTable[i];
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "macAddress", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            sprintf(macAddress, "%02x:%02x:%02x:%02x:%02x:%02x",tDpoe_Mac.macAddress[0], tDpoe_Mac.macAddress[1],
                tDpoe_Mac.macAddress[2], tDpoe_Mac.macAddress[3], tDpoe_Mac.macAddress[4],tDpoe_Mac.macAddress[5]);
            strcpy( pValue, macAddress);
            *pUlSize = strlen(pValue);
        }
    }
    else
    {
        AnscTraceWarning(("<%s:%d> Unsupported parameter %s\n", __FUNCTION__, __LINE__, ParamName));
        status = FALSE;
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return ( (status == RETURN_OK) ? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE );
}

ULONG DPoE_OnuLinkStatistics_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
)
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return 1; //TODO
}

ANSC_HANDLE DPoE_OnuLinkStatistics_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return ((ANSC_HANDLE)&onuLinkTrafficStats[0]);
}

BOOL DPoE_OnuLinkStatistics_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
)
{
    BOOL status = dpoe_getOnuLinkStatistics(&onuLinkTrafficStats[0], i);
    *puLong = 0L;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "    ParamName is: %s\n", ParamName)

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "rxUnicastFrames", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_RxUnicastFrames;
        }
    }
    else if( AnscEqualString(ParamName, "txUnicastFrames", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_TxUnicastFrames;
        }
    }
    else if( AnscEqualString(ParamName, "rxFrameTooShort", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_RxFrameTooShort;
        }
    }
    else if( AnscEqualString(ParamName, "rxFrame64", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_RxFrame64;
        }
    }
    else if( AnscEqualString(ParamName, "rxFrame65_127", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_RxFrame65_127;
        }
    }
    else if( AnscEqualString(ParamName, "rxFrame128_255", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_RxFrame128_255;
        }
    }
    else if( AnscEqualString(ParamName, "rxFrame256_511", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_RxFrame256_511;
        }
    }
    else if( AnscEqualString(ParamName, "rxFrame512_1023", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_RxFrame512_1023;
        }
    }
    else if( AnscEqualString(ParamName, "rxFrame1024_1518", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_RxFrame1024_1518;
        }
    }
    else if( AnscEqualString(ParamName, "rxFrame1519_Plus", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_RxFrame1519_Plus;
        }
    }
    else if( AnscEqualString(ParamName, "txFrame64", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_TxFrame64;
        }
    }
    else if( AnscEqualString(ParamName, "txFrame65_127", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_TxFrame65_127;
        }
    }
    else if( AnscEqualString(ParamName, "txFrame128_255", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_TxFrame128_255;
        }
    }
    else if( AnscEqualString(ParamName, "txFrame256_511", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_TxFrame256_511;
        }
    }
    else if( AnscEqualString(ParamName, "txFrame512_1023", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_TxFrame512_1023;
        }
    }
    else if( AnscEqualString(ParamName, "txFrame_1024_1518", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_TxFrame_1024_1518;
        }
    }
    else if( AnscEqualString(ParamName, "txFrame_1519_Plus", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_TxFrame_1519_Plus;
        }
    }
    else if( AnscEqualString(ParamName, "framesDropped", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_FramesDropped;
        }
    }
    else if( AnscEqualString(ParamName, "bytesDropped", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_BytesDropped;
        }
    }
    else if( AnscEqualString(ParamName, "opticalMonVcc", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_OpticalMonVcc;
        }
    }
    else if( AnscEqualString(ParamName, "opticalMonTxBiasCurrent", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_OpticalMonTxBiasCurrent;
        }
    }
    else if( AnscEqualString(ParamName, "opticalMonTxPower", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_OpticalMonTxPower;
        }
    }
    else if( AnscEqualString(ParamName, "opticalMonRxPower", TRUE) )
    {
        if ( status == RETURN_OK )
        {
            *puLong = onuLinkTrafficStats[i].link_TrafficStats.port_OpticalMonRxPower;
        }
    }
    else
    {
        AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
        status = FALSE;
    }
    EPONAGENTLOG(INFO, "Exiting from <%s>\n\n", __FUNCTION__)
    return ( (status == RETURN_OK) ? TRUE : FALSE );
}

