/*  vi: tabstop=3
 *
 *  xml.h: XML formatter for Darik's Wipe.
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
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/tree.h>

int dwipe_get_status_xml( dwipe_context_t* context )
{
        int rc, buffer_size;
        int i = 0;
        xmlTextWriterPtr writer;
        xmlDocPtr doc;
        xmlNodePtr node;
        xmlChar *xml_buffer;
        dwipe_context_t* context;

        doc = xmlNewDoc( BAD_CAST XML_DEFAULT_VERSION );
        if( doc == NULL )
        {
                return "Error: cannot create the xml document tree";
        }

        node = xmlNewDocNode( doc, NULL, BAD_CAST "dwipe", NULL );
        if( node == NULL )
        {
                return "Error: cannot create the xml node";
        }

        xmlDocSetRootElement( doc, node );

        writer = xmlNewTextWriterTree( doc, node, 0 );
        if( writer == NULL )
        {
                return "Error: cannot create the xml writer";
        }

        rc = xmlTextWriterStartDocument( writer, NULL, "UTF-8", NULL );
        rc = xmlTextWriterStartElement( writer, BAD_CAST "disks" );

        if ( context != NULL && sizeof( context ) > 0 )
        {
                for( ; i < sizeof( context ) + 1; i++ )
                {
                        if( context[i].device_name == NULL )
                        {
                                continue;
                        }

                        rc = xmlTextWriterStartElement( writer, BAD_CAST "disk" );
                        rc = xmlTextWriterWriteAttribute( writer, BAD_CAST "name", BAD_CAST context[i].device_name );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "select", "%d" , context[i].select );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "label", "%s" , context[i].label );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "eta", "%llu" , context[i].eta );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "block_size" , "%d" , context[i].block_size );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "sector_size", "%d" , context[i].sector_size );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "sync_status", "%d" , context[i].sync_status );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "throughput", "%llu" , context[i].throughput );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "verify_errors", "%llu" , context[i].verify_errors );

                        rc = xmlTextWriterStartElement( writer, BAD_CAST "device" );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "bus" , "%d" , context[i].device_bus );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "fd"  , "%d" , context[i].device_fd );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "host", "%d" , context[i].device_host );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "lun",   "%d" , context[i].device_lun );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "major", "%d" , context[i].device_major );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "minor", "%d" , context[i].device_minor );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "part", "%d" , context[i].device_part );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "size", "%llu" , context[i].device_size );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "type", "%d" , context[i].device_type );
                        rc = xmlTextWriterEndElement( writer );

                        rc = xmlTextWriterStartElement( writer, BAD_CAST "pass" );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "count", "%d" , context[i].pass_count );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "done", "%llu" , context[i].pass_done );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "errors", "%llu" , context[i].pass_errors );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "size", "%llu" , context[i].pass_size );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "type", "%d" , context[i].pass_type );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "working", "%d" , context[i].pass_working );
                        rc = xmlTextWriterEndElement( writer );

                        rc = xmlTextWriterStartElement( writer, BAD_CAST "round" );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "count", "%d" , context[i].round_count );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "done", "%llu" , context[i].round_done );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "errors", "%llu" , context[i].round_errors );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "size", "%llu" , context[i].round_size );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "percent", "%.2f" , context[i].round_percent );
                        rc = xmlTextWriterWriteFormatElement( writer, BAD_CAST "working", "%d" , context[i].round_working );
                        rc = xmlTextWriterEndElement( writer );

                        rc = xmlTextWriterEndElement( writer );
                }
        }

        rc = xmlTextWriterEndDocument( writer );
        xmlFreeTextWriter( writer );
        xmlDocDumpFormatMemory( doc, &xml_buffer, &buffer_size, 1 );
        return ((char *)xml_buffer);
}
