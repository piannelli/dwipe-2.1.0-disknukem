/*  vi: tabstop=3
 *
 *  notify.h: Notifier for Darik's Wipe.
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
#include "logging.h"
#include <curl/curl.h>

/* The global options struct */
dwipe_options_t dwipe_options;

size_t dwipe_notify_do_nothing( char *ptr, size_t size, size_t nmemb, void *userdata);

int dwipe_notify_curl( char* address );
int dwipe_notify_start( void );
int dwipe_notify_success( void );
int dwipe_notify_fail( void );
