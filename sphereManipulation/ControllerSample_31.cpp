#include <string>    
#include "ControllerEvent.h"  
#include "Controller.h"  
#include "Logger.h"  
#include <iostream>
#include <fstream>
#include <iomanip>
#include <typeinfo>
#include <ctime>
#include <unistd.h>
#include <sys/time.h>

#include "EntityMotionStatus.cpp"



#define PI 3.141592      
#define DEG2RAD(DEG) ( (PI) * (DEG) / 180.0 )

using namespace std;
ofstream myfile ("example_21.csv", ios::out | ios::app );

class MyController : public Controller {  
public:  
  void onInit(InitEvent &evt);  
  double onAction(ActionEvent&);  
  void onRecvMsg(RecvMsgEvent &evt); 
  void onCollision(CollisionEvent &evt); 


private:

  const char* linkName;
  CParts *link;

  Vector3d initTargetPos, initToolPos, currentToolPos, currentTargetPos;
  Vector3d appliedToolVel, toolVelAtHit, targetVelAtHit, finalTargetVel, finalToolVel;
  Rotation initTargetRotation,  initToolRotation,  finalTargetRotation, finalToolRotation; 
 
 
  bool isTargetAtRest;   // to check if target is not moving
  bool isToolAtRest;     // to check if tool is not moving

  bool isXForceVarianceAllowed ;
  bool isZForceVarianceAllowed ;
  

  double f_x, f_z;
  bool flag;
  bool Colli; 
  bool insideTimer;
  int counterOfCollision; 
  int counterOfAction; 
 
};  
  
Vector3d forceOnTool;
double forceOnTool_x, forceOnTool_z;

void MyController::onInit(InitEvent &evt) {  

  // handle of target and tool
  SimObj *target = getObj("box_007");
  SimObj *toolName = getObj("TShapeTool_007");

  toolName->setMass(10.0); // mass of all the tools should be set uniformly
  
  target->getPosition(initTargetPos);  // initial target position
  target->getRotation(initTargetRotation); // initial target rotation in quaternion

  toolName->getPosition(initToolPos);  // initial tool position
  toolName->getRotation(initToolRotation); // initial tool rotation in quaternion

  isTargetAtRest = true;     // target and tool both are at rest initially.
  isToolAtRest = true;  
  insideTimer = true; 
  f_x = 0 ;
  f_z = 0 ;

  int xForceVariance = 4000;
  int zForceVariance = 4000;

  flag = true;
  Colli = false; 
  counterOfCollision = 0; 
  counterOfAction = 0; 


  // Reset the forces applied
  double r;
  r = ((double) rand() / (RAND_MAX)) ;
  f_x = r * int(xForceVariance);
  f_z = r * int(zForceVariance);

  forceOnTool_x = -8000 - f_x;
  forceOnTool_z = 8000 + f_x;

  forceOnTool.set(forceOnTool_x, 0 , forceOnTool_z);

  toolName->addForce(forceOnTool.x(), forceOnTool.y(), forceOnTool.z());
  toolName->getVelocity(appliedToolVel);


  myfile.flush(); // I have uncommented the file, because I want to overwrite the file.
  



//   if (myfile.is_open())
//   {
//      myfile << "toolVelAtHit_X" << " , " << "toolVelAtHit_Z" << " , ";  
//      myfile <<  "targetInitialVel_X" << " , " << "targetInitialVel_Z"  << " , ";
//      myfile << "Action" << " , " << "FunctionalFeature" << " , " ;
//      myfile << "forceOnTool_X" << " , " << "forceOnTool_Z " <<  " , ";
//      myfile << "InitialToolPos_X" << " , " << "InitialToolPos_Z" << " , ";
//      myfile << "InitialTargetPos_X" << " , " << "InitialTargetPos_Z" << " , ";
//      myfile <<  "targetFinalPos_X" << " , " << "targetFinalPos_Z"  << " , ";
//      myfile << "targetDisplacement_X" << " , " << "targetDisplacement_Z";
//      myfile <<  "\n" ;

// }


}  



  
double MyController::onAction(ActionEvent &evt) {  

  int actionNumber = 3;
  int functionalFeature = 1;
  int targetType = 3;

  myfile << setprecision(2) << std::fixed;

  // handle of target and tool
  SimObj *target = getObj("box_007");
  SimObj *toolName = getObj("TShapeTool_007");

  if (evt.time() < 5.0)
  {
    // cout << "Time" << endl;
    cout << evt.time() << endl;

    toolName->getPosition(currentToolPos); // get the current tool position
    toolName->getRotation(finalToolRotation);
    toolName->getVelocity(finalToolVel);
    isToolAtRest   = checkEntityMotionStatus(toolName);  // checks whether the tool is moving by calculating its velocity

    target->getPosition(currentTargetPos);
    target->getRotation(finalTargetRotation);
    target->getVelocity(finalTargetVel);
    isTargetAtRest = checkEntityMotionStatus(target);    // checks whether the object is moving by calculating its velocity
    
  }

  if (evt.time() > 5.0)
  {
     insideTimer = false;
     counterOfAction ++ ; 
  }

  if(!insideTimer && counterOfAction == 1 )
  {
           myfile << actionNumber  << " , " << functionalFeature << " , " ;
           myfile << initToolRotation.qw() <<  " , " << initToolRotation.qx() <<  " , " << initToolRotation.qy() <<  " , " << initToolRotation.qz() << " , " ;
           myfile << initTargetRotation.qw() <<  " , " << initTargetRotation.qx() <<  " , " << initTargetRotation.qy() <<  " , " << initTargetRotation.qz() << " , " ;
           myfile << finalTargetRotation.qw() <<  " , " << finalTargetRotation.qx() <<  " , " << finalToolRotation.qy() <<  " , " << finalToolRotation.qz() << " , " ;
           myfile << initToolPos.x() << " , " << initToolPos.z() << " , " ;
           myfile << initTargetPos.x() << " , " << initTargetPos.z() << " , " ;
           myfile << forceOnTool_x  << " , " << forceOnTool_z << " , " ;
           myfile << appliedToolVel.x()  << " , " << appliedToolVel.z() << " , " ;
           myfile << toolVelAtHit.x() << " , " << toolVelAtHit.z() << " , " ;
           myfile << targetVelAtHit.x() << " , " << targetVelAtHit.z() << " , " ;
           myfile << currentToolPos.x() << " , " << currentToolPos.z() << " , " ;
           myfile << currentTargetPos.x() << " , " << currentTargetPos.z() << " , " ;
           myfile << finalToolVel.x() << " , " << finalToolVel.z() << " , " ;
           myfile << finalTargetVel.x() << " , " << finalTargetVel.z() << " , " ;
           myfile   << isToolAtRest <<  " , " << isTargetAtRest << " , " ;
           myfile << currentToolPos.x() -  initToolPos.x() << " , " << currentToolPos.z() - initToolPos.z() << " , " ;
           myfile << currentTargetPos.x() -  initTargetPos.x() << " , " << currentTargetPos.z() - initTargetPos.z();
           myfile << "\n"; 
           cout << "The simulation for " << actionNumber << " , " << functionalFeature << " has been recorded" << endl;
           // exit(0);
           flag = false;  
  }

      
  return 0.01;    

 }

        
std::string msg = " ";

void MyController::onRecvMsg(RecvMsgEvent &evt) {  

  }  

void MyController::onCollision(CollisionEvent &evt) { 

  Colli = true; 
  counterOfCollision ++ ; 
    // handle of target and tool
  SimObj *target = getObj("box_007");
  SimObj *toolName = getObj("TShapeTool_007");

  if(Colli && counterOfCollision == 1 )
  {
      toolName->getVelocity(toolVelAtHit);
      target->getVelocity(targetVelAtHit);
      cout << "The tool velocity is " << toolVelAtHit.x() << " , " << toolVelAtHit.z() << endl;
      cout << "The target velocity is " << targetVelAtHit.x() << " , " << targetVelAtHit.z() << endl;
      // myfile << toolVelAtHit.x() << " , " << toolVelAtHit.z() << " , " ;
      // myfile << targetVelAtHit.x() << " , " << targetVelAtHit.z() << " , " ;
      Colli = false;

  }



}



  
extern "C" Controller * createController() {  
  return new MyController;  
}  

