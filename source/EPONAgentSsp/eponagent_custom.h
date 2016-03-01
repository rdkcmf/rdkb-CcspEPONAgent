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

    module: epon_agent_custom.h

        For CCSP EPON Agent Component,

**********************************************************************/

#ifndef  _EPON_AGENT_CUSTOM_H_
#define  _EPON_AGENT_CUSTOM_H_

/**********************************************************************
                      GLOBAL CUSTOM DEFINITIONS
**********************************************************************/

#define  MESSAGE_BUS_CONFIG_FILE                    "msg_daemon.cfg"

#define  CCSP_EPON_START_CFG_FILE                    "CcspEPON.cfg"

/* TBD  Move to platform specific header file */
#define  CCSP_EPON_BACKTRACE_FILE                    "/nvram/epon_agent_backtrace"

#define  CCSP_DATAMODEL_XML_FILE                    "TR181-EPON.XML"

#endif   /*_EPON_AGENT_CUSTOM_H_*/
