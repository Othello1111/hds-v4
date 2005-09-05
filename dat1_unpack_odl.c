#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "hds1.h"                /* Global definitions for HDS              */
#include "rec.h"                 /* Public rec_ definitions                 */
#include "dat1.h"                /* Internal dat_ definitions               */
#include "dat_err.h"             /* DAT__ error code definitions            */

int
dat1_unpack_odl( const unsigned char *podl, struct ODL *odl )
{
/*+                                                                         */
/* Name:                                                                    */
/*    dat1_unpack_odl                                                       */

/* Purpose:                                                                 */
/*    Unpack an Object Descriptor Label from an Object Record.              */

/* Invocation:                                                              */
/*    dat1_unpack_odl( podl, odl )                                          */
/*                                                                          */
/* Description:                                                             */
/*    This function unpacks the Object Descriptor Label information stored  */
/*    in the Static Domain of an Object Record and puts it into an ODL      */
/*    structure. This is done so that the Static Domain format need not     */
/*    depend on the details of the way that an ODL structure is stored in   */
/*    memory.                                                               */

/* Parameters:                                                              */
/*    const unsigned char *podl                                             */
/*       Pointer to an array of Static Domain chars to be unpacked.         */
/*    struct ODL *odl                                                       */
/*       Pointer to an ODL structure to receive the unpacked information.   */

/* Returned Value:                                                          */
/*    int dat1_unpack_odl                                                   */
/*       The global status value current on exit.                           */

/* Authors:                                                                 */
/*    RFWS: R.F. Warren-Smith (STARLINK)                                    */
/*    BKM:  B.K. McIlwrath    (STARLINK)                                    */
/*    {@enter_new_authors_here@}                                            */

/* History:                                                                 */
/*    11-APR-1991 (RFWS):                                                   */
/*       Original version.                                                  */
/*    12-JUN-1991 (RFWS):                                                   */
/*       Changed unpacking of naxes field to reflect a new internal type of */
/*       int.                                                               */
/*    06-MAY-2004: (BKM)                                                    */
/*       Modify to cope with either 32 or 64-bit axes dimension addressing  */
/*    {@enter_further_changes_here@}                                        */

/* Bugs:                                                                    */
/*    {@note_any_bugs_here@}                                                */

/*-                                                                         */

/* Local Variables:                                                         */
   int i;                        /* Loop counter; unpacked array index      */
   int j;                        /* Loop counter; packed array index        */
   int naxes;                    /* Number of object axes                   */

/*.                                                                         */

/* Check the inherited global status.                                       */
   if ( !_ok( hds_gl_status ) ) return hds_gl_status;

/* Extract the first 15 chars which contain the ODL type field.             */
   for ( i = 0; i < 15; i++ )
   {
      odl->type[ i ] = *( (const char *) ( podl + i ) );
   }

/* Extract the number of axes from the next char.                           */
   odl->naxes = podl[ 15 ];

/* Ensure that the number of axes is in range.                              */
   naxes = odl->naxes < DAT__MXDIM ? odl->naxes : DAT__MXDIM;

/* Loop to unpack the axis sizes, each of which occupies a further 4 or 8   */
/* chars depending on HDS version                                           */
#if !defined(HDS_64)
      for ( i = 0, j = 16; i < naxes; i++, j += 4 )
      {
         odl->axis[ i ] = 0;
         odl->axis[ i ] = ( ( ( ( ( podl[ j + 3 ] << 8 ) |
                                    podl[ j + 2 ] ) << 8 ) |
                                    podl[ j + 1 ] ) << 8 ) |
                                    podl[ j ];
      }
#else
      for ( i = 0, j = 16; i < naxes; i++, j += 8 )
      {
         odl->axis[ i ] = ((((((((((((( podl[ j + 7 ] << 8 ) |
                                        podl[ j + 6 ] ) << 8 ) |
                                        podl[ j + 5 ] ) << 8 ) |
                                        podl[ j + 4 ] ) << 8 ) |
                                        podl[ j + 3 ] ) << 8 ) |
                                        podl[ j + 2 ] ) << 8 ) |
                                        podl[ j + 1 ] ) << 8 ) |
                                        podl[ j ];
      }
#endif    

/* Return the current global status value.                                  */
   return hds_gl_status;
}
