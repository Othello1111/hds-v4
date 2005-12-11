/*
*+
*  Name:
*     hdsTest

*  Purpose:
*     Test the C interface to HDS

*  Language:
*     Starlink ANSI C

*  Description:
*     This program tests some of the C API to HDS. It is not meant
*     to be an exhaustive test of all the API (at least not initially).

*  Copyright:
*     Copyright (C) 2005 Particle Physics and Astronomy Research Council.
*     All Rights Reserved.

*  Authors:
*     TIMJ: Tim Jenness (JAC, Hawaii)
*     {enter_new_authors_here}

*  History:
*     04-NOV-2005 (TIMJ):
*        Original.
*     {enter_further_changes_here}

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public License as
*     published by the Free Software Foundation; either version 2 of
*     the License, or (at your option) any later version.
*
*     This program is distributed in the hope that it will be
*     useful, but WITHOUT ANY WARRANTY; without even the implied
*     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*     PURPOSE. See the GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public
*     License along with this program; if not, write to the Free
*     Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
*     MA 02111-1307, USA

*  Bugs:
*     {note_any_bugs_here}

*-
*/

#if HAVE_CONFIG_H
# include <config.h>
#endif


#if HAVE_FC_MAIN
void FC_MAIN () {}
#endif

#include "hds1.h"
#include "hds.h"
#include <stdlib.h>
#include "ems.h"
#include "dat_err.h"
#include <stdio.h>

int main () {
      
  /*  Local Variables: */
  const char path[] = "hds_ctest";
  int status = DAT__OK;
  hdsdim dim[] = { 10, 20 };
  char *chararr[] = { "TEST1", "TEST2", "Longish String" };
  char *retchararr[3];
  char buffer[1024];  /* plenty large enough */
  double darr[] = { 4.5, 2.5 };
  double retdarr[2];
  HDSLoc * loc1 = NULL;
  HDSLoc * loc2 = NULL;
  HDSLoc * loc3 = NULL;
  size_t actval;
  int i;

  emsBegin(&status);

  /* Force 64-bit mode */
  hdsTune( "64BIT", 1, &status );

  /* Create a new container file */
  hdsNew( path, "HDS_TEST", "NDF", 0, dim, &loc1, &status );

  /* Some components */
  datNew1C( loc1, "ONEDCHAR", 14, 3, &status );
  datNew1D( loc1, "ONEDD", 2, &status );
  
  /* Populate */
  datFind( loc1, "ONEDCHAR", &loc2, &status );
  datPutVC( loc2, 3, chararr, &status );

  /* Check contents */
  datGet1C(loc2, 3, 8, buffer, retchararr, &actval, &status);
  if (status == DAT__OK) {
    if (actval == 3) {
      for (i = 0; i < 3; i++ ) {
	if (strcmp( chararr[i], retchararr[i] ) ) {
           status = DAT__DIMIN;
	   emsSetc( "IN", chararr[i]);
	   emsSetc( "OUT", retchararr[i] );
           emsRep( "GET1C","Values from Get1C differ (^IN != ^OUT)", &status);
           break;
         }
      }
    } else {
      status = DAT__DIMIN;
      emsRep( "GET1C","Did not get back as many strings as put in", &status);
    }
  }

  datAnnul( &loc2, &status );

  
  datFind( loc1, "ONEDD", &loc2, &status );
  datPutVD( loc2, 2, darr, &status );

  /* Check contents */
  datGetVD( loc2, 2, retdarr, &actval, &status);
  if (status == DAT__OK) {
    if (actval == 2) {
      for (i = 0; i < 2; i++ ) {
         if (darr[i] != retdarr[i]) {
           status = DAT__DIMIN;
           emsRep( "GETVD","Values from getVD differ", &status);
           break;
         }
      }
    } else {
      status = DAT__DIMIN;
      emsRep( "GETVD","Did not get back as many values as put in", &status);
    }
  }
  datAnnul( &loc2, &status );

  /* Tidy up and close */
  hdsErase( &loc1, &status );

  if (status == DAT__OK) {
    printf("HDS C installation test succeeded\n");
    emsEnd(&status);
    return EXIT_SUCCESS;
  } else {
    printf("HDS C installation test failed\n");
    emsEnd(&status);
    return EXIT_FAILURE;
  }


}
