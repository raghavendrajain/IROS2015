#ifndef _RESETENTITY_H
#define _RESETENTITY_H

#include "ControllerEvent.h"  
#include "Controller.h"  
#include "Logger.h"  
#include <iostream>
#include <fstream>
#include <iomanip>
#include <boost/python.hpp>
#include <Python.h>
#include <dlfcn.h>

#include "ControlPosition.h"


using namespace std;


bool resetEntityToolPosition(SimObj *entity, Vector3d desiredPosition);
bool resetEntityTargetPosition(SimObj *entity, SimObj *baseTool, Vector3d desiredPosition); // baseTool is the tool whose initial position is taken as the origin. 
bool resetEntityOrientation(SimObj *entity, Rotation goalRotation);
float randomFloat(float min, float max);
bool newEntityPose(SimObj *entity, Rotation initToolRotation, double randomStep);
bool resetRandomTargetOrientation(SimObj *entity);
// double* resetToNewToolRotation(SimObj *entity, Rotation initialRotation);

void maintainOrientationOfTool(SimObj * entity, Rotation initialToolRot);


#endif