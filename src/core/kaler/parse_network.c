#include "kaler_config.h"
#include "../json/json.h"
#include "parse_network.h"

#ifdef undprif
#undef dprif
#define dprif(fmt, ...)
#endif

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int HexStringToDecimal(char a)
{
    if (a >= '0' && a <= '9')
    {
        return a - '0';
    }
    else if (a >= 'A' && a <= 'F')
    {
        return a - 'A' + 10;
    }
    else if (a >= 'a' && a <= 'f')
    {
        return a - 'a' + 10;
    }
    else
    {
        return -1;
    }
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_NetworkConfig(const json_object *new_obj, NetworkConfig_t *network, Record_t *record)
{
    char vstring[JSON_STRING_LEN];
    jboolean vbool;
    int vint;

    memset(network, 0, sizeof(NetworkConfig_t));

    json_object_object_foreach((json_object *)new_obj, key, val)
    {
        if (!strcmp(key, "enReport"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                network->BMask |= NETWORK_BMASK_REPORT;
            else
                network->BMask &= ~NETWORK_BMASK_REPORT;

            dprif("BMask(enReport) : %.4X\n", network->BMask);
        }
        else if (!strcmp(key, "enDHCP"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                network->BMask |= NETWORK_BMASK_DHCP;
            else
                network->BMask &= ~NETWORK_BMASK_DHCP;

            dprif("BMask(enDHCP) : %.4X\n", network->BMask);
        }
        else if (!strcmp(key, "enServer1Domain"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                network->BMask |= NETWORK_BMASK_SERVER1_DOMAIN;
            else
                network->BMask &= ~NETWORK_BMASK_SERVER1_DOMAIN;

            dprif("BMask(enServer1Domain) : %.4X\n", network->BMask);
        }
        else if (!strcmp(key, "enServer2Domain"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                network->BMask |= NETWORK_BMASK_SERVER2_DOMAIN;
            else
                network->BMask &= ~NETWORK_BMASK_SERVER2_DOMAIN;

            dprif("BMask(enServer2Domain) : %.4X\n", network->BMask);
        }
        else if (!strcmp(key, "enAutoDNS"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                network->BMask |= NETWORK_BMASK_AUTO_DNS;
            else
                network->BMask &= ~NETWORK_BMASK_AUTO_DNS;

            dprif("BMask(enAutoDNS) : %.4X\n", network->BMask);
        }
        else if (!strcmp(key, "updateMac"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                network->BMask |= NETWORK_BMASK_UPDATE_MAC;
            else
                network->BMask &= ~NETWORK_BMASK_UPDATE_MAC;

            dprif("BMask(updateMac) : %.4X\n", network->BMask);
        }
        else if (!strcmp(key, "updateIP"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
            {
                network->BMask |= NETWORK_BMASK_UPDATE_IP;
                network->BMask |= NETWORK_BMASK_UPDATE_GATEWAY;
                network->BMask |= NETWORK_BMASK_UPDATE_MASK;
            }
            else
            {
                network->BMask &= ~NETWORK_BMASK_UPDATE_IP;
                network->BMask &= ~NETWORK_BMASK_UPDATE_GATEWAY;
                network->BMask &= ~NETWORK_BMASK_UPDATE_MASK;
            }

            dprif("BMask(updateIP) : %.4X\n", network->BMask);
        }
        else if (!strcmp(key, "updatePort"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                network->BMask |= NETWORK_BMASK_UPDATE_PORT;
            else
                network->BMask &= ~NETWORK_BMASK_UPDATE_PORT;

            dprif("BMask(updatePort) : %.4X\n", network->BMask);
        }
        else if (!strcmp(key, "updateDNS"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
                network->BMask |= NETWORK_BMASK_UPDATE_DNS;
            else
                network->BMask &= ~NETWORK_BMASK_UPDATE_DNS;

            dprif("BMask(updateDNS) : %.4X\n", network->BMask);
        }
        else if (!strcmp(key, "updateServerIP"))
        {
            vbool = json_object_get_boolean(val);
            if (vbool == TRUE)
            {
                network->BMask |= NETWORK_BMASK_UPDATE_SERVERIP;
                network->BMask |= NETWORK_BMASK_UPDATE_SERVERPORT;
            }
            else
            {
                network->BMask &= ~NETWORK_BMASK_UPDATE_SERVERIP;
                network->BMask &= ~NETWORK_BMASK_UPDATE_SERVERPORT;
            }

            dprif("BMask(updateServerIP) : %.4X\n", network->BMask);
        }
        else if (!strcmp(key, "macAddr"))
        {
            int i;
            char *ptmp;

            strcpy(vstring, json_object_get_string(val));

            ptmp = strtok(vstring, "-");
            for (i = 0; i < 6; i++)
            {
                if (ptmp != NULL)
                {
                    network->MacAddr[i] = 
                        HexStringToDecimal(ptmp[0])*16 + HexStringToDecimal(ptmp[1]);
                }

                if (i < 5)
                    ptmp = strtok(NULL, "-");
                else
                    break;
            }

            dprif("MacAddr : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
                network->MacAddr[0],
                network->MacAddr[1],
                network->MacAddr[2],
                network->MacAddr[3],
                network->MacAddr[4],
                network->MacAddr[5]);
        }
        else if (!strcmp(key, "ipAddr"))
        {
            int i;
            char *ptmp;

            strcpy(vstring, json_object_get_string(val));

            ptmp = strtok(vstring, ".");
            for (i = 0; i < 4; i++)
            {
                if (ptmp != NULL)
                {
                    network->IPAddr[i] = (uint8)atoi((char*)ptmp);
                }

                if (i < 3)
                    ptmp = strtok(NULL, ".");
                else
                    break;
            }

            dprif("IPAddr : %d.%d.%d.%d\n",
                network->IPAddr[0],
                network->IPAddr[1],
                network->IPAddr[2],
                network->IPAddr[3]);
        }
        else if (!strcmp(key, "subnetMask"))
        {
            int i;
            char *ptmp;

            strcpy(vstring, json_object_get_string(val));

            ptmp = strtok(vstring, ".");
            for (i = 0; i < 4; i++)
            {
                if (ptmp != NULL)
                {
                    network->SubnetMask[i] = (uint8)atoi((char*)ptmp);
                }

                if (i < 3)
                    ptmp = strtok(NULL, ".");
                else
                    break;
            }

            dprif("SubnetMask : %d.%d.%d.%d\n",
                network->SubnetMask[0],
                network->SubnetMask[1],
                network->SubnetMask[2],
                network->SubnetMask[3]);
        }
        else if (!strcmp(key, "gateway"))
        {
            int i;
            char *ptmp;

            strcpy(vstring, json_object_get_string(val));

            ptmp = strtok(vstring, ".");
            for (i = 0; i < 4; i++)
            {
                if (ptmp != NULL)
                {
                    network->Gateway[i] = (uint8)atoi((char*)ptmp);
                }

                if (i < 3)
                    ptmp = strtok(NULL, ".");
                else
                    break;
            }

            dprif("Gateway : %d.%d.%d.%d\n",
                network->Gateway[0],
                network->Gateway[1],
                network->Gateway[2],
                network->Gateway[3]);
        }
        else if (!strcmp(key, "dnsAddr"))
        {
            int i;
            char *ptmp;

            strcpy(vstring, json_object_get_string(val));

            ptmp = strtok(vstring, ".");
            for (i = 0; i < 4; i++)
            {
                if (ptmp != NULL)
                {
                    network->DNSAddr[i] = (uint8)atoi((char*)ptmp);
                }

                if (i < 3)
                    ptmp = strtok(NULL, ".");
                else
                    break;
            }

            dprif("DNSAddr : %d.%d.%d.%d\n",
                network->DNSAddr[0],
                network->DNSAddr[1],
                network->DNSAddr[2],
                network->DNSAddr[3]);
        }
        else if (!strcmp(key, "server1IP"))
        {
            int i;
            char *ptmp;

            strcpy(vstring, json_object_get_string(val));

            ptmp = strtok(vstring, ".");
            for (i = 0; i < 4; i++)
            {
                if (ptmp != NULL)
                {
                    network->Server1IPAddr[i] = (uint8)atoi((char*)ptmp);
                }

                if (i < 3)
                    ptmp = strtok(NULL, ".");
                else
                    break;
            }

            dprif("Server1IPAddr : %d.%d.%d.%d\n",
                network->Server1IPAddr[0],
                network->Server1IPAddr[1],
                network->Server1IPAddr[2],
                network->Server1IPAddr[3]);
        }
        else if (!strcmp(key, "port"))
        {
            int i;
            char *ptmp;

            strcpy(vstring, json_object_get_string(val));

            ptmp = strtok(vstring, "\0");
            network->Port = (uint16)atoi((char*)ptmp);

            dprif("Port : %d\n", network->Port);
        }
        else if (!strcmp(key, "server1Port"))
        {
            int i;
            char *ptmp;

            strcpy(vstring, json_object_get_string(val));

            ptmp = strtok(vstring, "\0");
            network->Server1Port = (uint16)atoi((char*)ptmp);

            dprif("Server1Port : %d\n", network->Server1Port);
        }
        else if (!strcmp(key, "server1Domain"))
        {
            strcpy(network->Server1Domain, json_object_get_string(val));

            dprif("Server1Domain : %s\n", network->Server1Domain);
        }
        else
        {
            dprif("NetworkConfig key \"%s\" can not recognized\n", key);
        }
    }
    return 0;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int Inspect_NetworkConfig(NetworkConfig_t *network, Record_t *record)
{
    dprif("* Inspect_NetworkConfig\n");
    
    dprif("NetworkErrorno : %d\n", record->NetworkErrorno);

    return record->NetworkErrorno;
}

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int CreatFile_NetworkConfig(const char *path, const NetworkConfig_t *network, uint8 endian, Record_t *record)
{
    uint32 cnt;
    FILE *fp = NULL;
    FileHeader_t header;

    dprif("CreatFile_NetwordConfig\n");

	if ((path == NULL) || (network == NULL))
	{
		dprif("CreatFile_NetwordConfig parameter error\n");
		goto out;
	}

	if (path[strlen(path) - 1] != '/')
	{
		dprif("CreatFile_NetwordConfig path error\n");
		goto out;
	}

	if (CreateDirectoryK(path) < 0)
        goto out;

    char file[PATH_LEN];

    if (endian == FILE_LITTLE_ENDIAN)
    {
        sprintf(file, "%s%s", path, "ALL.ncf");
        memcpy(header.Sign, "NCF", 3);
    }
    else
    {
        sprintf(file, "%s%s", path, "ALL.ncf");
        memcpy(header.Sign, "NCF", 3);
    }

    header.Version = FILE_VERSION;
    header.Length = sizeof(FileHeader_t) + sizeof(NetworkConfig_t);
    header.Reserved = 0;

    if ((fp = fopen(file, "wb+")) == NULL)
    {
        dprif("fopen %s failed\n", file);
        goto out;
    }

    fseek(fp, 0, SEEK_SET);
    cnt = fwrite(&header, sizeof(FileHeader_t), 1, fp);

    fseek(fp, sizeof(FileHeader_t), SEEK_SET);
    cnt = fwrite(network, sizeof(NetworkConfig_t), 1, fp);

    fclose(fp);

    dprif("CreatFile_NetwordConfig OK!\n");

    return 0;

out:
    record->NetworkErrorno = RECORD_ERRORNO_CREATE_FAIL;
    return -1;
}

