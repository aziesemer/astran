/**
 \file compaction.h
 
 
 \date 25-sep-2006 \author Cristiano Lazzari <clazz@inf.ufrgs.br>
 */

#pragma once

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

/** Algortihm used to compact layout. */
enum cp_algo { CP_LP, /**< Linear programing. */
    CP_SHORTEST /**< Shortest path. */
};

/** Compaction Constraints type. */
enum cp_cons_tp { CP_BIG_ZERO,    /**< Variable bigger than zero. */
		  CP_BIG_EQ_ZERO, /**< Variable equal or bigger than zero. */
		  CP_EQ_ZERO,     /**< Variable equal to zero. */
		  CP_MIN,         /**< Distance must be bigger than a value. */
		  CP_MAX,         /**< Distance must be lesser than a value. */
		  CP_EQ,          /**< Distance must be equal to a value. */
		  CP_MIN_VAR_VAL, /**< Distance must be bigger than a value times a variable. */
		  CP_MAX_VAR_VAL, /**< Distance must be lesser than a value times a variable. */
		  CP_EQ_VAR_VAL,  /**< Distance must be equal to a value times a variable. */
		  CP_MIN_VAR,     /**< Distance must be bigger than a variable. */
		  CP_MAX_VAR,     /**< Distance must be lesser than a variable. */
		  CP_EQ_VAR,      /**< Distance must be equal to a variable. */
		  CP_UPPER_BOUND,  /**< Set upper bound to a variable. */
		  CP_LOWER_BOUND   /**< Set lower bound to a variable. */
};

/** Compaction constraints. */
class cp_constraint {
public:
    map<string,int>::iterator v1;
    map<string,int>::iterator v2;
    cp_cons_tp type;
    map<string,int>::iterator t;
    int val;
};

/** Compaction class. */
class Compaction {
public:
    Compaction( cp_algo a, string name );
    //  ~Compaction();
    
    void insertVal( string name );
    void insertConstraint( string v1 );
    void insertConstraint( string v1, string v2, cp_cons_tp type, int val );
    void insertConstraint( string v1, string v2, cp_cons_tp type, string t );
    void insertConstraint( string v1, string v2, cp_cons_tp type, string t, int val );
    void insertConstraintBTZ( string v1 );
    void insertConstraintEBTZ( string v1 );
    void insertConstraintEZ( string v1 );
    void insertUpperBound( string v1, int val );
    void insertLowerBound( string v1, int val );
    void forceIntegerVar( string v );
    void forceBinaryVar( string v );
    void forceSecVar( string v );
    void forceSOS( string v );
    void insertLPMinVar( string v );
    void insertLPMinVar( string v, int i );
    
    int solve(string lpSolverFile, int timeLimit);
    
    int getVariableVal( string name );

private:
    cp_algo algo;    /**< Compaction algorithm. */
    
    string lp_filename; /**< Filename for LP prgramming. */
    
    map<string,int> variables;         /**< variables used in compaction. */
    vector<cp_constraint> constraints; /**< Compaction constraints vector. */
    vector<string> ctrts;  /**< Compaction constraints vector. */
    vector<string> lp_min_var;  /**< variables to minimize the sum in LP. */
    vector<int>    lp_min_val;  /**< value to multiply the variables to minimize the sum in LP. */
    
    vector<string> int_vars; /**< force to fined a integer value. */
    vector<string> bin_vars; /**< force to fined a binary value. */
    vector<string> sec_vars; /**< force to fined a semi continuous value. */
    vector<string> sos_vars; /**< defines special ordered sets. */
};



