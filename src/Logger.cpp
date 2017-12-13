//
//  Logger.cpp
//  Egypt_pub_2grammars
//
//  Created by Christodoulos Aspromallis on 21/09/2017.
//
//

#include "Logger.hpp"


void Logger::setup(){
    
    log_path = "/Users/christodoulosaspromallis/Documents/UCL/PhD_Y_3/OF/of_v0.9.8_osx_release/apps/myApps/ICMC_test/Egypt_pub_2grammars/bin/data/Log/";
    
    generate_ID();
    log_path += (ofToString(ID) + ".txt");
    
    //create new file (with ID name..)
    //ofFile File(ofToDataPath(log_path));
    //File.open(ofToDataPath(log_path),ofFile::WriteOnly);
    File.open(log_path,ofFile::WriteOnly);
    //File.create();
    //File.open(log_path);//File.open();
    //ofFile File(log_path,ofFile::Append);
    //File << "writing?";//writes nothing
    
    //ofLogToFile(log_path);//writes date - time
}


void Logger::update(){

    
    gather_all_elements();
    
    //cout <<
    
    //export to .txt file..
}

void Logger::gather_all_elements(){
    
    int gr_pops = 3;
    
}

void Logger::generate_ID(){

    ID = rand() % 10000; //float?
    
    cout << "";
    // --> int ofRandom(int val0, int val1)
        //float ofRandom(float val0, float val1) (even better for random??)

    //search log_path for name
    ofDirectory dir(log_path);
    
    
    //bool ID_exists;
    string ID_str = ofToString(ID) + ".txt";
    for (int i=0;  i < dir.listDir(); i++){//numFiles() - DEPRECATED
    
        //cout << "NAME " << i << ": " << dir.getName(i) << endl;
        if (dir.getName(i) == ID_str) {
            //ID = rand() % 10000;//generate_ID();//ID_exists = true;
            cout << dir.getName(i) << " already exists" << endl;
            generate_ID();
            cout << "NEW: " << ID << endl;
        }
    }
    
    //if (ID_exists) generate_ID();
    
    //ID = rand() % 10000;
    //if (dir.getName(dir.listDir()-1) == ofToString(ID)) generate_ID();//ID = rand() % 10000;
    
    //if existent, iterate generate_ID()
}


void Logger::tick(){}

void Logger::beat(){}

void Logger::bar(){

    //ofFile File(log_path,ofFile::Append);
    //File << "writing?";//writes nothing
    global_bt ++;
    
    store_final_cycle();
    
    
    string log_str = ofToString(global_bt);
    
    //ofLog() << global_bt;//log_str;
    File << global_bt << ", ";
}


void Logger::store_final_cycle(){

    //final_cycle_names.push_back("isussy");//(curr_term_name);
    //final_cycle_times.push_back(global_bt);//k(curr_term_time);
    
}


void Logger::log_final_cycle(){

    //ofLog() << "FINAL CYCLE" << endl;
    //ofLog() << "symbols: ";
    //ofFile File(log_path,ofFile::Append);
    File << endl << endl << "FINAL CYCLE" << endl;
    File << "[symbols : times]" << endl;
    int size = final_cycle_names.size();
    //for(int i=0; i < size;  i++) ofLog() << final_cycle_names[i] << " ";
    //for(int i=0; i < size;  i++) ofLog() << final_cycle_times[i] << " ";
    for(int i=0; i < size;  i++) File << "[" << final_cycle_names[i] << " : " << final_cycle_times[i] << "]";
    File << endl;
    //for(int i=0; i < size;  i++) File << final_cycle_times[i] << " ";
}


void Logger::print_final_log(){

    log_final_cycle();

    File << endl << endl << "==============" << endl << "LOG END" << endl << "==============";
    File.close();//dont' allow remainders to be logged after 'L'..
}


void Logger::trans(){
    
    
}



//pass_log_values (ofApp.)
/*
 //clock (in logger)
 //overall: how long does transition/recover take etc..
 //descritise time in some way..
 //assume
 
 start
 generate ID
 compare for uniqueness
 appear screen
 goes in questionnaiere
 get current time -> T (baseline time)
 start logging
 (use prepared logger class?)
 find difference between times.. current time - actual time etc..
 stop button
 commit the log.. (use 'touch' to say one psecific time..)
 have a button for deleting the log..
 
 
 keep MIDI file, AUDIO file, log file..
 
 //run_ID (in logger)
 
 
 //tracked VS randomised
 logger.tracked_randomised = tracked_randomised; //0 for tracked 1 for randomised
 
 //tracking state
 logger.room1 = OSC.room1;
 logger.room2 = OSC.room2;
 logger.new_read = OSC.new_read;
 logger.previous_read = OSC.previous_read;
 
 //transition stage
 logger.transitioning = transitioning;
 logger.transition_entered = transition_entered;
 logger.transition_complete = transition_complete;
 logger.curr_gr = r_comp.curr_gr;
 logger.next_gr = r_comp.next_gr;
 
 //music decisions
 //rules - rewrites
 
 logger.curr_func_chunks = r_comp.curr_func_chunks;
 logger.next_func_chunks = r_comp.next_func_chunks;
 
 //scores
 //choices (from current andnext gr)
 
 //context awareness
 
 //final cycle choices.. - states!!
 */
