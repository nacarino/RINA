#include <SimpleTable/SimpleTable.h>
#include <iostream>

const char* SIG_STAT_PDUFT_LENGTH = "PDUFT_Length";

Register_Class(SimpleTable::SimpleTable);

namespace SimpleTable {

using namespace std;

#include <sstream>
#include <Utils.h>


void SimpleTable::initialize(){
    //Inits
    initSignalsAndListeners();
    // Display active policy name.
    cDisplayString& disp = getDisplayString();
    disp.setTagArg("t", 1, "t");
    disp.setTagArg("t", 0, getClassName());

    onPolicyInit();
}

void SimpleTable::initSignalsAndListeners() {
    sigStatPDUFTLENGTH = registerSignal(SIG_STAT_PDUFT_LENGTH);
}

// Lookup function, return a list of RMTPorts to forward a PDU/Address+qos.
vector<RMTPort * > SimpleTable::lookup(const PDU * pdu){
    return lookup(pdu->getDstAddr(), pdu->getConnId().getQoSId());
}
vector<RMTPort * > SimpleTable::lookup(const Address &dst, const unsigned short &qos){
    vector<RMTPort* > ret;
    string dstAddr = dst.getIpcAddress().getName();
    qos2Port * t = &table[dstAddr];
    if(t->find(qos) != t->end()){
        ret.push_back((*t)[qos]);
    } else if (qos == 0 && t->size() > 0){
        ret.push_back(t->begin()->second);
    }

    return ret;
}

// Returns a representation of the Forwarding Knowledge
string SimpleTable::toString(){
    std::ostringstream os;

    os << this->getName()<<endl;
    for(FWDTableIt tIt = table.begin(); tIt != table.end(); tIt++){
        os << "\t" <<tIt->first<<endl;
        for(qos2PortIt qIt = tIt->second.begin(); qIt != tIt->second.end(); qIt++){
            os << "\t\t->(" << qIt->first << " , "<<qIt->second->getFullPath()<<")" <<endl;
        }
    }

    return os.str();
}

//Insert/Remove an entry
void SimpleTable::insert(const Address &addr, const unsigned short &qos, RMTPort * port){
    insert(addr.getIpcAddress().getName(), qos, port);

    std::cout<<"New insert "<<std::endl;
    for (FWDTableIt it = table.begin(); it!=table.end(); it++)
    {
        std::cout<<it->first <<std::endl;
    }
        emit(sigStatPDUFTLENGTH, (long)table.size());
}
void SimpleTable::remove(const Address &addr, const unsigned short &qos){
    remove(addr.getIpcAddress().getName(), qos);
    emit(sigStatPDUFTLENGTH, (long)table.size());
}
void SimpleTable::insert(const string &addr, const unsigned short &qos, RMTPort * port){
    table[addr][qos] = port;
    std::cout<<"New insert "<<std::endl;
    for (FWDTableIt it = table.begin(); it!=table.end(); it++)
    {
        std::cout<<it->first<<std::endl;
    }
    emit(sigStatPDUFTLENGTH, (long)table.size());
}
void SimpleTable::remove(const string &addr, const unsigned short &qos){
    table[addr].erase(qos);
    if(table[addr].empty()){
        table.erase(addr);
    }
    emit(sigStatPDUFTLENGTH, (long)table.size());
}


// Called after initialize
void SimpleTable::onPolicyInit(){}

void SimpleTable::finish(){
 //   EV << toString() <<endl;


}

}
