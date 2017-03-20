#ifndef __CORE_EXPORT_H__
#define __CORE_EXPORT_H__

#include "kaler_config.h"
#include "kaler_struct.h"
#include "parse_protocol.h"

int JsonParseAndCreateFile(const char *jsonpath);
int SplitBMPFileHorizontal(const char *bmpfilename, const char *dir, const char *startname, const Coord_t *coord, uint32 *splitnum);
int SplitBMPFileVerticle(const char *bmpfilename, const char *dir, const char *startname, const Coord_t *coord, uint32 *splitnum);
int Protocol_SendFrameContent(const SendIdeology_t *send, ProtocolAimData_t *aim, Record_t *record);
ProtocolAimData_t Protocol_ReturnFrameContent(const SendIdeology_t *send, Record_t *record);
int Protocol_ParseReplyBuffer(const char *buf, uint16 len, ProtocolAimData_t *aim);

#endif // __CORE_EXPORT_H__
