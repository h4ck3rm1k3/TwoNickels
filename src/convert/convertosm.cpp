/**************************************************************************\
 *
 *  This source file is part of DIME.
 *  Copyright (C) 1998-2001 by Systems In Motion.  All rights reserved.
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
 *  NORWAY                                               Fax: +47 22207097
 *
\**************************************************************************/

#include <dime/convert/convertOSM.h>
#include <dime/convert/layerdata.h>
#include "convert_funcs.h"

#include <dime/entities/Entity.h>
#include <dime/entities/Insert.h>
#include <dime/sections/HeaderSection.h>
#include <dime/Model.h>
#include <dime/State.h>
#include <dime/Layer.h>
#include <iostream>

#include <dime/entities/Point.h>
#include <dime/entities/Line.h>
#include <dime/entities/Polyline.h>
#include <dime/entities/LWPolyline.h>
#include <dime/entities/MText.h>

#include <dime/entities/Vertex.h>

using namespace std;

/*!
  \class dxfConverterOSM convert.h
  \brief The dxfConverterOSM class offers a simple interface for dxf converting.
  It makes it possible to extract all geometry from dxf files, and store
  it in internal geometry sturctures, which again can be exported as 
  osm.
*/


/*!
  \fn void dxfConverterOSM::setNumSub(const int num)
  Sets the number of subdivisions for a circle or ellipse. 
  This overrides the value set in dxfConverterOSM::setMaxerr() and 
  should normally not be used

  \sa dxfConverterOSM::getNumSub()
*/

/*!
  \fn int dxfConverterOSM::getNumSub() const
  Returns the numner of subdivisions set by dxfConverterOSM::setNumSub()
*/

/*!
  \fn void dxfConverterOSM::setMaxerr(const dxfdouble maxerr)
  Sets the maximum error allowed when converting circles, arcs
  and ellipses into lines/polygons.
*/

/*!
  \fn dxfdouble dxfConverterOSM::getMaxerr() const
  Returns the maximum allowed error when converting circles, arcs
  ellipses.
*/
  
/*!
  \fn void dxfConverterOSM::setFillmode(const bool fill)
  Sets whether polylines with width and SOLID and TRACE should be filled.
*/

/*!
  \fn bool dxfConverterOSM::getFillmode() const
  Returns whether polylines with width and SOLID and TRACE should be filled.
*/

/*!
  \fn bool dxfConverterOSM::getLayercol() const
  Returns whether only layers should be used (and not color index) when
  converting
*/
  
/*!
  \fn void dxfConverterOSM::setLayercol(const bool v)
  Sets whether only layer (and not color index) should be used when converting.
  This method should normally no be used.
*/

/*!
  \fn int dxfConverterOSM::getCurrentInsertColorIndex() const
  Returns the color index of the current INSERT entity. If no INSERT
  entity is current, the color index 7 (white) will be returned.
*/


/*!
  Constructor
 */
dxfConverterOSM::dxfConverterOSM()
{
  this->maxerr = 0.1f;
  this->numsub = -1;
  this->fillmode = true;
  this->layercol = false;
  this->currentInsertColorIndex =  7;
  this->currentPolyline = NULL;
  for (int i = 0; i < 255; i++) layerData[i] = NULL;
}

/*!
  Destructor
*/
dxfConverterOSM::~dxfConverterOSM()
{
  for (int i = 0; i < 255; i++) {
    delete layerData[i];
  }
}

/*!
  Returns a dxfLayerData instance for the color with color index \a colidx.
*/
dxfLayerData *
dxfConverterOSM::getLayerData(const int colidx)
{
  assert(colidx >= 1 && colidx <= 255);
  if (layerData[colidx-1] == NULL) {
    layerData[colidx-1] = new dxfLayerData(colidx);
  }
  return layerData[colidx-1];
}

/*!
  Finds the color index for \a entity, and returns the dxfLayerData for it.
*/
dxfLayerData *
dxfConverterOSM::getLayerData(const dimeEntity *entity)
{
  // special case for VERTEX
  if (this->currentPolyline && entity->typeId() == dimeBase::dimeVertexType) {
    if (!(entity->getEntityFlags() & FLAG_COLOR_NUMBER))
      return getLayerData(this->currentPolyline);
  }

  int colidx = getColorIndex(entity);
  if (colidx == 0) { // BYBLOCK
    colidx = this->currentInsertColorIndex;
  }
  // we don't care if layer is turned off (negative color)
  if (colidx < 0) colidx = -colidx;
    
  if (colidx < 1 || colidx > 255) { // just in case
    fprintf(stderr,"Illegal color number %d. Changed to 7 (white)\n",
	    colidx);
    colidx = 7;
  }
  return getLayerData(colidx);
}

/*!
  Returns a pointer to the dxfLayerData array.
*/
dxfLayerData **
dxfConverterOSM::getLayerData()
{
  return layerData;
}


/*!
  Converts \a model to the internal geometry structures.
  \sa dxfConverterOSM::writeOSM()
*/
bool 
dxfConverterOSM::doConvert(dimeModel &model)
{  
  //
  // remove these 6 lines, and you may merge several dxf
  // files into a single osm file by calling doConvert() several
  // times before calling writeOsm
  //
  for (int i = 0; i < 255; i++) {
    if (layerData[i]) {
      delete layerData[i];
      layerData[i] = NULL;
    }
  }
  cout << "<?xml version='1.0' encoding='UTF-8'?>\n<osm version='0.6' generator='TwoNickels'>\n\n";
  bool ret= model.traverseEntities(dime_callback, this, 
				false, true, false);

  cout << "</osm>\n\n";
  return ret;

}

/*!
  Writes the internal geometry structures to \a out.
*/
bool
dxfConverterOSM::writeOSM(FILE *out)
{
#ifndef NOWRLEXPORT
  //
  // write header
  //
 


  // write each used layer/color
  //
  for (int i = 0; i < 255; i++) {
    if (layerData[i] != NULL) {
      layerData[i]->writeOSM(out, 0);
      delete layerData[i]; layerData[i] = NULL;
    }
  }
#endif // NOWRLEXPORT
  return true;
}

/*!
  Finds the correct color index for \a entity. Handles the BYLAYER case.
*/
int
dxfConverterOSM::getColorIndex(const dimeEntity *entity)
{
  int colnum = entity->getColorNumber();
  if (this->layercol || colnum == 256) {
    const dimeLayer *layer = entity->getLayer();
    colnum = layer->getColorNumber();
  }
  return colnum;
}

//
// forward the call to the correct class instance
//
bool 
dxfConverterOSM::dime_callback(const dimeState * const state, 
			    dimeEntity *entity, void *data)
{
  return ((dxfConverterOSM*)data)->private_callback(state, entity);
}

template <class T> void convert(dxfConverterOSM & rthis,const dimeState * const state,  T & r);

template <> void convert(dxfConverterOSM & rthis,dimeState const*, dimePolyline & rObj)
{
  //  std::cout << "<way  id='-"<< rObj.getEntityNum () 	    <<  "' version='" << 1<< "' />"  << std::endl;
  //  rObj.print();
  //  rObj.print();
  for (int i = 0; i < rObj.getNumCoordVertices(); i++) { 
    
    const dimeVec3f &  rCoords = rObj.getCoordVertex(i)->getCoords();
    double e=rCoords[0];
    double n=rCoords[1];

    cPoint p(e,n);
    long id = rthis.lookup(p);
    if (id) 
      {
	//	  cerr << "found id " << id  << "|" << p.key()<< endl;
      }
    else
      {
	id = dimeEntity::getNewEntityCount();
	rthis.add(p,id);
	id = rthis.lookup(p);
	
	double lat,lon;
	convertPoint(e, n, lat, lon) ;
	
	//	cerr << "added found id " << id << "|"  << p.key() << endl;
	std::cout << "<node  id='-"<< id 	    <<  "' version='" << 1<< "' "  << std::endl;
	
	std::cout.width(10);
	std::cout.precision(10);	
	std::cout  <<  "lat='" << lat<< "'";
	std::cout.width(10);
	std::cout.precision(10);  
	std::cout  <<  "lon='" << lon   << "' >" << std::endl;	  

	std::cout.width(10);
	std::cout.precision(10);
	
	std::cout << "<tag k='ac:easting' v='" <<   e << "'/>\n";
	std::cout.width(10);
	std::cout.precision(10);
	
	std::cout << "<tag k='ac:northing' v='" <<   n << "'/>\n";	
	std::cout << "<tag k='ac:part_of_poly' v='" <<   rObj.getEntityNum () << "'/>\n";
	std::cout << "<tag k='ac:layer_name' v='" <<   rObj.getLayerName() << "'/>\n";
	std::cout << "<tag k='ac:entityFlags' v='" <<   rObj.getEntityFlags() << "'/>\n";
	std::cout << "<tag k='ac:color' v='" <<   rObj.getColorNumber() << "'/>\n";
	
	std::cout << "</node>"  << std::endl;	
	
      }
  } 

  
  std::cout << "<way  id='-"<< rObj.getEntityNum () 	    <<  "' version='" << 1<< "' >"  << std::endl;

  std::cout << "<tag k='ac:type' v='dimePolyline'/>\n";

  std::cout << "<tag k='ac:layer_name' v='" <<   rObj.getLayerName() << "'/>\n";
  std::cout << "<tag k='ac:entityFlags' v='" <<   rObj.getEntityFlags() << "'/>\n";
  std::cout << "<tag k='ac:color' v='" <<   rObj.getColorNumber() << "'/>\n";

  for (int i = 0; i < rObj.getNumCoordVertices(); i++) { 
    
    const dimeVec3f &  rCoords = rObj.getCoordVertex(i)->getCoords();
    double e=rCoords[0];
    double n=rCoords[1];
    cPoint p(e,n);
      
    long id = rthis.lookup(p);      
    if (id) 
      {
	cout << "<nd ref=\'-" << id  << "\'/>" << endl;
      }
    else
      {
	cerr << "BUG not found id " << id << "|" << p.key() << endl;
      }
  } 

  std::cout << "</way>"  << std::endl;

}

template <> void convert(dxfConverterOSM & rthis,dimeState const*, dimeLWPolyline & rObj)
{
  //  rObj.print();
  for (int i = 0; i < rObj.getNumVertices(); i++) { 
    double e=rObj.getXCoords()[i];
    double n=rObj.getYCoords()[i];
    cPoint p(e,n);
    long id = rthis.lookup(p);
    if (id) 
      {
	//	  cerr << "found id " << id  << "|" << p.key()<< endl;
      }
    else
      {
	id = dimeEntity::getNewEntityCount();
	rthis.add(p,id);
	id = rthis.lookup(p);
	
	double lat,lon;
	convertPoint(e, n, lat, lon) ;
	
	//	cerr << "added found id " << id << "|"  << p.key() << endl;
	std::cout << "<node  id='-"<< id 	    <<  "' version='" << 1<< "' "  << std::endl;
	
	std::cout.width(10);
	std::cout.precision(10);	
	std::cout  <<  "lat='" << lat<< "'";
	std::cout.width(10);
	std::cout.precision(10);  
	std::cout  <<  "lon='" << lon   << "' >" << std::endl;	  

	std::cout.width(10);
	std::cout.precision(10);
	
	std::cout << "<tag k='ac:easting' v='" <<   e << "'/>\n";
	std::cout.width(10);
	std::cout.precision(10);
	
	std::cout << "<tag k='ac:northing' v='" <<   n << "'/>\n";	
	std::cout << "<tag k='ac:part_of_poly' v='" <<   rObj.getEntityNum () << "'/>\n";
	std::cout << "<tag k='ac:layer_name' v='" <<   rObj.getLayerName() << "'/>\n";
	std::cout << "<tag k='ac:entityFlags' v='" <<   rObj.getEntityFlags() << "'/>\n";
	std::cout << "<tag k='ac:color' v='" <<   rObj.getColorNumber() << "'/>\n";
	
	std::cout << "</node>"  << std::endl;	
	
      }
  } 

  
  std::cout << "<way  id='-"<< rObj.getEntityNum () 	    <<  "' version='" << 1<< "' >"  << std::endl;
  std::cout << "<tag k='ac:type' v='dimeLWPolyline'/>\n";
  std::cout << "<tag k='ac:layer_name' v='" <<   rObj.getLayerName() << "'/>\n";
  std::cout << "<tag k='ac:entityFlags' v='" <<   rObj.getEntityFlags() << "'/>\n";
  std::cout << "<tag k='ac:color' v='" <<   rObj.getColorNumber() << "'/>\n";

  for (int i = 0; i < rObj.getNumVertices(); i++) { 
      //    fprintf(stderr,"coord: %f %f\n", xcoord[i], ycoord[i]);   
      cPoint p(rObj.getXCoords()[i],rObj.getYCoords()[i]);
      
      long id = rthis.lookup(p);      
      if (id) 
	{
	  cout << "<nd ref=\'-" << id  << "\'/>" << endl;
	}
      else
	{
	  cerr << "BUG not found id " << id << "|" << p.key() << endl;
	}
  } 

  std::cout << "</way>"  << std::endl;

}

template <> void convert(dxfConverterOSM & rthis,dimeState const*, dimeLine & rObj)
{
  for (int i = 0; i < 2; i++) { 
    
    const dimeVec3f &  rCoords = rObj.getCoords(i);
    double e=rCoords[0];
    double n=rCoords[1];

    cPoint p(e,n);
    long id = rthis.lookup(p);
    if (id) 
      {
	//	  cerr << "found id " << id  << "|" << p.key()<< endl;
      }
    else
      {
	id = dimeEntity::getNewEntityCount();
	rthis.add(p,id);
	id = rthis.lookup(p);
	
	double lat,lon;
	convertPoint(e, n, lat, lon) ;
	
	//	cerr << "added found id " << id << "|"  << p.key() << endl;
	std::cout << "<node  id='-"<< id 	    <<  "' version='" << 1<< "' "  << std::endl;
	
	std::cout.width(10);
	std::cout.precision(10);	
	std::cout  <<  "lat='" << lat<< "'";
	std::cout.width(10);
	std::cout.precision(10);  
	std::cout  <<  "lon='" << lon   << "' >" << std::endl;	  

	std::cout.width(10);
	std::cout.precision(10);
	
	std::cout << "<tag k='ac:easting' v='" <<   e << "'/>\n";
	std::cout.width(10);
	std::cout.precision(10);
	
	std::cout << "<tag k='ac:northing' v='" <<   n << "'/>\n";	
	std::cout << "<tag k='ac:part_of_poly' v='" <<   rObj.getEntityNum () << "'/>\n";
	std::cout << "<tag k='ac:layer_name' v='" <<   rObj.getLayerName() << "'/>\n";
	std::cout << "<tag k='ac:entityFlags' v='" <<   rObj.getEntityFlags() << "'/>\n";
	std::cout << "<tag k='ac:color' v='" <<   rObj.getColorNumber() << "'/>\n";
	
	std::cout << "</node>"  << std::endl;	
	
      }
  } 

  
  std::cout << "<way  id='-"<< rObj.getEntityNum () 	    <<  "' version='" << 1<< "' >"  << std::endl;


  std::cout << "<tag k='ac:type' v='dimeLine'/>\n";
  std::cout << "<tag k='ac:layer_name' v='" <<   rObj.getLayerName() << "'/>\n";
  std::cout << "<tag k='ac:entityFlags' v='" <<   rObj.getEntityFlags() << "'/>\n";
  std::cout << "<tag k='ac:color' v='" <<   rObj.getColorNumber() << "'/>\n";

  for (int i = 0; i < 2; i++) { 
    
    const dimeVec3f &  rCoords = rObj.getCoords(i);
    double e=rCoords[0];
    double n=rCoords[1];
    cPoint p(e,n);
      
    long id = rthis.lookup(p);      
    if (id) 
      {
	cout << "<nd ref=\'-" << id  << "\'/>" << endl;
      }
    else
      {
	cerr << "BUG not found id " << id << "|" << p.key() << endl;
      }
  } 

  std::cout << "</way>"  << std::endl;

}

template <> void convert(dxfConverterOSM & rthis,dimeState const*, dimePoint & rObj)
{
  std::cout << "<node  id='-"<< rObj.getEntityNum () 	    <<  "' version='" << 1<< "' "  << std::endl;

  const dimeVec3f &  rCoords = rObj.getCoords();
  double e=rCoords[0];
  double n=rCoords[1];
  cPoint p(e,n);
  double lat,lon;
  convertPoint(e, n, lat, lon) ;

  std::cout.width(10);
  std::cout.precision(10);
  
  std::cout  <<  "lat='" << lat<< "'";
  
  std::cout.width(10);
  std::cout.precision(10);
  
  std::cout  <<  "lon='" << lon   << "' >" << std::endl;	  
  
  long id = rthis.lookup(p);
  if (id) 
    {
      cerr << "found id " << id  << "|" << p.key()<< endl;
    }
  else
    {
      id = rObj.getEntityNum ();
      rthis.add(p,id);
      id = rthis.lookup(p);
      if (id) 
	{
	  cerr << "added point id " << id  << "|" << p.key()<< endl;
	}
      else
	{
	  cerr << "BUG not found id " << id  << "|" << p.key()<< endl;
	}
      
    }


  std::cout << "<tag k='ac:type' v='dimePoint'/>\n";

  std::cout.width(10);
  std::cout.precision(10);

  std::cout << "<tag k='ac:easting' v='" <<   rCoords[0] << "'/>\n";
  std::cout.width(10);
  std::cout.precision(10);

  std::cout << "<tag k='ac:northing' v='" <<   rCoords[1] << "'/>\n";

  std::cout << "<tag k='ac:layer_name' v='" <<   rObj.getLayerName() << "'/>\n";
  std::cout << "<tag k='ac:entityFlags' v='" <<   rObj.getEntityFlags() << "'/>\n";
  std::cout << "<tag k='ac:color' v='" <<   rObj.getColorNumber() << "'/>\n";

  rObj.print();
  std::cout << "</node>"  << std::endl;

}

string escape(string s)
{
    
  //  {"quot", 34},
  //    {"amp", 38},
  //    {"lt", 60},
  //    {"gt", 62},
  //  s.replace(begIndex, 1, "&whatever");
  const string delims("<>\'&\"");
  string::size_type begIndex, endIndex;
  
  begIndex = s.find_first_of(delims);
  while (begIndex != string::npos)
    {
      cerr << "Looking at "  << s[begIndex]  << " at " << begIndex << " in " << s << endl;

      if (s[begIndex] == '&')
	{
	  s.replace(begIndex,1, "&amp;");
	  begIndex += 4;
	}
      else if (s[begIndex] == '<')
	{
	  s.replace(begIndex,1, "&lt;");
	  begIndex += 4;
	}
      else if (s[begIndex] == '>')
	{
	  s.replace(begIndex,1, "&gt;");
	  begIndex += 4;
	}
      else if (s[begIndex] == '"')
	{
	  s.replace(begIndex,1, "&quot;");
	  begIndex += 5;
	}
      else if (s[begIndex] == '\'')
	{
	  s.replace(begIndex,1, "&#39;");
	  begIndex += 4;
	}      
      begIndex = s.find_first_of(delims, begIndex);
    }

  return s;
}

// mTEXT
template <> void convert<dimeMText>(dxfConverterOSM & rthis, dimeState const*, dimeMText & rObj)
{
  //  rObj.print();

  long east = rObj.getEasting();
  long north = rObj.getNorthing();
  cPoint p(east,north);

  long id = rthis.lookup(p);
  if (id) 
    {
      cerr << "found mtext id " << id  << "|" << p.key()<< endl;
    }
  else
    {
      id = rObj.getEntityNum ();


      rthis.add(p,id);
      id = rthis.lookup(p);
      if (id) 
	{
	  cerr << "added mtext id " << id  << "|" << p.key()<< endl;
	}
      else
	{
	  cerr << "BUG not found id " << id  << "|" << p.key()<< endl;
	}
      
    }


  std::cout << "<node id='-"<< rObj.getEntityNum ()
	    <<  "' version='" << 1<< "' " ;

  std::cout.width(10);
  std::cout.precision(10);
  
  std::cout  <<  "lat='" << rObj.getLat()<< "'";
  
  std::cout.width(10);
  std::cout.precision(10);
  
  std::cout  <<  "lon='" << rObj.getLon()   << "' >" << std::endl;	  
  std::cout << "<tag k='type' v='" << rObj.getType() << "'/>\n";

  std::cout << "<tag k='ac:type' v='dimeMText'/>\n";
  
  std::cout << "<tag k='name' v='" << escape(rObj.getText()) << "'/>\n";
  std::cout.width(10);
  std::cout.precision(10);
  
  std::cout << "<tag k='easting' v='" << rObj.getEasting() << "' />"<< std::endl;
  std::cout.width(10);
  std::cout.precision(10);
  std::cout << "<tag k='northing' v='" << rObj.getNorthing() << "' />" << std::endl;

  std::cout << "<tag k='ac:layer_name' v='" <<   rObj.getLayerName() << "'/>\n";
  std::cout << "<tag k='ac:entityFlags' v='" <<   rObj.getEntityFlags() << "'/>\n";
  std::cout << "<tag k='ac:color' v='" <<   rObj.getColorNumber() << "'/>\n";

  std::cout << "</node>" << std::endl;

}

  void dxfConverterOSM::add(double lat, double lon,int id)
  {
    cPoint temp(lat, lon);
    points[temp] = id;

  }

  void dxfConverterOSM::add(const cPoint & temp,int id)
  {
    points[temp] = id;

  }

long dxfConverterOSM::lookup(double lat, double lon)
  {
    cPoint temp(lat, lon);
    return lookup(temp);
  }

long dxfConverterOSM::lookup(const  cPoint & point)
  {
    TMapCoordID::iterator j= points.find(point);
    if (j !=  points.end())
      {
	if (j->first == point)
	  {
	    return j->second;
	  }
	else
	  {
	    return 0;
	  }
      }
    else
      {
	return 0;
      }

  }


//
// handles the callback from the dime-library
//
bool 
dxfConverterOSM::private_callback(const dimeState * const state, 
			       dimeEntity *entity)
{ 
  if (entity->typeId() == dimeBase::dimePolylineType) {
    this->currentPolyline = entity;
  }

  if (state->getCurrentInsert()) {
    this->currentInsertColorIndex = 
      getColorIndex((dimeEntity*)state->getCurrentInsert());
  }
  else {
    this->currentInsertColorIndex = 7;
  }

  dxfLayerData *ld = getLayerData(entity);

  // fillmode on by default. entities which will not fill its polygons
  // should turn it off (layerData::addQuad() will create polygons,
  // not lines)
  //
  ld->setFillmode(true);
  
  switch (entity->typeId()) { 
  case dimeBase::dime3DFaceType:
    ///    convert_3dface(entity, state, ld, this);
    break;
  case dimeBase::dimeSolidType:
    //    convert_solid(entity, state, ld, this);
    break;
  case dimeBase::dimeTraceType:
    //    convert_solid(entity, state, ld, this);
    break;
  case dimeBase::dimeArcType:
    //    convert_arc(entity, state, ld, this);
    break;
  case dimeBase::dimeCircleType:
    //    convert_circle(entity, state, ld, this);
    break;
  case dimeBase::dimeEllipseType:
    //    convert_ellipse(entity, state, ld, this);
    break;
  case dimeBase::dimeInsertType:
    // handled in traverseEntities
    break;
  case dimeBase::dimeBlockType:
    // handled in traverseEntities
    break;
  case dimeBase::dimeLineType:
    //convert_line(entity, state, ld, this);
    convert( *this,state,*(dimeLine*)entity);    
    break;
  case dimeBase::dimeLWPolylineType:
    //convert_lwpolyline(entity, state, ld, this);
    convert( *this,state,*(dimeLWPolyline*)entity);    
    break;
  case dimeBase::dimePointType:
    convert( *this,state,*(dimePoint*)entity);    
    //    convert_point(entity, state, ld, this);
    break;
  case dimeBase::dimePolylineType:
    //    convert_polyline(entity, state, ld, this);
    convert( *this,state,*(dimePolyline*)entity);    
    break;

  case dimeBase::dimeMTextType:
    convert( *this,state,*(dimeMText*)entity);    
    break;
  case dimeBase::dimeHatchType:
    break;
  case dimeBase::dimeEndBlockType:
    break;
  default:


    std::cerr << "Unknown type encounterd " << "\t";
    std::cerr << "Name:" << entity->getEntityName() << "\t";
    std::cerr << "Type:" << entity->typeId() <<std::endl; 

    break;
  }
  return true;
}

/*!
  Finds the state of supported header variables in \a model. This
  method should be called before dxfxConverter::doConvert()
*/
void 
dxfConverterOSM::findHeaderVariables(dimeModel &model)
{
  dimeHeaderSection *hs = (dimeHeaderSection*)
    model.findSection("HEADER");

  if (hs) {
    dimeParam param;
    int groupcode;

    if (hs->getVariable("$FILLMODE", &groupcode, &param, 1) == 1) {
      if (groupcode == 70)
	this->fillmode = (bool) param.int16_data;
    }
  }
}


