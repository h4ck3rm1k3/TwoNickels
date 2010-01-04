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

#include <dime/entities/Hatch.h>
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

dimeHatch::dimeHatch() 
{
}

//!

dimeEntity *
dimeHatch::copy(dimeModel * const model) const
{
  dimeHatch *c = new(model->getMemHandler()) dimeHatch;
  if (!c) return NULL;
  
  if (!this->copyRecords(c, model)) {
    // check if allocated on heap.
    if (!model->getMemHandler()) delete c;
    c = NULL;
  }
  else {

  }
  return c;  
}

/*!
  This method writes a DXF \e Hatch entity to \a file.
*/

bool 
dimeHatch::write(dimeOutput * const file)
{
  dimeEntity::preWrite(file);
  
  file->writeGroupCode(10);

  
  return dimeEntity::write(file); // write unknown records.
}

//!

int 
dimeHatch::typeId() const
{
  return dimeBase::dimeHatchType;
}

/*!
  Handles the callback from dimeEntity::readRecords().
*/

bool 
dimeHatch::handleRecord(const int groupcode,
			const dimeParam &param,
			dimeMemHandler * const memhandler)
{

  return dimeEntity::handleRecord(groupcode, param, memhandler);
}

//!

const char *
dimeHatch::getEntityName() const
{
  return entityName;
}

//!

bool 
dimeHatch::getRecord(const int groupcode,
		     dimeParam &param,
		     const int index) const
{
  
  return dimeEntity::getRecord(groupcode, param, index);  
}

//!

void
dimeHatch::print() const
{
  fprintf(stderr,"HATCH:\n");

}

//!

dimeEntity::GeometryType 
dimeHatch::extractGeometry(dimeArray <dimeVec3f> &verts,
			   dimeArray <int> &/*indices*/,
			   dimeVec3f &extrusionDir,
			   dxfdouble &thickness)
{
 return dimeEntity::POLYGONS;
}

//!

int
dimeHatch::countRecords() const
{
  // header + center point + radius
  return 5 + dimeEntity::countRecords();
}

