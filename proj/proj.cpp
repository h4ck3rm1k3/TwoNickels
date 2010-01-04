#include <iostream>
#include <fstream>
//#include "../../proj/trunk/src/proj_api.h"
#include "../../proj/trunk/src/projects.h"
//#include "../../proj/trunk/src/emess.h"
#include "../../proj/trunk/src/proj_config.h"

using namespace std;

/************************************************************************/
/*                     pj_geodetic_to_geocentric()                      */
/************************************************************************/

int pj_geodetic_to_geocentric2( double a, double es, 
                               double &x, double &y, double &z )

{
  return -1;
}

int pj_transform1( PJ *srcdefn, PJ *dstdefn, double &x, double & y, double & z )

{
    long      i;
    int       need_datum_shift;

    pj_errno = 0;


/* -------------------------------------------------------------------- */
/*      Transform geocentric source coordinates to lat/long.            */
/* -------------------------------------------------------------------- */
    if( srcdefn->is_geocent )
    {
        if( z == 0 )
        {
            pj_errno = PJD_ERR_GEOCENTRIC;
            return PJD_ERR_GEOCENTRIC;
        }

        if( srcdefn->to_meter != 1.0 )
        {
	  if( x != HUGE_VAL )
	    {
	      x *= srcdefn->to_meter;
	      y *= srcdefn->to_meter;
	    }

        }

	//        if( pj_geocentric_to_geodetic( srcdefn->a_orig, srcdefn->es_orig,       point_count, point_offset,         x, y, z ) != 0) 
	//           return pj_errno;
    }

/* -------------------------------------------------------------------- */
/*      Transform source points to lat/long, if they aren't             */
/*      already.                                                        */
/* -------------------------------------------------------------------- */
    else if( !srcdefn->is_latlong )
    {
        if( srcdefn->inv == NULL )
        {
            pj_errno = -17; /* this isn't correct, we need a no inverse err */
            if( getenv( "PROJ_DEBUG" ) != NULL )
            {
                fprintf( stderr, 
                       "pj_transform(): source projection not invertable\n" );
            }
            return pj_errno;
        }

        //for( i = 0; i < point_count; i++ )
        {
            XY         projected_loc;
            LP	       geodetic_loc;
	    projected_loc.u = 0.0;
            projected_loc.v = 0.0;

            projected_loc.u = x;
            projected_loc.v = y;

            if( projected_loc.u == HUGE_VAL )
	      return -1;
	      //                continue;

            geodetic_loc = pj_inv( projected_loc, srcdefn );
            if( pj_errno != 0 )
            {
                if( (pj_errno != 33 /*EDOM*/ && pj_errno != 34 /*ERANGE*/ )
                    && (pj_errno > 0 || pj_errno < -44 
                        //|| transient_error[-pj_errno] == 0 
			) )
                    return pj_errno;
                else
                {
                    geodetic_loc.u = HUGE_VAL;
                    geodetic_loc.v = HUGE_VAL;
                }
            }

            x = geodetic_loc.u;
            y = geodetic_loc.v;
        }
    }
/* -------------------------------------------------------------------- */
/*      But if they are already lat long, adjust for the prime          */
/*      meridian if there is one in effect.                             */
/* -------------------------------------------------------------------- */
    if( srcdefn->from_greenwich != 0.0 )
    {
      //        for( i = 0; i < point_count; i++ )
        {
            if( x != HUGE_VAL )
                x += srcdefn->from_greenwich;
        }
    }

/* -------------------------------------------------------------------- */
/*      Convert datums if needed, and possible.                         */
/* -------------------------------------------------------------------- */
    // if( pj_datum_transform( srcdefn, dstdefn, point_count, point_offset, 
    //                         x, y, z ) != 0 )
    //     return pj_errno;

/* -------------------------------------------------------------------- */
/*      But if they are staying lat long, adjust for the prime          */
/*      meridian if there is one in effect.                             */
/* -------------------------------------------------------------------- */
    if( dstdefn->from_greenwich != 0.0 )
    {
      //        for( i = 0; i < point_count; i++ )
        {
            if( x != HUGE_VAL )
                x -= dstdefn->from_greenwich;
        }
    }


/* -------------------------------------------------------------------- */
/*      Transform destination latlong to geocentric if required.        */
/* -------------------------------------------------------------------- */
    if( dstdefn->is_geocent )
    {
        if( z == 0 )
        {
            pj_errno = PJD_ERR_GEOCENTRIC;
            return PJD_ERR_GEOCENTRIC;
        }

	//        pj_geodetic_to_geocentric( dstdefn->a_orig, dstdefn->es_orig,
	//                                   point_count, point_offset, x, y, z );

        if( dstdefn->fr_meter != 1.0 )
        {
	  //            for( i = 0; i < point_count; i++ )
            {
                if( x != HUGE_VAL )
                {
                    x *= dstdefn->fr_meter;
                    y *= dstdefn->fr_meter;
                }
            }
        }
    }

/* -------------------------------------------------------------------- */
/*      Transform destination points to projection coordinates, if      */
/*      desired.                                                        */
/* -------------------------------------------------------------------- */
    else if( !dstdefn->is_latlong )
    {
      //        for( i = 0; i < point_count; i++ )
        {
            XY         projected_loc;
            LP	       geodetic_loc;

            geodetic_loc.u = x;
            geodetic_loc.v = y;

            if( geodetic_loc.u == HUGE_VAL )
	      return -1;
	      // continue;

            projected_loc = pj_fwd( geodetic_loc, dstdefn );
            if( pj_errno != 0 )
            {
                if( (pj_errno != 33 /*EDOM*/ && pj_errno != 34 /*ERANGE*/ )
                    && (pj_errno > 0 || pj_errno < -44 //|| //point_count == 1
                        //|| transient_error[-pj_errno] == 0 
			) )
                    return pj_errno;
                else
                {
                    projected_loc.u = HUGE_VAL;
                    projected_loc.v = HUGE_VAL;
                }
            }

            x = projected_loc.u;
            y = projected_loc.v;
        }
    }

/* -------------------------------------------------------------------- */
/*      If a wrapping center other than 0 is provided, rewrap around    */
/*      the suggested center (for latlong coordinate systems only).     */
/* -------------------------------------------------------------------- */
    else if( dstdefn->is_latlong && dstdefn->long_wrap_center != 0 )
    {
      //        for( i = 0; i < point_count; i++ )
        {
            if( x == HUGE_VAL )
	      return -1;
	      //                continue;

            while( x < dstdefn->long_wrap_center - HALFPI )
                x += PI;
            while( x > dstdefn->long_wrap_center + HALFPI )
                x -= PI;
        }
    }

    return 0;
}


void convertPoint(double x, double y, double & rx, double & ry) 
{
  projPJ fromProj, toProj;
  //+proj=utm +south +ellps=intl +zone=24K +units=m
  //  #s2cs +proj=utm +south +ellps=intl +zone=24K +units=m +proj=tmerc 
  if (!(fromProj = pj_init_plus("+proj=utm +south +ellps=intl +zone=24K +units=m -f \"%.7f\" ")) )
    exit(1);
    
  //  if (!(pj_merc = pj_init_plus("+proj=merc +ellps=clrk66 +lat_ts=33")) )
  //               exit(1);
  //            if (!(pj_latlong = pj_init_plus("+proj=latlong +ellps=clrk66")) )
  //               exit(1);
  if (!(toProj = pj_latlong_from_proj( fromProj )))
    exit(1);


  int islatlon= pj_is_latlong( fromProj );
  int isgeocent=pj_is_geocent( fromProj );
  char * name = pj_get_def( fromProj , 0);
  std::cerr  << "FROm NAME:"<< name << "|" << islatlon << "|" << isgeocent << std::endl;

   islatlon= pj_is_latlong( toProj );
   isgeocent=pj_is_geocent( toProj );
   name = pj_get_def( toProj , 0);
  std::cerr  << "TO NAME:"<< name << "|" << islatlon << "|" << isgeocent << std::endl;

  //  while (scanf("%lf %lf", &x, &y) == 2) {
  //  rx = x * DEG_TO_RAD;
  //  ry = y * DEG_TO_RAD;
  //  int p = pj_transform(pj_latlong, pj_merc, 1, 1, &rx, &ry, NULL );
  
  //  cerr << 
  //int pj_transform( projPJ srcdefn, projPJ dstdefn, long point_count, int point_offset, double *x, double *y, double *z );
  //printf("%.2f\t%.2f\n", x, y);
  //            }
  //            exit(0);
  //  double ax[1], ay[1], az[1];
  std::cerr.precision(10);
  std::cerr.width(10);
  std::cerr << x  << "|" << y  << std::endl;
  //  const double D2R =DEG_TO_RAD;
  double ax = x;// * DEG_TO_RAD;
  double ay = y;// * DEG_TO_RAD ;
  double az = 0;
  
    /** end of "caution" section. */
    std::cerr.precision(10);
  std::cerr.width(10);
  
  std::cerr << ax  << "|" << ay  << "|" << std::endl;
  int ret=pj_transform1(fromProj, toProj, ax, ay, az);
  
  std::cerr.precision(10);
  std::cerr.width(10);
  
    std::cerr << ax  << "|" << ay  << "|" << ret  << "|" << std::endl;

    ax *= RAD_TO_DEG;
    ay *= RAD_TO_DEG;

    std::cerr << ax  << "|" << ay  << "|" << ret  << "|" << std::endl;

    //printf("%.4f\t%.4f -> %.4f\t%.4f\n", *lat, *lon, y[0], x[0]);    
    rx = ay;
    ry = ax;

    pj_free(fromProj); 
    pj_free(toProj); 

}

int main ()
{
  //cs2cs  +proj=utm +south +ellps=intl +zone=24K +units=m -f %.7f

  // std::fstream cs2cs( "cs2cs  +proj=utm +south +ellps=intl +zone=24K +units=m -f %.7f |");
  
  while(std::cin)
    {
      double a;
      double b;

      double lat;
      double lon;
      double h=0;

      std::cin >> a;
      std::cin >> b;

      convertPoint(a, b, lat,  lon) ;

      std::cout << lat  << ":" << lon << std::endl;

      lat *= DEG_TO_RAD;
      lon *= DEG_TO_RAD;

      
    }
}
