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
* @file ssp_internal.h
 *
 * @description This is the template file of ssp_internal.h for ccsp component.
 */

#ifndef  _SSP_INTERNAL_H_
#define  _SSP_INTERNAL_H_

/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/

#define  CCSP_COMMON_COMPONENT_HEALTH_Red                   1
#define  CCSP_COMMON_COMPONENT_HEALTH_Yellow                2
#define  CCSP_COMMON_COMPONENT_HEALTH_Green                 3

#define  CCSP_COMMON_COMPONENT_STATE_Initializing           1
#define  CCSP_COMMON_COMPONENT_STATE_Running                2
#define  CCSP_COMMON_COMPONENT_STATE_Blocked                3
#define  CCSP_COMMON_COMPONENT_STATE_Paused                 3

#define  CCSP_COMMON_COMPONENT_FREERESOURCES_PRIORITY_High  1
#define  CCSP_COMMON_COMPONENT_FREERESOURCES_PRIORITY_Low   2

/**
 * @brief Defines component Structure
 */
typedef  struct
_COMPONENT_COMMON_DM
{
    char*                           Name;
    ULONG                           Version;
    char*                           Author;
    ULONG                           Health;
    ULONG                           State;

    BOOL                            LogEnable;
    ULONG                           LogLevel;

    ULONG                           MemMaxUsage;
    ULONG                           MemMinUsage;
    ULONG                           MemConsumed;
}
COMPONENT_COMMON_DM,  *PCOMPONENT_COMMON_DM;

/**
 * @brief Initializes component
 * param[in] component_com object
 */
#define ComponentCommonDmInit(component_common_dm)                                          \
        {                                                                                   \
            AnscZeroMemory(component_common_dm, sizeof(COMPONENT_COMMON_DM));               \
            component_common_dm->Name        = NULL;                                        \
            component_common_dm->Version     = 1;                                           \
            component_common_dm->Author      = NULL;                                        \
            component_common_dm->Health      = CCSP_COMMON_COMPONENT_HEALTH_Red;            \
            component_common_dm->State       = CCSP_COMMON_COMPONENT_STATE_Running;         \
            if(g_iTraceLevel >= CCSP_TRACE_LEVEL_EMERGENCY)                                 \
                component_common_dm->LogLevel = (ULONG) g_iTraceLevel;                     \
            component_common_dm->LogEnable   = TRUE;                                        \
            component_common_dm->MemMaxUsage = 0;                                           \
            component_common_dm->MemMinUsage = 0;                                           \
            component_common_dm->MemConsumed = 0;                                           \
        }

/**
 * @brief clears component
 * param[in] component_com object
 */
#define  ComponentCommonDmClean(component_common_dm)                                        \
         {                                                                                  \
            if ( component_common_dm->Name )                                                \
            {                                                                               \
                AnscFreeMemory(component_common_dm->Name);                                  \
            }                                                                               \
                                                                                            \
            if ( component_common_dm->Author )                                              \
            {                                                                               \
                AnscFreeMemory(component_common_dm->Author);                                \
            }                                                                               \
         }

/**
 * @brief free agent component
 * param[in] component_com object
 */
#define  ComponentCommonDmFree(component_common_dm)                                         \
         {                                                                                  \
            ComponentCommonDmClean(component_common_dm);                                    \
            AnscFreeMemory(component_common_dm);                                            \
         }

int  cmd_dispatch(int  command);

void load_data_model();

/**
 * @brief ssp_create function definition to create component
 */
ANSC_STATUS
ssp_create
(
    PCCSP_COMPONENT_CFG         pStartCfg
);

/**
 * @brief ssp_engage function definition to engage component
 */
ANSC_STATUS
ssp_engage
(
    PCCSP_COMPONENT_CFG         pStartCfg
);

/**
 * @brief ssp_cancel function definition to cancel component
 */
ANSC_STATUS
ssp_cancel
(
    PCCSP_COMPONENT_CFG         pStartCfg
);



char*
ssp_CcdIfGetComponentName
    (
        ANSC_HANDLE                     hThisObject
    );

ULONG
ssp_CcdIfGetComponentVersion
    (
        ANSC_HANDLE                     hThisObject
    );

char*
ssp_CcdIfGetComponentAuthor
    (
        ANSC_HANDLE                     hThisObject
    );

ULONG
ssp_CcdIfGetComponentHealth
    (
        ANSC_HANDLE                     hThisObject
    );

ULONG
ssp_CcdIfGetComponentState
    (
        ANSC_HANDLE                     hThisObject
    );

BOOL
ssp_CcdIfGetLoggingEnabled
    (
        ANSC_HANDLE                     hThisObject
    );

ANSC_STATUS
ssp_CcdIfSetLoggingEnabled
    (
        ANSC_HANDLE                     hThisObject,
        BOOL                            bEnabled
    );

ULONG
ssp_CcdIfGetLoggingLevel
    (
        ANSC_HANDLE                     hThisObject
    );

ANSC_STATUS
ssp_CcdIfSetLoggingLevel
    (
        ANSC_HANDLE                     hThisObject,
        ULONG                           LogLevel
    );

ULONG
ssp_CcdIfGetMemMaxUsage
    (
        ANSC_HANDLE                     hThisObject
    );

ULONG
ssp_CcdIfGetMemMinUsage
    (
        ANSC_HANDLE                     hThisObject
    );

ULONG
ssp_CcdIfGetMemConsumed
    (
        ANSC_HANDLE                     hThisObject
    );

ANSC_STATUS
ssp_CcdIfApplyChanges
    (
        ANSC_HANDLE                     hThisObject
    );


#endif
