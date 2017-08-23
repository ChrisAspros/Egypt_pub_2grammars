//
//  Rule_comparer.cpp
//  Egypt_pub_2grammars
//
//  Created by Christodoulos Aspromallis on 22/08/2017.
//
//

#include "Rule_comparer.hpp"


void Rule_comparer::update(vector<int>& seq_t){
    
    //do this on every translation - keep the function level..
    //keep_func_hist(seq_t);
}





void Rule_comparer::get_curr_position(){

    
}


void Rule_comparer::find_best_rule(vector<int>& seq_t, vector<G_parser::gr>& _all_gr){

    /*
    //checking grammar elements arrive...
    for(int i=0; i<_all_gr.size(); i++){
        
        cout << endl << endl << endl << "RULE COMPARER:" << endl;
        cout << "grammar number: " << parser.all_gr[i].gr_pop << endl;
        cout << "GR1 form length: " << parser.all_gr[i].form_length << endl << endl;
    }
     */
    
    //if same/similar scores ---> run compare history, say extra 4, extra4, extra 4 a.s.o..
    //if no more available then chose at random
}
