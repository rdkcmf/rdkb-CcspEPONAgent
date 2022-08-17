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

/**************************************************************************

    module: cosa_device_info_api.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlDIInit
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

//!!!  This code assumes that all data structures are the SAME in middle-layer APIs and HAL layer APIs
//!!!  So it uses casting from one to the other
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "cosa_device_info_apis.h"
#include "cm_hal.h"
#include "cosa_device_info_internal.h"
#include <syscfg/syscfg.h>
#define MAX_PROTOCOL  16

static int GetFirmwareName (char *pValue, unsigned long maxSize)
{
    static char name[64];

    if (name[0] == 0)
    {
        FILE *fp;
        char buf[128];  /* big enough to avoid reading incomplete lines */
        char *s = NULL;
        size_t len = 0;

        if ((fp = fopen ("/version.txt", "r")) != NULL)
        {
            while (fgets (buf, sizeof(buf), fp) != NULL)
            {
                /*
                   The imagename field may use either a ':' or '=' separator
                   and the value may or not be quoted. Handle all 4 cases.
                */
                if ((memcmp (buf, "imagename", 9) == 0) && ((buf[9] == ':') || (buf[9] == '=')))
                {
                    s = (buf[10] == '"') ? &buf[11] : &buf[10];

                    while (1)
                    {
                        int inch = s[len];

                        if ((inch == '"') || (inch == '\n') || (inch == 0))
                        {
                            break;
                        }

                        len++;
                    }

                    break;
                }
            }

            fclose (fp);
        }

        if (len >= sizeof(name))
        {
            len = sizeof(name) - 1;
        }

        memcpy (name, s, len);
        name[len] = 0;
    }

    if (name[0] != 0)
    {
        size_t len = strlen(name);

        if (len >= maxSize)
        {
            len = maxSize - 1;
        }

        memcpy (pValue, name, len);
        pValue[len] = 0;

        return 0;
    }

    pValue[0] = 0;

    return -1;
}


ANSC_STATUS
CosaDmlDIInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
	UNREFERENCED_PARAMETER(hDml);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject    = (PCOSA_DATAMODEL_DEVICEINFO)phContext;

    CosaDmlDIGetDLFlag((ANSC_HANDLE)pMyObject);
    
    if(pMyObject->Download_Control_Flag)
    	CosaDmlDIGetFWVersion((ANSC_HANDLE)pMyObject);
    CosaDmlDIGetURL((ANSC_HANDLE)pMyObject);
    CosaDmlDIGetImage((ANSC_HANDLE)pMyObject); 
    return ANSC_STATUS_SUCCESS;
}
ANSC_STATUS CosaDmlDIGetDLFlag(ANSC_HANDLE hContext)
{
	PCOSA_DATAMODEL_DEVICEINFO      pMyObject    = (PCOSA_DATAMODEL_DEVICEINFO)hContext;
	FILE *fp;
	char buff[64]={0};

	pMyObject->Download_Control_Flag = FALSE;
	
	if((fp = fopen("/etc/device.properties", "r")) == NULL)
	{
		CcspTraceError(("Error while opening the file device.properties \n"));
		CcspTraceInfo((" Download_Control_Flag is %d \n", pMyObject->Download_Control_Flag));
		return ANSC_STATUS_FAILURE;
	}

	while(fgets(buff, 64, fp) != NULL) 
	{
		if(strstr(buff, "BUILD_TYPE") != NULL) 
		{
		//RDKB-22703: Enabled support to allow TR-181 firmware download on PROD builds.
			if((strcasestr(buff, "dev")) || (strcasestr(buff, "vbn")) || (strcasestr(buff, "prod")))
			{
				pMyObject->Download_Control_Flag = TRUE;
				break;
			}
		}
	}

	if(fp)
		fclose(fp);

	CcspTraceInfo((" Download_Control_Flag is %d \n", pMyObject->Download_Control_Flag));
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDIGetFWVersion(ANSC_HANDLE hContext)
{
	PCOSA_DATAMODEL_DEVICEINFO pMyObject = (PCOSA_DATAMODEL_DEVICEINFO) hContext;

	if (GetFirmwareName(pMyObject->Current_Firmware, sizeof(pMyObject->Current_Firmware)) != 0)
	{
		CcspTraceError(("GetFirmwareName() failed\n"));
		return ANSC_STATUS_FAILURE;
	}

	CcspTraceInfo((" Current FW Version is %s \n", pMyObject->Current_Firmware));
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDIGetDLStatus(ANSC_HANDLE hContext, char *DL_Status)
{
	UNREFERENCED_PARAMETER(hContext);
	int dl_status = 0;
	
	dl_status = cm_hal_Get_HTTP_Download_Status();
	CcspTraceInfo((" Download status is %d \n", dl_status));
	
	if(dl_status == 0)
		AnscCopyString(DL_Status, "Not Started");
	else if(dl_status > 0 && dl_status <= 100)
		AnscCopyString(DL_Status, "In Progress");
	else if(dl_status == 200)
		AnscCopyString(DL_Status, "Completed");
	else if(dl_status >= 400)
		AnscCopyString(DL_Status, "Failed");

	return ANSC_STATUS_SUCCESS;
}

static int CosaDmlDISetProtocol (PCOSA_DATAMODEL_DEVICEINFO pMyObject)
{
    char *protocol = NULL;

    if (strncasecmp(pMyObject->DownloadURL, "https", 5) == 0)
    {
        protocol = "HTTPS";
    }
    else if (strncasecmp(pMyObject->DownloadURL, "http", 4) == 0)
    {
        protocol = "HTTP";
    }
    else if (strlen(pMyObject->DownloadURL) == 0)
    {
        protocol = "";
    }
    else
    {
        protocol = "INVALID";
    }

    if (syscfg_set_commit(NULL, "fw_dl_protocol", protocol) != 0)
    {
        CcspTraceWarning(("%s: syscfg_set failed\n", __FUNCTION__));
        return -1;
    }

    return 0;
}

ANSC_STATUS CosaDmlDIGetProtocol(char *Protocol)
{
    int rc = -1;
    char buf[64]={0};
    if(syscfg_get( NULL, "fw_dl_protocol", buf, sizeof(buf)) == 0)
    {
        rc = snprintf(Protocol,MAX_PROTOCOL,buf);
    }
    if(rc < 0)
    {
        return ANSC_STATUS_FAILURE;
    }
    CcspTraceInfo((" Download Protocol is %s \n", Protocol));
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDIGetURL(ANSC_HANDLE hContext)
{
    PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)hContext;
    if(syscfg_get(NULL,"xconf_url",pMyObject->DownloadURL,128) == 0)
    {
        return ANSC_STATUS_SUCCESS;
    }
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS CosaDmlDIGetImage(ANSC_HANDLE hContext)
{
    PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)hContext;
    char Last_reboot_reason[14]={0};
    int rc = -1;
    //On Factory reset Syscfg values will be empty. So sync fw_to_upgrade with current image version
    if(syscfg_get(NULL,"X_RDKCENTRAL-COM_LastRebootReason",Last_reboot_reason,14) == 0)
    {
        if (strcmp(Last_reboot_reason, "factory-reset") == 0)
        {
            rc = snprintf(pMyObject->Firmware_To_Download,128,pMyObject->Current_Firmware);
            if(rc < 0)
            {
                return ANSC_STATUS_FAILURE;
            }
            if(syscfg_set(NULL, "fw_to_upgrade", pMyObject->Firmware_To_Download) == 0)
            {
                if(syscfg_commit() != 0)
                    CcspTraceWarning(("%s: syscfg commit failed \n", __FUNCTION__));
            }
            return ANSC_STATUS_SUCCESS;
        }
        else
        {
            if(syscfg_get(NULL,"fw_to_upgrade",pMyObject->Firmware_To_Download,128) == 0)
            {
                return ANSC_STATUS_SUCCESS;
            }
        }
    }
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS CosaDmlDIDownloadNow(ANSC_HANDLE hContext)
{
	PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)hContext;
	int dl_status = 0;
	int ret = RETURN_ERR, res = 0;
	char valid_fw[128]={0};
	
	if(strlen(pMyObject->Firmware_To_Download) && strlen(pMyObject->DownloadURL))
	{
		convert_to_validFW(pMyObject->Firmware_To_Download,valid_fw);
		if (strcasecmp(valid_fw, pMyObject->Current_Firmware) == 0)
		{
			CcspTraceError((" Current FW is same, Ignoring request \n"));
			return ANSC_STATUS_FAILURE;		
		}		
		ret = RETURN_ERR;
		ret = cm_hal_Set_HTTP_Download_Url(pMyObject->DownloadURL , pMyObject->Firmware_To_Download);

		if( ret == RETURN_ERR)
		{
			CcspTraceError((" Failed to set URL, Ignoring request \n"));
			return ANSC_STATUS_FAILURE;
		}
		
	}
	else
	{
		CcspTraceError((" URL or FW Name is missing, Ignoring request \n"));
		return ANSC_STATUS_FAILURE;		
	}

	dl_status = cm_hal_Get_HTTP_Download_Status();
	if(dl_status > 0 && dl_status <= 100)
	{
		CcspTraceError((" Already Downloading In Progress, Ignoring request \n"));
		return ANSC_STATUS_FAILURE;
	}
	else if(dl_status == 200)
	{
		CcspTraceError((" Image is already downloaded, Ignoring request \n"));
		return ANSC_STATUS_FAILURE;	
	}
	
	ret = RETURN_ERR;
	ret = cm_hal_Set_HTTP_Download_Interface(1); // interface=0 for wan0, interface=1 for erouter0

	if( ret == RETURN_ERR)
	{
		CcspTraceError((" Failed to set Interface, Ignoring request \n"));
		return ANSC_STATUS_FAILURE;
	}

//Moving cm_hal_HTTP_Download() to thread. TR-181 query is getting time out as it is taking more time to download.
/*
	ret = RETURN_ERR;
	ret = cm_hal_HTTP_Download ();

	if( ret == RETURN_ERR)
	{
		CcspTraceError((" Failed to start download \n"));
		return ANSC_STATUS_FAILURE;
	}
	else
	{
		CcspTraceInfo(("Image downloading triggered successfully \n"));
		pthread_t FWDL_Thread;
		res = pthread_create(&FWDL_Thread, NULL, FWDL_ThreadFunc, "FWDL_ThreadFunc"); 
		if(res != 0)
		{
			CcspTraceError(("Create FWDL_Thread error %d\n", res));
		}	

	}
*/

        pthread_t FWDL_Thread;
        res = pthread_create(&FWDL_Thread, NULL, FWDL_ThreadFunc, "FWDL_ThreadFunc");
        if(res != 0)
        {
                CcspTraceError(("Create FWDL_Thread error %d\n", res));
        }
	
	return ANSC_STATUS_SUCCESS;
}

BOOL IsFileUpdateNeeded( ANSC_HANDLE hContext ) 
{
	PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)hContext;
	if(strlen(pMyObject->Firmware_To_Download) && strlen(pMyObject->DownloadURL))
	{
		CcspTraceInfo((" Factory reset image :%s and url: %s \n", pMyObject->Firmware_To_Download, pMyObject->DownloadURL));
		return TRUE;
	}
	return FALSE;
}

void WriteFactoryResetParams( ANSC_HANDLE hContext )
{
	CcspTraceInfo((" In %s \n", __FUNCTION__));
	PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)hContext;
    int syscfg_fail=0;
/*    FILE *fp;
	if((fp = fopen("/tmp/FactoryReset.txt", "w")) == NULL)
	{
		CcspTraceError(( "FactoryReset.txt file open failed.\n"));
		return;
	}
	fprintf(fp, "Url=%s\n", pMyObject->DownloadURL);
	fprintf(fp, "Image=%s\n", pMyObject->Firmware_To_Download);
	fclose(fp); */ //Replaced this code for RDKB-35095
    if (syscfg_set(NULL, "xconf_url", pMyObject->DownloadURL) != 0)
    {
        CcspTraceWarning(("%s: syscfg_set failed for xconf_url\n", __FUNCTION__));
        syscfg_fail++;
    }
    if(syscfg_set(NULL, "fw_to_upgrade", pMyObject->Firmware_To_Download) != 0)
    {
        CcspTraceWarning(("%s: syscfg_set failed for fw_to_upgrade\n", __FUNCTION__));
        syscfg_fail++;
    }
    if (syscfg_fail <= 1)
    {
        if(syscfg_commit() != 0)
            CcspTraceWarning(("%s: syscfg commit failed \n", __FUNCTION__));
    }
}

ANSC_STATUS CosaDmlDISetURL(ANSC_HANDLE hContext, char *URL)
{
	CcspTraceInfo((" In %s \n", __FUNCTION__))
	PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)hContext;

	AnscCopyString(pMyObject->DownloadURL, URL);
	CcspTraceInfo((" URL is %s \n", pMyObject->DownloadURL));
    if(CosaDmlDISetProtocol(pMyObject) == -1 )
    {
        CcspTraceInfo((" Image upgrade protocol set failed in %s\n",__FUNCTION__));
    }
	WriteFactoryResetParams( (ANSC_HANDLE)pMyObject);
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDISetImage(ANSC_HANDLE hContext, char *Image)
{
	PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)hContext;

	AnscCopyString(pMyObject->Firmware_To_Download, Image);
	CcspTraceInfo((" FW DL is %s \n", pMyObject->Firmware_To_Download));
	WriteFactoryResetParams((ANSC_HANDLE)pMyObject);
	return ANSC_STATUS_SUCCESS;	
}

void *FWDL_ThreadFunc(void *arg)
{
	UNREFERENCED_PARAMETER(arg);
	int dl_status = 0;
	int ret = RETURN_ERR;
	ULONG reboot_ready_status = 0;
	FILE *log_wget = NULL;
	    
	pthread_detach(pthread_self());

        CcspTraceInfo((" Waiting for FW DL ... \n"));

        /* XF3-3359 : Adding print statement as requested by automation team */
        CcspTraceInfo(("Image downloading triggered successfully \n"));

        ret = cm_hal_HTTP_Download ();
        if( ret == RETURN_ERR)
        {
                CcspTraceError((" Failed to start download \n"));
        }
        else
        {
                ret = RETURN_ERR;

        	/* Sleeping since the status returned is 500 on immediate status query */
	        CcspTraceInfo((" Sleeping to prevent 500 error \n")); 
        	sleep(10);

	        /* Check if the /tmp/wget.log file was created, if not wait an adidtional time */
        	log_wget= fopen("/tmp/wget.log", "r");

	        if (log_wget == NULL) 
	        {
		        CcspTraceInfo(("/tmp/wget.log doesn't exist. Sleeping an additional 10 seconds \n"));
		        sleep(10);
	        }
        	else 
	        {
		        fclose(log_wget);
        		CcspTraceInfo(("/tmp/wget.log created . Continue ...\n"));
	        }

	        while(1)
	        {
		        dl_status = cm_hal_Get_HTTP_Download_Status();

        		if(dl_status >= 0 && dl_status <= 100)
	        		sleep(2);
		        else if(dl_status == 200)
			        break;
        		else if(dl_status >= 400)
		        {
			        CcspTraceError((" FW DL is failed with status %d \n", dl_status));
			        return NULL;
		        }
	        }

        	CcspTraceInfo((" FW DL is over \n"));
                CcspTraceInfo(("Image downloaded successfully \n"));

	        CcspTraceInfo((" Waiting for reboot ready ... \n"));
        	while(1)
        	{
	        	ret = cm_hal_Reboot_Ready(&reboot_ready_status);

		        if(ret == RETURN_OK && reboot_ready_status == 1)
			        break;
        		else
	        		sleep(5);
        	}
	        CcspTraceInfo((" Waiting for reboot ready over, setting last reboot reason \n"));

            if (system("dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_LastRebootReason string Forced_Software_upgrade") == -1 )
            {
                CcspTraceError(("Failed to set LastRebootReason.\n"));
            }
        	ret = RETURN_ERR;
	        ret = cm_hal_HTTP_Download_Reboot_Now();

        	if(ret == RETURN_OK)
        	{
	        	CcspTraceInfo((" Rebooting the device now!\n"));
        	}
	        else
	        {
		        CcspTraceError((" Reboot Already in progress!\n"));
	        }
        }
    return NULL;
}

void convert_to_validFW(char *fw,char *valid_fw)
{
	/* Valid FW names
		TG1682_DEV_stable2_20170717081507sdy
		TG1682_DEV_stable2_20170717081507sdy.bin
		TG1682_DEV_stable2_20170717081507sdy_signed.bin
		tg1682_dev_stable2_20170717081507sdy_signed.bin.ccs
	*/
	
	char *buff = NULL;
	int buff_len = 0;

	if((buff = strstr(fw,"_signed")) != NULL){}
	else if((buff = strstr(fw,"-signed")) != NULL){}
	else if((buff = strstr(fw,".")) != NULL){}

	if(buff)
		buff_len = strlen(buff);

	strncpy(valid_fw,fw,strlen(fw)-buff_len);

	CcspTraceInfo((" Converted image name = %s \n", valid_fw));
}
