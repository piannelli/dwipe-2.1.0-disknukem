/*  vi: tabstop=3
 *
 *  notify.c:  Notifier for Darik's Wipe.
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

#include "notify.h"

size_t dwipe_notify_do_nothing( char *ptr, size_t size, size_t nmemb, void *userdata)
{
	return size * nmemb;
}

int dwipe_notify_curl( char* address )
{
	CURL *dwipe_curl;
	CURLcode dwipe_curl_res;

	dwipe_curl = curl_easy_init();

	if( dwipe_curl )
	{
		curl_easy_setopt( dwipe_curl, CURLOPT_URL, address );
		curl_easy_setopt( dwipe_curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt( dwipe_curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt( dwipe_curl, CURLOPT_USERAGENT, _DWIPE_VERSION );
		curl_easy_setopt( dwipe_curl, CURLOPT_HEADER, 0 );
		curl_easy_setopt( dwipe_curl, CURLOPT_WRITEFUNCTION, &dwipe_notify_do_nothing );
		dwipe_curl_res = curl_easy_perform( dwipe_curl );
		curl_easy_cleanup( dwipe_curl );

		if( dwipe_curl_res == CURLE_OK )
		{
		        dwipe_log( DWIPE_LOG_NOTICE, "Notified successfully URL: %s", address );
		}
		else
		{

			dwipe_log( DWIPE_LOG_ERROR, "Failed to notify URL: %s - Error code: %d", address, dwipe_curl_res );
		}

		return dwipe_curl_res;
	}

	return -1;
}

int dwipe_notify_start( void )
{
	if( dwipe_options.notify_start_url )
	{
		return dwipe_notify_curl( dwipe_options.notify_start_url );
	}

	return 0;
}

int dwipe_notify_success( void )
{
	if( dwipe_options.notify_success_url )
	{
		return dwipe_notify_curl( dwipe_options.notify_success_url );
	}

	return 0;
}

int dwipe_notify_fail( void )
{
	if( dwipe_options.notify_fail_url )
	{
		return dwipe_notify_curl( dwipe_options.notify_fail_url );
	}

	return 0;
}
