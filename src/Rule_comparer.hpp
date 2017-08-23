//
//  Rule_comparer.hpp
//  Egypt_pub_2grammars
//
//  Created by Christodoulos Aspromallis on 22/08/2017.
//
//

#ifndef Rule_comparer_hpp
#define Rule_comparer_hpp

#include <stdio.h>

//class G_parser;

#include "Grammar_parser.h"

class Rule_comparer{
    
public:
    
    G_parser parser;
    
    int curr_gr;
    int next_gr;
    
    void update(vector<int>& seq_t);//to keep track of the cycle history
    
    void find_best_rule(vector<int>& seq_t, vector<G_parser::gr>& _all_gr);//the main/central function of the class

    void get_curr_position();
    void get_goal_point();
    int length_to_goal;//number of bars until goal
    
    void project_curr_gr();//projects current (ending) grammar up to goal point
    void project_next_gr();//projects next (upcoming) grammar up to goal point
    
};

#endif /* Rule_comparer_hpp */
