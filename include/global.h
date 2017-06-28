/*
 * global.h
 *
 *  Created on: 02.06.2017
 *      Author: David Paul
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include <drivers/bsp.h>
#include "drivers/lcd.h" // also used by Console Module
// #include "drivers/sd.h" used by FAT-Filesystem
// #include "drivers/qspi.h" used by FAT_Filesystem

#include <middleware/fat/fatfs.h>
#include <middleware/graphics/console.h>
#include <middleware/graphics/statusbar.h>

#include <main.h>

#endif /* GLOBAL_H_ */
