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
    
    bool transitioning;
    bool trans_complete;
    
    int curr_gr;
    int next_gr;
    
    void combine_rules(vector<int>& seq_t);//the main/central function of the class
    
    struct aug_sect_rule{//augmented sect rules
    
        string sect_name;
        G_parser::rule s_rule;
        int r_len;
        int score;
        string next_sect;//also indirect identifier between same "Sects" in different places..
    };
    
    vector<aug_sect_rule> aug_sect_rules;
    
    vector<G_parser::elem_ID> find_best_rule(vector<int>& seq_t);
    vector<G_parser::elem_ID> best_r;//needs to be global?
    
    //vector<G_parser::elem_ID> aux_cycle;
    
    //bool comb_set_up = 0;
    void setup_combination(vector<int>& seq_t);
    int get_distance_to_goal(vector<int>& seq_t);
    void rewrite_t_g();
    void rewrite_curr_gr();
    void rewrite_next_gr();
    
    int dist;
    int thread;
    void get_goal_point();
    int g_p;
    int length_to_goal;//number of bars until goal
    
    void update_combination(vector<int>& seq_t);
    
    void project_curr_gr();//projects current (ending) grammar up to goal point
    void project_next_gr();//projects next (upcoming) grammar up to goal point
    
};

#endif /* Rule_comparer_hpp */