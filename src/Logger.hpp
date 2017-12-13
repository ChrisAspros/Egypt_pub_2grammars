//
//  Logger.hpp
//  Egypt_pub_2grammars
//
//  Created by Christodoulos Aspromallis on 21/09/2017.
//
//

#ifndef Logger_hpp
#define Logger_hpp

#include <stdio.h>
#include "ofxMidiOut.h"


class Logger{
    
public:

    string log_path;// = "/Users/christodoulosaspromallis/Documents/UCL/PhD_Y_3/OF/of_v0.9.8_osx_release/apps/myApps/ICMC_test/Egypt_pub_2grammars/bin/data/Log/";
    
    ofFile File;
    
    void setup();
    void update();
    void tick();
    void beat();
    void bar();
    void cycle();
    void trans();
    void MIDI_spit();
    void tracker();//incl. position data / episode state..
    
    //final_cycle
    vector<string> final_cycle_names;
    vector<int> final_cycle_times;
    void store_final_cycle();
    void log_final_cycle();
    
    void print_final_log();
    
    int global_bt = 0;
    
    
    int ID;
    void generate_ID();
    
    void gather_all_elements();
  
    
    //
    string curr_term_name;
    int curr_term_time;
    
    
    bool tracked_randomised; //0 for tracked 1 for randomised
    
    //tracking state
    bool room1;
    bool room2;
    vector<string> new_read;
    vector<string> previous_read;
    
    //transition stage
    bool transitioning;
    bool transition_entered;
    bool transition_complete;
    int curr_gr;
    int next_gr;
    
    //music decisions
    //rules - rewrites
    
    //?????
    //logger.curr_func_chunks = r_comp.curr_func_chunks;
    //logger.next_func_chunks = r_comp.next_func_chunks;

    
};


#endif /* Logger_hpp */
