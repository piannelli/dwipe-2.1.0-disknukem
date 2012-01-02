/*
 *  device.c:  Device routines for dwipe.
 *
 *  Copyright Darik Horn <dajhorn-dban@vanadac.com>.
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

#include <netinet/in.h>

#include "dwipe.h"
#include "context.h"
#include "method.h"
#include "options.h"
#include "identify.h"
#include "scsicmds.h"


static int disk_count = 0;

void dwipe_device_identify( dwipe_context_t* c )
{
	/**
	 * Gets device information and creates the context label.
	 *
	 * @parameter  c         A pointer to a device context.
	 * @modifies   c->label  The menu description that the user sees.
	 *
	 */
	 
	int result;
	char buffer [FILENAME_MAX];
	FILE* fp;
	 
	/* Allocate memory for the label. */
 	c->label = malloc( DWIPE_KNOB_LABEL_SIZE );
 	
 	snprintf( buffer, sizeof(buffer), "%s.label", c->device_name );

	fp = fopen( buffer, "r" );

	if( fp == NULL )
	{
		perror( "dwipe_device_identify: fopen" );
		fprintf( stderr, "Error: Unable to open '%s'.\n", buffer );
		exit( errno );
	}
	
	if( fgets( buffer, sizeof(buffer), fp ) != NULL )
	{
		strncpy( c->label, buffer, DWIPE_KNOB_LABEL_SIZE );
	}
	else
	{
		strncpy( c->label, "Uninitialized Device", DWIPE_KNOB_LABEL_SIZE );
	}
	
	
	
} /* dwipe_device_identify */


int dwipe_device_scan( char*** device_names )
{
	/**
	 * Scans the the filesystem for storage device names.
	 *
	 * @parameter device_names  A reference to a null array pointer.
	 * @modifies  device_names  Populates device_names with an array of strings.
	 * @returns                 The number of strings in the device_names array.
	 *
	 */

	/* The partitions file pointer.  Usually '/proc/partitions'. */
	FILE* fp;

	/* The input buffer. */
	char b [FILENAME_MAX];

	/* Buffer for the major device number. */
	int dmajor;

	/* Buffer for the minor device number. */
	int dminor;

	/* Buffer for the device block count.  */
	loff_t dblocks;

	/* Buffer for the device file name.    */
	char dname [FILENAME_MAX];

	/* Names in the partition file do not have the '/dev/' prefix. */
	char dprefix [] = DWIPE_KNOB_PARTITIONS_PREFIX;

	/* The number of devices that have been found. */
	int dcount = 0;
 
	/* Open the partitions file. */
	fp = fopen( DWIPE_KNOB_PARTITIONS, "r" );

	if( fp == NULL )
	{
		perror( "dwipe_device_scan: fopen" );
		fprintf( stderr, "Error: Unable to open the partitions file '%s'.\n", DWIPE_KNOB_PARTITIONS );
		exit( errno );
	}

	/* Copy the device prefix into the name buffer. */
	strcpy( dname, dprefix );

	/* Sanity check: If device_name is non-null, then it is probably being used. */
	if( *device_names != NULL )
	{
		fprintf( stderr, "Sanity Error: dwipe_device_scan: Non-null device_names pointer.\n" );
		exit( -1 );
	}

	/* Read every line in the partitions file. */
	while( fgets( b, sizeof( b ), fp ) != NULL )
	{
		/* Scan for a device line. */
		if( sscanf( b, "%i %i %lli %s", &dmajor, &dminor, &dblocks, &dname[ strlen( dprefix ) ] ) == 4 )
		{
			/* Increment the device count. */
			dcount += 1;

			/* TODO: Check whether the device numbers are sensible. */

			/* Allocate another name pointer. */
			*device_names = realloc( *device_names, dcount * sizeof(char*) );

			/* Allocate the device name string. */
			(*device_names)[ dcount -1 ] = malloc( strlen( dname ) +1 );
			
			/* Copy the buffer into the device name string. */
			strcpy( (*device_names)[ dcount -1 ], dname );

		} /* if sscanf */

	} /* while fgets */

	/* Pad the array with a null pointer. */
	*device_names = realloc( *device_names, ( dcount +1 ) * sizeof(char*) );
	(*device_names)[ dcount ] = NULL;

	/* Return the number of devices that were found. */
	return dcount;

} /* dwipe_device_scan */

/* eof */
