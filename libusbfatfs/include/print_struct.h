#ifndef _PRINT_STRUCT_H_
#define _PRINT_STRUCT_H_

#include <ps4.h>
#include "flash_drive.h"
#include "MassStoreCommands.h"

void print_hex_ascii(uint8_t *buf, uint16_t no_of_char);
void print_struct_CBW(CommandBlockWrapper_t *CommandBlockWrapper);
void print_struct_CSW(CommandStatusWrapper_t *CommandStatusWrapper);
void print_struct_RS(SCSI_Request_Sense_Response_t *RequestSenseResponse);
void print_struct_INQ(SCSI_Inquiry_Response_t *InquiryResponse);
void print_struct_CAP(SCSI_Capacity_t *Capacity);

#endif
