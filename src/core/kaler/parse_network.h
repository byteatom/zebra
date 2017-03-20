#ifndef __PARSE_NETWORK_H__
#define __PARSE_NETWORK_H__

#include "kaler_config.h"
#include "kaler_struct.h"
#include "../json/json.h"
#include "file_ops.h"

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
#define NETWORK_BMASK_REPORT            (1ul<<15)
#define NETWORK_BMASK_DHCP              (1ul<<14)
#define NETWORK_BMASK_SERVER1_DOMAIN    (1ul<<13)
#define NETWORK_BMASK_SERVER2_DOMAIN    (1ul<<12)
#define NETWORK_BMASK_AUTO_DNS          (1ul<<11)
#define NETWORK_BMASK_UPDATE_IP         (1ul<<10)
#define NETWORK_BMASK_UPDATE_MASK       (1ul<<9)
#define NETWORK_BMASK_UPDATE_GATEWAY    (1ul<<8)
#define NETWORK_BMASK_UPDATE_MAC        (1ul<<7)
#define NETWORK_BMASK_UPDATE_PORT       (1ul<<6)
#define NETWORK_BMASK_UPDATE_SERVERIP   (1ul<<5)
#define NETWORK_BMASK_UPDATE_SERVERPORT (1ul<<4)
#define NETWORK_BMASK_UPDATE_DNS        (1ul<<1)

#pragma pack(push, 1)
typedef struct _NetworkConfig_
{
    uint16  BMask;
    uint8   IPAddr[6];
    uint8   SubnetMask[6];
    uint8   Gateway[6];
    uint8   MacAddr[6];
    uint16  Port;
    uint8   Server1IPAddr[6];
    uint16  Server1Port;
    uint8   Server2IPAddr[6];
    uint16  Server2Port;
    char    Server1Domain[64];
    char    Server2Domain[64];
    uint8   DNSAddr[6];
} NetworkConfig_t;
#pragma pack(pop)

/*
 *******************************************************************************
 * author:
 * date:
 * description:
 *******************************************************************************
 */
int JsonParse_NetworkConfig(const json_object *new_obj, NetworkConfig_t *network, Record_t *record);
int Inspect_NetworkConfig(NetworkConfig_t *network, Record_t *record);
int CreatFile_NetworkConfig(const char *path, const NetworkConfig_t *network, uint8 endian, Record_t *record);

#endif // __PARSE_NETWORK_H__
