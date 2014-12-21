//
// Copyright © 2014 PRISTINE Consortium (http://ict-pristine.eu)
//  
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//
/*
 * DTCP.h
 *
 *  Created on: Apr 17, 2014
 *      Author: Marcel Marek
 */

#ifndef DTCP_H_
#define DTCP_H_

#include <csimplemodule.h>

#include "FlowControl.h"
#include "RXControl.h"
#include "DTCPState.h"
#include "DTPState.h"
#include "DTP.h"
#include "EFCP_defs.h"
#include "ControlPDU_m.h"
#include "DTCPTimers_m.h"

/* Policies */
#include "DTCPECNSetPolicyBase.h"

class DTP;
class FlowControl;

class DTCP: public cSimpleModule {
    friend class DTP;
  private:

    DTP* dtp;
    DTCPState *dtcpState;
    FlowControl* flowControl;
    RXControl* rxControl;

    DTCPECNSetPolicyBase* ecnSetPolicy;



    /*Timers*/
    WindowTimer* windowTimer;




    void schedule(DTCPTimers *timer, double time = 0.0);
    void resetWindowTimer();

    void sendAckPDU();

public:

    DTCP();
    virtual ~DTCP();

    void setDTP(DTP* dtp);


    void handleWindowTimer(WindowTimer* timer);

    void updateRcvRtWinEdge(DTPState* dtpState);

    unsigned int getNextSndCtrlSeqNum();

    unsigned int getLastCtrlSeqNumRcv();

    void setLastCtrlSeqnumRec(unsigned int ctrlSeqNum);


    void setSndRtWinEdge(unsigned int sndRtWinEdge);
    unsigned int getSndRtWinEdge();
    void setRcvRtWinEdge(unsigned int rcvRtWinEdge);
    unsigned int getRcvRtWinEdge();

    void setSndRate(unsigned int sendingRate);
    unsigned int getSndRate();

    void setRcvRate(unsigned int rcvrRate);
    unsigned int getRcvRate();

    unsigned int getRcvCredit();

    unsigned long getSendingTimeUnit();

    bool isSendingRateFullfilled() const;
    void setSendingRateFullfilled(bool rateFullfilled);

    void runECNSetPolicy(DTPState* dtpState);


protected:
    virtual void handleMessage(cMessage *msg);
    virtual void initialize(int step);
};

#endif /* DTCP_H_ */
