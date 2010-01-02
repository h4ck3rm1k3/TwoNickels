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

#ifdef _WIN32
#define M_PI 3.14159265357989
#endif

#ifdef macintosh
#define M_PI 3.14159265357989
#endif

static char entityName[] = "CIRCLE";

// FIXME: configurable

#define CIRCLE_NUMPTS 16

/*!
  Constructor.
*/

dimeMText::dimeMText() 
  : text()
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
  
  file->writeGroupCode(10); // TODO


  file->writeString(this->text.c_str());


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
      case 10: // UTM COORD Easting
      case 20: // Northing

	std::cerr.width(10);
	std::cerr.precision(10);
  std::cerr << "Text Record GC:" << groupcode << " Param:"
    
	    <<"|"	    << param.double_data 
	    << std::endl;

  break;
      case 30: // number
      case 5: // 
      case -1:
      case 100 :
      case 1:
      case 256:
      case 2:
      case 40:
      case 41:
      case 71:
      case 72:
      case 73:
      case 44:


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

      case 7:// name
      case 8:
      case 6:
      default:
  std::cerr << "Text Record GC:" << groupcode << " StringParam:"
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
  fprintf(stdout,"<tag k='mtext' v='%s' />\n",text.c_str());
  fprintf(stderr, " TEXT: %s\n", text.c_str());
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

