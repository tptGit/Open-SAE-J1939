/*
 * J1939_Enums.h
 *
 *  Created on: 8 juli 2021
 *      Author: Daniel Mårtensson
 */

#ifndef SAE_J1939_ENUMS_SAE_J1939_ENUM_DM14_DM15_H_
#define SAE_J1939_ENUMS_SAE_J1939_ENUM_DM14_DM15_H_

typedef enum {
	/* Status */
	STATUS_DM15_PROCEED = 0x0,
	STATUS_DM15_BUSY = 0x1,
	STATUS_DM15_OPERATION_COMPLETED = 0x4,
	STATUS_DM15_OPERATION_FAILED = 0x5,
	/* EDCP extesion */
	EDCP_EXTENSION_ALL_EDC_PARAMETER_BEEN_SENT = 0x0,
	EDCP_EXTENSION_CONCATENATE_FOLLOWING_DATA_HIGHER_ORDER = 0x2,
	EDCP_EXTENSION_CONCATENATE_FOLLOWING_DATA_LOWER_ORDER = 0x3,
	EDCP_EXTENSION_DATA_IN_EDC_PARAMETER_IS_ERROR_INDICATOR = 0x4,
	EDCP_EXTENSION_DATA_IN_EDC_PARAMETER_IS_ERROR_INDICATOR_SEED_IS_TIME_TO_COMPLETE = 0x7,
	EDCP_EXTENSION_NOT_USED = 0xFF,
	/* EDC parameter */
	EDC_PARAMETER_PROCESSING_ERASE_REQUEST = 0x10,
	EDC_PARAMETER_PROCESSING_READ_REQUEST = 0x11,
	EDC_PARAMETER_PROCESSING_WRITE_REQUEST = 0x12,
	EDC_PARAMETER_PROCESSING_STATUS_REQUEST = 0x13,
	EDC_PARAMETER_PROCESSING_BOOT_LOAD_REQUEST = 0x16,
	EDC_PARAMETER_NOT_VERIFY_RAM_WRITE = 0x21,
	EDC_PARAMETER_NOT_VERIFY_FLASH_WRITE = 0x22,
	EDC_PARAMETER_NOT_VERIFY_EEPROM_WRITE = 0x23,
	EDC_PARAMETER_INVALID_KEY = 0x1003,
	/* Seed */
	SEED_NO_MORE_KEYS_NEED_FOR_THE_PROCESS = 0x0,
	SEED_USE_LONG_KEY = 0x1,
	SEED_NO_KEY_USED = 0xFFFF
} ENUM_DM15_CODES;

typedef enum {
	/* Command */
	COMMAND_DM14_READ = 0x1,
	COMMAND_DM14_WRITE = 0x2,
	COMMAND_DM14_OPERATION_COMPLETED = 0x4,
	COMMAND_DM14_OPERATION_FAILED = 0x5,
	COMMAND_DM14_BOOT_LOAD = 0x6,
	/* Pointer type */
	POINTER_TYPE_JOIN_POINTER_WITH_POINTER_EXTENSION = 0x0,
	POINTER_TYPE_POINTER_EXTENSION_IS_A_COMMAND = 0x1,
	/* Pointer extension */
	POINTER_EXTENSION_DM14_FLASH_ACCESS = 0x0,
	POINTER_EXTENSION_DM14_EEPROM_ACCESS = 0x1,
	POINTER_EXTENSION_DM14_VARIABLE_ACCESS = 0x2,
	USER_KEY_LEVEL_DM14_NO_KEY_AVAILABLE = 0xFFFF
}ENUM_DM14_CODES;

#endif /* SAE_J1939_ENUMS_SAE_J1939_ENUM_DM14_DM15_H_ */
