//%2005////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000, 2001, 2002 BMC Software; Hewlett-Packard Development
// Company, L.P.; IBM Corp.; The Open Group; Tivoli Systems.
// Copyright (c) 2003 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation, The Open Group.
// Copyright (c) 2004 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation; VERITAS Software Corporation; The Open Group.
// Copyright (c) 2005 Hewlett-Packard Development Company, L.P.; IBM Corp.;
// EMC Corporation; VERITAS Software Corporation; The Open Group.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// THE ABOVE COPYRIGHT NOTICE AND THIS PERMISSION NOTICE SHALL BE INCLUDED IN
// ALL COPIES OR SUBSTANTIAL PORTIONS OF THE SOFTWARE. THE SOFTWARE IS PROVIDED
// "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//==============================================================================
//
// Author: Frank Scheffler
//
// Modified By:  Adrian Schuur (schuur@de.ibm.com)
//
//%/////////////////////////////////////////////////////////////////////////////

/*!
  \file ticket.h
  \brief Broker ticket management header file.

  \author Frank Scheffler
*/

#ifndef _REMOTE_CMPI_TICKET_H
#define _REMOTE_CMPI_TICKET_H

typedef struct comm_ticket comm_ticket;


#ifndef CMPI_VER_100
#define CMPI_VER_100
#endif

#ifndef CONST 
  #ifdef CMPI_VER_100 
  #define CONST const
  #else
  #define CONST 
  #endif
#endif
#include <Pegasus/Provider/CMPI/cmpidt.h>

//! Communication ticket given to remote providers to grant them up-calls.
/*!
  This ticket is being generated by the proxy provider and passed along with
  MI and MB calls to remote providers. This way MB calls can be verified and
  associated to the proper broker handle.
 */
struct comm_ticket {
	unsigned long id;	/*!< ticket id */
	CONST CMPIBroker * broker;	/*!< originating broker */
	struct comm_ticket * next; /*! next element pointer */
};


/****************************************************************************/

CONST CMPIBroker * verify_ticket ( const comm_ticket * ticket );
comm_ticket generate_ticket ( CONST CMPIBroker * broker );
int revoke_ticket ( comm_ticket * ticket );
int compare_ticket ( const comm_ticket *, const comm_ticket * );
void cleanup_ticket ( void );

#endif

/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
