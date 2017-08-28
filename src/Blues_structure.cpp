//
//  Blues_structure.cpp
//  
//
//  Created by Christodoulos Aspromallis on 04/03/2016.
//
//

#include "Blues_structure.h"
#include "Sequencer.h"
//#include "Grammar_parser.h"


void Blues_structure::setup(){
    
    //seq.parser.gr_pop = 0;
    seq.r_comp.parser.initiate_cycle();
    
    seq.setup();
    
    t = {0, 0, 0, 0, 0};
    //fin_t = {0};
    //cad_t = {0};
    
    stopping = false;
    
    cadencing = false;
    finishing = false;
    
    ch_chords = 1;
    /*swing drums: *///ch_drums = 2;//bass drum, snare, tom1 (floor), tom2, tom 3, high-hat(closed?), ride, crash
    ch_bass = 3;
    ch_egypt_cymbal = 4;
    
    drum_notes = {36, 38, 40, 41, 43, 45, 47, 48};//respectively..
    
    QN_dur = seq.QN_subdivision;//quarternote duration in its subdivisions
    
    controller_counter = 0;
    upwards = 1;
    //scan_file();
    //seq.r_comp.parser.setup();
    //play_chord_sequence();
    
    //for (int i=0; i<5; i++) curr_t[i] = 0;
    //better in constructor..??
    
}


void Blues_structure::update(){
    
    //different value every second (for orchestration variety)
    elapsed_mins = (ofGetElapsedTimeMillis() / 5000) % 5;
    //cout << elapsed_mins << endl;
    
    //trying to set up volume automations for DAW
    if (controller_counter == 0) upwards = 1;
    if (controller_counter == 127) upwards = 0;
    
    if (upwards) controller_counter += 1;
    else controller_counter -= 1;
    //seq.midiOut::sendControlMessage(1, 1, controller_counter);
    seq.controls(1, 1, controller_counter);
    
    //cout << "controller counter: " << controller_counter << endl;
    
    //seq.update();
    //seq.metronome(t);
    
    //debugging for finishing etc..??
    /*
    if (fin_t.size()==0) t = seq.timer();
    else if (!(fin_t[3]==t[3] && fin_t[4]==t[4] && t[1]==2 && t[2]==3 && t[0]==7))
     */
    
    seq.update();//only for cycle_len for now..
    t = seq.timer();
    
     
    if (seq.only_on("beat", t)){
     
        //cout << "beat: " << t[2] << endl;
        //if (fin_t.size()!=0) cout << "fin_t: " << fin_t[3] << " " << fin_t[4] << endl;
    }
    
    if (seq.only_on("beat", t)) {
     
        //cout << "ending: " << ending << ", goal_reached: " << goal_reached << endl;
        seq.r_comp.parser.ending = ending;
        seq.r_comp.parser.goal_reached = goal_reached;
    }
    
    //find_rule only on every bar (exclude ticks & )
    if (seq.only_on("bar", t)){
        
        
        
        seq.r_comp.parser.start_cycle(t);//places "S" at start of cycle (if not ending etc..) OR if grammar just changed //HERE or in seq.r_comp.parser.update_cycle();
        
        //RE-STARTING CYCLE (disrupting from top for new grammar)
        if (seq.r_comp.parser.gr_changed){
            
            //check if bar is an end_time
            vector<int> e_t = seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].end_times;
            vector<int>::iterator it_e_t = find(e_t.begin(), e_t.end(), t[3]);
            
            if (it_e_t!=e_t.end()){
            
                seq.r_comp.parser.initiate_cycle();
                //resetting timer values
                seq.bar = 0;
                t[3] = 0;
                
                seq.r_comp.parser.gr_pop = !seq.r_comp.parser.gr_pop;
                seq.r_comp.parser.gr_changed = 0;
            }
        }
        
        
        if(seq.r_comp.parser.transitioning) seq.r_comp.combine_rules(t);
        else seq.r_comp.parser.find_rule(t);
        
        //if(!seq.r_comp.parser.transitioning || transition_complete) seq.r_comp.parser.find_rule(t);
        //else seq.r_comp.combine_rules(t);
        string terminal = seq.r_comp.parser.return_terminal(t);
        chord = terminal_to_midi(terminal);
    }
    
    /*
    update_state();
    
    string s = r_comp.parser.curr_cycle[t[3]].name;
    //cout << "time bar: " << blues.t[3] << ", s: " << s << endl;
    //bool finished = goal_reached && (s=="i"||s=="i6"||s=="i7");
    bool last_chord = (s=="i"||s=="i6"||s=="i7");
    */
    /*
    if (!finished && !stopping){

        play_main(chord);
    }
    else
    */
    if (seq.r_comp.parser.stop_seq){//finishing && goal_reached && last_chord)
    
        stopping = true;
        if (fin_t.size()==0) fin_t = t;//keeping constant value
        if (fin_t[3]==t[3] && fin_t[4]==t[4]) play_finale(chord);//play only for one bar more and only on current cycle
    }
    else play_main(chord);
}


void Blues_structure::update_state(){

    string term = seq.r_comp.parser.return_terminal(t);
    
    if (term == "cad"){
        
        if (cadencing == false) cad_t = t;//keep temporary time for second bar of cad (drum closing in last bar of cadence for now..)
        cadencing = true;
        finishing = false;
    }
    if (term == "rec") {
        
        cadencing = false;
        finishing = false;
    }
    if (term == "fin"){
        
        cout << "FINISHING!" << endl;
        finishing = true;
        cadencing = false;
    }
}


void Blues_structure::play_main(vector<int>& chord){
    
    play_chords(chord);
    play_bass(chord);
    play_drums();
    
    if (elapsed_mins == 5){
        //seq.controls(1, 10, 0);
        //cout << "ch 1, control 1, 0" << endl;
    }
    if (elapsed_mins == 1){
        //seq.controls(1, 10, 50);
        //cout << "ch 1, control 1, 50" << endl;
    }
    
    /*
    if (elapsed_mins == 1 || elapsed_mins == 2) seq.controls(1, 10, 0);
    if (elapsed_mins == 1 || elapsed_mins == 4 || elapsed_mins == 2)
    if (elapsed_mins == 0 || elapsed_mins == 3)
     */
}


void Blues_structure::play_finale(vector<int>& chord){
    //2 bars with rock enging.. (+"thank you goodnight :P")
    
    play_chords(chord);
    play_bass_finale(chord);
    play_drums_finale();
}


//TO BLUES
void Blues_structure::play_drums(){
    
    //if (term == "cad") drum_pattern = drums_closing;
    //else if (term == "fin") drum_pattern = drums_end;
    //else drum_pattern = drums_shullfe;
    
    if (cadencing && !finishing && cad_t[3]+1==t[3]) play_drums_cad();
    else if (finishing) play_drums_fin();
    //else play_drums_normal();
    else play_egypt_cymbal();
    
    //apo fin se rec pws pame sto grammar??? me rec pali??
    
}


void Blues_structure::play_egypt_cymbal(){
    
    //printf ("cymbal");
    
    notes_v = {55, 60, 94};
    for (int i=0; i<seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1; i++) multiple_bars.push_back(i);
    
    ///*
    for (int i=0; i<multiple_bars.size(); i++){
        
        pos_drums = {0, 0, 0, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_egypt_cymbal);
        pos_drums = {0, 1, 1, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_egypt_cymbal);
        pos_drums = {0, 0, 2, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_egypt_cymbal);
    }
     //*/
    multiple_bars.clear();
}


void Blues_structure::play_drums_normal(){
    
    //BASS DRUM
    notes_v = {drum_notes[0]};
    for (int i=0; i<seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1; i++) multiple_bars.push_back(i);
    
    for (int i=0; i<multiple_bars.size(); i++){
    
        pos_drums = {0, 0, 0, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 2, 0, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 2, 1, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 0, 2, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //bar 12 XXX LAST BAR??
    for (int i=0; i<seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length; i+=2){
    
        pos_drums = {0, i%3, i/3, seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }

    //SNARE
    notes_v = {drum_notes[1]};//{drum_notes[0]};
    //for (int i=0; i<11; i++) multiple_bars.push_back(i);
    
    for (int i=0; i<seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1; i++){
    
        pos_drums = {0, 0, 1, i};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 0, 3, i};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    multiple_bars = {1, 5, 9};
    
    for (int i=0; i<multiple_bars.size(); i++){
        
        //pos_drums = {0, 0, 3, multiple_bars[i]};//covered above..
        //seq.time_placement(t, notes_v, pos_drums, 6, ch_drums);//covered above..
        pos_drums = {0, 2, 3, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    multiple_bars = {3, 7};
    
    for (int i=0; i<multiple_bars.size(); i++){
        
        pos_drums = {0, 1, 2, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 0, 3, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 1, 3, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 2, 3, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //bar 12
    for (int i=0; i<3; i++){
    
        pos_drums = {0, i, 0, seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
     
    //RIDE
    notes_v = {drum_notes[6]};//{drum_notes[0]};
    multiple_bars = {0, 1, 2, 4, 5, 6, 8, 9, 10};//to avoid playing when e.g. snare is too busy (drummer had only two arms..)
    
    for (int i=0; i<multiple_bars.size(); i++){
        
        pos_drums = {0, 0, t[2], multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 2, t[2], multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    multiple_bars = {3, 7};
    
    for (int i=0; i<multiple_bars.size(); i++){
        
        pos_drums = {0, 0, 0, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 2, 0, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 0, 1, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 2, 1, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 0, 2, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 2, 2, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //TOM(s)
    //bar 12
    notes_v = {drum_notes[4]};
    for (int i=0; i<3; i++){

        pos_drums = {0, i, 1, seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    notes_v = {drum_notes[3]};
    for (int i=0; i<3; i++){
        
        pos_drums = {0, i, 2, seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    notes_v = {drum_notes[2]};
    for (int i=0; i<3; i++){
        
        pos_drums = {0, i, 3, seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }

    //CRASH
    notes_v = {drum_notes[7]};//{drum_notes[drum_notes.size()-1]};
    pos_drums = {0, 0, 0, 0};
    if (t[4] != 0) seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    pos_drums = {0, 0, 0, 4};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    
    multiple_bars.clear();
}


void Blues_structure::play_drums_cad(){
    
    //BASS DRUM
    notes_v = {drum_notes[0]};
    //bar 12
    for (int i=0; i<seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length; i+=2){
        
        pos_drums = {0, i%3, i/3, cad_t[3]+1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //SNARE
    notes_v = {drum_notes[1]};
    //bar 12
    for (int i=0; i<3; i++){
        
        pos_drums = {0, i, 0, cad_t[3]+1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //TOM(s)
    //bar 12
    //tom high
    notes_v = {drum_notes[4]};
    for (int i=0; i<3; i++){
        
        pos_drums = {0, i, 1, cad_t[3]+1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //tom mid
    notes_v = {drum_notes[3]};
    for (int i=0; i<3; i++){
        
        pos_drums = {0, i, 2, cad_t[3]+1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //tom low
    notes_v = {drum_notes[2]};
    for (int i=0; i<3; i++){
        
        pos_drums = {0, i, 3, cad_t[3]+1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //CRASH
    notes_v = {drum_notes[7]};//{drum_notes[drum_notes.size()-1]};
    pos_drums = {0, 0, 0, (cad_t[3]+1)%seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
}


void Blues_structure::play_drums_fin(){

    //if very last bar, then no drums
    //else very open/simple counting
}


void Blues_structure::play_drums_finale(){
    
    //if very last bar, then no drums
    //else very open/simple counting
    
    //BASS DRUM
    notes_v = {drum_notes[0]};
    pos_drums = {0, 1, fin_t[2], fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    pos_drums = {0, 0, fin_t[2]+1, fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    pos_drums = {0, 2, fin_t[2]+1, fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    
    //TOM HIGH
    notes_v = {drum_notes[4]};
    pos_drums = {0, 0, fin_t[2], fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    //TOM MID
    notes_v = {drum_notes[3]};
    pos_drums = {0, 1, fin_t[2], fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    
    //SNARE
    notes_v = {drum_notes[1]};
    pos_drums = {0, 2, fin_t[2], fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    pos_drums = {0, 0, fin_t[2]+1, fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    
    //CRASH
    notes_v = {drum_notes[7]};
    pos_drums = {0, 2, fin_t[2] + 1, fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
}


void Blues_structure::play_bass(vector<int>& chord){
    
    vector<int> b_note;
    vector<int> pos_bass;
    int dur_bass = QN_dur;
    
    vector<int> bass_patt;
    int count = 0;
    
    if (chord.size()==4) bass_patt = {0, 1, 2, 1, 3, 2, 1, 2};
    else if (chord.size()==3) bass_patt = {0, 1, 2, 1, 0, 2, 1, 2};
        
    if (seq.only_on("beat", t)){
        
        b_note = {chord[bass_patt[t[2]*2]]};
        pos_bass = {0, 0, t[2], t[3]};
        seq.time_placement(t, b_note, pos_bass, dur_bass, ch_bass);
        b_note = {chord[bass_patt[t[2]*2+1]]};
        pos_bass = {0, 2, t[2], t[3]};
        seq.time_placement(t, b_note, pos_bass, dur_bass, ch_bass);
    }
}


void Blues_structure::play_bass_fin(vector<int>& chord){
}


void Blues_structure::play_bass_finale(vector<int>& chord){

    vector<int> b_note;
    int tonic = chord[0];
    vector<int> pos_bass;
    int dur_bass = QN_dur;
        
    b_note = {tonic};
    pos_bass = {0, 0, fin_t[2], fin_t[3]};
    seq.time_placement(t, b_note, pos_bass, dur_bass, ch_bass);
    b_note = {tonic-2};
    pos_bass = {0, 2, fin_t[2], fin_t[3]};
    seq.time_placement(t, b_note, pos_bass, dur_bass, ch_bass);
    b_note = {tonic-1};
    pos_bass = {0, 0, fin_t[2]+1, fin_t[3]};
    seq.time_placement(t, b_note, pos_bass, dur_bass, ch_bass);
    b_note = {tonic};
    pos_bass = {0, 2, fin_t[2]+1, fin_t[3]};
    seq.time_placement(t, b_note, pos_bass, dur_bass, ch_bass);
}


void Blues_structure::play_chords(vector<int>& chord){
    
    vector<int> pos_chord = {0, 0, 0, t[3]};
    seq.time_placement(t, chord, pos_chord, QN_dur*4, ch_chords);
    //can't' accept get_chord() by reference
}


vector<int> Blues_structure::terminal_to_midi(string& terminal){
    
    int oct_off = 1; //octave offset in chord_translation
    
    int root_pitch = chord_translation[terminal][0] + oct_off * 12;
    //cout << "root_pitch: " << root_pitch << endl;
    int type = chord_translation[terminal][1];
    //cout << "type: " << type << endl;
    
    return get_chord(root_pitch, type);
}


map<string, vector<int>> Blues_structure::chord_translation{//no static here

    {"i", {48, 0}},
    {"isus4", {48, 9}},
    {"im", {48, 1}},
    {"immaj", {48, 8}},
    {"i6", {48, 3}},
    {"i7", {48, 6}},
    {"bii", {49, 0}},
    {"biisus2", {49, 10}},
    {"iim7", {50, 4}},
    {"iiim", {52, 1}},
    {"iiim7", {52, 4}},
    {"iiio", {52, 7}},
    {"iv", {53, 0}},
    {"ivsus4", {53, 9}},
    {"iv6", {53, 3}},
    {"iv7", {53, 6}},
    {"ivm", {53, 1}},
    {"v7", {55, 6}},
    {"vo", {55, 7}},
    {"bvi", {44, 0}},
    {"bviim", {46, 1}},
    
};


vector<int> Blues_structure::get_chord(int root_pitch, int type, int invert, bool position){
    
    //chord structure
    vector<int> chord_set;
    if (type == 0) chord_set = {0, 4, 7};
    else if (type == 1) chord_set = {0, 3, 7};
    else if (type == 2) chord_set = {0, 4, 7, 11};
    else if (type == 3) chord_set = {0, 4, 7, 9};
    else if (type == 4) chord_set = {0, 3, 7, 10};
    else if (type == 5) chord_set = {0, 3, 7, 9};
    else if (type == 6) chord_set = {0, 4, 7, 10};
    else if (type == 7) chord_set = {0, 3, 6};//chord_set.resize()?
    else if (type == 8) chord_set = {0, 3, 7, 11};
    else if (type == 9) chord_set = {0, 5, 7};//sus4
    else if (type == 10) chord_set = {0, 2, 7};//sus2
    //else if (more chord types)
    
    /*
    int invert = 0;
    //for root position leave inv as is..
    if (inversion == "1st") invert = 1;
    else if (inversion == "2nd") invert = 2;
    else if (inversion == "3rd" && chord_set.size()>=4) invert = 3;
    */
     
    //appending first element(s) to vector end
    for (int i=0; i<invert; i++){
        int first_elem = chord_set[0];
        chord_set.erase(chord_set.begin());
        chord_set.push_back(first_elem + seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length);
    }
    
    //3rd inversion 1 octave lower (better register)
    if (invert == 2 || invert == 3){
        for (int i=0; i<chord_set.size(); i++){
            chord_set[i] -= seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length;
        }
    }
    
    if (position) chord_set[1] += seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length;
    
    //make chord pitches
    vector<int> chord;
    //{0, 4, 7} ==> {4, 0, 7} ?? --> {4, 7, 8}
    for (int i=0; i<chord_set.size(); i++){
        chord.push_back(chord_set[i]+root_pitch);
    }
    //needs clearing? (chord)
    
    return chord;
}


//TO BLUES
void Blues_structure::time_misplace_chord(){
    //compute 16th/8th/QN misplacement back or forth (based on style..)
    //a little grammar for it?
    //simple markov..?!
}

/*
 int blues_structure::chord_array(int harmony_type, int tonal_centre){
 //rnr type
 int tc = tonal_centre;
 if (harmony_type == 1){ // <----- <-----
 chord_list_1[0] = tc;
 chord_list_1[4] = ;
 triad_pool =
 
 }
 //jazz(-er) type
 else if (harmony_type == 2){
 for (int i=0; i<12; i++){
 7th_chords = ;
 chord_list_2
 }
 }
 //alice blues
 else if (harmony_type == 3){
 
 }
 }
 */
