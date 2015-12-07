//
// Copyright © 2014 PRISTINE Consortium (http://ict-pristine.eu)
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

/**
 * @file DTPRTTEstimatorPolicyBase.h
 * @author Marcel Marek (imarek@fit.vutbr.cz)
 * @date Jan 22, 2015
 * @brief
 * @detail
 */

#ifndef DTPRTTESTIMATORPOLICYBASE_H_
#define DTPRTTESTIMATORPOLICYBASE_H_

#include <omnetpp.h>

#include "DTPState.h"
#include "DTCPState.h"

/*
 *
 */
class DTPRTTEstimatorPolicyBase : public cSimpleModule
{
  public:
    DTPRTTEstimatorPolicyBase();
    virtual ~DTPRTTEstimatorPolicyBase();
    virtual bool run(DTPState* dtpState, DTCPState* dtcpState) = 0;

  protected:
    virtual void initialize(){};
    virtual void handleMessage(cMessage* msg){};
};

#endif /* DTPRTTESTIMATORPOLICYBASE_H_ */
