#ifndef _A7COMMAND_H
#define _A7COMMAND_H

typedef unsigned char	byte;

// A10 MiniPrinter Control Commands List

// - Executing Cmds
//
const byte cmd_HT[] = { 0x09 };
const byte cmd_LF[] = { 0x0A };
const byte cmd_FF[] = { 0x0C };
const byte cmd_CR[] = { 0x0D };
const byte cmd_CAN[] = { 0x18 };
const byte cmd_DLE_EOT[] = { 0x10, 0x04, 1 };
const byte cmd_ESC_FF[] = { 0x1B, 0x0C };
const byte cmd_ESC_$[] = { 0x1B, 0x24, 10, 10 };
const byte cmd_ESC_star[] = { 0x1B, 0x24, 10, 10 };

const byte cmd_GS_V[] = { 0x1D, 0x56, 0x42, 0x06 };

// - Setting Cmds
//
const byte cmd_ESC_SP[] = { 0x1B, 0x20, 1 };
const byte cmd_ESC_emark[] = { 0x1B, 0x21, 1 };
const byte cmd_ESC_persent[] = { 0x1B, 0x25, 0 };
const byte cmd_ESC_and[] = { 0x1B, 0x26, 3, 1, 1 };



#endif//_A7COMMAND_H
