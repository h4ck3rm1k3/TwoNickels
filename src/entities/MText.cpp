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
    switch(groupcode) {
    case -1:
    case 100 :
    case 10:
  case 1:
  case 20:
  case 256:
  case 2:
  case 30:
  case 370:
  case 40:
  case 40:
  case 41:
  case 5:
  case 62:
  case 6:
  case 71:
  case 72:
  case 7:
  case 8:
    
  }
*/
  std::cerr << "Text Record GC:" << groupcode << " Param:"
 	    << param.string_data 
 <<"|" << param.hex_data 
 <<"|"	    << param.int8_data 
 <<"|"	    << param.int16_data 
 <<"|"	    << param.int32_data 
 <<"|"	    << param.float_data 
 <<"|"	    << param.double_data 
	    << std::endl;

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

