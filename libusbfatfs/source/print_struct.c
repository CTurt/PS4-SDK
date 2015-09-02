#include <ps4.h>
#include "MassStoreCommands.h"
#include "print_struct.h"

#define printf(...) (void)0

// IT automatically puts \n at start and at the end
void print_hex_ascii(uint8_t *buf, uint16_t no_of_char)
{
	uint16_t i,j;

	//
	printf("\n     ");

	// put the numbering for identification at first
	for (i=0;i<0x10;i++)
	printf(" %02X ",i);

	// for loop for printing array of data in hex and ascii
	for(i=0;i<no_of_char;i++)
	{
		// check for counter to be multiple of 0x10
		if(i%(0x10)==0)
		{
			//print in standard ascii (not in extended ASCII)
			printf("\t");
			for(j=(i-0x10);j<i;j++)
			{
				// check condition for printing (Printable Character)
				if(buf[j]>0x20 && buf[j]<0x80)
					printf("%c",buf[j]);
				else
					printf(".");
			}

			// print the data bit
			printf("\n%04X ",i);
		}
		//print the byte in hex form
		printf(" %02X ",buf[i]);

	}

	// calculating no of bytes in last line
	uint16_t byte_index_start_last_line=(no_of_char/0x10)*0x10;
	if(no_of_char==byte_index_start_last_line)
	{
		byte_index_start_last_line = no_of_char - 0x10;
	}

	uint16_t no_of_bytes_last_line = no_of_char-byte_index_start_last_line;

	// no of blanks bytes in last line
	uint16_t no_of_blanks_last_line=0x10-no_of_bytes_last_line;

	// fill blanks
	for(j=0;j<no_of_blanks_last_line;j++)
	printf("    ");

	printf("\t");

	for(j=byte_index_start_last_line;j<no_of_char;j++)
	{
		// check condition for printing
		if(buf[j]>0x20 && buf[j]<0x80)
			printf("%c",buf[j]);
		else
			printf(".");
	}

	//
	printf("\n");

}

void print_struct_CBW(CommandBlockWrapper_t *CommandBlockWrapper)
{
	printf("\nPrinting Command Block Wrapper:\n");
	printf("Signature:\t\t%04X\n",			CommandBlockWrapper->Signature);
	printf("Tag:\t\t\t%04X\n",				CommandBlockWrapper->Tag);
	printf("DataTransferLength:\t%04X\n",	CommandBlockWrapper->DataTransferLength);
	printf("Flags:\t\t\t%01X\n",			CommandBlockWrapper->Flags);
	printf("LUN:\t\t\t%01X\n",				CommandBlockWrapper->LUN);
	printf("SCSICommandLength:\t%01X\n",	CommandBlockWrapper->SCSICommandLength);
	printf("SCSICommandData: ");
	print_hex_ascii(&(CommandBlockWrapper->SCSICommandData[0]), 0x10);
	printf("\nEnd Command Block Wrapper.\n");
}

void print_struct_CSW(CommandStatusWrapper_t *CommandStatusWrapper)
{
	printf("\nPrinting Command Status Wrapper:\n");
	printf("Signature:\t\t%04X\n",			CommandStatusWrapper->Signature);
	printf("Tag:\t\t\t%04X\n",				CommandStatusWrapper->Tag);
	printf("DataTransferResidue:\t%04X\n",	CommandStatusWrapper->DataTransferResidue);
	printf("Status:\t\t\t%01X",				CommandStatusWrapper->Status);

	// Parse the Command STATUS from the Status byte of the CSW.
	if(CommandStatusWrapper->Status==0)
		printf("\t(Command Passed.)\n");
	else if(CommandStatusWrapper->Status==1)
		printf("\t(Command Error.)\n");
	else if(CommandStatusWrapper->Status==2)
		printf("\t(Phase Recovery Reset.)\n");
	else
		printf("\tError: Could not parse the STATUS");

	printf("\nEnd Command Status Wrapper.\n");

}

void print_struct_RS(SCSI_Request_Sense_Response_t *RequestSenseResponse)
{
	printf("\nPrinting SCSI Request Sense Response:\n");
	printf("ResponseCode:\t\t%01X\n",			RequestSenseResponse->ResponseCode);
	printf("SegmentNumber:\t\t%01X\n",			RequestSenseResponse->SegmentNumber);
	printf("SenseKey:\t\t%01X\n",				RequestSenseResponse->SenseKey);
	printf("ILI:\t\t\t%01X\n",					RequestSenseResponse->ILI);
	printf("EOM:\t\t\t%01X\n",					RequestSenseResponse->EOM);
	printf("Information:");
	print_hex_ascii(&(RequestSenseResponse->Information[0]), 0x04);
	printf("AdditionalLength:\t\t%01X\n",		RequestSenseResponse->AdditionalLength);
	printf("CmdSpecificInformation:");
	print_hex_ascii(&(RequestSenseResponse->CmdSpecificInformation[0]), 0x04);
	printf("AdditionalSenseCode:\t\t%01X\n",	RequestSenseResponse->AdditionalSenseCode);
	printf("AdditionalSenseQualifier:\t%01X\n",	RequestSenseResponse->AdditionalSenseQualifier);
	printf("FieldReplaceableUnitCode:\t%01X\n",	RequestSenseResponse->FieldReplaceableUnitCode);
	printf("SenseKeySpecific:");
	print_hex_ascii(&(RequestSenseResponse->SenseKeySpecific[0]), 0x03);
	printf("\nEnd SCSI Request Sense Response.\n");
}

void print_struct_INQ(SCSI_Inquiry_Response_t *InquiryResponse)
{
	printf("\nPrinting SCSI Inquiry Response:\n");
	printf("DeviceType:\t\t%02X\n",					InquiryResponse->DeviceType);
	printf("PeripheralQualifier:\t%02X\n",			InquiryResponse->PeripheralQualifier);
	printf("Removable:\t\t%01X\n",					InquiryResponse->Removable);
	printf("Version:\t\t%02X\n",					InquiryResponse->Version);
	printf("ResponseDataFormat:\t%02X\n",			InquiryResponse->ResponseDataFormat);
	printf("NormACA:\t\t%01X\n",					InquiryResponse->NormACA);
	printf("TrmTsk:\t\t\t%01X\n",					InquiryResponse->TrmTsk);
	printf("AERC:\t\t\t%01X\n",						InquiryResponse->AERC);
	printf("AdditionalLength:\t%02X\n",				InquiryResponse->AdditionalLength);
	printf("SoftReset:\t\t%01X\n",					InquiryResponse->SoftReset);
	printf("CmdQue:\t\t\t%01X\n",					InquiryResponse->CmdQue);
	printf("Linked:\t\t\t%01X\n",					InquiryResponse->Linked);
	printf("Sync:\t\t\t%01X\n",						InquiryResponse->Sync);
	printf("WideBus16Bit:\t\t%01X\n",				InquiryResponse->WideBus16Bit);
	printf("WideBus32Bit:\t\t%01X\n",				InquiryResponse->WideBus32Bit);
	printf("RelAddr:\t\t%01X\n",					InquiryResponse->RelAddr);
	printf("VendorID:");
	print_hex_ascii(&(InquiryResponse->VendorID[0]), 0x08);
	printf("ProductID:");
	print_hex_ascii(&(InquiryResponse->ProductID[0]), 0x10);
	printf("RevisionID:");
	print_hex_ascii(&(InquiryResponse->RevisionID[0]), 0x04);
	printf("\nEnd SCSI Inquiry Response.\n");
}

void print_struct_CAP(SCSI_Capacity_t *Capacity)
{
	printf("\nPrinting Capacity of Device (MassStore_ReadCapacity()):\n");
	printf("Blocks:\t\t%04X\n",			Capacity->Blocks);
	printf("BlockSize:\t%04X\n",		Capacity->BlockSize);
	printf("\nEnd Capacity of Device (MassStore_ReadCapacity()):\n");
}
