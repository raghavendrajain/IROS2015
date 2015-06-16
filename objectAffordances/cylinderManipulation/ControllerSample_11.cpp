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
ofstream myfile ("example_11.csv", ios::out | ios::app );

class MyController : public Controller {  
public:  
  void onInit(InitEvent &evt);  
  double onAction(ActionEvent&);  
  void onRecvMsg(RecvMsgEvent &evt); 
  void onCollision(CollisionEvent &evt); 
private:


  Vector3d initTargetPos, currentTargetPos;
  Vector3d appliedTargetVel, finalTargetVel;
  Rotation initTargetRotation, finalTargetRotation;
  bool isTargetAtRest;   // to check if target is not moving
  bool isXForceVarianceAllowed ;
  bool isZForceVarianceAllowed ;
  double f_x, f_z; 
  bool insideTimer;
  int counterOfAction; 
 
};  
  
Vector3d forceOnTarget;
double forceOnTarget_x, forceOnTarget_z;

void MyController::onInit(InitEvent &evt) {  

  // handle of target 
  SimObj *target = getObj(myname()); 
  target->getPosition(initTargetPos);  // initial target position
  target->getRotation(initTargetRotation); // initial target rotation in quaternion

  isTargetAtRest = true;     // target is at rest initially.
  insideTimer = true; 
  f_x = 0 ;
  f_z = 0 ;
  int xForceVariance = 200;
  int zForceVariance = 200;
  counterOfAction = 0; 

  // Reset the forces applied
  double r;
  r = ((double) rand() / (RAND_MAX)) ;
  f_x = r * int(xForceVariance);
  f_z = r * int(zForceVariance);

  forceOnTarget_z = 400 + f_z;
  forceOnTarget_x = 0;

  forceOnTarget.set(forceOnTarget_x, 0 , forceOnTarget_z);
  target->addForce(forceOnTarget.x(), forceOnTarget.y(), forceOnTarget.z());
  target->getVelocity(appliedTargetVel);
  myfile.flush(); // I have uncommented the file, because I want to overwrite the file.
  

}  

  
double MyController::onAction(ActionEvent &evt) {  

  int actionNumber = 1;
  int targetType = 2;

  myfile << setprecision(2) << std::fixed;

  SimObj *target = getObj(myname());

  if (evt.time() < 5.0)
  {
    cout << evt.time() << endl;
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

           // cout << initTargetRotation.qw() <<  " , " << initTargetRotation.qx() <<  " , " << initTargetRotation.qy() <<  " , " << initTargetRotation.qz() << " , " ;

           myfile << actionNumber  << " , " << targetType << " , " ;
           myfile << initTargetRotation.qw() <<  " , " << initTargetRotation.qx() <<  " , " << initTargetRotation.qy() <<  " , " << initTargetRotation.qz() << " , " ;
           myfile << finalTargetRotation.qw() <<  " , " << finalTargetRotation.qx() <<  " , " << finalTargetRotation.qy() <<  " , " << finalTargetRotation.qz() << " , " ;
           myfile << initTargetPos.x() << " , " << initTargetPos.z() << " , " ;
           myfile << forceOnTarget_x  << " , " << forceOnTarget_z << " , " ;
           myfile << appliedTargetVel.x()  << " , " << appliedTargetVel.z() << " , " ;
           myfile << currentTargetPos.x() << " , " << currentTargetPos.z() << " , " ;
           myfile << finalTargetVel.x() << " , " << finalTargetVel.z() << " , " ;
           myfile << isTargetAtRest << " , " ;
           myfile << currentTargetPos.x() -  initTargetPos.x() << " , " << currentTargetPos.z() - initTargetPos.z();
           myfile << "\n"; 
           cout << "The simulation for " << actionNumber << " for target type " << targetType << " has been recorded" << endl;
  }

      
  return 0.01;    

 }

        
std::string msg = " ";

void MyController::onRecvMsg(RecvMsgEvent &evt) {  

  }  

void MyController::onCollision(CollisionEvent &evt) { 

 }



  
extern "C" Controller * createController() {  
  return new MyController;  
}  

