//
//  Rule_comparer.cpp
//  Egypt_pub_2grammars
//
//  Created by Christodoulos Aspromallis on 22/08/2017.
//
//

#include "Rule_comparer.hpp"


//I can use

void Rule_comparer::get_curr_position(vector<G_parser::gr>& _all_gr){

    for(int i=0; i<_all_gr.size(); i++){
        
        cout << endl << endl << endl << "RULE COMPARER:" << endl;
        cout << "grammar number: " << parser.all_gr[i].gr_pop << endl;
        cout << "GR1 form length: " << parser.all_gr[i].form_length << endl << endl;
    }
}
