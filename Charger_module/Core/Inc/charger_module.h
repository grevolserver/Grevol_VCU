/*
 * Charger_module.h
 *
 *  Created on: 27-Feb-2025
 *      Author: richa
 */

#ifndef CHARGER_MODULE_H_
#define CHARGER_MODULE_H_

#include "main.h"
#include "can.h"
#include <math.h>

#define CHARGER_ID 	0x10FD151A
#define BMS_ID 		0x18FF005B


typedef enum{
	SYNC_STATE = 0,
	DEM_58V_50A,
	DEM_58V_40A,
	DEM_58V_30A,
	ERR_STATE
}bms_state;

typedef struct{
	uint8_t chg_status;
	uint8_t out_volt_set_point_low;
	uint8_t out_volt_set_point_high;
	uint8_t out_curr_set_point_low;
	uint8_t out_curr_set_point_high;
	uint8_t reserve_b1;
	uint8_t reserve_b2;
	uint8_t reserve_b3;
}bms_to_charger;

typedef struct{
	uint8_t bat_chg_status;
	uint8_t bat_chg_out_volt_low;
	uint8_t bat_chg_out_volt_high;
	uint8_t bat_chg_out_curr_low;
	uint8_t bat_chg_out_curr_high;
	uint8_t bat_chg_comm_status;
	uint8_t reserve_b2;
	uint8_t reserve_b3;
}charger_to_bms;

void Format_Bms_Msg(uint8_t *data, bms_to_charger msg);
void Bms_Demand(uint8_t status, uint8_t curr, uint8_t vol);
uint16_t Read_Current(charger_to_bms data);
uint16_t Read_Voltage(charger_to_bms data);
charger_to_bms Charger_response(void);


#endif /* CHARGER_MODULE_H_ */
