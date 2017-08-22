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

    void get_curr_position(vector<G_parser::gr>& _all_gr);
    void get_goal_point();
    int length_to_goal;//number of bars until goal
    
    
};

#endif /* Rule_comparer_hpp */
