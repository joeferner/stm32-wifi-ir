#ifndef IR_RX_H_INCLUDED
#define IR_RX_H_INCLUDED

#include "ring_buffer.h"
#include "ir_code.h"

void ir_rx_setup();
IrCode* ir_rx_recv();

#endif // IR_RX_H_INCLUDED
