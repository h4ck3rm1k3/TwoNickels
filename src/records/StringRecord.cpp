/**************************************************************************\
 * 
 *  FILE: StringRecord.cpp
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
  \class dimeStringRecord dime/records/StringRecord.h
  \brief The dimeStringRecord class is a container class for string records.
*/

#include <dime/records/StringRecord.h>
#include <dime/Input.h>
#include <dime/Output.h>
#include <dime/util/MemHandler.h>
#include <string.h>
#include <iostream>

/*! 
  Constructor.
*/

dimeStringRecord::dimeStringRecord(const int group_code)
  : dimeRecord(group_code)
{
  this->string = NULL;
}

/*!
  Destructor.
*/

dimeStringRecord::~dimeStringRecord()
{
  delete[] this->string;
}

//!

dimeRecord *
dimeStringRecord::copy(dimeMemHandler * const mh) const
{
  dimeStringRecord *s= new(mh) dimeStringRecord(this->groupCode);
  if (s) {
    s->setString(this->string, mh);
  }
  return s;
}

/*!
  Will store a copy of string \a s. If \a memhandler != NULL, it
  will be used to allocate the needed memory. If \a memhandler == NULL,
  the memory will be allocated from the heap.
*/

bool
dimeStringRecord::setString(const char * const s, 
			   dimeMemHandler * const memhandler)
{
  DXF_STRCPY(memhandler, this->string, s);
  return this->string != NULL;
}

/*!  
  Sets the objects string pointer to point to 's'. Be aware that if
  the destructor is called for this object, the object will attempt to
  delete the string. \sa dimeStringRecord::setString().  
*/

void
dimeStringRecord::setStringPointer(char * const s)
{
  this->string = s;
}

/*!
  Returns a pointer to the string.
*/

char *
dimeStringRecord::getString()
{
  return string;
}

//!

bool
dimeStringRecord::isEndOfSectionRecord() const
{
  return (this->groupCode == 0) && !strcmp(string, "ENDSEC");
}

//!

bool
dimeStringRecord::isEndOfFileRecord() const
{
  return (this->groupCode == 0) && !strcmp(string, "EOF");
}

//!

int 
dimeStringRecord::typeId() const
{
  return dimeBase::dimeStringRecordType;
}

//!

bool 
dimeStringRecord::read(dimeInput * const in)
{
  this->string = NULL;
  const char *ptr = in->readString();
  if (ptr){
    std::cerr << "Read String :" << ptr << std::endl;
// Read String :AC1015
// Read String :$ACADMAINTVER
// Read String :$ACADVER
// Read String :AcDbDimStyleTable
// Read String :AcDbSymbolTable
// Read String :$ANGBASE
// Read String :$ANGDIR
// Read String :ANSI_1252
// Read String :$ATTMODE
// Read String :$AUNITS
// Read String :$AUPREC
// Read String :ByLayer
// Read String :$CECOLOR
// Read String :$CELTSCALE
// Read String :$CELTYPE
// Read String :$CELWEIGHT
// Read String :$CEPSNTYPE
// Read String :$CHAMFERA
// Read String :$CHAMFERB
// Read String :$CHAMFERC
// Read String :$CHAMFERD
// Read String :$CLAYER
// Read String :$CMLJUST
// Read String :$CMLSCALE
// Read String :$CMLSTYLE
// Read String :$DIMADEC
// Read String :$DIMALT
// Read String :$DIMALTD
// Read String :$DIMALTF
// Read String :$DIMALTRND
// Read String :$DIMALTTD
// Read String :$DIMALTTZ
// Read String :$DIMALTU
// Read String :$DIMALTZ
// Read String :$DIMASO
// Read String :$DIMASZ
// Read String :$DIMATFIT
// Read String :$DIMAUNIT
// Read String :$DIMAZIN
// Read String :$DIMCEN
// Read String :$DIMCLRD
// Read String :$DIMCLRE
// Read String :$DIMCLRT
// Read String :$DIMDEC
// Read String :$DIMDLE
// Read String :$DIMDLI
// Read String :$DIMDSEP
// Read String :$DIMEXE
// Read String :$DIMEXO
// Read String :$DIMFRAC
// Read String :$DIMGAP
// Read String :$DIMJUST
// Read String :$DIMLFAC
// Read String :$DIMLIM
// Read String :$DIMLUNIT
// Read String :$DIMLWD
// Read String :$DIMLWE
// Read String :$DIMRND
// Read String :$DIMSAH
// Read String :$DIMSCALE
// Read String :$DIMSD1
// Read String :$DIMSD2
// Read String :$DIMSE1
// Read String :$DIMSE2
// Read String :$DIMSHO
// Read String :$DIMSOXD
// Read String :$DIMSTYLE
// Read String :$DIMTAD
// Read String :$DIMTDEC
// Read String :$DIMTFAC
// Read String :$DIMTIH
// Read String :$DIMTIX
// Read String :$DIMTM
// Read String :$DIMTMOVE
// Read String :$DIMTOFL
// Read String :$DIMTOH
// Read String :$DIMTOL
// Read String :$DIMTOLJ
// Read String :$DIMTP
// Read String :$DIMTSZ
// Read String :$DIMTVP
// Read String :$DIMTXSTY
// Read String :$DIMTXT
// Read String :$DIMTZIN
// Read String :$DIMUPT
// Read String :$DIMZIN
// Read String :$DISPSILH
// Read String :$DWGCODEPAGE
// Read String :$ELEVATION
// Read String :$ENDCAPS
// Read String :ENDSEC
// Read String :$EXTMAX
// Read String :$EXTMIN
// Read String :$EXTNAMES
// Read String :FFFF
// Read String :$FILLETRAD
// Read String :$FILLMODE
// Read String :$FINGERPRINTGUID
// Read String :$HANDSEED
// Read String :$INSBASE
// Read String :$INSUNITS
// Read String :$JOINSTYLE
// Read String :$LIMCHECK
// Read String :$LIMMAX
// Read String :$LIMMIN
// Read String :$LTSCALE
// Read String :$LUNITS
// Read String :$LUPREC
// Read String :$LWDISPLAY
// Read String :$MAXACTVP
// Read String :$MEASUREMENT
// Read String :$MENU
// Read String :$MIRRTEXT
// Read String :$OLESTARTUP
// Read String :$ORTHOMODE
// Read String :$PDMODE
// Read String :$PDSIZE
// Read String :$PELEVATION
// Read String :$PEXTMAX
// Read String :$PEXTMIN
// Read String :$PINSBASE
// Read String :$PLIMCHECK
// Read String :$PLIMMAX
// Read String :$PLIMMIN
// Read String :$PLINEGEN
// Read String :$PLINEWID
// Read String :$PROXYGRAPHICS
// Read String :$PSLTSCALE
// Read String :$PSTYLEMODE
// Read String :$PSVPSCALE
// Read String :$PUCSORG
// Read String :$PUCSORGBACK
// Read String :$PUCSORGBOTTOM
// Read String :$PUCSORGFRONT
// Read String :$PUCSORGLEFT
// Read String :$PUCSORGRIGHT
// Read String :$PUCSORGTOP
// Read String :$PUCSORTHOVIEW
// Read String :$PUCSXDIR
// Read String :$PUCSYDIR
// Read String :$QTEXTMODE
// Read String :$REGENMODE
// Read String :$SHADEDGE
// Read String :$SHADEDIF
// Read String :$SKETCHINC
// Read String :$SKPOLY
// Read String :$SPLFRAME
// Read String :$SPLINESEGS
// Read String :$SPLINETYPE
// Read String :Standard
// Read String :$SURFTAB1
// Read String :$SURFTAB2
// Read String :$SURFTYPE
// Read String :$SURFU
// Read String :$SURFV
// Read String :$TDCREATE
// Read String :$TDINDWG
// Read String :$TDUCREATE
// Read String :$TDUPDATE
// Read String :$TDUSRTIMER
// Read String :$TDUUPDATE
// Read String :$TEXTSIZE
// Read String :$TEXTSTYLE
// Read String :$THICKNESS
// Read String :$TILEMODE
// Read String :$TRACEWID
// Read String :$TREEDEPTH
// Read String :$UCSORG
// Read String :$UCSORGBACK
// Read String :$UCSORGBOTTOM
// Read String :$UCSORGFRONT
// Read String :$UCSORGLEFT
// Read String :$UCSORGRIGHT
// Read String :$UCSORGTOP
// Read String :$UCSORTHOVIEW
// Read String :$UCSXDIR
// Read String :$UCSYDIR
// Read String :$UNITMODE
// Read String :$USERI1
// Read String :$USERI2
// Read String :$USERI3
// Read String :$USERI4
// Read String :$USERI5
// Read String :$USERR1
// Read String :$USERR2
// Read String :$USERR3
// Read String :$USERR4
// Read String :$USERR5
// Read String :$USRTIMER
// Read String :$VERSIONGUID
// Read String :$VISRETAIN
// Read String :$WORLDVIEW
// Read String :$XEDIT

    return this->setString(ptr, in->getMemHandler());
  }
  else return false;
}

//!

bool 
dimeStringRecord::write(dimeOutput * const out)
{
  if (dimeRecord::write(out)) { // write group code
    return out->writeString(this->string);
  }
  return false;
}

//!

void 
dimeStringRecord::setValue(const dimeParam &param, dimeMemHandler * const memhandler)
{
  if (memhandler) {
    this->string = memhandler->stringAlloc(param.string_data);
  }
  else {
    this->string = new char[strlen(param.string_data)+1];
    if (this->string) strcpy(this->string, param.string_data);
  }
}

//!

void 
dimeStringRecord::getValue(dimeParam &param) const
{
  param.string_data = this->string;
}

