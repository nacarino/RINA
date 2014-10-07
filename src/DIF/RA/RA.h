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
 * @file RA.h
 * @author Tomas Hykel (xhykel01@stud.fit.vutbr.cz)
 * @brief
 * @detail
 */

#ifndef __RINA_RA_H_
#define __RINA_RA_H_

#include <omnetpp.h>
#include "DA.h"
#include "Flow.h"
#include "FABase.h"
#include "RMT.h"
#include "RABase.h"

//Consts
extern const char* PAR_QOSDATA;
extern const char* ELEM_QOSCUBE;
extern const char* ELEM_AVGBW;
extern const char* ELEM_AVGSDUBW;
extern const char* ELEM_PEAKBWDUR;
extern const char* ELEM_PEAKSDUBWDUR;
extern const char* ELEM_BURSTPERIOD;
extern const char* ELEM_BURSTDURATION;
extern const char* ELEM_UNDETECTBITERR;
extern const char* ELEM_MAXSDUSIZE;
extern const char* ELEM_PARTIALDELIVER;
extern const char* ELEM_INCOMPLETEDELIVER;
extern const char* ELEM_FORCEORDER;
extern const char* ELEM_MAXALLOWGAP;
extern const char* ELEM_DELAY;
extern const char* ELEM_JITTER;
extern const char* ELEM_COSTTIME;
extern const char* ELEM_COSTBITS;

class RA : public RABase {

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

  private:
    DA* DifAllocator;
    RMT* rmt;
    std::string processName;

    void initQoSCubes();

    void createFlow(std::string dstIpc);
    void removeFlow();

    void bindFlowToRMT(cModule* ipc, Flow *flow);
    void bindMediumToRMT();

    void initSignalsAndListeners();

/*
 * XXX: Vesely @Hykel ->
 *      Takhle tedy signaly urcite ne, kdyz uz, tak s RABase a RAListeners tridami,
 *      aby nedochazelo k cyklickym zavislostem mezi FA - RA
 *      Navic jsem si vzpomnel, proc byly FA signaly v RIBDemonovi. RIBDemon bude emitovat
 *      signaly pro vsechny DIF manangement komponenty, protoze je zpracovatelem ridicich CDAP
 *      zprav. Vysvetleni kdyztak na dalsi schuzce.
 */

/*
    //--------------------- FA ---------------------
    //Signals
    simsignal_t sigFACreReq;
    simsignal_t sigFACreRes;
    simsignal_t sigDelReq;
    simsignal_t sigDelRes;
    //Signaling
    void signalizeFACreateRequestFlow();
    void signalizeFACreateResponseFlow();
    void signalizeFADeleteRequestFlow();
    void signalizeFADeleteResponseFlow();
    //--------------------- FAI ---------------------
*/
};


#endif
