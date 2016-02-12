//
//  ofxProgMesh.h
//
//  Created by satcy on 2015/07/28.
//
//
// Based on Jeff Somers 's Mesh Simplification Viewer
// http://www.jsomers.com/vipm_demo/meshsimp.html


#pragma once

#ifdef FLT_MAX
#else
#define FLT_MAX 3.402823466e+38F
#endif

#include "Vertex.h"
#include "Triangle.h"
#include "Mesh.h"
#include "Prog.h"
#include "ProgThread.h"

namespace ofxProgMesh = ofx::ProgMesh;