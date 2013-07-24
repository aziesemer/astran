#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#define ever (;;)

#include <iostream>
#include <vector>
#include <math.h>
#include <ctime>


using namespace std;

inline int  GetNextTemperature(int temperature, double accept_ratio, double dp) {

  if (accept_ratio > 0.98)
    return int(double(temperature)*0.4);

  if (temperature == 0 || accept_ratio == 0 || dp == 0)
    return 0;

  double inv_temp = double(1)/double(temperature);
  
  //olhar paper do Jimmy Lam
  return (int) (double(1)/(inv_temp + 5000 * double(4*accept_ratio*(1-accept_ratio)*(1-accept_ratio)) / double(dp*dp*dp*inv_temp*inv_temp*(2-accept_ratio)*(2-accept_ratio))));
}

inline bool Accept(double delta, int temperature) {
  double rand_num = double(rand())/double(RAND_MAX);

  if (delta <= 0)
    return true;

  return (rand_num <= exp(-delta/double(temperature)));
}

template<class toOptimize>
int FindInitialTemperature(toOptimize& obj) {

  cerr << " <FindInitialTemperature";

  int temp_max=0xFFFFFFF;
  int temp_min=0;

  int temp;
  double avg_delta;
  double delta;
  int cont_delta;

  toOptimize obj_copy = obj;

  cerr << "[Cost";
  double ini_cost = obj.GetCost();
  cerr << "]";

//  LOG("Running FindInitialTemperature...\n");

  do {

    cerr << ".";

    obj_copy = obj;
    temp = (temp_max-temp_min)/2 + temp_min;
    cont_delta = 0;

  //  LOG("  trying temp %i [ %i ; %i ] ", temp, temp_min, temp_max);

    for (int r=0; r<5000; r++) {
      delta = obj_copy.Perturbation();

      if (Accept(delta,temp)) {
	avg_delta += delta;
	cont_delta++;
      }
      else
	obj_copy.UndoPerturbation();
    }

    avg_delta/=cont_delta;
    //LOG(" avg_delta = %.6f ini cost = %.6f norm avg_delta = %.6f\n",avg_delta, ini_cost, avg_delta/ini_cost);

    if (avg_delta <0)
      temp_min = (temp_max-temp_min)/2 + temp_min;
    else
      temp_max = (temp_max-temp_min)/2 + temp_min;

  } while (!((avg_delta/ini_cost < 0.000000001 && avg_delta/ini_cost>-0.000000001) || ((temp_max - temp_min) <= 1)));

  cerr << ">";
  
  return max(temp,0);
}




//Quality factor is generally between (0;1] but can be more than 1. Controls the quality vs speed trade-off
template <class toOptimize>
toOptimize SimulatedAnnealing_SingleIteration(toOptimize& initial_solution, double quality_factor, bool greedy/*=false*/, 
					      bool high_temperature/*=false*/, int iteration, int& temp, bool& finish, int& return_cont_same_cost){
  int temperature = temp;

  if (greedy)
    temperature = 0;

  if (iteration == 0)
    if (high_temperature)
      temperature = 99999999;
    else
      temperature = FindInitialTemperature<toOptimize>(initial_solution);

  double cost = initial_solution.GetCost();
  double delta = 0;
  double avg_delta; 
  double accept_ratio;
  double average_cost;
  int cont_accepts;
  int num_repetitions = initial_solution.GetProblemSize();
  num_repetitions *= int(num_repetitions*quality_factor);

  //  cerr << " Running SASI cost = " << cost << " iter = " << iteration << " temp = " << temperature << "\n";

  finish = false;

  if (cost == 0 ) {
    finish = true;
    return initial_solution;
  }

  if (num_repetitions < 1)
    num_repetitions = 1;

  double var_1,var_2;

  double previous_cost = cost;
  int cont_same_cost = return_cont_same_cost;
  double cost_ratio;

  toOptimize solution = initial_solution;

//  LOG("Running Simulated Annealing Iteration number = %i ... Initial cost =  %..3fi\n",iteration,cost);
  int iter = iteration;

  iter++;

  cont_accepts = 0;
  average_cost = 0;
  var_1 = 0;
  var_2 = 0;
  avg_delta = 0;
  
  int time_of_iteration_ini = clock(); 

  for (int i=0; i<num_repetitions; i++) {
    
    delta = solution.Perturbation();
    if (Accept(delta,temperature)) {
      cont_accepts++;
      cost += delta;
      avg_delta += delta;
    }
    else {
      solution.UndoPerturbation();
    }
    
    var_1 += cost*cost/num_repetitions;
    var_2 += cost/num_repetitions;

    //    cerr << "rep cost = " << cost << " (" << solution.GetCost() << ")\n";
    //    if (cost != solution.GetCost()) {
    //      cerr << " Premature inconsistency..\n";
    //      exit(1);
    //    }
    
    average_cost = (i*average_cost + cost) / (i+1);
  }

  double variancia = var_1-var_2*var_2;
  double desvio_padrao;
  
  double time_of_iteration = double(clock() - time_of_iteration_ini)/double(CLK_TCK);
  
  if (cont_accepts > 1)
    desvio_padrao = sqrt(variancia);
  else
    desvio_padrao = 0;
  
  if (cont_accepts>0)
    avg_delta /= cont_accepts;
  else
    avg_delta = 0;
  
  if (cont_accepts>0)
    accept_ratio = double(cont_accepts)/double(num_repetitions);
  else
    accept_ratio = 0.0;

  cost_ratio = cost/previous_cost;
  if (cost==0 || (cost_ratio >= 0.99 && cost_ratio <= 1.01 && (accept_ratio < 0.8 || cost == previous_cost))) {
    cont_same_cost++;
  }
  else {
    cont_same_cost = 0;
    previous_cost = cost;
  }
  
  if (cont_same_cost > 9) {
    finish = true;
    return solution;
  }

//  LOG("bla\n");
  double bla = solution.GetCost();
  
  if (bla != cost) {
    cerr << "INCONSISTENCY ERROR " << bla << " <> " << cost << "\n";
    exit(1);
  }
  
//  LOG(" Iteration %i ; cost = %.3f (%.3f) ; p_cost = %.3f ; accept = %.3f ; avg_delta = %.3f ; dp = %.3f ; temp = %i ; cost_ratio = %.3f ; [%i]\n",
//	  iter,cost,bla,previous_cost,accept_ratio,avg_delta,desvio_padrao,temperature,cost_ratio,cont_same_cost);
  
  temperature = GetNextTemperature(temperature,accept_ratio, desvio_padrao);

  return_cont_same_cost = cont_same_cost;
  temp = temperature;
  finish = false;

  return solution;
}

//Quality factor is generally between (0;1] but can be more than 1. Controls the quality vs speed trade-off
template <class toOptimize>
toOptimize SimulatedAnnealing(toOptimize& initial_solution, double quality_factor, bool greedy=false, bool print_progress=true, bool high_temperature=false){
  int temperature = (greedy?0:(high_temperature?999999:FindInitialTemperature<toOptimize>(initial_solution)));
  //int temperature = 99999999;
  //int temperature = 0;
  double cost = initial_solution.GetCost();
  double delta = 0;
  double avg_delta; 
  double accept_ratio;
  double average_cost;
  int cont_accepts;
  int num_repetitions = int(initial_solution.GetProblemSize()*quality_factor);

  if (cost == 0 )
    return initial_solution;

  if (num_repetitions < 1)
    num_repetitions = 1;

  double var_1,var_2;

  double previous_cost = cost;
  int cont_same_cost = 0;
  double cost_ratio;

  toOptimize solution = initial_solution;

  if (print_progress)
    cout << " ( initial cost = " << cost << " num reps = " << num_repetitions << " problem size = " << initial_solution.GetProblemSize() << " ) ";

//  LOG("Running Simulated Annealing... Initial cost =  %..3fi\n",cost);
  int iter = 0;

  for ever {
    iter++;

    cont_accepts = 0;
    average_cost = 0;
    var_1 = 0;
    var_2 = 0;
    avg_delta = 0;


    int time_of_iteration_ini = clock(); 

    for (int i=0; i<num_repetitions; i++) {

      //      cerr << "0";
 
      delta = solution.Perturbation();

      //      cerr << "1";
      if (Accept(delta,temperature)) {
	cont_accepts++;
	cost += delta;
	avg_delta += delta;
      }
      else {
	//	cerr << "2";
	solution.UndoPerturbation();
	//	cerr << "3";
      }

      var_1 += cost*cost/num_repetitions;
      var_2 += cost/num_repetitions;

      average_cost = (i*average_cost + cost) / (i+1);
    }

    double variancia = var_1-var_2*var_2;
    double desvio_padrao;

    double time_of_iteration = double(clock() - time_of_iteration_ini)/double(CLK_TCK);

    //    if (time_of_iteration > 600)
    //      num_repetitions = int((time_of_iteration/600.0)*num_repetitions);


    if (cont_accepts > 1)
      desvio_padrao = sqrt(variancia);
    else
      desvio_padrao = 0;

    if (cont_accepts>0)
      avg_delta /= cont_accepts;
    else
      avg_delta = 0;
 
    if (cont_accepts>0)
      accept_ratio = double(cont_accepts)/double(num_repetitions);
    else
      accept_ratio = 0.0;

    cost_ratio = cost/previous_cost;
    if (cost==0 || (cost_ratio >= 0.993 && cost_ratio <= 1.007 && (accept_ratio < 0.8 || cost == previous_cost))) {
      cont_same_cost++;
    }
    else {
      cont_same_cost = 0;
      previous_cost = cost;
    }

    if (cont_same_cost > 9)
      break;

//    LOG("bla\n");
    double bla = solution.GetCost();

    if (bla != cost) {
      cerr << "INCONSISTENCY ERROR " << bla << " <> " << cost << "\n";
//     exit(1);
    }

printf(" Iteration %i ; cost = %.3f (%.3f) ; p_cost = %.3f ; accept = %.3f ; avg_delta = %.3f ; dp = %.3f ; temp = %i ; cost_ratio = %.3f ; [%i]\n",
    	    iter,cost,bla,previous_cost,accept_ratio,avg_delta,desvio_padrao,temperature,cost_ratio,cont_same_cost);

    if (print_progress)
      cerr << ".";

    temperature = GetNextTemperature(temperature,accept_ratio, desvio_padrao);
  }

  if (print_progress)
    cout << " final cost = " << cost << " ";

  return solution;
}




#endif
