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

/**
 * description: This file defines the apis for objects to support Data Model Library.
 */

#include "slap_definitions.h"

BOOL
DPoE_GetParamUlongValue
    (
		ANSC_HANDLE                 hInsContext,
		char*                       ParamName,
		ULONG*                      puLong
    );

BOOL
DPoE_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    );

BOOL
DPoE_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

BOOL
DPoE_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );
BOOL
DPoE_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
DPoE_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
DPoE_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        INT*                        iValue
    );

BOOL
DPoE_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        INT                        iValue
    );

BOOL
DPoE_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
DPoE_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
DPoE_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL DPoE_FirmwareInfo_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
);

BOOL DPoE_ChipInfo_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
);


BOOL DPoE_ManufacturerInfo_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
);

BOOL DPoE_DeviceSysDescrInfo_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
);

BOOL DPoE_OnuPacketBufferCapabilities_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
);

ULONG DPoE_LlidForwardingState_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
);

ANSC_HANDLE DPoE_LlidForwardingState_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
);

BOOL DPoE_LlidForwardingState_GetParamBoolValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    BOOL*                       pBool
);

BOOL DPoE_LlidForwardingState_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
);

ULONG DPoE_OamFrameRate_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
);

ANSC_HANDLE DPoE_OamFrameRate_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
);

BOOL DPoE_OamFrameRate_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
);

ULONG DPoE_DynamicMacTable_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
);

ANSC_HANDLE DPoE_DynamicMacTable_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
);

BOOL DPoE_DynamicMacTable_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
);

BOOL DPoE_DynamicMacTable_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
);

ULONG DPoE_StaticMacTable_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
);

ANSC_HANDLE DPoE_StaticMacTable_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
);

BOOL DPoE_StaticMacTable_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
);

BOOL DPoE_StaticMacTable_GetParamStringValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    char*                       pValue,
    ULONG*                      pUlSize
);

ULONG DPoE_OnuLinkStatistics_GetEntryCount
(
    ANSC_HANDLE                 hInsContext
);

ANSC_HANDLE DPoE_OnuLinkStatistics_GetEntry
(
    ANSC_HANDLE                 hInsContext,
    ULONG                       nIndex,
    ULONG*                      pInsNumber
);

BOOL DPoE_OnuLinkStatistics_GetParamUlongValue
(
    ANSC_HANDLE                 hInsContext,
    char*                       ParamName,
    ULONG*                      puLong
);









