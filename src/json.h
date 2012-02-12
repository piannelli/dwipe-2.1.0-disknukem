/*  vi: tabstop=3
 *
 *  json.h: JSON formatter for Darik's Wipe.
 *
 *  Copyright Paolo Iannelli <info@paoloiannelli.com>.
 *
 *  This program is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free Software
 *  Foundation, version 2.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along with
 *  this program; if not, write to the Free Software Foundation, Inc., 675 Mass
 *  Ave, Cambridge, MA 02139, USA.
 *
 */

#include "dwipe.h"
#include "context.h"
#include "method.h"
#include "prng.h"
#include "options.h"
#include <json/json.h>

/* The combined number of errors of all processes. */
u64 dwipe_errors;

/* The starting time as string */
char* dwipe_runtime;

/* The remaining time as string */
char* dwipe_remaining;

/* The combined througput of all processes. */
u64 dwipe_throughput;

/* The load average as string */
char* dwipe_loadavg;

/* The number of contexts that have been enumerated. */
int dwipe_enumerated;

/* The number of contexts that have been selected.   */
int dwipe_selected;

const char* dwipe_get_info_json( void );
const char* dwipe_get_status_json( dwipe_context_t* context );
