/*
 * main.cpp
 *
 *  Created on: Apr 1, 2019
 *      Author: mirco
 */

#include <DAG/MaxProduct.h>
#include <Evaluation/Evaluation.h>
#include <VariableTaskSet/VariableTaskSet.h>
#include <eigen3/Eigen/Core>
#include <iostream>
#include "MultiRate/MultiRateTaskset.h"

#include <set>

#include "MultiRate/DummyNodes.h"

#include "Evaluation/Scheduling.h"
#include <algorithm>
void
taskset1()
{
	MultiRateTaskset taskSet;

	auto task1 = taskSet.addTask(10, 2, "sensor1");
	auto task2 = taskSet.addTask(10, 4, "sensor2");
	auto task3 = taskSet.addTask(20, 5, "controller");
	auto task4 = taskSet.addTask(20, 3, "actuator");

	taskSet.addPrecedenceEdge(task3, task4);
	taskSet.addDataEdge(task1, task3, 0);
	taskSet.addDataEdge(task2, task3, 1);

	taskSet.createBaselineDAG();

	auto dags = taskSet.createDAGs();

	dags[2].toTikz("test.tex");

}

int
taskset3()
{
	time_t tstart, tend;
	tstart = time(0);
	MultiRateTaskset taskSet;

	auto task1 = taskSet.addTask(5, 3, "imu");
	auto task2 = taskSet.addTask(10, 2, "gps");
	auto task3 = taskSet.addTask(10, 2, "planner");
	auto task4 = taskSet.addTask(5, 2, "controller");
	auto task5 = taskSet.addTask(20, 10, "act");

	task5->bcet = 5;
	task1->bcet = 1;
	//auto task6 = taskSet.addTask(40, 5, "train");
	//auto task7 = taskSet.addTask(160, 50, "independent");

	taskSet.addDataEdge(task3, task4, 0);

	taskSet.addDataEdge(task1, task3, 0);
	taskSet.addDataEdge(task1, task4, 1);
	taskSet.addDataEdge(task2, task3, 1);
	//taskSet.addDataEdge(task2, task4, 2);
	//taskSet.addDataEdge(task4, task5, 1);
	//taskSet.addDataEdge(task1, task6, 6);
	//taskSet.addDataEdge(task2, task6, 0);


	taskSet.createBaselineDAG();

	auto dags = taskSet.createDAGs();

	if (dags.empty())
		return 1;

	float numEdges = 10000;
	unsigned id = 0;
	unsigned k = 0;
	int n_processors = 4;

	for (auto& dag : dags)
	{
		auto info = dag.getLatencyInfo( { 0, 0, 2, 3, 4 });
		if (info.reactionTime < numEdges)
		{
			numEdges = info.reactionTime;
			id = k;
		}
		k++;

		scheduling::scheduleDAG(dag,n_processors);
	}

	dags[id].toTikz("prova.tex");
	scheduling::scheduleDAG(dags[id],n_processors,"schedule_test.tex", true);
	dags[id].getOriginatingTaskset()->toTikz("cool.tex");
	std::cout << dags[id].getNodeInfo() << std::endl;
	std::cout << dags[id].getLatencyInfo( { 0,4 }) << std::endl;
	//dags[id].getLatencyInfoIterative( { 1,5,3,4});

	tend = time(0);
	std::cout << "It took " << difftime(tend, tstart) << " second(s)." << std::endl;

	return 0;

}

int
taskset2()
{
	MultiRateTaskset taskSet;

	auto task1 = taskSet.addTask(10, 9, "sensor1");
	auto task2 = taskSet.addTask(10, 5, "sensor2");
	auto task3 = taskSet.addTask(20, 2, "act");

	taskSet.addDataEdge(task1, task2, 1);
	taskSet.addDataEdge(task1, task3, 0);

//	taskSet.addPrecedenceEdge(task1, task2);

	taskSet.createBaselineDAG();

	auto dags = taskSet.createDAGs();

	return 0;
}

int
multiTaskset()
{
	time_t tstart, tend;
	tstart = time(0);
	VariableTaskSet taskSet;

	auto task1 = taskSet.addTask(5, 2, "imu");
	auto task2 = taskSet.addTask(20, 4, "gps");
	auto task3 = taskSet.addTask(10, 2, "planner");
	auto task4 = taskSet.addTask(10, 2, "controller");
	auto task5 = taskSet.addTask(20, 2, "act");
	// auto task6 = taskSet.addTask(40, 13, "train");

	// task6->bcet = 10;
//	task3->bcet = 4;
//	auto task7 = taskSet.addTask(80, 50, "independent");

	taskSet.addDataEdge(task3, task4, { 0, 1});

	taskSet.addDataEdge(task1, task3, { 0, 1, 2 });
	taskSet.addDataEdge(task1, task4, { 0, 1, 2 });
	// taskSet.addDataEdge(task2, task3, { 0, 1, 2 });
	// taskSet.addDataEdge(task2, task4, { 0, 1, 2 });
	// taskSet.addDataEdge(task4, task5, { 0, 1, 2 });
//	taskSet.addDataEdge(task1, task6,  { 7, 8 });
//	taskSet.addDataEdge(task2, task6,  { 1,2 });
//	taskSet.addDataEdge(task5, task6,  { 1,2 });

	taskSet.createBaselineTaskset();

	auto& allDags = taskSet.createDAGs();

	std::cout << allDags.size() << " total valid DAGs were created" << std::endl;

	Evaluation eval;
	eval.addLatency({task1, task1,task3, task4, task5}, LatencyCost(1,15), LatencyConstraint(500,500));
	// eval.addLatency({task2, task2, task3, task4, task5}, LatencyCost(1,3), LatencyConstraint(70,70));
//	eval.addLatency({task1, task3}, LatencyCost(1,1), LatencyConstraint(25,25));
	eval.addScheduling(SchedulingCost(20), SchedulingConstraint(16));

	const auto& bestDAG = eval.evaluate(allDags);

	bestDAG.toTikz("prova.tex");
	bestDAG.getOriginatingTaskset()->toTikz("cool.tex");
	std::cout << bestDAG.getNodeInfo()<< std::endl;
	bestDAG.getLatencyInfo({1,1,2,3,4});
//	scheduling::scheduleDAG(bestDAG, 4, "schedule_test.tex");

	std::cout<<"Casini on best"<<scheduling::scheduleDAGCasini(bestDAG, 16, "dag.dot")<<std::endl;

	tend = time(0);
	std::cout << "It took " << difftime(tend, tstart) << " second(s)." << std::endl;

	return 0;
}


int
RTSS21IndustryChallenge()
{
	time_t tstart, tend;
	tstart = time(0);
	VariableTaskSet taskSet;

	auto task1 	= taskSet.addTask(100,	2, 	"radar");
	auto task2 	= taskSet.addTask(100,	2, 	"lidar");
	auto task3 	= taskSet.addTask(40, 	2, 	"camera");
	auto task4 	= taskSet.addTask(10, 	2, 	"gps");
	auto task5 	= taskSet.addTask(40, 	2, 	"perception2D");
	auto task6 	= taskSet.addTask(100,	2, 	"perception3D");
	auto task7 	= taskSet.addTask(100,	2, 	"perceptionFusion");
	auto task8 	= taskSet.addTask(100,	2, 	"localization");
	auto task9 	= taskSet.addTask(100,	2, 	"tracking");
	auto task10 = taskSet.addTask(100,	2, 	"prediction");
	auto task11 = taskSet.addTask(100,	2, 	"planning");
	auto task12 = taskSet.addTask(10, 	2, 	"control");

	taskSet.addPrecedenceEdge(task1, 	task7);
	taskSet.addDataEdge(task2, 	task5, 	{ 0, 1}); //{ 0, 1, 2, 3, 4 });
	taskSet.addPrecedenceEdge(task3, 	task6);
	taskSet.addDataEdge(task4, 	task8, 	{ 0, 1}); //{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
	taskSet.addDataEdge(task5, 	task7, 	{ 0, 1}); //{ 0, 1, 2, 3, 4 });
	taskSet.addPrecedenceEdge(task6, 	task7);
	taskSet.addPrecedenceEdge(task7, 	task9);
	taskSet.addPrecedenceEdge(task8, 	task11);
	taskSet.addPrecedenceEdge(task9, 	task10);
	taskSet.addPrecedenceEdge(task10, 	task11);
	taskSet.addDataEdge(task11, task12, { 0, 1}); //{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

	taskSet.createBaselineTaskset();

	auto& allDags = taskSet.createDAGs();

	std::cout << allDags.size() << " total valid DAGs were created" << std::endl;

	Evaluation eval;
	eval.addLatency({task1, task7, task9, task10, task11, task12}, LatencyCost(1,15), LatencyConstraint(1500,1500));
	// eval.addLatency({task3, task5, task7, task9, task10, task11, task12}, LatencyCost(1,15), LatencyConstraint(1500,1500));
	// eval.addLatency({task2, task6, task7, task9, task10, task11, task12}, LatencyCost(1,15), LatencyConstraint(1500,1500));
	// eval.addLatency({task2, task8, task11, task12}, LatencyCost(1,15), LatencyConstraint(1500,1500));
	// eval.addLatency({task4, task8, task11, task12}, LatencyCost(1,15), LatencyConstraint(1500,1500));

	eval.addScheduling(SchedulingCost(20), SchedulingConstraint(16));

	const auto& bestDAG = eval.evaluate(allDags);

	bestDAG.toTikz("prova.tex");
	bestDAG.getOriginatingTaskset()->toTikz("cool.tex");
	std::cout << bestDAG.getNodeInfo()<< std::endl;
	// bestDAG.getLatencyInfo({1,1,2,3,4});

//	scheduling::scheduleDAG(bestDAG, 4, "schedule_test.tex");

	std::cout<<"Casini on best"<<scheduling::scheduleDAGCasini(bestDAG, 16, "dag.dot")<<std::endl;

	tend = time(0);
	std::cout << "It took " << difftime(tend, tstart) << " second(s)." << std::endl;

	return 0;
}


int
multiTaskset2()
{
	time_t tstart, tend;
	tstart = time(0);
	VariableTaskSet taskSet;

	auto task1 = taskSet.addTask(5, 3, "imu");
	auto task2 = taskSet.addTask(20, 4, "planner");

	task2->bcet = 3;

	taskSet.addDataEdge(task1, task2, { 0, 1, 2,3,4 });

	taskSet.createBaselineTaskset();

	auto& allDags = taskSet.createDAGs();

	std::cout << allDags.size() << " total valid DAGs were created" << std::endl;

	Evaluation eval;
	eval.addLatency({task2, task1, task1}, LatencyCost(-1,-1), LatencyConstraint(60, 60));

	const auto& bestDAG = eval.evaluate(allDags);

	bestDAG.toTikz("prova.tex");
	bestDAG.getOriginatingTaskset()->toTikz("cool.tex");

	std::cout << bestDAG.getNodeInfo()<< std::endl;

	tend = time(0);
	std::cout << "It took " << difftime(tend, tstart) << " second(s)." << std::endl;

	return 0;
}

int
main()
{
	return RTSS21IndustryChallenge();

}

