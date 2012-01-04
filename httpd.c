/*  vi: tabstop=3
 *
 *  httpd.c:  Web server for Darik's Wipe.
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
#include "httpd.h"

/* The global context */
dwipe_context_t* c1;
dwipe_context_t* c2;

int handle_request( void *cls, struct MHD_Connection *connection,
                          const char *url,
                          const char *method, const char *version,
                          const char *upload_data,
                          size_t *upload_data_size, void **con_cls)
{
	const char *page;

	if( strcmp( url, "/disks/all.xml" ) == 0 )
	{
		page  = dwipe_get_status_xml(c1);
	}
	else if( strcmp( url, "/disks/all.json" ) == 0 )
	{
		page  = dwipe_get_status_json(c1);
	}
	else if( strcmp( url, "/disks/wiping.xml" ) == 0 )
	{
		page  = dwipe_get_status_xml(c2);
	}
	else if( strcmp( url, "/disks/wiping.json" ) == 0 )
	{
		page  = dwipe_get_status_json(c2);
	}
	else
	{
		page = "This server is currently being wiped";
	}

	struct MHD_Response *response;
	int ret;

	response = MHD_create_response_from_buffer (strlen (page),
                                                    (void*) page, MHD_RESPMEM_PERSISTENT);

	ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	MHD_destroy_response (response);

	return ret;
}

int dwipe_start_web_server( void )
{
	struct MHD_Daemon *daemon;

	daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, dwipe_options.web_port, NULL, NULL,
                                   &handle_request, NULL, MHD_OPTION_END);
	if (NULL == daemon)
	{
		fprintf( stderr, "Error: unable to start the web server - '%d'", errno );
		exit ( EINVAL );
	}

}
