/*
 * SAE_J1939_Transport_Protocol_Data_Transfer.c
 *
 *  Created on: 14 juli 2021
 *      Author: Daniel Mårtensson
 */

#include "SAE_J1939-21_Transport_Layer.h"

/*
 * Store the sequence data packages from other ECU
 * PGN: 0x00EB00 (60160)
 */
void SAE_J1939_Read_Transport_Protocol_Data_Transfer(J1939 *j1939, uint8_t SA, uint8_t data[]) {
	/* Save the sequence data */
	j1939->tp_dt.sequence_number = data[0];
	uint8_t index = data[0] - 1;
	for (uint8_t i = 1; i < 7; i++)
		j1939->tp_dt.data[i][index] = data[i];

	/* Check if we have completed our message - Return = Not completed */
	if (j1939->tp_cm.number_of_packages != j1939->tp_dt.sequence_number && j1939->tp_cm.number_of_packages < 2)
		return;

	/* Our message are complete - Build it and call it complete_data[total_message_size] */
	uint32_t PGN = j1939->tp_cm.PGN_of_the_packeted_message;
	uint8_t complete_data[j1939->tp_cm.total_message_size];
	uint16_t inserted_bytes = 0;
	for (uint8_t i = 0; i < j1939->tp_dt.sequence_number; i++)
		for (uint8_t j = 0; j < 7; i++)
			if (inserted_bytes < j1939->tp_cm.total_message_size)
				complete_data[inserted_bytes++] = j1939->tp_dt.data[j][i];

	/* Send an end of message ACK back */
	if(j1939->tp_cm.control_byte == CONTROL_BYTE_TP_CM_RTS)
		SAE_J1939_Send_Acknowledgement(j1939, SA, CONTROL_BYTE_TP_CM_EndOfMsgACK, GROUP_FUNCTION_VALUE_NORMAL, PGN);

	/* Check what type of function that message want this ECU to do */
	switch (PGN) {
	case PGN_COMMANDED_ADDRESS:
		/* Insert new name and new destination address to this ECU */
		j1939->this_name.identity_number = ((complete_data[2] & 0b00011111) << 16) | (complete_data[1] << 8) | complete_data[0];
		j1939->this_name.manufacturer_code = (complete_data[3] << 3) | (complete_data[2] >> 5);
		j1939->this_name.function_instance = complete_data[4] >> 3;
		j1939->this_name.ECU_instance = complete_data[4] & 0b00000111;
		j1939->this_name.function = complete_data[5];
		j1939->this_name.vehicle_system = complete_data[6] >> 1;
		j1939->this_name.arbitrary_address_capable = complete_data[7] >> 7;
		j1939->this_name.industry_group = (complete_data[7] >> 4) & 0b0111;
		j1939->this_name.vehicle_system_instance = complete_data[7] & 0b00001111;
		j1939->this_ECU_address = complete_data[8]; 							/* New address of this ECU */
		break;
	case PGN_DM1:
		SAE_J1939_Read_Response_Request_DM1(j1939, complete_data, j1939->tp_dt.sequence_number); /* Sequence number is the amount of errors */
		break;
	case PGN_DM2:
		SAE_J1939_Read_Response_Request_DM2(j1939, complete_data, j1939->tp_dt.sequence_number); /* Sequence number is the amount of errors */
		break;
	case PGN_DM16:
		SAE_J1939_Read_Binary_Data_Transfer_DM16(j1939, complete_data);
		break;
	case PGN_SOFTWARE_IDENTIFICATION:
		SAE_J1939_Read_Response_Request_Software_Identification(j1939, complete_data);
		break;
	case PGN_ECU_IDENTIFICATION:
		SAE_J1939_Read_Response_Request_ECU_Identification(j1939, complete_data);
		break;
	case PGN_COMPONENT_IDENTIFICATION:
		SAE_J1939_Read_Response_Request_Component_Identification(j1939, complete_data);
		break;
		/* Add more here */
	}

	/* Delete TP DT and TP CM */
	memset(&j1939->tp_dt, 0, sizeof(j1939->tp_dt));
	memset(&j1939->tp_cm, 0, sizeof(j1939->tp_cm));
}

/*
 * Send sequence data packages to other ECU
 * PGN: 0x00EB00 (60160)
 */
ENUM_J1939_STATUS_CODES SAE_J1939_Send_Transport_Protocol_Data_Transfer(J1939 *j1939, uint8_t DA, uint8_t data[], uint16_t total_message_size, uint8_t number_of_packages){
	uint32_t ID = (0x1CEB << 16) | (DA << 8) | j1939->this_ECU_address;
	uint8_t package[8];
	uint16_t bytes_sent = 0;
	ENUM_J1939_STATUS_CODES status = STATUS_SEND_OK;
	for(uint8_t i = 1; i <= number_of_packages; i++) {
		package[0] = i; 													/* Number of package */
		for(uint8_t j = 0; j < 7; j++)
			if(bytes_sent < total_message_size)
				package[j+1] = data[bytes_sent++];							/* Data */
			 else
				package[j+1] = 0xFF; 										/* Reserved */

		status = CAN_Send_Message(ID, package, 100);						/* 100 ms delay */
		if(status != STATUS_SEND_OK)
			return status;
	}
	return status;
}
