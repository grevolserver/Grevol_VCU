/*
 * can.h
 *
 *  Created on: 27-Feb-2025
 *      Author: richa
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "main.h"

extern FDCAN_TxHeaderTypeDef Tx_header;
extern FDCAN_RxHeaderTypeDef Rx_header;
extern FDCAN_HandleTypeDef hfdcan1;

void CAN_init(void);
void CAN_Tx(uint32_t msg_id, uint8_t *data);
void CAN_Rx(void);
void Charger_CAN_config(void);

#endif /* INC_CAN_H_ */
