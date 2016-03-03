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
SUBDIRS = source

ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES=libepon_tr181.la

libepon_tr181_la_CPPFLAGS= -D_ANSC_LINUX -D_ANSC_USER -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/custom -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/include -I$(top_srcdir)/../CcspCommonLibrary/source/debug_api/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/include/linux -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/components/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/package/slap/include -I$(top_srcdir)/../CcspCommonLibrary/source/util_api/http/include -I$(top_srcdir)/../CcspCommonLibrary/source/util_api/ansc/include -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/components/common/MessageBusHelper/include -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/components/common/PoamIrepFolder -I$(top_srcdir)/../Utopia/source/include -I$(top_srcdir)/source/TR-181/ml -I$(top_srcdir)/source/TR-181/include -I$(top_srcdir)/include/TR-181/ml -I$(top_srcdir)/source/wecb -I$(top_srcdir)/source/Custom -I$(top_srcdir)/./include

libepon_tr181_la_LDFLAGS= -lccsp_common -lhal_pon -lsysevent -lsyscfg -lulog -lupnp -lxml2 -lcurl -ldbus-1

libepon_tr181_la_SOURCES= \
    source/TR-181/plugin_main.c \
    source/TR-181/cosa_apis_epon_plugin.c

