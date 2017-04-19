/* Copyright 2017 Julian Ingram
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * 	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "p32mz2048efg100.h"

#define PIC_CFG_SEC(reg, sec, val) \
    (((val) << _ ## reg ## _ ## sec ## _POSITION) | \
    (~(unsigned long int)_ ## reg ## _ ## sec ## _MASK))

/* PGEC2/PGED2 */
#define DEVCFG0_VAL (_DEVCFG0_w_MASK & \
	PIC_CFG_SEC(DEVCFG0, DEBUG, 0x03) & \
	PIC_CFG_SEC(DEVCFG0, JTAGEN, 0x01) & \
	PIC_CFG_SEC(DEVCFG0, ICESEL, 0x02) & \
	PIC_CFG_SEC(DEVCFG0, TRCEN, 0x00) & \
	PIC_CFG_SEC(DEVCFG0, BOOTISA, 0x01) & \
	PIC_CFG_SEC(DEVCFG0, FECCCON, 0x03) & \
	PIC_CFG_SEC(DEVCFG0, FSLEEP, 0x01) & \
	PIC_CFG_SEC(DEVCFG0, DBGPER, 0x07) & \
	PIC_CFG_SEC(DEVCFG0, SMCLR, 0x01) & \
	PIC_CFG_SEC(DEVCFG0, SOSCGAIN, 0x01) & \
	PIC_CFG_SEC(DEVCFG0, SOSCBOOST, 0x00) & \
	PIC_CFG_SEC(DEVCFG0, POSCGAIN, 0x01) & \
	PIC_CFG_SEC(DEVCFG0, POSCBOOST, 0x00) & \
	PIC_CFG_SEC(DEVCFG0, EJTAGBEN, 0x00))

/* EC mode, SPLL */
#define DEVCFG1_VAL (_DEVCFG1_w_MASK & \
	PIC_CFG_SEC(DEVCFG1, FNOSC, 0x01) & \
	PIC_CFG_SEC(DEVCFG1, DMTINTV, 0x01) & \
	PIC_CFG_SEC(DEVCFG1, FSOSCEN, 0x00) & \
	PIC_CFG_SEC(DEVCFG1, IESO, 0x00) & \
	PIC_CFG_SEC(DEVCFG1, POSCMOD, 0x00) & \
	PIC_CFG_SEC(DEVCFG1, OSCIOFNC, 0x01) & \
	PIC_CFG_SEC(DEVCFG1, FCKSM, 0x00) & \
	PIC_CFG_SEC(DEVCFG1, WDTPS, 0x0a) & \
	PIC_CFG_SEC(DEVCFG1, WDTSPGM, 0x01) & \
	PIC_CFG_SEC(DEVCFG1, WINDIS, 0x01) & \
	PIC_CFG_SEC(DEVCFG1, FWDTEN, 0x00) & \
	PIC_CFG_SEC(DEVCFG1, FWDTWINSZ, 0x01) & \
	PIC_CFG_SEC(DEVCFG1, DMTCNT, 0x00) & \
	PIC_CFG_SEC(DEVCFG1, FDMTEN, 0x00))

/* POSC as input, PLL div 3, input range 5-10 MHz, mul 50, div 2 */
#define DEVCFG2_VAL (_DEVCFG2_w_MASK & \
	PIC_CFG_SEC(DEVCFG2, FPLLIDIV, 0x02) & \
	PIC_CFG_SEC(DEVCFG2, FPLLRNG, 0x01) & \
	PIC_CFG_SEC(DEVCFG2, FPLLICLK, 0x00) & \
	PIC_CFG_SEC(DEVCFG2, FPLLMULT, 0x31) & \
	PIC_CFG_SEC(DEVCFG2, FPLLODIV, 0x00) & \
	PIC_CFG_SEC(DEVCFG2, UPLLFSEL, 0x01))

/* USB pins controlled by port function, allow 1 peripheral reconfiguration */
#define DEVCFG3_VAL (_DEVCFG3_w_MASK & \
	PIC_CFG_SEC(DEVCFG3, USERID, 0x2424) & \
	PIC_CFG_SEC(DEVCFG3, FMIIEN, 0x01) & \
	PIC_CFG_SEC(DEVCFG3, FETHIO, 0x01) & \
	PIC_CFG_SEC(DEVCFG3, PGL1WAY, 0x01) & \
	PIC_CFG_SEC(DEVCFG3, PMDL1WAY, 0x01) & \
	PIC_CFG_SEC(DEVCFG3, IOL1WAY, 0x01) & \
	PIC_CFG_SEC(DEVCFG3, FUSBIDIO, 0x00))

#define DEVCP0_VAL (_DEVCP0_w_MASK)

#define SEQ_VAL (_SEQ3_w_MASK)
/* the MSB of this reg is hardwired to 0, this value enables code verification to succeed */
#define DEVSIGN_VAL (0x7ffffffful)

const unsigned long int DEVCFG3_LA __attribute__((section(".devcfg3_la"))) = DEVCFG3_VAL;
const unsigned long int ADEVCFG3_LA __attribute__((section(".adevcfg3_la"))) = DEVCFG3_VAL;
const unsigned long int DEVCFG3_B1 __attribute__((section(".devcfg3_b1"))) = DEVCFG3_VAL;
const unsigned long int ADEVCFG3_B1 __attribute__((section(".adevcfg3_b1"))) = DEVCFG3_VAL;

const unsigned long int DEVCFG2_LA __attribute__ ((section(".devcfg2_la"))) = DEVCFG2_VAL;
const unsigned long int ADEVCFG2_LA __attribute__ ((section(".adevcfg2_la"))) = DEVCFG2_VAL;
const unsigned long int DEVCFG2_B1 __attribute__ ((section(".devcfg2_b1"))) = DEVCFG2_VAL;
const unsigned long int ADEVCFG2_B1 __attribute__ ((section(".adevcfg2_b1"))) = DEVCFG2_VAL;

const unsigned long int DEVCFG1_LA __attribute__ ((section(".devcfg1_la"))) = DEVCFG1_VAL;
const unsigned long int ADEVCFG1_LA __attribute__ ((section(".adevcfg1_la"))) = DEVCFG1_VAL;
const unsigned long int DEVCFG1_B1 __attribute__ ((section(".devcfg1_b1"))) = DEVCFG1_VAL;
const unsigned long int ADEVCFG1_B1 __attribute__ ((section(".adevcfg1_b1"))) = DEVCFG1_VAL;

const unsigned long int DEVCFG0_LA __attribute__ ((section(".devcfg0_la"))) = DEVCFG0_VAL;
const unsigned long int ADEVCFG0_LA __attribute__ ((section(".adevcfg0_la"))) = DEVCFG0_VAL;
const unsigned long int DEVCFG0_B1 __attribute__ ((section(".devcfg0_b1"))) = DEVCFG0_VAL;
const unsigned long int ADEVCFG0_B1 __attribute__ ((section(".adevcfg0_b1"))) = DEVCFG0_VAL;

const unsigned long int DEVCP0_LA	__attribute__ ((section(".devcp0_la"))) = DEVCP0_VAL;
const unsigned long int ADEVCP0_LA __attribute__ ((section(".adevcp0_la"))) = DEVCP0_VAL;
const unsigned long int DEVCP0_B1	__attribute__ ((section(".devcp0_b1"))) = DEVCP0_VAL;
const unsigned long int ADEVCP0_B1 __attribute__ ((section(".adevcp0_b1"))) = DEVCP0_VAL;

const unsigned long int SEQ_LA[4] __attribute__ ((section(".seq_la"))) = {SEQ_VAL, SEQ_VAL, SEQ_VAL, SEQ_VAL};
const unsigned long int SEQ_B1[4] __attribute__ ((section(".seq_b1"))) = {SEQ_VAL, SEQ_VAL, SEQ_VAL, SEQ_VAL};

const unsigned long int DEVSIGN_LA  __attribute__ ((section(".devsign_la"))) = DEVSIGN_VAL;
const unsigned long int ADEVSIGN_LA __attribute__ ((section(".adevsign_la"))) = DEVSIGN_VAL;
const unsigned long int DEVSIGN_B1  __attribute__ ((section(".devsign_b1"))) = DEVSIGN_VAL;
const unsigned long int ADEVSIGN_B1 __attribute__ ((section(".adevsign_b1"))) = DEVSIGN_VAL;
const unsigned long int DEVSIGN_B2  __attribute__ ((section(".devsign_b2"))) = DEVSIGN_VAL;
const unsigned long int ADEVSIGN_B2 __attribute__ ((section(".adevsign_b2"))) = DEVSIGN_VAL;
