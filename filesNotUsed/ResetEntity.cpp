#include "ResetEntity.h"
#include "ControlPosition.h"

bool resetEntityToolPosition(SimObj *entity, Vector3d desiredPosition){

  Vector3d currentPos;
  entity->getPosition(currentPos);	
 

  Vector3d differenceInPos;
  differenceInPos.x(desiredPosition.x() - currentPos.x());
  differenceInPos.y(desiredPosition.y() - currentPos.y());
  differenceInPos.z(desiredPosition.z() - currentPos.z());

  double* ptr1 = NULL;
  // cout << "The desiredPosition = " << desiredPosition.x() << "  ,  " << desiredPosition.z() << endl ; 
  ptr1 = controlPosition(desiredPosition, currentPos, 4.0,0.2, 1.2);
  // cout << "The velocity should be applied on tool for position reset is  " << ptr1[0] << " , " << ptr1[1] << endl; 
  double xVel = ptr1[0];
  double yVel = ptr1[1];
  // cout << "xVel = " << xVel << endl;
  // cout << "yVel = " << yVel << endl;


  entity->setLinearVelocity( xVel , 0, yVel );
    
  // Vector3d currentLinearVelocity;  
  // entity->getLinearVelocity(currentLinearVelocity);
  // cout << "currentLinearVelocity = " << currentLinearVelocity.x() << " , " << currentLinearVelocity.z() << endl;


  // LOG_MSG((" current entity pos is : %f %f %f ", currentPos.x(), currentPos.y(), currentPos.z() ));

  if ( abs(differenceInPos.x()) < 0.1 && abs(differenceInPos.z()) < 0.1)
  {
            // LOG_MSG((" Tool reset at : %f %f %f ", currentPos.x(), currentPos.y(), currentPos.z() ));
            // LOG_MSG (("The difference in position is: %f, %f, %f ", differenceInPos.x(), differenceInPos.y(), differenceInPos.z() )) ; 
            differenceInPos.set(0,0,0);
            // cout << "The entity is reset" << endl;
            return true;

  }


}



bool resetEntityTargetPosition(SimObj *entity, SimObj *baseTool, Vector3d desiredPosition){

  Vector3d currentPos;
  entity->getPosition(currentPos); 

  Vector3d finalTargetPos;
  finalTargetPos = desiredPosition; 

  Vector3d cordinateOrigin;
  baseTool->getPosition(cordinateOrigin);
  cout << "The initial position of the tool is taken as origin of cordinate system. The origin is " << cordinateOrigin.x() << " , " << cordinateOrigin.z();
  cout << endl;


  double xCurrentCord = currentPos.x() - cordinateOrigin.x();
  double zCurrentCord = currentPos.z() - cordinateOrigin.z();

  double xInitCord = desiredPosition.x() - cordinateOrigin.x();
  double zInitCord = desiredPosition.z() - cordinateOrigin.z();

  double currentDistFromOrgin = sqrt( pow(xCurrentCord, 2.0) +  pow(zCurrentCord, 2.0) );
  double initDistFromOrgin    = sqrt( pow(xInitCord, 2.0) + pow(zInitCord, 2.0) );



  int originalQuadOfTarget;
  int currentQuadOfTarget ;
  bool farOnZ = true;
  bool farOnX = true;

  cout << "The current x and z cordinates of target are  " << xCurrentCord << " , " << zCurrentCord << endl;
  cout << "The initial x and z cordinates of target are  " << xInitCord << " , " << zInitCord << endl;

    // checking the initial quadrant based on the initial position of target object.

  if (xInitCord < 0 && zInitCord < 0)
  {
    originalQuadOfTarget = 3;
    cout << "The initial quadrant is " << originalQuadOfTarget << endl;
  }

  if (xInitCord < 0 && zInitCord > 0)
  {
    originalQuadOfTarget = 2;
    cout << "The initial quadrant is " << originalQuadOfTarget << endl;
  }

  if (xInitCord > 0 && zInitCord < 0)
  {
    originalQuadOfTarget = 4;
    cout << "The initial quadrant is " << originalQuadOfTarget << endl;
  }

  if (xInitCord > 0 && zInitCord > 0)
  {
    originalQuadOfTarget = 1;
    cout << "The initial quadrant is " << originalQuadOfTarget << endl;
  }

  if (zCurrentCord > zInitCord )
  {
      farOnZ = false;   
      cout << "Target faces obstacle in current position " << farOnZ << endl;     
  }


  // checking the current quadrant based on the current position of target object.

  if (xCurrentCord < 0 && zCurrentCord < 0)
  {
    currentQuadOfTarget = 3;
    cout << "The current quadrant is " << currentQuadOfTarget << endl;
  }

  if ( xCurrentCord  < 0 && zCurrentCord > 0)
  {
     currentQuadOfTarget = 2;
     cout << "The current quadrant is " << currentQuadOfTarget << endl;
  }

  if ( xCurrentCord > 0 && zCurrentCord < 0)
  {
     currentQuadOfTarget = 4;
     cout << "The current quadrant is " << currentQuadOfTarget << endl;
  }

  if ( xCurrentCord > 0 && zCurrentCord > 0)
  {
    currentQuadOfTarget = 1;
    cout << "The current quadrant is " << currentQuadOfTarget << endl;
  }

  // Rules of Movement of Target based on the initial and current Qudrants

  Vector3d intermediateTargetPos;
  Vector3d differenceInPos;
  Vector3d differenceWithFinalPos;
  bool interMedPosNeeded; 

  if ( originalQuadOfTarget == 3 )
  {
    if ( currentQuadOfTarget == 3 || currentQuadOfTarget == 2)
    {
      // dirrect traversal will work
      interMedPosNeeded = false; 
      intermediateTargetPos  =  finalTargetPos; 
    } 

    if (currentQuadOfTarget == 1)
    {
      interMedPosNeeded = true;
      intermediateTargetPos.set(currentPos.x(), currentPos.y(), 100);
    }
  }



    
      differenceInPos.x(intermediateTargetPos.x() - currentPos.x());
      differenceInPos.y(intermediateTargetPos.y() - currentPos.y());
      differenceInPos.z(intermediateTargetPos.z() - currentPos.z());

      differenceWithFinalPos.x(finalTargetPos.x() - currentPos.x());
      differenceWithFinalPos.y(finalTargetPos.y() - currentPos.y());
      differenceWithFinalPos.z(finalTargetPos.z() - currentPos.z());

      double* ptr1 = NULL;
      // cout << "The desiredPosition = " << desiredPosition.x() << "  ,  " << desiredPosition.z() << endl ; 
      ptr1 = controlPosition(intermediateTargetPos, currentPos, 4.0,0.2, 1.2);
      // cout << "The velocity should be applied on tool for position reset is  " << ptr1[0] << " , " << ptr1[1] << endl; 
      double xVel = ptr1[0];
      double yVel = ptr1[1];
      // cout << "xVel = " << xVel << endl;
      // cout << "yVel = " << yVel << endl;


      entity->setLinearVelocity( xVel , 0, yVel );
        
      // Vector3d currentLinearVelocity;  
      // entity->getLinearVelocity(currentLinearVelocity);
      // cout << "currentLinearVelocity = " << currentLinearVelocity.x() << " , " << currentLinearVelocity.z() << endl;


      // LOG_MSG((" current entity pos is : %f %f %f ", currentPos.x(), currentPos.y(), currentPos.z() ));

  
    if ( abs(differenceInPos.x()) < 0.1 && abs(differenceInPos.z()) < 0.1)
    {
            // LOG_MSG((" Tool reset at : %f %f %f ", currentPos.x(), currentPos.y(), currentPos.z() ));
            // LOG_MSG (("The difference in position is: %f, %f, %f ", differenceInPos.x(), differenceInPos.y(), differenceInPos.z() )) ; 
            differenceInPos.set(0,0,0);
            cout << "The intermediate position is reset" << endl;      
    }

    if ( abs(differenceWithFinalPos.x()) < 0.1 && abs(differenceWithFinalPos.z()) < 0.1)
    {
            // LOG_MSG((" Tool reset at : %f %f %f ", currentPos.x(), currentPos.y(), currentPos.z() ));
            // LOG_MSG (("The difference in position is: %f, %f, %f ", differenceInPos.x(), differenceInPos.y(), differenceInPos.z() )) ; 
            differenceInPos.set(0,0,0);
            cout << "The entity position is reset" << endl;     
            return true; 
    }
  




  // exit(0);




}




void maintainOrientationOfTool(SimObj * entity, Rotation initialToolRot)
{
    double torque;
    Vector3d angularVel;
    entity->getAngularVelocity(angularVel); 

    if ( abs(angularVel.y()) > 0.1  )
    {
        // LOG_MSG((" Current angular Velocity is  : %f %f %f ", angularVel.x(), angularVel.y(), angularVel.z() ));
        double* ptr1 = NULL;
        ptr1 = controlAngularVelocity(angularVel, 1.0, 0, 3.0);
        torque  =  ptr1[0] * 10000;
        // torque =  ptr1[0] * 12000 ;
        // cout << "The torque applied for controlling angular velocity is " << torque << "   N. m" << endl; 
        entity->addTorque( 0 , torque, 0);
   } 

    Rotation currentToolRot;
    entity->getRotation(currentToolRot);
    double *ptrTool = NULL;
    ptrTool = controlRotationThroughout(initialToolRot, currentToolRot, 20.0, 0.0, 20.0);
    torque  =  ptrTool[1] * 10000 ;
    // cout << "The torque applied for controlling the rotation = " << torque << endl;
    entity->addTorque(0, torque,0); 

   
}



bool resetEntityOrientation(SimObj *entity, Rotation goalRotation){

  double *ptrTool = NULL;
  double torque;
  ptrTool = controlRotation(entity, goalRotation, 1.0, 0.0, 10.0);
  cout << "The difference in entity orientation about y axis is " << ptrTool[3] << endl;
  // cout << ptrTool[1] << endl;
  torque =  ptrTool[1] * 3000 ;
  cout << " torque to reset the orientation of tool  = " << torque << endl;
  entity->addTorque(0, torque, 0);

  if ( abs(ptrTool[3]) < 0.05)
  {
    return true;
  }

}




bool resetRandomTargetOrientation(SimObj *entity){

  double *ptrTool = NULL;
  double torque;
  Rotation currentRot;
  entity->getRotation(currentRot);

  ptrTool = reachZeroPose(entity, currentRot, 4.0, 0.1, 1.2);
  // cout << "The difference in entity orientation about y axis is " << ptrTool[3] << endl;
  // cout << ptrTool[1] << endl;
  torque =  ptrTool[1] * 100 ;
  // cout << " torque = " << torque << endl;
  entity->addTorque(0, torque, 0);

  if ( abs(ptrTool[3]) < 0.05)
  {
   
    cout << " The entity is set to a new pose" << endl;
    return true;
    // exit(0);

  }


}


float randomFloat(float min, float max) {
    
    static bool init = false;
    if (!init) {
    srand(time(NULL));
    init = true;
  }
    
  return  (max - min) * ((((float) rand()) / (float) RAND_MAX)) + min ;
}

bool newEntityPose(SimObj *entity, Rotation initToolRotation, double randomStep)
{

   

  double *ptrTool = NULL;
  double torque;
  // entity->addTorque(0, 10000, 0);

  // ptrTool = controlRotation(entity, initToolRotation, 1.0, 0.4, 1.2);
  ptrTool = reachRotation(entity, initToolRotation, randomStep, 20, 0, 20); 
  // cout << "The difference in entity orientation about y axis is " << ptrTool[3] << endl;
  // cout << ptrTool[1] << endl;
  torque =  ptrTool[1] * 1000 ;
  // cout << " torque = " << torque << endl;
  entity->addTorque(0, torque, 0);

  if ( abs(ptrTool[3]) < 0.1)
  {
   
    
    cout << " The entity is set to a new pose" << endl;
    return true;
    // exit(0);

  }



}