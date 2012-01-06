/*  vi: tabstop=3
 *
 *  json.c:  JSON Formatter for Darik's Wipe.
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

char* dwipe_get_info_json( void )
{
	char* tmp;
	json_object* jdwipe = json_object_new_object();
	json_object* jinfo  = json_object_new_object();

	json_object_object_add( jinfo, "entropy", json_object_new_string( "Linux Kernel (urandom)" ) );
	json_object_object_add( jinfo, "prng", json_object_new_string( dwipe_options.prng->label ) );
	json_object_object_add( jinfo, "method", json_object_new_string( dwipe_method_label( dwipe_options.method ) ) );
	json_object_object_add( jinfo, "verify", json_object_new_int( dwipe_options.verify ) );
	json_object_object_add( jinfo, "rounds", json_object_new_int( dwipe_options.rounds ) );
	asprintf(&tmp, "%s", dwipe_runtime ? dwipe_runtime : "" );
	json_object_object_add( jinfo, "runtime", json_object_new_string( tmp ) );
	asprintf(&tmp, "%s", dwipe_remaining ? dwipe_remaining : "" );
	json_object_object_add( jinfo, "remaining", json_object_new_string( tmp ) );
	asprintf(&tmp, "%s", dwipe_loadavg ? dwipe_loadavg : "" );
	json_object_object_add( jinfo, "load_avg", json_object_new_string( tmp ) );
	json_object_object_add( jinfo, "throughput", json_object_new_double( dwipe_throughput ) );
	json_object_object_add( jinfo, "errors", json_object_new_double( dwipe_errors ) );
	json_object_object_add( jinfo, "total_disks", json_object_new_int( dwipe_enumerated ) );
	json_object_object_add( jinfo, "wiping_disks", json_object_new_int( dwipe_selected ) );

	json_object_object_add( jdwipe, "info", jinfo );
	free( tmp );

        return json_object_to_json_string( jdwipe );
}

char* dwipe_get_status_json( dwipe_context_t* context )
{
	json_object* jdwipe = json_object_new_object();
	json_object* jdisks = json_object_new_object();

	int i = 0;

        if ( context != NULL && sizeof( context ) > 0 )
        {
                for( ; i < sizeof( context ) + 1; i++ )
                {
                        if( context[i].device_name == NULL )
                        {
                                continue;
                        }

			json_object* jdisk = json_object_new_object();
			json_object* jdevice = json_object_new_object();
			json_object* jround = json_object_new_object();
			json_object* jpass = json_object_new_object();

			json_object_object_add( jdisk, "select", json_object_new_int( context[i].select ) );
			json_object_object_add( jdisk, "label", json_object_new_string( context[i].label ) );
			json_object_object_add( jdisk, "eta", json_object_new_double( context[i].eta ) );
			json_object_object_add( jdisk, "block_size", json_object_new_int( context[i].block_size ) );
			json_object_object_add( jdisk, "sector_size", json_object_new_int( context[i].sector_size ) );
			json_object_object_add( jdisk, "sync_status", json_object_new_int( context[i].sync_status ) );
			json_object_object_add( jdisk, "throughput", json_object_new_double( context[i].throughput ) );
			json_object_object_add( jdisk, "verify_errors", json_object_new_double( context[i].verify_errors ) );

			json_object_object_add( jdevice, "bus", json_object_new_int( context[i].device_bus ) );
			json_object_object_add( jdevice, "fd", json_object_new_int( context[i].device_fd ) );
			json_object_object_add( jdevice, "host", json_object_new_int( context[i].device_host ) );
			json_object_object_add( jdevice, "lun", json_object_new_int( context[i].device_lun ) );
			json_object_object_add( jdevice, "major", json_object_new_int( context[i].device_major ) );
			json_object_object_add( jdevice, "minor", json_object_new_int( context[i].device_minor ) );
			json_object_object_add( jdevice, "part", json_object_new_int( context[i].device_part ) );
			json_object_object_add( jdevice, "size", json_object_new_double( context[i].device_size ) );
			json_object_object_add( jdevice, "type", json_object_new_int( context[i].device_type ) );
			json_object_object_add( jdisk, "device", jdevice );

			json_object_object_add( jpass, "count", json_object_new_int( context[i].pass_count ) );
			json_object_object_add( jpass, "done", json_object_new_double( context[i].pass_done ) );
			json_object_object_add( jpass, "errors", json_object_new_double( context[i].pass_errors ) );
			json_object_object_add( jpass, "size", json_object_new_double( context[i].pass_size ) );
			json_object_object_add( jpass, "type", json_object_new_int( context[i].pass_type ) );
			json_object_object_add( jpass, "working", json_object_new_int( context[i].pass_working ) );
			json_object_object_add( jdisk, "pass" , jpass );

			json_object_object_add( jround, "count", json_object_new_int( context[i].round_count ) );
			json_object_object_add( jround, "done", json_object_new_double( context[i].round_done ) );
			json_object_object_add( jround, "errors", json_object_new_double( context[i].round_errors ) );
			json_object_object_add( jround, "size", json_object_new_double( context[i].round_size ) );
			json_object_object_add( jround, "percent", json_object_new_double( context[i].round_percent ) );
			json_object_object_add( jround, "working", json_object_new_int( context[i].round_working ) );
			json_object_object_add( jdisk, "round", jround );

			json_object_object_add( jdisks, context[i].device_name, jdisk );
                }
        }

	json_object_object_add( jdwipe, "disks", jdisks );
        return json_object_to_json_string( jdwipe );
}
