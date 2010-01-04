/**************************************************************************\
 * 
 *  FILE: MText.h
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
 * James Michael DuPont
 *
\**************************************************************************/

#ifndef DIME_MTEXT_H
#define DIME_MTEXT_H

#include <dime/Basic.h>
#include <dime/entities/ExtrusionEntity.h>
#include <dime/util/Linear.h>
#include <string>
#include <iostream>

class dimeModel;
class dimeMemHandler;

class DIME_DLL_API dimeMText : public dimeEntity
{
 protected:
  std::string text; // the data
  std::string layer; // the layer
  std::string type; // the type of data
  std::string font; // the font data
  double easting;
  double northing;

  double lat;
  double lon;

public:
  double getEasting() {return easting;}
  double getNorthing() {return northing;}

  double getLat() {return lat;}
  double getLon() {return lon;}

  const char * getType() {return type.c_str();}
  const char * getText() {return text.c_str();}


  dimeMText();

  virtual dimeEntity *copy(dimeModel * const model) const;
  virtual bool getRecord(const int groupcode,
			 dimeParam &param,
			 const int index = 0) const;
  virtual const char *getEntityName() const;
  virtual void print() const;
  virtual bool write(dimeOutput * const out);
  virtual int typeId() const;
  virtual int countRecords() const;

  /*  virtual GeometryType extractGeometry(dimeArray <dimeVec3f> &verts,
				       dimeArray <int> &indices,
				       dimeVec3f &extrusionDir,
				       dxfdouble &thickness);
  */
protected:  
  virtual bool handleRecord(const int groupcode,
			    const dimeParam &param,
			    dimeMemHandler * const memhandler);  
private:


}; // class dimeMText


#endif // ! DIME_MTEXT_H

