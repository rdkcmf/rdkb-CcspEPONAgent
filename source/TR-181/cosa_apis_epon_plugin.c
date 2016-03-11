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

static ULONG                           i               = 0;

#ifndef EPONAGENTLOG
#define INFO  0
#define WARNING  1
#define ERROR 2
#define EPONAGENTLOG(x, ...) {fprintf(stderr, "EponAgentLog<%s:%d> ", __FUNCTION__, __LINE__);fprintf(stderr, __VA_ARGS__);}
#endif

BOOL
DPoE_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    EPONAGENTLOG(INFO, "ParamName is: %s\n", ParamName)
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "NumberOfNetworkPorts", TRUE) )
    {
        //return dpoe_getNumberOfNetworkPorts(puLong);
    }
    else if( AnscEqualString(ParamName, "NumberOfS1Interfaces", TRUE) )
    {
        //return dpoe_getNumberOfS1Interfaces( puLong );
    }
    else if( AnscEqualString(ParamName, "DynamicMacAddressAgeLimit", TRUE) )
    {
        //return dpoe_getDynamicMacAddressAgeLimit( puLong );
    }
    else if( AnscEqualString(ParamName, "DynamicMacLearningTableSize", TRUE) )
    {
        //return dpoe_getDynamicMacLearningTableSize( puLong );
    }
    else if( AnscEqualString(ParamName, "MacLearningAggregateLimit", TRUE) )
    {
        //return dpoe_getMacLearningAggregateLimit( puLong );
    }

    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return FALSE;
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

    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
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
    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
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
EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Mac_address", TRUE) )
    {

    }

    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return FALSE;
}


BOOL
DPoE_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return FALSE;
}
BOOL
DPoE_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "ResetOnu", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "ClearOnuLinkStatistics", TRUE) )
    {

    }
EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
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
    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
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
    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
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
    BOOL status = TRUE;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "bootVersion", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "appVersion", TRUE) )
    {

    }
    else
    {
        AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
        status = FALSE;
    }

    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return status;
}

BOOL DPoE_FirmwareInfo_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      pLong
)
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "bootCrc32", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "appCrc32", TRUE) )
    {

    }
    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

BOOL DPoE_ChipInfo_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
)
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "chipModel", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "chipVersion", TRUE) )
    {

    }
    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

BOOL DPoE_ChipInfo_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      pLong
)
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "jedecId", TRUE) )
    {

    }

    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

BOOL DPoE_ManufacturerInfo_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
)
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "info", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "organizationName", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "manufacturerDate", TRUE) )
    {

    }

    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

BOOL DPoE_DeviceSysDescrInfo_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
)
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "vendorName", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "modelNumber", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "hardwareVersion", TRUE) )
    {

    }
    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

BOOL DPoE_OnuPacketBufferCapabilities_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
)
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "upstreamQueues", TRUE) )
    {
    }
    else if( AnscEqualString(ParamName, "upQueuesMaxPerLink", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "upQueueIncrement", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "downstreamQueues", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "dnQueuesMaxPerPort", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "dnQueueIncrement", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "totalPacketBuffer", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "dnPacketBuffer", TRUE) )
    {

    }
    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

ULONG DPoE_LlidForwardingState_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
)
{
    AnscTraceWarning(("Unsupported parameter \n"));
    return 0;
}

ANSC_HANDLE DPoE_LlidForwardingState_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    AnscTraceWarning(("Unsupported parameter \n"));
    return NULL;
}

BOOL DPoE_LlidForwardingState_GetParamBoolValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    BOOL*                       pBool
)
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "forwardingState", TRUE) )
    {

    }
    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

BOOL DPoE_LlidForwardingState_GetParamIntValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    INT*                        iValue
)
{
    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

ULONG DPoE_OamFrameRate_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
)
{
    AnscTraceWarning(("Unsupported parameter \n"));
    return 0;
}

ANSC_HANDLE DPoE_OamFrameRate_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    AnscTraceWarning(("Unsupported parameter \n"));
    return NULL;
}

BOOL DPoE_OamFrameRate_GetParamIntValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    INT*                        iValue
)
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "maxRate", TRUE) )
    {

    }

    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

ULONG DPoE_DynamicMacTable_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
)
{
    AnscTraceWarning(("Unsupported parameter \n"));
    return 0;
}

ANSC_HANDLE DPoE_DynamicMacTable_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    AnscTraceWarning(("Unsupported parameter \n"));
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
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "macAddress", TRUE) )
    {

    }

    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

BOOL DPoE_DynamicMacTable_GetParamIntValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    INT*                        iValue
)
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "link", TRUE) )
    {

    }

    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

ULONG DPoE_StaticMacTable_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
)
{
    AnscTraceWarning(("Unsupported parameter \n"));
    return 0;
}

ANSC_HANDLE DPoE_StaticMacTable_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    AnscTraceWarning(("Unsupported parameter \n"));
    return NULL;
}

BOOL DPoE_StaticMacTable_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
)
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "link", TRUE) )
    {

    }

    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

BOOL DPoE_StaticMacTable_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
)
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "macAddress", TRUE) )
    {

    }

    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

ULONG DPoE_OnuLinkStatistics_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
)
{
    AnscTraceWarning(("Unsupported parameter \n"));
    return 0;
}

ANSC_HANDLE DPoE_OnuLinkStatistics_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
)
{
    AnscTraceWarning(("Unsupported parameter \n"));
    return NULL;
}

BOOL DPoE_OnuLinkStatistics_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
)
{
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "rxUnicastFrames", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "txUnicastFrames", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "rxFrameTooShort", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "rxFrame64", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "rxFrame65_127", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "rxFrame128_255", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "rxFrame256_511", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "rxFrame512_1023", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "rxFrame1024_1518", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "rxFrame1519_Plus", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "txFrame64", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "txFrame65_127", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "txFrame128_255", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "txFrame256_511", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "txFrame512_1023", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "txFrame_1024_1518", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "txFrame_1519_Plus", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "framesDropped", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "bytesDropped", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "opticalMonVcc", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "opticalMonTxBiasCurrent", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "opticalMonTxPower", TRUE) )
    {

    }
    else if( AnscEqualString(ParamName, "opticalMonRxPower", TRUE) )
    {

    }
    AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName));
    return FALSE;
}

