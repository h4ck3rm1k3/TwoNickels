/**************************************************************************\
 * 
 *  FILE: MText.cpp
 *
 *  This source file is part of DIME.
 *  Copyright (C) 1998-1999 by Systems In Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License, version 2, as
 *  published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License (the accompanying file named COPYING) for more
 *  details.
 *
 **************************************************************************
 *
 *  If you need DIME for a non-GPL project, contact Systems In Motion
 *  to acquire a Professional Edition License:
 *
 *  Systems In Motion                                   http://www.sim.no/
 *  Prof. Brochs gate 6                                       sales@sim.no
 *  N-7030 Trondheim                                   Voice: +47 22114160
 *  NORWAY                                               Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class dimeMText dime/entities/MText.h
  \brief The dimeMText class handles a CIRCLE \e entity.
*/

#include <dime/entities/MText.h>
#include <dime/records/Record.h>
#include <dime/Input.h>
#include <dime/Output.h>
#include <dime/util/MemHandler.h>
#include <dime/Model.h>
#include <math.h>
//#include <proj_api.h>
//#include "../../proj/trunk/src/proj_api.h"
#include "../../proj/trunk/src/projects.h"
//#include "../../proj/trunk/src/emess.h"
#include "../../proj/trunk/src/proj_config.h"

#ifdef _WIN32
#define M_PI 3.14159265357989
#endif

#ifdef macintosh
#define M_PI 3.14159265357989
#endif

static char entityName[] = "MTEXT";

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

void convertPoint2(double x, double y, double & rx, double & ry) 
{
  projPJ pj_merc, pj_latlong;

  //  #s2cs +proj=utm +south +ellps=intl +zone=24K +units=m +proj=tmerc -f "%.7f"
  if (!(pj_merc = pj_init_plus("+proj=utm +south +ellps=intl +zone=24K +units=m")) )
    exit(1);

  //  if (!(pj_merc = pj_init_plus("+proj=merc +ellps=clrk66 +lat_ts=33")) )
  //               exit(1);
  //            if (!(pj_latlong = pj_init_plus("+proj=latlong +ellps=clrk66")) )
  //               exit(1);

  if (!(pj_latlong = pj_init_plus("+proj=latlong +ellps=intl")) )
    exit(1);

  //  while (scanf("%lf %lf", &x, &y) == 2) {
  //  rx = x * DEG_TO_RAD;
  //  ry = y * DEG_TO_RAD;
  //  int p = pj_transform(pj_latlong, pj_merc, 1, 1, &rx, &ry, NULL );
  
  //  cerr << 
  //int pj_transform( projPJ srcdefn, projPJ dstdefn, long point_count, int point_offset, double *x, double *y, double *z );
  //printf("%.2f\t%.2f\n", x, y);
  //            }
  //            exit(0);
  double ax[1], ay[1], az[1];
  ax[0] = x ;
  ay[0] = y ;
  az[0] = 0;
  
    /** end of "caution" section. */
    
  pj_transform(pj_merc, pj_latlong, 1, 1, ax, ay, az);
  
  
    //printf("%.4f\t%.4f -> %.4f\t%.4f\n", *lat, *lon, y[0], x[0]);    
    rx = ay[0];
    ry = ax[0];

    rx *= DEG_TO_RAD;
    ry *= DEG_TO_RAD;

}


// FIXME: configurable

#define CIRCLE_NUMPTS 16

/*!
  Constructor.
*/

dimeMText::dimeMText() 
  : text(),
    easting(0),
    northing(0),
    lat(0),
    lon(0)
    
{
}

//!

dimeEntity *
dimeMText::copy(dimeModel * const model) const
{
  dimeMText *c = new(model->getMemHandler()) dimeMText;
  if (!c) return NULL;
  
  if (!this->copyRecords(c, model)) {
    // check if allocated on heap.
    if (!model->getMemHandler()) delete c;
    c = NULL;
  }
  else {
    //    c->copyExtrusionData(this);
    //c->center = this->center;
    c->text = this->text;
  }
  return c;  
}

/*!
  This method writes a DXF \e MText entity to \a file.
*/

bool 
dimeMText::write(dimeOutput * const file)
{
  dimeEntity::preWrite(file);

    convertPoint(easting, northing, lat, lon) ;
    //lat=  easting;
    //  lon=  northing;

  file->writeGroupCode(10); // TODO


  file->writeString(this->text.c_str());

  print();


  //  this->writeExtrusionData(file);
  
  return dimeEntity::write(file); // write unknown records.
}

//!

int 
dimeMText::typeId() const
{
  return 1; //dimeMTextType;
}

/*!
  Handles the callback from dimeEntity::readRecords().
*/

/*
MTEXT
  5
281CF
100
AcDbEntity
100
AcDbMText
  8
TXT_EQUIPAMENTO_PUBLICO
 62
256
370
-1
  6
ByLayer
 10
364752.2501553706242703
 20
7753231.1186565160751343
 30
0.0
 40
1.6000000000000001
 41
17.3714442079071887
 71
1
 72
2
  1
SANTA\~HELENA\~SPECIAL\~FLAT
  7
ROMANS
 50
0.0005599687904916
 73
0
 44
1.0
  0

 */
bool 
dimeMText::handleRecord(const int groupcode,
			const dimeParam &param,
			dimeMemHandler * const memhandler)
{

  /*
    Text Record GC:5 Param:1C2|1C2|�|4832|-1081339168|-1.09433|-1.09433
    Text Record GC:100 Param:AcDbEntity|AcDbEntity|�|4832|-1081339168|-1.09433|-1.09433
    Text Record GC:100 Param:AcDbMText|AcDbMText|�|4832|-1081339168|-1.09433|-1.09433
    Text Record GC:8 Param:TXT_CONSTRUCAO|TXT_CONSTRUCAO|�|4832|-1081339168|-1.09433|-1.09433
    Text Record GC:62 Param:|||256|-1081343744|-1.09378|-1.09378
    Text Record GC:370 Param:-1|-1|�|4832|-1081339168|-1.09433|-1.09433
    Text Record GC:6 Param:ByLayer|ByLayer|�|4832|-1081339168|-1.09433|-1.09433


Text Record GC:5 Param:|�|6784|-1074128256|-1.95393|-1.95393
Text Record GC:100 Param:|�|6784|-1074128256|-1.95393|-1.95393
Text Record GC:100 Param:|�|6784|-1074128256|-1.95393|-1.95393
---Text Record GC:8 Param:TXT_CONSTRUCAO|TXT_CONSTRUCAO|�|6784|-1074128256|-1.95393|-1.95393
Text Record GC:62 Param:|||256|-1074134784|-1.95316|-1.95316
Text Record GC:370 Param:-1|-1|�|6784|-1074128256|-1.95393|-1.95393
---Text Record GC:6 Param:ByLayer|ByLayer|�|6784|-1074128256|-1.95393|-1.95393
Text Record GC:10 Param:||-752|1219427600|358376|358376
Text Record GC:20 Param:|�|-24958|1257021058|7.75354e+06|7.75354e+06
Text Record GC:30 Param:||0|0|0|0
Text Record GC:40 Param:|�|-13107|1070386381|1.6|1.6
Text Record GC:41 Param:||0|1120403456|100|100
Text Record GC:71 Param:||7|1120403463|100|100
Text Record GC:72 Param:||2|1120403458|100|100
Text Record GC:1 Param:|�|6784|-1074128256|-1.95393|-1.95393
--Text Record GC:7 Param:ROMANS|ROMANS|�|6784|-1074128256|-1.95393|-1.95393
Text Record GC:50 Param:||0|0|0|0
Text Record GC:73 Param:||0|0|0|0
Text Record GC:44 Param:||0|1065353216|1|1


*/

    switch(groupcode) 
      {
      case 44:
	convertPoint(easting, northing, lat, lon) ;
	//lat=  easting;
	//lon=  northing;
	
	print(); // just print out the whole record so far.
	break;

      case 10: // UTM COORD Easting
	//	easting=param.double_data;
	easting=param.double_data;
	break;

      case 20: // Northing

	northing=param.double_data;
	break;

      case 30: // number
      case 5: // 
      case -1:
      case 100 :

      case 256:
      case 2:
      case 40:
      case 41:
      case 71:
      case 72:
      case 73:

	

      case 50: //
      case 62:
      case 370:

  std::cerr << "Text Record GC:" << groupcode << " Param:"
 <<"|"	    << param.int8_data 
 <<"|"	    << param.int16_data 
 <<"|"	    << param.int32_data 
 <<"|"	    << param.float_data 
 <<"|"	    << param.double_data 
	    << std::endl;

	break;

      case 1:
	// emit a take for the name
	//	std::cout << "<tag k=\"name\" v=\"" <<  param.string_data  << "\" />" << std::endl;
	//<< groupcode << " StringParam:"
	text=param.string_data;
	break;

      case 8:
	//	std::cout << "<tag k=\"layer\" v=\"" <<  param.string_data  << "\" />" << std::endl;
	layer=param.string_data;
	break;
 	   
      case 7:// name
	//	std::cout << "<tag k=\"font\" v=\"" <<  param.string_data  << "\" />" << std::endl;
	type=param.string_data;
	break;
      case 6:
	// the byLayer element
	break;

      case 3:
      case 4:


      case 9:// name
      default:
  std::cout << "<tag GC:" << groupcode << " StringParam:"
 	    << param.string_data 
    // <<"|" << param.hex_data 
    // <<"|"	    << param.int8_data 
    // <<"|"	    << param.int16_data 
    // <<"|"	    << param.int32_data 
    // <<"|"	    << param.float_data 
    // <<"|"	    << param.double_data 
	    << std::endl;

  }


  return dimeEntity::handleRecord(groupcode, param, memhandler);
}

//!

const char *
dimeMText::getEntityName() const
{
  return entityName;
}

//!

bool 
dimeMText::getRecord(const int groupcode,
		     dimeParam &param,
		     const int index) const
{
  switch(groupcode) {
  case 10:
  case 20:
  case 30:
    return true;
  case 40:
    return true;
  }
  return dimeEntity::getRecord(groupcode, param, index);  
}

//!



void
dimeMText::print() const
{
  std::cout << "<node id='-"<< entityNum 
	    <<  "' version='" << 1<< "' " ;

	std::cout.width(10);
	std::cout.precision(10);

  std::cout  <<  "lat='" << lat<< "'";

	std::cout.width(10);
	std::cout.precision(10);

  std::cout  <<  "lon='" << lon   << "' />" << std::endl;	  
  std::cout << "<tag k='type' v='" << type.c_str() << "/>\n";

  std::cout << "<tag k='name' v='" << text.c_str() << "/>\n";
	std::cout.width(10);
	std::cout.precision(10);

  std::cout << "<tag k='easting' v=" << easting << " />"<< std::endl;
	std::cout.width(10);
	std::cout.precision(10);
	std::cout << "<tag k='northing' v=" << northing << " />" << std::endl;

  std::cout << "</node>" << std::endl;
}

//!

/*
dimeEntity::GeometryType 
dimeMText::extractGeometry(dimeArray <dimeVec3f> &verts,
			   dimeArray <int> & // indices
			   ,
			   dimeVec3f &extrusionDir,
			   dxfdouble &thickness)
{
  thickness = this->thickness;
  extrusionDir = this->extrusionDir;

   // tessellate the circle/cylinder
  dxfdouble inc = (2*M_PI) / CIRCLE_NUMPTS;
  dxfdouble rad = 0.0f;
  int i;
  for (i = 0; i < CIRCLE_NUMPTS; i++) {
    verts.append(dimeVec3f(this->center.x + this->radius * cos(rad),
			   this->center.y + this->radius * sin(rad),
			   this->center.z));
    rad += inc;
  }
  
  dimeVec3f tmp = verts[0];
  verts.append(tmp); // closed line/polygon
  
  if (this->thickness == 0.0) return dimeEntity::LINES;
  else return dimeEntity::POLYGONS;
}
*/

//!

int
dimeMText::countRecords() const
{
  // header + center point + radius
  return 5 + dimeEntity::countRecords();
}

