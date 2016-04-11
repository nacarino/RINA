//
// The MIT License (MIT)
//
// Copyright (c) 2014-2016 Brno University of Technology, PRISTINE project
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "QTAMux/PS4/PS4.h"
#include "QTAMonitor.h"

namespace QTAMux {
using namespace std;

CU4::CU4(double r, char u, char c) :
        prob(r), urgency(u), cherish(c) {}
CU4List::CU4List(double r) :
        rate(r) {}

PS4::PS4(string _name, QTAMonitor * _parent, cXMLElement* config) :
        PS(_name, _parent), running(false), waiting(0), Bl(0),  intervalData(0) {

    header = 8;
    rtvar = 0.1;
    intervalSize = 10;
    maxBacklog = 1;
    maxQSize = 100;
    if (config != nullptr) {
        if (config->getAttribute("header") != nullptr) {
            header = atoi(config->getAttribute("header"));
        }
        if (config->getAttribute("rtvar") != nullptr) {
            rtvar = atof(config->getAttribute("rtvar"));
        }
        if (config->getAttribute("intervalSize") != nullptr) {
            intervalSize = atof(config->getAttribute("intervalSize"));
        }
        if (config->getAttribute("maxQSize") != nullptr) {
            maxQSize = atoi(config->getAttribute("maxQSize"));
        }
        if (config->getAttribute("maxBacklog") != nullptr) {
            maxBacklog = atof(config->getAttribute("maxBacklog"));
        }

        if (header < 0) {
            header = 0;
        }
        if (rtvar < 0.0) {
            rtvar = -rtvar;
        }
        if (rtvar > 1.0) {
            rtvar = 1.0;
        }
        if (intervalSize < 4) {
            intervalSize = 4;
        }

        //List CUs
        cXMLElementList CThConf = config->getChildrenByTagName("CUList");
        for (auto & thConf : CThConf) {
            double r = atof(thConf->getAttribute("r"));
            CU4List cl(r);
            cXMLElementList CUConf = config->getChildrenByTagName("CU");
            for (auto & thConf : CUConf) {
                double p = atof(thConf->getAttribute("p"));
                char u = atoi(thConf->getAttribute("U"));
                char c = atoi(thConf->getAttribute("C"));
                cl.CUs.push_back(CU4(p, u, c));
            }
            CU4Lists.push_back(cl);
        }
    }
}

PS * PS4::clone(RMTPort * _port, RMTQueue * _queue, Mux * _mux) {
    PS4 * c = new PS4(name, parent, nullptr);
    c->set(_port, _queue, _mux);
    c->setCU(CU4Lists);
    c->setHeader(header);
    c->setRateVar(rtvar);
    c->setMaxBacklog(maxBacklog);
    c->setMaxQSize(maxQSize);
    c->setIntervalSize(intervalSize);

    long portRate = _port->getFlow()->getQosCube().getAvgBand();

    cout << " Port Rate : " << portRate << endl;
    c->setBtXbit((1.0 - rtvar) / portRate, (1.0 + rtvar) / portRate);
    return c;
}

void PS4::setCU(list<CU4List> & _CUs) {
    CU4Lists = _CUs;
}

void PS4::setHeader(int _header) {
    header = _header;
}

void PS4::setRateVar(int _rtvar) {
    rtvar = _rtvar;
}

void PS4::setBtXbit(double min, double max) {
    minWtXbit = min;
    maxWtXbit = max;
}

void PS4::setIntervalSize(int s) {
    intervalSize = s;
}

void PS4::setMaxQSize(int s) {
    maxQSize = s;
}

void PS4::setMaxBacklog(double s) {
    maxBacklog = s;
}

void PS4::run() {
    running = true;
    if (waiting <= 0) {
        running = false;
        return;
    }

    char u = U.front();
    char c = C.front();
    U.pop_front();
    C.pop_front();

    mux->add(q, u, c);
    waiting--;

    if (W.empty()) {
        running = false;
    } else {
        double w = W.front();
        W.pop_front();
        Bl -= w;
        parent->schedulePS(this, w);
    }
}

const CU4 & randomCU(const CU4List & cl) {
    double r = uniform(0.0, 1.0);
    for(const CU4 & c : cl.CUs) {
        r -= c.prob;
        if(r <= 0.0) {
            return c;
        }
    }
    const CU4 & c = cl.CUs.back();
    return c;
}

void PS4::inserted() {
    int inS = (q->getLastPDU()->getBitLength() + header);
    double w = inS * uniform(minWtXbit, maxWtXbit);

    if (q->getLength() >= maxQSize || w + Bl > maxBacklog) {
        parent->recDelete(q->dropLast());
        return;
    }
    W.push_back(w);
    Bl += w;

    simtime_t now = simTime();

    if (arrivalTime.size() <= (unsigned int) intervalSize) {
        CU4List & cl = CU4Lists.front();
        const CU4 & c = randomCU(cl);
        U.push_back(c.urgency);
        C.push_back(c.cherish);
    } else {
        simtime_t tvar = now - arrivalTime.front();

        if (tvar == 0) {
            CU4List & cl = CU4Lists.back();
            const CU4 & c = randomCU(cl);
            U.push_back(c.urgency);
            C.push_back(c.cherish);
        } else {

            double crate = intervalData / tvar.dbl();

            double tcrate = crate;
            cout << "Current Rate : ";

            if (tcrate < 1000) {
                cout << tcrate << " bps";
            } else {
                tcrate /= 1000;
                if (tcrate < 1000) {
                    cout << tcrate << " Kbps";
                } else {
                    tcrate /= 1000;
                    if (tcrate < 1000) {
                        cout << tcrate << " Mbps";
                    } else {
                        tcrate /= 1000;
                        cout << tcrate << " Gbps";
                    }
                }
            }
          //  cout << "  /  "<< intervalData << " / " << tvar.dbl() << endl;
          //  cout << endl;

            bool found = false;
            for (const CU4List & cl : CU4Lists) {
                if (cl.rate > crate) {
                    const CU4 & c = randomCU(cl);
                    U.push_back(c.urgency);
                    C.push_back(c.cherish);
                    found = true;
                    cout << "  /  "<< (int)c.urgency << " / " << (int)c.cherish << endl;
                    break;
                }
            }
            if (!found) {
                CU4List & cl = CU4Lists.back();
                const CU4 & c = randomCU(cl);
                U.push_back(c.urgency);
                C.push_back(c.cherish);
                cout << "  /  "<< (int)c.urgency << " / " << (int)c.cherish << endl;
            }
        }

        intervalData -= arrivalSize.front();
        arrivalTime.pop_front();
        arrivalSize.pop_front();
    }

    arrivalTime.push_back(now);
    arrivalSize.push_back(inS);
    intervalData += inS;

    waiting++;

    if (!running) {
        run();
    }
}

}
