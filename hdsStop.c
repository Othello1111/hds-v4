#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "ems.h"                 /* EMS error reporting routines            */
#include "hds1.h"                /* Global definitions for HDS              */
#include "rec.h"                 /* Public rec_ definitions                 */
#include "str.h"                 /* Character string import/export macros   */
#include "dat1.h"                /* Internal dat_ definitions               */
#include "dat_err.h"             /* DAT__ error code definitions            */

int
hdsStop( int *status)
{
/*
*+
*  Name:
*     HDS_STOP

*  Purpose:
*     Close down HDS.

*  Language:
*     ANSI C

*  Invocation:
*     CALL HDS_STOP( STATUS )

*  Description:
*     This routine closes down HDS, annulling all active locators,
*     closing all container files and releasing all associated
*     resources.  It returns without action if HDS is not active.

*  Arguments:
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Notes:
*     This routine attempts to execute even if STATUS is set on entry,
*     although no further error report will be made if it subsequently
*     fails under these circumstances.

*  Copyright:
*     Copyright (C) 1992 Science & Engineering Research Council

*  Authors:
*     RFWS: R.F. Warren-Smith (STARLINK, RAL)
*     BKM:  B.K. McIlwrath    (STARLINK, RAL)
*     {enter_new_authors_here}

*  History:
*     4-APR-1991 (RFWS):
*        Added prologue and error handling and made portable.
*     13-DEC-2001 (BKM)
*        Convert to a C function with FORTRAN wrapper.
*     {enter_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-
*/

/* Local Variables:                                                         */
  struct LCP *lcp;               /* Pointer to Locator Control Packet       */

/*.                                                                         */

/* Begin a new error reporting context.                                     */
   ems_begin_c( status );
   hds_gl_status = *status;

/* Check that HDS is active. There is nothing to do if it is not.           */
   if ( hds_gl_active )
   {

/* Defuse all Locator Control Packets.                                      */
      while ( dat_ga_wlq != NULL )
      {
         lcp = dat_ga_wlq;
         dau_defuse_lcp( &lcp );
      }

/* Close down the rec_ facility.                                            */
      rec_stop( );

/* Note that HDS is no longer active.                                       */
      hds_gl_active = 0;

/* If an error occurred, then report contextual information.                */
      if ( !_ok( hds_gl_status ) )
      {
         ems_rep_c( "HDS_STOP_ERR",
                    "HDS_STOP: Error deactivating the Hierarchical Data \
System (HDS).",
                    &hds_gl_status );
      }
   }

/* End the error reporting context.                                         */
   *status = hds_gl_status;
   ems_end_c( status );

/* Exit the routine.                                                        */
   return *status;
}
