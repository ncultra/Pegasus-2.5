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
  \file datetime.c
  \brief Native CMPIDateTime implementation.

  This is the native CMPIDateTime implementation as used for remote
  providers. It reflects the well-defined interface of a regular
  CMPIDateTime, however, it works independently from the management broker.

  It is part of a native broker implementation that simulates CMPI data
  types rather than interacting with the entities in a full-grown CIMOM.

  \author Frank Scheffler
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef PEGASUS_PLATFORM_ZOS_ZSERIES_IBM
#define _ALL_SOURCE
#endif
#include <sys/time.h>
#ifdef PEGASUS_PLATFORM_ZOS_ZSERIES_IBM
#undef _ALL_SOURCE
#endif
#include "mm.h"
#include "native.h"

#ifdef PEGASUS_PLATFORM_ZOS_ZSERIES_IBM
#define atoll(X) strtoll(X, NULL, 10)
#endif
//! Native extension of the CMPIDateTime data type.
/*!
  This structure stores the information needed to represent time for
  CMPI providers.
 */
struct native_datetime {
	CMPIDateTime dt;	/*!< the inheriting data structure  */
	int mem_state;		/*!< states, whether this object is
				  registered within the memory mangagement or
				  represents a cloned object */

	CMPIUint64 msecs;	/*!< microseconds since 01/01/1970 00:00  */
	CMPIBoolean interval;	/*!< states if the date-time object is to be
				  treated as an interval or as absolute time */
};


static struct native_datetime * __new_datetime ( int,
						 CMPIUint64,
						 CMPIBoolean,
						 CMPIStatus * );

/****************************************************************************/

//! Releases a previously cloned CMPIDateTime object from memory.
/*!
  To achieve this, the object is simply added to the thread-based memory
  management to be freed later.

  \param dt the object to be released

  \return CMPI_RC_OK.
 */
static CMPIStatus __dtft_release ( CMPIDateTime * dt )
{
	struct native_datetime * ndt = (struct native_datetime *) dt;

	if ( ndt->mem_state == TOOL_MM_NO_ADD ) {

		ndt->mem_state = TOOL_MM_ADD;
		tool_mm_add ( ndt );

		CMReturn ( CMPI_RC_OK );
	}

	CMReturn ( CMPI_RC_ERR_FAILED );
}


//! Clones an existing CMPIDateTime object.
/*!
  The function simply calls __new_datetime() with the data fields
  extracted from dt.

  \param dt the object to be cloned
  \param rc return code pointer

  \return a copy of the given CMPIDateTime object that won't be freed
  from memory before calling __dtft_release().
 */
static CMPIDateTime * __dtft_clone ( CONST CMPIDateTime * dt, CMPIStatus * rc )
{
	struct native_datetime * ndt   = (struct native_datetime *) dt;
	struct native_datetime * new = __new_datetime ( TOOL_MM_NO_ADD,
							ndt->msecs,
							ndt->interval,
							rc );

	return (CMPIDateTime *) new;
}


//! Extracts the binary time from the encapsulated CMPIDateTime object.
/*!
  \param dt the native CMPIDateTime to be extracted.
  \param rc return code pointer

  \return an amount of microseconds.
 */
static CMPIUint64 __dtft_getBinaryFormat ( CONST CMPIDateTime * dt,
					   CMPIStatus * rc )
{
	struct native_datetime * ndt   = (struct native_datetime *) dt;

	if ( rc ) CMSetStatus ( rc, CMPI_RC_OK );
	return ndt->msecs;
}


//! Gives back a string representation of the time object.
/*!
  \param dt the native CMPIDateTime to be converted.
  \param rc return code pointer

  \return a string that has one of the following formats:

  - yyyymmddhhmmss.mmmmmmsutc (for absolute times)
  - ddddddddhhmmss.mmmmmm:000 (for time intervals)
 */
static CMPIString * __dtft_getStringFormat ( CONST CMPIDateTime * dt,
					     CMPIStatus * rc )
{
	struct native_datetime * ndt   = (struct native_datetime *) dt;

	time_t secs         = ndt->msecs / 1000000;
        unsigned long usecs = ndt->msecs % 1000000;

	char str_time[26];


	if ( ndt->interval ) {

		unsigned long mins, hrs, days;

		mins  = secs / 60;
		secs %= 60;
		hrs   = mins / 60;
		mins %= 60;
		days  = hrs / 24;
		hrs  %= 24;

		sprintf ( str_time, "%8.8ld%2.2ld%2.2ld%2.2ld.%6.6ld:000",
			   days, hrs, mins, secs, usecs );

	} else {

		struct tm tm_time;
		char us_utc_time[11];

		if ( localtime_r ( &secs, &tm_time ) == NULL ) {

			if ( rc ) CMSetStatus ( rc, CMPI_RC_ERR_FAILED );
			return NULL;
		}

		tzset ();
#ifdef PEGASUS_PLATFORM_ZOS_ZSERIES_IBM
		sprintf ( us_utc_time, "%6.6ld%+4.3ld",
			   usecs, ( daylight != 0 ) * 60 - timezone / 60 );
#else
		snprintf ( us_utc_time, 11, "%6.6ld%+4.3ld",
			   usecs, ( daylight != 0 ) * 60 - timezone / 60 );
#endif
		strftime ( str_time, 26, "%Y%m%d%H%M%S.", &tm_time );

		strcat ( str_time, us_utc_time );
	}


	return native_new_CMPIString ( str_time, rc );
}


//! States, whether the time object represents an interval.
/*!
  \param dt the native CMPIDateTime to be checked.
  \param rc return code pointer

  \return zero, if it is an absolute time, non-zero for intervals.
 */
static CMPIBoolean __dtft_isInterval ( CONST CMPIDateTime * dt, CMPIStatus * rc )
{
	struct native_datetime * ndt   = (struct native_datetime *) dt;

	if ( rc ) CMSetStatus ( rc, CMPI_RC_OK );
	return ndt->interval;
}


//! Creates a new native_datetime object.
/*!
  The newly allocated object's function table is initialized to point
  to the native functions in this file.

  \param mm_add TOOL_MM_ADD for a regular object, TOOL_MM_NO_ADD for
  cloned ones
  \param msecs the binary time to be stored
  \param interval the interval flag to be stored
  \param rc return code pointer

  \return a fully initialized native_datetime object pointer.
 */
static struct native_datetime * __new_datetime ( int mm_add,
						 CMPIUint64 msecs,
						 CMPIBoolean interval,
						 CMPIStatus * rc )
{
	static CMPIDateTimeFT dtft = {
		NATIVE_FT_VERSION,
		__dtft_release,
		__dtft_clone,
		__dtft_getBinaryFormat,
		__dtft_getStringFormat,
		__dtft_isInterval
	};

	static CMPIDateTime dt = {
		"CMPIDateTime",
		&dtft
	};

	struct native_datetime * ndt =
		(struct native_datetime *)
		tool_mm_alloc ( mm_add, sizeof ( struct native_datetime ) );

	ndt->dt        = dt;
	ndt->mem_state = mm_add;
	ndt->msecs     = msecs;
	ndt->interval  = interval;

	if ( rc ) CMSetStatus ( rc, CMPI_RC_OK );
	return ndt;
}


//! Creates a native CMPIDateTime representing the current time.
/*!
  This function calculates the current time and stores it within
  a new native_datetime object.

  \param rc return code pointer

  \return a pointer to a native CMPIDateTime.
 */
CMPIDateTime * native_new_CMPIDateTime ( CMPIStatus * rc )
{
	struct timeval tv;
	struct timezone tz;
	CMPIUint64 msecs;

	gettimeofday ( &tv, &tz );

	msecs = (CMPIUint64) 1000000 * (CMPIUint64) tv.tv_sec
		+ (CMPIUint64) tv.tv_usec;

	return (CMPIDateTime *) __new_datetime ( TOOL_MM_ADD,
						 msecs,
						 0,
						 rc );
}


//! Creates a native CMPIDateTime given a fixed binary time.
/*!
  This calls is simply passed on to __new_datetime().

  \param time fixed time-stamp in microseconds
  \param interval states, if the time-stamp is to be treated as interval
  \param rc return code pointer

  \return a pointer to a native CMPIDateTime.

  \sa __dtft_getBinaryFormat()
 */
CMPIDateTime * native_new_CMPIDateTime_fromBinary ( CMPIUint64 time,
						    CMPIBoolean interval,
						    CMPIStatus * rc )
{
	return (CMPIDateTime *) __new_datetime ( TOOL_MM_ADD,
						 time,
						 interval,
						 rc );
}


//! Creates a native CMPIDateTime given a fixed time in string representation.
/*!
  This function assumes the given string to have one of the following formats:

  - for absolute times: yyyymmddhhmmss.mmmmmmsutc
  - for time intervals: ddddddddhhmmss.mmmmmm:000

  \param string the time to be converted into internal representation
  \param rc return code pointer

  \return a pointer to a native CMPIDateTime.

  \sa __dtft_getStringFormat()
 */
CMPIDateTime * native_new_CMPIDateTime_fromChars ( const char * string,
						   CMPIStatus * rc )
{
	CMPIUint64 msecs;
	CMPIBoolean interval = ( string[21] == ':' );
	char * str = strdup ( string );

	str[21] = 0;
	msecs  = atoll ( str + 15 );
	str[14] = 0;
	msecs += atoll ( str + 12 ) * 1000000;
	str[12] = 0;
	msecs += atoll ( str + 10 ) * 1000000 * 60;
	str[10] = 0;
	msecs += atoll ( str + 8 )  * 1000000 * 60 * 60;
	str[8]  = 0;

	if ( interval ) {

		msecs += atoll ( str )  * 1000000 * 60 * 60 * 24;

	} else {
		struct tm tmp;

		memset ( &tmp, 0, sizeof ( struct tm ) );
		tzset ();
#ifndef PEGASUS_PLATFORM_ZOS_ZSERIES_IBM
		tmp.tm_gmtoff = timezone;
#endif
		tmp.tm_isdst  = daylight;
		tmp.tm_mday   = atoi ( str + 6 );
		str[6] = 0;
		tmp.tm_mon    = atoi ( str + 4 ) - 1;
		str[4] = 0;
		tmp.tm_year   = atoi ( str ) - 1900;

		msecs += (CMPIUint64) mktime ( &tmp ) * 1000000;
	}

	free ( str );

	return (CMPIDateTime *)
		__new_datetime ( TOOL_MM_ADD, msecs, interval, rc );
}


/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
