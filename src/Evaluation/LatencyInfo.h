/*
 * LatencyInfo.h
 *
 *  Created on: May 2, 2019
 *      Author: mirco
 */

#ifndef EVALUATION_LATENCYINFO_H_
#define EVALUATION_LATENCYINFO_H_
#include <ostream>

struct LatencyInfo
{
	unsigned minLatency; //Not implemented yet
	unsigned maxLatency; // Also Data Age
	unsigned reactionTime;
};

inline std::ostream&
operator <<(std::ostream& o, const LatencyInfo& info)
{
	o << "MinLatency: " << info.minLatency << std::endl;
	o << "MaxLatency: " << info.maxLatency << std::endl;
	o << "ReactionTime: " << info.reactionTime << std::endl;
	return o;
}

#endif /* EVALUATION_LATENCYINFO_H_ */