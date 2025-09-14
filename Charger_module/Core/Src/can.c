/*
 * can.c
 *
 *  Created on: 27-Feb-2025
 *      Author: richa
 */

#include "can.h"

uint8_t Rx_data[8];

void CAN_init(void)
{
	if(HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
 	{
		Error_Handler();
 	}

	if(HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
	{
		Error_Handler();
	}
}

/*Function To configure CAN Message Type*/
void Charger_CAN_config(void)
{
	Tx_header.IdType = FDCAN_EXTENDED_ID;
	Tx_header.TxFrameType = FDCAN_DATA_FRAME;
	Tx_header.DataLength = FDCAN_DLC_BYTES_8;
	Tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	Tx_header.BitRateSwitch = FDCAN_BRS_OFF;
	Tx_header.FDFormat = FDCAN_CLASSIC_CAN;
	Tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	Tx_header.MessageMarker = 0;
}

/*Function to transmit data over CAN*/
void CAN_Tx(uint32_t msg_id, uint8_t *data)
{
	Tx_header.Identifier = msg_id;

	if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &Tx_header, data) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_Delay(250);
}

/*Function to store data received over CAN*/
void CAN_Rx(void)
{
	if(HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &Rx_header, Rx_data) != HAL_OK)
	{
		Error_Handler();
	}

}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	{
		CAN_Rx();
	}

}

