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
    if (!comb_set_up) set_up_combination(seq_t);

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


void Rule_comparer::set_up_combination(vector<int>& seq_t){

    //must be managed dynamically
    curr_gr = 0;
    next_gr = 1;
    
    //dist only ONCE??
    //int dist;
    dist = get_distance_to_goal(seq_t);
    //cout << "DIST_TO_GOAL: " << dist << endl;
    
    find_best_rule(seq_t);
    
    comb_set_up = 1;
    
    
    //run possible FUNC productions till goal (GR_1)
    //run possible FUNC tail productions of (GR_2)
    //keep best scores
    //check past
    //run pre-tail productions of (GR_2)
    //compare with function History
    //get optimal functions of GR1 AND GR2 until g_p
}


int Rule_comparer::get_distance_to_goal(vector<int>& seq_t){
    
    int distance, curr_pos, g_p;
    
    //find current position
    curr_pos = seq_t[3];
    
    //calcuate goal point (we are transitioning)
    if (curr_gr == 0){
    
        if (curr_pos > 3 & curr_pos <= 11) g_p = 16;//includes -1
        if (curr_pos > 11 & curr_pos <= 19) g_p = 24;
        if (curr_pos > 19 & curr_pos <= 27) g_p = 0;
        if (curr_pos > 27 || curr_pos <= 3) g_p = 8;
    }
    else {//if gr_2
        
        if (curr_pos > 3 & curr_pos <= 11) g_p = 16;//includes -1
        if (curr_pos > 11 || curr_pos <= 3) g_p = 8;
    }
    
    //make more dynamic later, based on tempo, player speed, form morphology etc..
    
    if (curr_pos > g_p) distance = (parser.all_gr[curr_gr].form_length - curr_pos) + g_p;//+-1??
    else distance = g_p - curr_pos;//+-1??
    
    return distance;//watch out it may return the previously help number OR returning it one bar too late..
}



vector<G_parser::elem_ID> Rule_comparer::find_best_rule(vector<int>& seq_t){
    
    //check all sects tails for enough length
    
    //find/store the S rule
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
        
        /*
         //find that Sect rule - EINAI "SectA" h "SectA(1)"
         //(find that sect in left and keep)
         bool sect_found = 0;
         for (int j=0; j < parser.all_gr[next_gr].general_rules.size(); j++){
         
         for (int k=0; k < parser.all_gr[next_gr].general_rules[j].left_str.size(); k++){
         
         if (parser.all_gr[next_gr].general_rules[j].left_str[k] == S_rule.right_side[0].right_str[i]){
         
         long_rules.push_back(parser.all_gr[next_gr].general_rules[j]);
         sect_found = 1;
         }
         if (sect_found) break;
         }
         if (sect_found) break;
         }
         
         //check rule lengths & store that rule or (if not long enough) combine with previous
         if (dist <= len){
         
         prolonged_rules[][] =
         }
         else {
         //for now just combine with previous
         //in future check&combine iteratively
         }
         */
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
        
        //r_len not right in 2nd
        //no next_sect in 2nd
        //check ypoloipa stoixeia
        
        //fix aug storage
        //git
        //find score..
        
        //what if SectA is in two points in the form, i.e. preceeded by 2 different sections..?
        //both must be found and recorded
        //for now just keep the first one - it may be more definitive of next_gr (earlier in the form..), but maybe not.. - mention?
        
        //search in timed_rules
        //- EXPAND for TIMED RULES ALSO!!
        /*
        vector<G_parser::rule> aux_rule;
        for (map<vector<int>, vector<G_parser::rule>>::iterator pair_it = parser.all_gr[next_gr].timed_rules.begin(); pair_it != parser.all_gr[next_gr].timed_rules.end(); ++pair_it){
            
            aux_rule = pair_it->second;
            if (aux_rule[0].right_side[0].right_str[0] == sect_names[i]){
                //sect_rules.push_back();
            }
        }
         */
    }
    
    cout << "";
    //test_thus far
    //git save
    
    //calculate score (& add to aug_sect_rules[...].score)
    //test lengths
    /*
    for (int i=0; aug_sect_rules.size(); i++){
    
        if (dist > ){
        
            //normal
        }
        else{
        
            //make new sub rule - manually assign right side etc..
        }
    }
     */
    
    //vector<G_parser::rule> long_rules;
    vector<vector<G_parser::rule>> prolonged_rules;

    
    /*
    if (parser.all_gr[next_gr].general_rules[j].left_str[k] == S_rule.right_side[0].right_str[i]){
        
        long_rules.push_back(parser.all_gr[next_gr].general_rules[j]);
        sect_found = 1;
    }
    if (sect_found) break;
     */
    //}
    //if (sect_found) break;


    

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
