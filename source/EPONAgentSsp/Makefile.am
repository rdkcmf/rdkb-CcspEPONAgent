##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2015 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################

AM_CFLAGS = -D_ANSC_LINUX
AM_CFLAGS += -D_ANSC_USER
AM_LDFLAGS = -lccsp_common
AM_LDFLAGS += -lsysevent
AM_LDFLAGS += -lsyscfg
AM_LDFLAGS += -lulog
AM_LDFLAGS += -lupnp
AM_LDFLAGS += -lxml2
AM_LDFLAGS += -lcurl
AM_LDFLAGS += -ldbus-1

AM_CPPFLAGS = -Wall -Werror
ACLOCAL_AMFLAGS = -I m4
hardware_platform = i686-linux-gnu
bin_PROGRAMS = CcspEPONAgentSsp

CcspEPONAgentSsp_CPPFLAGS = -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/custom -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/include -I$(top_srcdir)/../CcspCommonLibrary/source/debug_api/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/include/linux -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/components/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/package/slap/include -I$(top_srcdir)/../CcspCommonLibrary/source/util_api/http/include -I$(top_srcdir)/../CcspCommonLibrary/source/util_api/ansc/include -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/components/common/MessageBusHelper/include -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/components/common/PoamIrepFolder -I$(top_srcdir)/../Utopia/source/include -I$(top_srcdir)/source/TR-181/ml -I$(top_srcdir)/source/TR-181/include -I$(top_srcdir)/include/TR-181/ml -I$(top_srcdir)/source/wecb -I$(top_srcdir)/source/Custom -I$(top_srcdir)/./include $(CPPFLAGS)

CcspEPONAgentSsp_SOURCES = ssp_main.c ssp_action.c ssp_messagebus_interface.c
CcspEPONAgentSsp_LDFLAGS =  -rdynamic -lccsp_common -lsysevent -lsyscfg -lulog -lupnp -lxml2 -lcurl -ldbus-1
