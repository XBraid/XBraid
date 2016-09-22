/*BHEADER**********************************************************************
 * Copyright (c) 2013, Lawrence Livermore National Security, LLC. 
 * Produced at the Lawrence Livermore National Laboratory. Written by 
 * Jacob Schroder, Rob Falgout, Tzanio Kolev, Ulrike Yang, Veselin 
 * Dobrev, et al. LLNL-CODE-660355. All rights reserved.
 * 
 * This file is part of XBraid. Email xbraid-support@llnl.gov for support.
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License (as published by the Free Software
 * Foundation) version 2.1 dated February 1999.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the terms and conditions of the GNU General Public
 * License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 ***********************************************************************EHEADER*/
 

/** \file braid_test.c
 * \brief Define XBraid test routines.
 */

#include "braid.h"
#include "_braid.h"
#include "_util.h"

/*--------------------------------------------------------------------------
 * Some simple tests on the myinit, myaccess and myfree routines
 *--------------------------------------------------------------------------*/
braid_Int
braid_TestInitAccess( braid_App           app, 
                     MPI_Comm            comm_x,
                     FILE               *fp, 
                     braid_Real          t,
                     braid_PtFcnInit     myinit,
                     braid_PtFcnAccess   myaccess,
                     braid_PtFcnFree     myfree)
{
   
   braid_Vector          u ;
   braid_Status          status = _braid_CTAlloc(_braid_Status, 1);
   braid_AccessStatus    astatus = (braid_AccessStatus)status;
   braid_Int             myid_x = 0;
   
   _braid_AccessStatusInit(t, 0, 0.0, 0, 0, 0, 0, 0, 1, -1, astatus);
   MPI_Comm_rank( comm_x, &myid_x );

   /* Print intro */
   _braid_ParFprintfFlush(fp, myid_x, "\nStarting braid_TestInitAccess\n\n");

   /* Test */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestInitAccess:   Starting Test 1\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestInitAccess:   u = myinit(t=%1.2e)\n", t);
   myinit(app, t, &u);
   
   if(myaccess != NULL)
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestInitAccess:   myaccess(u) \n");
      myaccess(app, u, astatus);

      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestInitAccess:   check output: wrote u for initial condition at t=%1.2e. \n\n",t);
   }

   /* Free variables */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestInitAccess:   myfree(u) \n");
   myfree(app, u);
   _braid_StatusDestroy(status);
   
   _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestInitAccess\n");

   return 0;
}

braid_Int
braid_TestClone( braid_App        app,  
              MPI_Comm            comm_x,
              FILE               *fp, 
              braid_Real          t,
              braid_PtFcnInit     myinit,
              braid_PtFcnAccess   myaccess,
              braid_PtFcnFree     myfree,
              braid_PtFcnClone    clone)
{
   
   braid_Vector        u, v;
   braid_Status        status = _braid_CTAlloc(_braid_Status, 1);
   braid_AccessStatus  astatus = (braid_AccessStatus)status;
   braid_Int           myid_x;
   
   _braid_AccessStatusInit(t, 0, 0.0, 0, 0, 0, 0, 0, 1, -1, astatus);
   MPI_Comm_rank( comm_x, &myid_x );

   /* Print intro */
   _braid_ParFprintfFlush(fp, myid_x, "\nStarting braid_TestClone\n\n");

   /* Test 1 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   Starting Test 1\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   u = myinit(t=%1.2e)\n", t);
   myinit(app, t, &u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   v = clone(u)\n");
   clone(app, u, &v);
   
   if(myaccess != NULL)
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   myaccess(u)\n");
      myaccess(app, u, astatus);

      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   myaccess(v)\n");
      myaccess(app, v, astatus);
      
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   check output:  wrote u and v for initial condition at t=%1.2e.\n\n", t);

   }

   /* Free variables */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   myfree(u)\n");
   myfree(app, u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestClone:   myfree(v)\n");
   myfree(app, v);

   _braid_StatusDestroy(status);
   
   _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestClone\n");
   
   return 0;
}



braid_Int
braid_TestSum( braid_App        app, 
            MPI_Comm            comm_x,
            FILE               *fp,
            braid_Real          t,
            braid_PtFcnInit     myinit,
            braid_PtFcnAccess   myaccess,
            braid_PtFcnFree     myfree,
            braid_PtFcnClone    clone,
            braid_PtFcnSum      sum )
{
   
   braid_Vector        u, v;
   braid_Status        status  = _braid_CTAlloc(_braid_Status, 1);
   braid_AccessStatus  astatus = (braid_AccessStatus)status;
   braid_Int           myid_x;
   
   _braid_AccessStatusInit(t, 0, 0.0, 0, 0, 0, 0, 0, 1, -1, astatus);
   MPI_Comm_rank( comm_x, &myid_x );

   /* Print intro */
   _braid_ParFprintfFlush(fp, myid_x, "\nStarting braid_TestSum\n\n");
   
   /* Test 1 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   Starting Test 1\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   u = myinit(t=%1.2e)\n", t);
   myinit(app, t, &u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   v = clone(u)\n");
   clone(app, u, &v);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   v = u - v\n");
   sum(app, 1.0, u, -1.0, v); 

   if(myaccess != NULL)
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   myaccess(v)\n");
      myaccess(app, v, astatus);
      
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   check output:  v should equal the zero vector\n\n");
   }

   /* Test 2 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   Starting Test 2\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   v = 2*u + v\n");
   sum(app, 2.0, u, 1.0, v); 

   if(myaccess != NULL)
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   myaccess(v)\n");
      myaccess(app, v, astatus);
      
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   myaccess(u)\n");
      myaccess(app, u, astatus);
   }

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   check output:  v should equal 2*u \n\n");

   /* Free variables */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   myfree(u)\n");
   myfree(app, u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSum:   myfree(v)\n");
   myfree(app, v);

   _braid_StatusDestroy(status);
   
   _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestSum\n");

   return 0;
}

braid_Int
braid_TestSpatialNorm( braid_App              app, 
                       MPI_Comm               comm_x,
                       FILE                  *fp, 
                       braid_Real             t,
                       braid_PtFcnInit        myinit,
                       braid_PtFcnFree        myfree,
                       braid_PtFcnClone       clone,
                       braid_PtFcnSum         sum,  
                       braid_PtFcnSpatialNorm spatialnorm) 
{   
   braid_Vector  u, v, w;
   braid_Real    result1, result2;
   braid_Int     myid_x, correct;
   double     wiggle = 1e-12;
   
   MPI_Comm_rank( comm_x, &myid_x );

   /* Initialize the flags */
   correct = 1;
   braid_Int zero_flag = 0;

   /* Print intro */
   _braid_ParFprintfFlush(fp, myid_x, "\nStarting braid_TestSpatialNorm\n\n");
   
   /* Test 1 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   Starting Test 1\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   u = myinit(t=%1.2e)\n", t);
   myinit(app, t, &u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   spatialnorm(u) \n");
   spatialnorm(app, u, &result1);
   if( fabs(result1) == 0.0)
   {
      zero_flag = 1;
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   Warning:  spatialnorm(u) = 0.0\n"); 
   }
   else if( braid_isnan(result1) )
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   Warning:  spatialnorm(u) = nan\n"); 
      correct = 0;
   }

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   v = clone(u)\n");
   clone(app, u, &v);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   v = u - v \n");
   sum(app, 1.0, u, -1.0, v); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   spatialnorm(v) \n");
   spatialnorm(app, v, &result1);
   if( (fabs(result1) > wiggle) || braid_isnan(result1) )
   {
      correct = 0;
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   Test 1 Failed\n");
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   Test 1 Passed\n");
   }
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   actual output:    spatialnorm(v) = %1.2e  \n", result1);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   expected output:  spatialnorm(v) = 0.0 \n\n");
   

   /* Test 2 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   Starting Test 2\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   w = clone(u)\n");
   clone(app, u, &w);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   w = u + w \n");
   sum(app, 1.0, u, 1.0, w); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   spatialnorm(u)\n");
   spatialnorm(app, u, &result1);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   spatialnorm(w)\n");
   spatialnorm(app, w, &result2);
   if( (fabs(result2/result1 - 2.0) > wiggle) || braid_isnan(result2/result1) )
   {
      correct = 0;
      if(zero_flag)
      {
         _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   Test 2 Failed, Likely due to u = 0\n");
      }
      else
      {
         _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   Test 2 Failed\n");
      }
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   Test 2 Passed\n");
   }
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   actual output:    spatialnorm(w) / spatialnorm(u) = %1.2e / %1.2e = %1.2e \n",
         result2, result1, result2/result1);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   expected output:  spatialnorm(w) / spatialnorm(u) = 2.0 \n\n");

   /* Test 3 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   Starting Test 3\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   myfree(w)\n");
   myfree(app, w);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   w = clone(u)\n");
   clone(app, u, &w);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   w = 0.0*u + 0.5*w \n");
   sum(app, 0.0, u, 0.5, w); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   spatialnorm(u)\n");
   spatialnorm(app, u, &result1);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   spatialnorm(w)\n");
   spatialnorm(app, w, &result2);
   /* Check Result */
   if( (fabs(result2/result1 - 0.5) > wiggle) || braid_isnan(result2/result1) )

   {
      correct = 0;
      if(zero_flag)
      {
         _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   Test 3 Failed, Likely due to u = 0\n");
      }
      else
      {
         _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   Test 3 Failed\n");
      }
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   Test 3 Passed\n");
   }
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   actual output:    spatialnorm(w) / spatialnorm(u) = %1.2e / %1.2e = %1.2e \n",
         result2, result1, result2/result1);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   expected output:  spatialnorm(w) / spatialnorm(u) = 0.5 \n\n");

   /* Free variables */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   myfree(u)\n");
   myfree(app, u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   myfree(v)\n");
   myfree(app, v);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestSpatialNorm:   myfree(w)\n");
   myfree(app, w);

   if(correct == 1) 
      _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestSpatialNorm: all tests passed successfully\n");
   else
   {
      if(zero_flag)
      {
         _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestSpatialNorm: some tests failed, possibly due to u = 0\n");
      }
      else
      {
         _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestSpatialNorm: some tests failed\n");
      }
   }

   return correct;
}

braid_Int
braid_TestBuf( braid_App              app,
               MPI_Comm               comm_x,
               FILE                   *fp, 
               braid_Real             t,
               braid_PtFcnInit        myinit,
               braid_PtFcnFree        myfree,
               braid_PtFcnSum         sum,  
               braid_PtFcnSpatialNorm spatialnorm, 
               braid_PtFcnBufSize     bufsize,
               braid_PtFcnBufPack     bufpack,
               braid_PtFcnBufUnpack   bufunpack)
{   
   braid_Vector  u, v;
   braid_Real    result1;
   braid_Int     myid_x, size, correct;
   void      *buffer;
   double     wiggle = 1e-12;
   
   MPI_Comm_rank( comm_x, &myid_x );
   
   braid_Status            status = _braid_CTAlloc(_braid_Status, 1);
   braid_BufferStatus      bstatus = (braid_BufferStatus)status;
   _braid_BufferStatusInit( 0, 0, bstatus );
   /* Initialize the correct flag */
   correct = 1;

   /* Print intro */
   _braid_ParFprintfFlush(fp, myid_x, "\nStarting braid_TestBuf\n\n");
   
   /* Test 1 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   Starting Test 1\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   u = myinit(t=%1.2e)\n", t);
   myinit(app, t, &u);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   spatialnorm(u) \n");
   spatialnorm(app, u, &result1);
   if( fabs(result1) == 0.0)
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   Warning:  spatialnorm(u) = 0.0\n"); 
   }
   else if( braid_isnan(result1) )
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   Warning:  spatialnorm(u) = nan\n"); 
      correct = 0;
   }

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   size = bufsize()\n");
   bufsize(app, &size, bstatus);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   buffer = malloc(size)\n");
   buffer = malloc(size);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   buffer = bufpack(u, buffer))\n");
   
   _braid_DeriveStatusElt( bstatus, size_buffer ) = size;
   bufpack(app, u, buffer, bstatus);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   v = bufunpack(buffer)\n");
   bufunpack(app, buffer, &v, bstatus);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   v = u - v \n");
   sum(app, 1.0, u, -1.0, v); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   spatialnorm(v) \n");
   spatialnorm(app, v, &result1);
   if( (fabs(result1) > wiggle) || braid_isnan(result1) )

   {
      correct = 0;
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   Test 1 Failed\n");
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   Test 1 Passed\n");
   }
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   actual output:    spatialnorm(v) = %1.2e  \n", result1);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   expected output:  spatialnorm(v) = 0.0 \n\n");
   
   /* Free variables */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   myfree(u)\n");
   myfree(app, u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestBuf:   myfree(v)\n");
   myfree(app, v);
   
   if(correct == 1) 
      _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestBuf: all tests passed successfully\n");
   else      
      _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestBuf: some tests failed\n");

   _braid_StatusDestroy(status);
   return correct;
}

braid_Int
braid_TestCoarsenRefine( braid_App           app,
                      MPI_Comm               comm_x,
                      FILE                   *fp, 
                      braid_Real             t,
                      braid_Real             fdt,
                      braid_Real             cdt,
                      braid_PtFcnInit        myinit,
                      braid_PtFcnAccess      myaccess,
                      braid_PtFcnFree        myfree,
                      braid_PtFcnClone       clone,
                      braid_PtFcnSum         sum,
                      braid_PtFcnSpatialNorm spatialnorm, 
                      braid_PtFcnSCoarsen    coarsen,
                      braid_PtFcnSRefine     refine)
 {   
   braid_Vector            u, v, w, uc, vc, wc;
   braid_Real              result1;
   braid_Int               myid_x, level, correct;
   braid_Status            status  = _braid_CTAlloc(_braid_Status, 1);
   braid_AccessStatus      astatus = (braid_AccessStatus)status;
   braid_CoarsenRefStatus  cstatus = (braid_CoarsenRefStatus)status;
   
   _braid_CoarsenRefStatusInit(t, t-fdt, t+fdt, t-cdt, t+cdt, 0, 0, 0, cstatus);
   MPI_Comm_rank( comm_x, &myid_x );

   /* Initialize the correct flag */
   correct = 1;

   /* Print intro */
   _braid_ParFprintfFlush(fp, myid_x, "\nStarting braid_TestCoarsenRefine\n\n");
   
   /* Test 1 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Starting Test 1\n");
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   u = myinit(t=%1.2e)\n", t);
   myinit(app, t, &u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   spatialnorm(u) \n");
   spatialnorm(app, u, &result1);
   if( fabs(result1) == 0.0)
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Warning:  spatialnorm(u) = 0.0\n"); 
   }
   else if( braid_isnan(result1) )
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Warning:  spatialnorm(u) = nan\n"); 
      correct = 0;
   }

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   uc = coarsen(u)\n");
   coarsen(app, u, &uc, cstatus); 

   if(myaccess != NULL)
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   myaccess(uc) \n");
      level = 1;
      _braid_AccessStatusInit(t, 0, 0.0, 0, level, 0, 0, 0, 1, -1, astatus);
      myaccess(app, uc, astatus);

      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   myaccess(u) \n");
      level = 0;
      _braid_AccessStatusInit(t, 0, 0.0, 0, level, 0, 0, 0, 1, -1, astatus);
      myaccess(app, u, astatus);
   }

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   actual output:   wrote u and spatially coarsened u \n\n");

   /* Test 2 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Starting Test 2\n");
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   v = clone(u)\n");
   clone(app, u, &v);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   vc = coarsen(v)\n");
   coarsen(app, v, &vc, cstatus); 
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   wc = clone(vc)\n");
   clone(app, vc, &wc);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   wc = uc - wc \n");
   sum(app, 1.0, uc, -1.0, wc); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   spatialnorm(wc)\n");
   spatialnorm(app, wc, &result1);
   
   /* We expect exact equality between uc and vc */
   if( (fabs(result1) != 0.0) || braid_isnan(result1) )
   {
      correct = 0;
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Test 2 Failed\n");
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Test 2 Passed\n");
   }
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   actual output:    spatialnorm(wc) = %1.2e \n", result1);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   expected output:  spatialnorm(wc) = 0.0 \n\n");

   /* Test 3 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Starting Test 3\n");
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   w = clone(u)\n");
   clone(app, u, &w);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   myfree(u)\n");
   myfree(app, u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   myfree(v)\n");
   myfree(app, v);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   v = refine(vc)\n");
   refine(app, vc, &v, cstatus); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   u = refine(uc)\n");
   refine(app, uc, &u, cstatus); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   v = u - v \n");
   sum(app, 1.0, u, -1.0, v); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   spatialnorm(v)\n");
   spatialnorm(app, v, &result1);
   
   /* We expect exact equality between u and v */
   if( (fabs(result1) != 0.0) || braid_isnan(result1) )
   {
      correct = 0;
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Test 3 Failed\n");
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Test 3 Passed\n");
   }
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   actual output:    spatialnorm(v) = %1.2e \n", result1);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   expected output:  spatialnorm(v) = 0.0 \n\n");

   /* Test 4 */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   Starting Test 4\n");
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   w = w - u \n");
   sum(app, 1.0, u, -1.0, w); 

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   spatialnorm(w)\n");
   spatialnorm(app, w, &result1);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   actual output:    spatialnorm(w) = %1.2e \n", result1);
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   %s%s%s", 
                   "expected output:  For simple interpolation formulas\n",
                   "                             (e.g., bilinear) and a known function\n",
                   "                             (e.g., constant), spatialnorm(w) should = 0\n\n");

   /* Free variables */
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   myfree(u)\n");
   myfree(app, u);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   myfree(v)\n");
   myfree(app, v);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   myfree(w)\n");
   myfree(app, w);
   
   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   myfree(uc)\n");
   myfree(app, uc);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   myfree(vc)\n");
   myfree(app, vc);

   _braid_ParFprintfFlush(fp, myid_x, "   braid_TestCoarsenRefine:   myfree(wc)\n");
   myfree(app, wc);

   _braid_StatusDestroy(status);

   if(correct == 1) 
      _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestCoarsenRefine: all tests passed successfully\n");
   else      
      _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestCoarsenRefine: some tests failed\n");
   
   return correct;
}

braid_Int
braid_TestAll( braid_App            app,
            MPI_Comm                comm_x,
            FILE                    *fp, 
            braid_Real              t,
            braid_Real              fdt,
            braid_Real              cdt,
            braid_PtFcnInit         myinit,
            braid_PtFcnFree         myfree,
            braid_PtFcnClone        clone,
            braid_PtFcnSum          sum,
            braid_PtFcnSpatialNorm  spatialnorm, 
            braid_PtFcnBufSize      bufsize,
            braid_PtFcnBufPack      bufpack,
            braid_PtFcnBufUnpack    bufunpack,
            braid_PtFcnSCoarsen     coarsen,
            braid_PtFcnSRefine      refine)
{
   braid_Int    myid_x, flag = 0, correct = 1;
   MPI_Comm_rank( comm_x, &myid_x );
   
   /** 
    * We set the myaccess parameter to NULL below, because this function is
    * is designed to return only one value, the boolean correct
    **/

   /* Test myinit(), myfree() */
   braid_TestInitAccess( app, comm_x, fp, t, myinit, NULL, myfree);
   braid_TestInitAccess( app, comm_x, fp, fdt, myinit, NULL, myfree);

   /* Test clone() */
   braid_TestClone( app, comm_x, fp, t, myinit, NULL, myfree, clone);
   braid_TestClone( app, comm_x, fp, fdt, myinit, NULL, myfree, clone);

   /* Test sum() */
   braid_TestSum( app, comm_x, fp, t, myinit, NULL, myfree, clone, sum);
   braid_TestSum( app, comm_x, fp, fdt, myinit, NULL, myfree, clone, sum);

   /* Test spatialnorm() */
   flag = braid_TestSpatialNorm( app, comm_x, fp, t, myinit, myfree, clone, sum, spatialnorm);
   if(flag == 0)
   {
      _braid_ParFprintfFlush(fp, myid_x, "-> braid_TestAll:   TestSpatialNorm 1 Failed\n");
      correct = 0;
   }
   flag = braid_TestSpatialNorm( app, comm_x, fp, fdt, myinit, myfree, clone, sum, spatialnorm);
   if(flag == 0)
   {
      _braid_ParFprintfFlush(fp, myid_x, "-> braid_TestAll:   TestSpatialNorm 2 Failed\n");
      correct = 0;
   }

   /* Test bufsize(), bufpack(), bufunpack() */
   flag = braid_TestBuf( app, comm_x, fp, t, myinit, myfree, sum, spatialnorm, bufsize, bufpack, bufunpack);
   if(flag == 0)
   {
      _braid_ParFprintfFlush(fp, myid_x, "-> braid_TestAll:   TestBuf 1 Failed\n");
      correct = 0;
   }
   flag = braid_TestBuf( app, comm_x, fp, fdt, myinit, myfree, sum, spatialnorm, bufsize, bufpack, bufunpack);
   if(flag == 0)
   {
      _braid_ParFprintfFlush(fp, myid_x, "-> braid_TestAll:   TestBuf 2 Failed\n");
      correct = 0;
   }
 
   /* Test coarsen and refine */
   if( (coarsen != NULL) && (refine != NULL) )
   {
      flag = braid_TestCoarsenRefine(app, comm_x, fp, t, fdt, cdt, myinit,
                          NULL, myfree, clone, sum, spatialnorm, coarsen, refine);
      if(flag == 0)
      {
         _braid_ParFprintfFlush(fp, myid_x, "-> braid_TestAll:   TestCoarsenRefine 1 Failed\n");
         correct = 0;
      }
   }

   if(correct == 1)
   {
      _braid_ParFprintfFlush(fp, myid_x, "\n\n");
      _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestAll: all tests passed successfully\n\n");
   }
   else
   {
      _braid_ParFprintfFlush(fp, myid_x, "\n\n");
      _braid_ParFprintfFlush(fp, myid_x, "Finished braid_TestAll: some tests failed\n\n");
   }
   
   return correct;
}
