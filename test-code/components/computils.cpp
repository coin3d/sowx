/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include <stdlib.h>
#include <math.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText3.h>

// *************************************************************************

SoNode *
shape_to_scene_graph(
  SoShape * shape )
{
  SoSeparator * root = new SoSeparator;
  SoBaseColor * basecol = new SoBaseColor;
  basecol->rgb.setValue( float(rand())/float(RAND_MAX),
                         float(rand())/float(RAND_MAX),
                         float(rand())/float(RAND_MAX) );
  root->addChild( basecol );
  root->addChild( shape );
  return (SoNode *) root;
} // shape_to_scene_graph()

// *************************************************************************

SoNode *
get_scene_graph(
  int argc,
  char ** argv )
{
  srand( (unsigned int) time(NULL) );

  if ( argc > 1 ) {
#if HAVE_DIME2SO
#endif
#if HAVE_PROFIT2SO
    if ( prf_openflight_check_file( argv[1] ) )
      return prf_openinventor_import( argv[1] );
#endif
    SoInput in;
    if ( ! strcmp( argv[1], "--cube" ) ) {
      return shape_to_scene_graph( new SoCube );
    } else if ( ! strcmp( argv[1], "--sphere" ) ) {
      return shape_to_scene_graph( new SoSphere );
    } else if ( ! strcmp( argv[1], "--cone" ) ) {
      return shape_to_scene_graph( new SoCone );
    } else if ( ! strcmp( argv[1], "--cylinder" ) ) {
      return shape_to_scene_graph( new SoCylinder );
    } else if ( ! strcmp( argv[1], "--text3" ) ) {
      SoText3 * textnode = new SoText3;
      if ( argc > 2 ) textnode->string.setValue( argv[2] );
      else textnode->string.setValue( "Coin" );
      textnode->parts.setValue( SoText3::ALL );
      return shape_to_scene_graph( textnode );
    } else if ( ! in.openFile( argv[1] ) ) {
      return NULL;
    }
    return SoDB::readAll( &in );
  }

  int randval = rand() % 4;
  switch ( randval ) {
  case 0:  return shape_to_scene_graph( new SoCone );
  case 1:  return shape_to_scene_graph( new SoCube );
  case 2:  return shape_to_scene_graph( new SoCylinder );
  default: return shape_to_scene_graph( new SoSphere );
  }
} // get_scene_graph()

// *************************************************************************
