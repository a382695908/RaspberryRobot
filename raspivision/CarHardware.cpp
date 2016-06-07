//
//  CarHardware.cpp
//  dachuang_car_xcode
//
//  Created by 黄正跃 on 5/12/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//

#include "CarHardware.h"
CarHardware* CarHardware::car=nullptr;
void CarHardware::goForward(double speed){
    
    digitalWrite(MOTOR_LEFT_1, HIGH);
    digitalWrite(MOTOR_LEFT_2, LOW);
    digitalWrite(MOTOR_RIGHT_1, HIGH);
    digitalWrite(MOTOR_RIGHT_2, LOW);
    
}

void CarHardware::turnLeft(double speed){
    
    digitalWrite(MOTOR_LEFT_1, LOW);
    digitalWrite(MOTOR_LEFT_2, HIGH);
    digitalWrite(MOTOR_RIGHT_1, HIGH);
    digitalWrite(MOTOR_RIGHT_2, LOW);
}

void CarHardware::turnRight(double speed){
    
    digitalWrite(MOTOR_LEFT_1, HIGH);
    digitalWrite(MOTOR_LEFT_2, LOW);
    digitalWrite(MOTOR_RIGHT_1, LOW);
    digitalWrite(MOTOR_RIGHT_2, HIGH);
}

void CarHardware::goBackward(double speed){
    
    digitalWrite(MOTOR_LEFT_1, LOW);
    digitalWrite(MOTOR_LEFT_2, HIGH);
    digitalWrite(MOTOR_RIGHT_1, LOW);
    digitalWrite(MOTOR_RIGHT_2, HIGH);
}

void CarHardware::stop(){
    digitalWrite(MOTOR_LEFT_1, LOW);
    digitalWrite(MOTOR_LEFT_2, LOW);
    digitalWrite(MOTOR_RIGHT_1, LOW);
    digitalWrite(MOTOR_RIGHT_2, LOW);
}
