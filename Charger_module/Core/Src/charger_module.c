/*
 * Charger_module.c
 *
 *  Created on: 27-Feb-2025
 *      Author: richa
 */

#include "charger_module.h"

uint8_t Tx_data[8];
extern uint8_t Rx_data[8];


uint8_t set_curr_off = 0;
float set_curr_fact = 0.01;
int rec_curr_off = -1600;
float rec_curr_fact = 0.05;

uint8_t set_vol_off = 0;
float set_vol_fact = 0.05;
int rec_vol_off = 0;
float rec_vol_fact = 0.05;

/*Function to Format the BMS message to transmit over CAN*/
void Format_Bms_Msg(uint8_t *data, bms_to_charger msg)
{
	data[0] = msg.chg_status;
	data[1] = msg.out_volt_set_point_low;
	data[2] = msg.out_volt_set_point_high;
	data[3] = msg.out_curr_set_point_low;
	data[4] = msg.out_curr_set_point_high;
	data[5] = msg.reserve_b1;
	data[6] = msg.reserve_b2;
	data[7] = msg.reserve_b3;
}

/*Function to Demand Current and Voltage from Charger*/
void Bms_Demand(uint8_t status, uint8_t curr, uint8_t vol)
{
	uint8_t set_curr_low = ((curr - set_curr_off)/set_curr_fact);
	uint8_t set_curr_high = ((uint16_t)((curr - set_curr_off)/set_curr_fact) >> 8);

	uint8_t set_vol_low = ((vol - set_vol_off)/set_vol_fact);
	uint8_t set_vol_high = ((uint16_t)((vol - set_vol_off)/set_vol_fact) >> 8);

	bms_to_charger tx_msg = {status,set_vol_low,set_vol_high,set_curr_low,set_curr_high,0x00,0x00,0x00};
	Format_Bms_Msg(Tx_data, tx_msg);
	CAN_Tx(BMS_ID, Tx_data);
}

/*Response of the Charger to the Demand by BMS*/
charger_to_bms Charger_response(void)
{
	charger_to_bms rx_msg;
	if(Rx_header.Identifier == CHARGER_ID)
	{
		rx_msg.bat_chg_status 			=	Rx_data[0];
		rx_msg.bat_chg_out_volt_low 	=	Rx_data[1];
		rx_msg.bat_chg_out_volt_high	=	Rx_data[2];
		rx_msg.bat_chg_out_curr_low		=	Rx_data[3];
		rx_msg.bat_chg_out_curr_high	=	Rx_data[4];
		rx_msg.bat_chg_comm_status		=	Rx_data[5];
		rx_msg.reserve_b2				=	Rx_data[6];
		rx_msg.reserve_b3				=	Rx_data[7];
	}
	return rx_msg;
}

/*Read the Current Transmitted by Charger over CAN*/
uint16_t Read_Current(charger_to_bms data)
{
	double curr = (((data.bat_chg_out_curr_high << 8) | (data.bat_chg_out_curr_low)) * rec_curr_fact + rec_curr_off);
	return (uint16_t)round(curr);
}

/*Read the Voltage Transmitted by Charger over CAN*/
uint16_t Read_Voltage(charger_to_bms data)
{
	double vol = (((data.bat_chg_out_volt_high << 8) | (data.bat_chg_out_volt_low)) * rec_vol_fact + rec_vol_off);
	return (uint16_t)round(vol);
}

