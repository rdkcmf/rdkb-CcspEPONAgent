##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2016 RDK Management
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

lib_LTLIBRARIES = libepon_tr181.la

libepon_tr181_la_CPPFLAGS =

libepon_tr181_la_LDFLAGS = -lccsp_common -lhal_pon -lcm_mgnt

libepon_tr181_la_SOURCES = \
    source/TR-181/plugin_main.c \
    source/TR-181/plugin_main_apis.c \
    source/TR-181/cosa_apis_epon_plugin.c \
    source/TR-181/cosa_device_info_dml.c \
    source/TR-181/cosa_device_info_internal.c \
    source/TR-181/cosa_device_info_apis.c
