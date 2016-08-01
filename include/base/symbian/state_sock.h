/*
 * state_sock.h
 *
 *  Created on: 2010-4-9
 *      Author: Administrator
 */

#ifndef STATE_SOCK_H_
#define STATE_SOCK_H_

#define KISINIT       0x01           //object is initialized
#define KISCONNECTED  0x02           //socket is connected
#define KISSTART      0x04           //object is started
#define KERROR        0x80           
#define KCONNECTERR   0x90
#define KREADERR      0xA0
#define KWRITEERR     0xA0
#define KINITERR      0xC0
#define KDISCONNECTERR 0x08          //go wrong in the object stop

#endif /* STATE_SOCK_H_ */
