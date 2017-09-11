//
//  Rule_comparer.cpp
//  Egypt_pub_2grammars
//
//  Created by Christodoulos Aspromallis on 22/08/2017.
//
//

#include "Rule_comparer.hpp"



void Rule_comparer::combine_rules(vector<int>& seq_t){
    
    //STATIC process
    if (!parser.comb_setup) setup_combination(seq_t);

    //UPDATING - running the cycle now.. (pairnei ta hnia..)
    update_combination(seq_t);
    
    
    //((((
    
    //start parallel aux_cycle for incoming GR(_2)
    
    //decide which is the best section to be placed at goal point..
    
    //keep curr_gr previous until transition if complete.. then exchange
    
    
    //optimal transition functions will be returned here (2 function vectors)
    //look at GR1 OR GR2 for choice FUNCTION choice and production
        //weighted random, based on transition stage..
    //keep producing GR1 (according to forced compatible rule&production to the extent it plays a role) in case we go back..
    //stop producing GR1 if total transition to GR2 (and vice-versa..)
        //on full transition curr_gr = all_gr[1] & next_gr = all_gr[0]
    
    
    //prediction
        //if ending at specific bars: map bars to goal points
        //for smoothness map distance to weighted selection (FUNC->terminal GR1 || GR2)..
            //if transition exceeds goal point follow both GRs for as long as necessary
            //transition is cancelled only if we go outside it.. distance-wise in space..thus, a smooth weighted de-transition back to GR1
        // - instead of mere smooth stages, we aim at a goal point as a formal reference point (i.e. preserving better both form 1 and form 2) for introduction of sect of GR2, so that a better sense of form is preserved
            //SO PREDICTION and GOAL POINT are NECESSARY...!!!
    
    //return production, i.e. vector<G_parser::elem_ID> best_prod
    //best_r = find_best_rule(seq_t);//necessary??
    
    //Towards function-level restriction of space (optimal rules & productions, both locally and formally, i.e. historically)
}


void Rule_comparer::setup_combination(vector<int>& seq_t){

    //must be managed dynamically
    curr_gr = parser.gr_pop;//= 0;
    next_gr = !curr_gr;//= 1;
    
    //dist only ONCE??
    //int dist;
    dist = get_distance_to_goal(seq_t);
    //cout << "DIST_TO_GOAL: " << dist << endl;
    
    find_best_rule(seq_t);
    
    parser.comb_setup = 1;
    
    
    //run possible FUNC productions till goal (GR_1)
    //run possible FUNC tail productions of (GR_2)
    //keep best scores
    //check past
    //run pre-tail productions of (GR_2)
    //compare with function History
    //get optimal functions of GR1 AND GR2 until g_p
}


int Rule_comparer::get_distance_to_goal(vector<int>& seq_t){
    
    int distance, curr_pos;//, g_p;
    
    //find current position
    curr_pos = seq_t[3];
    
    //calcuate goal point (we are transitioning)
    //every 8 bars now.. - CAN CHANGE, based on form significance.., e.g. blues IV significance..!!!
    if (curr_gr == 0 || curr_gr == 1){
    
        if (curr_pos > 3 & curr_pos <= 11) g_p = 16;//includes -1
        if (curr_pos > 11 & curr_pos <= 19) g_p = 24;
        if (curr_pos > 19 & curr_pos <= 27) g_p = 0;
        if (curr_pos > 27 || curr_pos <= 3) g_p = 8;
    }
    else {//if gr_2
        
        if (curr_pos > 3 & curr_pos <= 11) g_p = 0;//includes -1
        if (curr_pos > 11 || curr_pos <= 3) g_p = 8;
    }
    
    //make more dynamic later, based on tempo, player speed, form morphology etc..
    
    if (curr_pos > g_p) distance = (parser.all_gr[curr_gr].form_length - curr_pos) + g_p;//+-1??
    else distance = g_p - curr_pos;//+-1??
    
    return distance;//watch out it may return the previously help number OR returning it one bar too late..
}



vector<G_parser::elem_ID> Rule_comparer::find_best_rule(vector<int>& seq_t){
    
    //check all sects tails for enough length
    
    //find/store the S rule (next_gr)
    G_parser::rule S_rule;
    for (int i=0; i<parser.all_gr[next_gr].general_rules.size(); i++){
        
        for (int j=0; j<parser.all_gr[next_gr].general_rules[i].left_str.size(); j++){
            
            if (parser.all_gr[next_gr].general_rules[i].left_str[j] == "S"){
                
                S_rule = parser.all_gr[next_gr].general_rules[i];
                break;
            }
        }
    }
    
    //find sect lengths
    vector<int> sect_lengths;
    vector<string> combined_sects;
    //check Sect lengths
    //compare prod times of Sects
    for (int i=0; i < S_rule.prod_times.size(); i++){
        
        //calculate sect lengths
        int len;
        if (i == S_rule.prod_times.size() - 1) len = parser.all_gr[next_gr].form_length - S_rule.prod_times[i];
        else len = S_rule.prod_times[i+1] - S_rule.prod_times[i];
        sect_lengths.push_back(len);
    }
    /*
    cout << endl << "sect_lengths: " << endl;
    for (int i=0; i < sect_lengths.size(); i++) cout << sect_lengths[i] << endl;
    cout << endl;
     */
    
    //find/store the sect rules
    vector<string> sect_names_aux;
    vector<G_parser::rule> sect_rules;
    //aug_sect_rule asr;//augmented section rule
        //get sect names
    for (int i=0; i < S_rule.right_side.size(); i++){
        
        for (int j=0; j < S_rule.right_side[i].right_str.size(); j++){
            
            string sect_name;//isolated from time sing, i.e. e.g. (5).
            int k=0;
            while (S_rule.right_side[i].right_str[j][k] != '('){
            
                sect_name.push_back(S_rule.right_side[i].right_str[j][k]);
                k++;
            }
            sect_names_aux.push_back(sect_name);
            aug_sect_rules.push_back(aug_sect_rule());
            aug_sect_rules[j].sect_name = sect_name;
        }
    }
    
        //search sect_names (in general rules) - EXPAND for TIMED RULES ALSO!!
    for (int i=0; i < sect_names_aux.size(); i++){
        
        //search in general rules
        for (int j=0; j < parser.all_gr[next_gr].general_rules.size(); j++){
        
            for (int k=0; k < parser.all_gr[next_gr].general_rules[j].left_str.size(); j++){
            
                if (sect_names_aux[i] == parser.all_gr[next_gr].general_rules[j].left_str[k]){
                
                    aug_sect_rules[i].s_rule = parser.all_gr[next_gr].general_rules[j];
                    break;//because only 1 "Sect" on the left_str of a sect rule
                }
            }
            aug_sect_rules[i].r_len = sect_lengths[i];
            aug_sect_rules[i].next_sect = sect_names_aux[(i+1) % sect_names_aux.size()];
        }
        
        //what if SectA is in two points in the form, i.e. preceeded by 2 different sections..?
        //both must be found and recorded
        //for now just keep the first one - it may be more definitive of next_gr (earlier in the form..), but maybe not.. - mention?
        
        //search in timed_rules
        //- EXPAND for TIMED RULES ALSO!! - Sect rules need to be general_rules (a.o.t. timed_) for now..
    }
    
    //vector<G_parser::rule> long_rules;
    vector<vector<G_parser::rule>> prolonged_rules;
    
    initiate_aux_cycle();//size of longest form of the grammars
    
    rewrite_funcs();//curr_gr
    //now we have parser.func_chunkcs (vector<elem_ID>)
    
    //compare next func chunks to next func chunks..
    //maybe 1 twice with different input??
    construct_next_lines();
    
    construct_curr_lines();
    
    //THEN COMPARE LINES BASED ON HARMONIC RHYTHMS...
    
    //then add existing funcs to the game..
        //correlation of history with most likely of each sect's tail..
            //variable history length..
    
    
    //test fitness
    //in 2 cycles (curr_gr & next_gr)
    //keep N best (2 sides: curr & next) - up to N^2 combinations..
    //higher N - less stress on locality / more on large scale form
    //look back
    //functions already played combined with
    //next_gr previous possible functions (for each decided point)
    
    //decide best point - place best sect at goal point.. (Sect rules have been stored!!)

    
    //nmz de xreiazete - aplws 8a koitaei parapisw an xreiazetai extra length..
    for (int i=0; aug_sect_rules.size(); i++){
    
        //if sect length is enough
        if (dist-1 <= aug_sect_rules[i].r_len){//dist-1 logw pre-production of next bar..
            
            //expand that sect
        }
        else{
            
            //expand that sect and another..
            
            //LATER
            //make new combo rule: manually build rule, assign right side etc..
                //prolonged_rules[][] =
                //for now just combine with previous
                //in future check&combine iteratively
        }
    }
     //*/
    
    
    //(((why not the MOST LIKELY till goal?? - because compatibility and smoothness is the major focus here..)))
    
    //look ahead??
    
    //get total scores of combinations
    //place best combination in 2 cycles
    
    //place next sect on g_p
    
    //mix 2 cycles into the main (playing)
        //based on transition phase
    
    //if g_p reached end transition (for now..) OR re do the proces..
    
    
    //get scores up to goal point
        //(((
        //of most compatible productions (on both sides..)
            //force the most compatible productions????
        //)))
    
        //search gr1 up to goal (existing + to-be-expaned)
        //search gr2 (for a sect start at goal) distance-to-goal-long before THAT sect, i.e. its TAIL!
    
    //keep rules&productions with 2/3 best scores.. (based on N, input etc..)
    
    //check history
        //compare function_cycle with MOST LIKELY production preceding the GR2 rule..
        //if same scores ---> check more history (bars one-by-one)
            //if no more available then chose at random (or 1st..)
    
    //??check future??
    
    //return
        //chosen (rule&)production of GR1 to goal point
        //chosen rule&production (i.e. sect & production) of GR2 around goal point (back and forth)
    
        //return the FUNCTION production(vector<string> OR vector<G_parser::elem_ID>)
    
    
    
    
//VHMA VHMA na vrw poses fores xreiazetai kai pou to trans_update()
    
    //next_gr: make vector of all possible functions till goal.. (irrespective of probabilities)
    
    //make map of all possible func_chunkcs.. linearly..
    //--expand all possible sects gr2
    //get scores..
    //mix in real time..
    //till goal
    //till updating goal
        //if g_p reached and still transitioning:
            //re do the test process of BEST TILL GOAL + what has been played..
                //nested..!!
    //finalise transitions / introduce next_gr
    //presentation
    //better grammars
        //update goal points..
        //rhythm changes - https://en.wikipedia.org/wiki/Rhythm_changes
            //https://www.freejazzlessons.com/4-jazz-turnarounds/
        //https://www.jazzadvice.com/7-killer-turnarounds-for-jazz-improvisation/
    //orchestrations
    //mix orchestrations

    
    
    /*
     //checking grammar elements arrive...
     for(int i=0; i<_all_gr.size(); i++){
     
     cout << endl << endl << endl << "RULE COMPARER:" << endl;
     cout << "grammar number: " << parser.all_gr[i].gr_pop << endl;
     cout << "GR1 form length: " << parser.all_gr[i].form_length << endl << endl;
     }
     */
    
    //logic
    /*
    compares:
			most likely (/ an average) upcoming
			most likely (/ an average) of GR2 rule
			N best rules are compared by further length.. - why not from start?
				in order to control / stress (by N) local VS formal compatibility
				UI: (impacting N) local smoothness VS large-scale smoothness
		keeps history for comparison
			run separate cycle: 
				if in function vector, then keep / replace.. 
				else don’t
     */
}


void Rule_comparer::rewrite_funcs(){

    rewrite_curr_gr_t_g();//till funcs till g_p (goal point)
    rewrite_next_all();//expand all next form till funcs
    
    //cout << endl << "for b.p." << endl;
}


void Rule_comparer::rewrite_curr_gr_t_g(){
    //curr_gr: make vector of all possible functions till goal.. (irrespective of probabilities)
    
    cout << endl << "entering rewrite_curr_gr_t_g()" << endl;
    
    parser.rewriting_curr = 1;
    
    //expand to functions
    parser.till_function = 1;
    parser.gr_pop = curr_gr;//not necessary here but might become..
    
    int f_l = parser.all_gr[curr_gr].form_length;
    
    
    int curr_bar = (f_l + g_p - (dist)) % f_l;
    //int curr_bar =
    
    cout << endl << "f_l: " << f_l << endl;
    cout << endl << "curr_bar is: " << curr_bar << endl;
    
    int to_g_p = g_p;
    if (to_g_p==0) to_g_p = f_l;
    if (curr_bar > to_g_p) to_g_p = to_g_p + f_l;
    
    //for (int j=curr_bar; j < to_g_p - 1; j++){//g_p - 1 to avoid rewriting on g_p
    for (int j=0; j < dist - 1; j++){//g_p - 1 to avoid rewriting on g_p
        
        //find rule with
        //dynamic assignment of aux_cycle
        //& is_function
        //& ALL productions (irrespective of production probabilities), i.e. musical space constraint
        
        vector<int> aux_t = {0, 0, 0, curr_bar % f_l, 0};
        //parser.setup_t = {0, 0, 0, j, 0};
        
        cout << endl << "curr_bar it:: " << curr_bar << endl;
        
        cout << endl << "aux_cycle: ";
        for (int k=0; k<parser.aux_cycle.size(); k++) cout << parser.aux_cycle[k].name << " ";
        cout << endl << "(keeps the last right_side(s?))" << endl;
        
        //going forward in cycle as long as functions already there..
        while (parser.is_function(aux_t)) {
            
            if (curr_bar == g_p){
                
                //parser.till_function = 0;
                cout << endl << "breaking in while" << endl;
                break;
            }
            
            curr_bar = (curr_bar + 1) % f_l;
            j++;
            
            //int ti = j % f_l;
            //parser.setup_t = {0, 0, 0, j, 0};
            aux_t = {0, 0, 0, curr_bar, 0};
            
            //manually restard aux_cycle (place S on top, else it loops looking for func in isus4
            if (curr_bar==0) parser.aux_cycle[0].name = "S";
            
            cout << endl << "g_p in wh: " << g_p;
            cout << endl << "curr_bar in while: " << curr_bar << endl;
            cout << "j in while: " << j << endl;
        }
        
        
        if (curr_bar==0) parser.aux_cycle[0].name = "S";
        
        //UPDATE DIST IN HERE for updated g_p
        
        //if ((j % f_l) == (to_g_p % f_l)) break;//to avoid rewriting on g_p due to manual j++
        if (curr_bar == g_p){
            
            //parser.till_function = 0;
            cout << endl << "breaking outside while" << endl;
            break;
        }
        
        cout << endl << "g_p in before find_rule: " << g_p;
        cout << endl << "curr_bar before find_rule: " << curr_bar << endl;
        cout << "j before find rule: " << j << endl;
        
        parser.find_rule(aux_t);
        
        curr_bar = (curr_bar + 1) % f_l;
        /*
        if (j==curr_bar){//filter with is_function only the first time..
        
            if (!parser.is_function(aux_t)) parser.find_rule(aux_t);//i.e. in order to avoid rewriting the next FUNCTION to type_level - then loop in (!is_function) find_rule())
        }
        else parser.find_rule(aux_t);
        */
         
        //else //aug_sect_rule. / a_s_r. = oti exei o curr_cycle ekei..
        
        /*
         cout << endl << "aux_cycle: ";
         for (int k=0; k<parser.aux_cycle.size(); k++) cout << parser.aux_cycle[k].name << " ";
         cout << endl;
         */
    }
    
    curr_func_chunks = parser.func_chunks;
    parser.func_chunks.clear();
    cout << endl << "curr_func_chunks size: " << curr_func_chunks.size() << endl;
    
    parser.rewriting_curr = 0;
}


void Rule_comparer::rewrite_next_all(){
    //if rewriting_next (r.left_str[0]!="S" || rewriting_next) do only once the whole S rule (in rewrite()), i.e. the whole form and that's it..
    
    cout << endl << "entering rewrite_next_all()" << endl;
    
    parser.rewriting_next = 1;
    
    //expand to functions
    parser.till_function = 1;
    parser.gr_pop = next_gr;//WATCH IT!! - make more dynamic??
    
    int f_l = parser.all_gr[next_gr].form_length;
    int curr_bar = 0;
    parser.aux_cycle[0].name = "S";
    
    vector<int> aux_t = {0, 0, 0, 0, 0};
    parser.find_rule(aux_t);
    
    next_func_chunks = parser.func_chunks;
    parser.func_chunks.clear();
    cout << endl << "next_func_chunks size: " << next_func_chunks.size() << endl;
    
    parser.rewriting_next = 0;
}


void Rule_comparer::initiate_aux_cycle(){
    
    //find longest form of the 2 grammars
    int longest_f_l;// = 800;
    for (int i=0; i < parser.all_gr.size(); i++){
    
        if (longest_f_l < parser.all_gr[i].form_length) longest_f_l = parser.all_gr[i].form_length;
    }
    
    //make size of aux cycle
    for (int i=0; i < longest_f_l; i++){
    
        parser.aux_cycle.push_back(G_parser::elem_ID());
    }
        
        //assign curr_cycle to aux_cycle
    parser.aux_cycle = parser.curr_cycle;
}

//make all linear combinations for next_gr
//

//for curr_lines
        //for (int k=0; k < dist; k++){
    //watch your k == curr_bar
    //watch your dist == g_p - curr_bar


//giati etsi - 8a volepsei?
//einai form-aware mexri edw?? - context-aware
//mhpws na ftiaxnontai oi grammes oloklhres apo chunk phase??
//check paper suggestion.. - will work??
    //exei nohma to expansion?? EINAI form-aware??
//go through method
//implement algor thought

void Rule_comparer::construct_next_lines(){
    //constructing all possible lines of next_gr till goal..
    
    int l_c;//line count
    
    //int k = 0;
    for (int k=0; k < parser.all_gr[next_gr].form_length; k++){
        
        int i=0;
        while (i < next_func_chunks.size()){// && next_func_chunks[i][0].time[1] == k){
            
            if (next_func_chunks[i][0].time[1] == k){
                
                next_func_lines.push_back(vector<G_parser::elem_ID>());
                l_c = next_func_lines.size() - 1;
                
                //if not the first, load previous line(s??)
                if (k != 0){
                    
                    for (int j=0; j < next_func_lines[l_c - 1].size(); j++){
                        
                        next_func_lines[l_c].push_back(next_func_lines[l_c - 1][j]);
                    }
                }
                
                //add new
                for (int j=0; j < next_func_chunks[i].size(); j++){
                    
                    next_func_lines[l_c].push_back(next_func_chunks[i][j]);
                }
                
                i++;
            }
            else k = next_func_chunks[i][0].time[1];
            
            //i++;
        }

        
    }
    
    /*
    int l_c;//line count
    
    //search for
    int k = 0;
    //for (int k=0; k < parser.all_gr[next_gr].form_length; k++){
     
    //for (int i=0; i < next_func_chunks.size(); i++){
    int i=0;
    while (i < next_func_chunks.size()){// && next_func_chunks[i][0].time[1] == k){
        
        if (next_func_chunks[i][0].time[1] == k){
        
            next_func_lines.push_back(vector<G_parser::elem_ID>());
            l_c = next_func_lines.size() - 1;
            
            //if not the first, load previous line(s??)
            if (k != 0){
            
                for (int j=0; j < next_func_lines[l_c - 1].size(); j++){
                    
                    next_func_lines[l_c].push_back(next_func_lines[l_c - 1][j]);
                }
            }
            
            //add new
            for (int j=0; j < next_func_chunks[i].size(); j++){
                
                next_func_lines[l_c].push_back(next_func_chunks[i][j]);
            }
            
            i++;
        }
        else k = next_func_chunks[i][0].time[1];
        
        //i++;
    }
    //ADD k++??
    //curr_func_chunks
    
    //}
     */
}


void Rule_comparer::construct_curr_lines(){

    //next_func_chunks
}


void Rule_comparer::update_combination(vector<int>& seq_t){

    //if still transitioning
    
    //before g_p
    
    //choose probabilistically - weighted
    //(re)place in cycle
    
    //on reaching g_p (for LATER..)
    
    //if stage (4 or) 5
    //place chosen sect non-T of GR_2 / restor cycle with offset bar count
    //produce based on GR_2
    
    //if not yet
    //set g_p = next ent_time
    //do same production search (return vector of functions) till there
    
    //if transition cancelled
    
    //(having kept next upcoming sect of curr_gr)
    //place next upcoming sect (of GR_1) in g_p
    //step back stages gradually (normalise till sect/g_p)
    //i.e. like reconciliation microgrammar, but more clever here..
}

/*COMMENTS
 //get func_chunks of next_gr
 //test fitness
 //in 2 cycles (curr_gr & next_gr)
 //keep N best (2 sides: curr & next) - up to N^2 combinations..
 //higher N - less stress on locality / more on large scale form
 //look back
 //functions already played combined with
 //next_gr previous possible functions (for each decided point)
 
 //(((why not the MOST LIKELY till goal?? - because compatibility and smoothness is the major focus here..)))
 
 //look ahead??
 
 //get total scores of combinations
 //place best combination in 2 cycles
 
 //place next sect on g_p
 
 //mix 2 cycles into the main (playing)
 //based on transition phase
 
 //if g_p reached end transition (for now..) OR re do the proces..
 
 
 //get scores up to goal point
 //(((
 //of most compatible productions (on both sides..)
 //force the most compatible productions????
 //)))
 
 //search gr1 up to goal (existing + to-be-expaned)
 //search gr2 (for a sect start at goal) distance-to-goal-long before THAT sect, i.e. its TAIL!
 
 //keep rules&productions with 2/3 best scores.. (based on N, input etc..)
 
 //check history
 //compare function_cycle with MOST LIKELY production preceding the GR2 rule..
 //if same scores ---> check more history (bars one-by-one)
 //if no more available then chose at random (or 1st..)
 
 //??check future??
 
 //return
 //chosen (rule&)production of GR1 to goal point
 //chosen rule&production (i.e. sect & production) of GR2 around goal point (back and forth)
 
 //return the FUNCTION production(vector<string> OR vector<G_parser::elem_ID>)
 
 
 
 
 //VHMA VHMA na vrw poses fores xreiazetai kai pou to trans_update()
 
 //next_gr: make vector of all possible functions till goal.. (irrespective of probabilities)
 
 //make map of all possible func_chunkcs.. linearly..
 //--expand all possible sects gr2
 //get scores..
 //mix in real time..
 //till goal
 //till updating goal
 //if g_p reached and still transitioning:
 //re do the test process of BEST TILL GOAL + what has been played..
 //nested..!!
 //finalise transitions / introduce next_gr
 //presentation
 //better grammars
 //update goal points..
 //rhythm changes - https://en.wikipedia.org/wiki/Rhythm_changes
 //https://www.freejazzlessons.com/4-jazz-turnarounds/
 //https://www.jazzadvice.com/7-killer-turnarounds-for-jazz-improvisation/
 //orchestrations
 //mix orchestrations
 
 
 
 /*
 //checking grammar elements arrive...
 for(int i=0; i<_all_gr.size(); i++){
 
 cout << endl << endl << endl << "RULE COMPARER:" << endl;
 cout << "grammar number: " << parser.all_gr[i].gr_pop << endl;
 cout << "GR1 form length: " << parser.all_gr[i].form_length << endl << endl;
 }
 */

//logic
/*
 compares:
 most likely (/ an average) upcoming
 most likely (/ an average) of GR2 rule
 N best rules are compared by further length.. - why not from start?
 in order to control / stress (by N) local VS formal compatibility
 UI: (impacting N) local smoothness VS large-scale smoothness
 keeps history for comparison
 run separate cycle:
 if in function vector, then keep / replace..
 else don’t
 */
