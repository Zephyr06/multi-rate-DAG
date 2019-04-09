/*
 * main.cpp
 *
 *  Created on: Apr 1, 2019
 *      Author: mirco
 */

#include <iostream>
#include "MultiRate/MultiRateTaskset.h"

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

	const auto& baseline = taskSet.createBaselineDAG();

	auto dags = taskSet.createDAGs();

	dags[2].DAGtotikz("test.tex");


}

void
taskset3()
{
	MultiRateTaskset taskSet;

	auto task1 = taskSet.addTask(5, 2, "imu");
	auto task2 = taskSet.addTask(20, 4, "gps");
	auto task3 = taskSet.addTask(10, 5, "planner");
	auto task4 = taskSet.addTask(10, 3, "controller");
	auto task5 = taskSet.addTask(20, 3, "act");

	taskSet.addPrecedenceEdge(task3, task4);

	taskSet.addDataEdge(task1, task3, 1);
	taskSet.addDataEdge(task1, task4, 1);
	taskSet.addDataEdge(task2, task3, 0);
	taskSet.addDataEdge(task2, task4, 0);
	taskSet.addDataEdge(task4, task5, 0);

	const auto& baseline = taskSet.createBaselineDAG();

	auto dags = taskSet.createDAGs();

	dags[69].DAGtotikz("test.tex");

}

void
taskset2()
{
	MultiRateTaskset taskSet;

	auto task1 = taskSet.addTask(10, 2, "sensor");
	auto task2 = taskSet.addTask(10, 5, "sensor");
	auto task3 = taskSet.addTask(20, 5, "act");

	taskSet.addDataEdge(task1, task2, 2);
//	taskSet.addDataEdge(task2, task3, 1);

//	taskSet.addPrecedenceEdge(task1, task2);

	auto baseline = taskSet.createBaselineDAG();

	baseline.printNodes();
	baseline.printEdges();

	auto dags = taskSet.createDAGs();

	for (const auto& dag : dags)
	{
		std::cout << "Next DAG:" << std::endl;
		dag.printEdges();
		std::cout << std::endl;
	}
}

int
main(int argc, char** argv)
{
	taskset3();


}