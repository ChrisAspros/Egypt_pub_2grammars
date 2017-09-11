//
//  Osc_receive.cpp
//  blues_osc
//
//  Created by Christodoulos Aspromallis on 13/06/2016.
//
//

#include "Osc_receive.hpp"

void Osc_receive::setup(){

    receive.setup(PORT);
}

void Osc_receive::update(){
    
    while(receive.hasWaitingMessages()){
    
        ofxOscMessage m;
        receive.getNextMessage(&m);
        
        if (m.getAddress() == "8000"){//"/vertical/x"){
            
            cout << "MESSAGE is THROUGH" << endl;
            
            oscX = m.getArgAsFloat(0);
            //_ending = m.getArgAsBool(1);
            //_goal_reached = m.getArgAsBool(0);
            cout << "KEY: " << oscX << endl;
        }
        
        if (m.getAddress() == "6161"){//"/horizontal/y"){
            cout << "MESSAGE is THROUGH" << endl;
            oscY = m.getArgAsFloat(0);
        }
    }
    
    //cout << oscX << " " << oscY << endl;
    //cout << "ending: " << _ending << ", g_r: " << _goal_reached << endl;
}
