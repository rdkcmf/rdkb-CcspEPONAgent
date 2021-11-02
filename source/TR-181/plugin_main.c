/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
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

/***********************************************************************

    module: plugin_main.c

        Implement COSA Data Model Library Init and Unload apis.

    ---------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    ---------------------------------------------------------------

    revision:

        09/28/2011    initial revision.

**********************************************************************/

#include "ansc_platform.h"
#include "ansc_load_library.h"
#include "cosa_plugin_api.h"
#include "plugin_main.h"
#include "plugin_main_apis.h"
#include "cosa_apis_epon_plugin.h"
#include "cosa_device_info_dml.h"

#define THIS_PLUGIN_VERSION                         1

PCOSA_BACKEND_MANAGER_OBJECT g_pCosaBEManager = NULL;

int ANSC_EXPORT_API
COSA_Init
    (
        ULONG                       uMaxVersionSupported,
        void*                       hCosaPlugInfo         /* PCOSA_PLUGIN_INFO passed in by the caller */
    )
{
    PCOSA_PLUGIN_INFO               pPlugInfo  = (PCOSA_PLUGIN_INFO)hCosaPlugInfo;

    EPONAGENTLOG(INFO, "Entering into <%s>\n", __FUNCTION__)

    if ( uMaxVersionSupported < THIS_PLUGIN_VERSION )
    {
        EPONAGENTLOG(INFO, "%lu version not supported\n", uMaxVersionSupported)
      /* this version is not supported */
        return -1;
    }

    EPONAGENTLOG(INFO, "plugin version is %u\n", THIS_PLUGIN_VERSION)

    pPlugInfo->uPluginVersion       = THIS_PLUGIN_VERSION;

    EPONAGENTLOG(INFO, "registering the back-end apis for the data model\n")

    /* register the back-end apis for the data model */
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_GetParamBoolValue",  DPoE_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_GetParamUlongValue", DPoE_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_GetParamIntValue",   DPoE_GetParamIntValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_GetParamStringValue", DPoE_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_SetParamBoolValue",  DPoE_SetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_SetParamUlongValue", DPoE_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_SetParamIntValue",  DPoE_SetParamIntValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_SetParamStringValue", DPoE_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_FirmwareInfo_GetParamStringValue", DPoE_FirmwareInfo_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_ChipInfo_GetParamStringValue", DPoE_ChipInfo_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_ManufacturerInfo_GetParamStringValue", DPoE_ManufacturerInfo_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_DeviceSysDescrInfo_GetParamStringValue", DPoE_DeviceSysDescrInfo_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_OnuPacketBufferCapabilities_GetParamUlongValue", DPoE_OnuPacketBufferCapabilities_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_LlidForwardingState_GetEntryCount", DPoE_LlidForwardingState_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_LlidForwardingState_GetEntry", DPoE_LlidForwardingState_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_LlidForwardingState_GetParamBoolValue", DPoE_LlidForwardingState_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_LlidForwardingState_GetParamUlongValue", DPoE_LlidForwardingState_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_OamFrameRate_GetEntryCount", DPoE_OamFrameRate_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_OamFrameRate_GetEntry", DPoE_OamFrameRate_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_OamFrameRate_GetParamUlongValue", DPoE_OamFrameRate_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_DynamicMacTable_GetEntryCount", DPoE_DynamicMacTable_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_DynamicMacTable_GetEntry", DPoE_DynamicMacTable_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_DynamicMacTable_GetParamStringValue", DPoE_DynamicMacTable_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_DynamicMacTable_GetParamUlongValue", DPoE_DynamicMacTable_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_StaticMacTable_GetEntryCount", DPoE_StaticMacTable_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_StaticMacTable_GetEntry", DPoE_StaticMacTable_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_StaticMacTable_GetParamUlongValue", DPoE_StaticMacTable_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_StaticMacTable_GetParamStringValue", DPoE_StaticMacTable_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_OnuLinkStatistics_GetEntryCount", DPoE_OnuLinkStatistics_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_OnuLinkStatistics_GetEntry", DPoE_OnuLinkStatistics_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DPoE_OnuLinkStatistics_GetParamUlongValue", DPoE_OnuLinkStatistics_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DeviceInfo_GetParamBoolValue",  DeviceInfo_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DeviceInfo_GetParamStringValue",  DeviceInfo_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DeviceInfo_SetParamBoolValue",  DeviceInfo_SetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "DeviceInfo_SetParamStringValue",  DeviceInfo_SetParamStringValue);


    /* Create backend framework */
    g_pCosaBEManager = (PCOSA_BACKEND_MANAGER_OBJECT)CosaBackEndManagerCreate();

    if ( g_pCosaBEManager && g_pCosaBEManager->Initialize )
    {
        g_pCosaBEManager->hCosaPluginInfo = pPlugInfo;

        g_pCosaBEManager->Initialize   ((ANSC_HANDLE)g_pCosaBEManager);
    }

    EPONAGENTLOG(INFO, "Exiting from <%s>\n", __FUNCTION__)
    return  0;
}

BOOL ANSC_EXPORT_API
COSA_IsObjectSupported
    (
        char*                        pObjName
    )
{
    UNREFERENCED_PARAMETER(pObjName);
    return TRUE;
}

void ANSC_EXPORT_API
COSA_Unload
    (
        void
    )
{
    /* unload the memory here */
}

void ANSC_EXPORT_API
COSA_MemoryCheck
    (
        void
    )
{

}

void ANSC_EXPORT_API
COSA_MemoryUsage
    (
        void
    )
{
    /*AnscTraceMemoryUsage();*/
}

void ANSC_EXPORT_API
COSA_MemoryTable
    (
        void
    )
{
    /*CcspTraceMemoryTable();*/
}
