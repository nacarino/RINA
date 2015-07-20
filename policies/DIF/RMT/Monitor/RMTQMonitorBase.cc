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

#include <RMTQMonitorBase.h>

Define_Module(RMTQMonitorBase);

void RMTQMonitorBase::initialize()
{
    // display active policy name
    cDisplayString& disp = getDisplayString();
    disp.setTagArg("t", 1, "t");
    disp.setTagArg("t", 0, getClassName());

    rmtAllocator = check_and_cast<RMTModuleAllocator*>
        (getModuleByPath("^.allocator"));

    schedPolicy = check_and_cast<RMTSchedulingBase*>
        (getModuleByPath("^.schedulingPolicy"));

    addrComparator = check_and_cast<AddressComparatorBase*>
        (getModuleByPath("^.^.resourceAllocator.addressComparator"));

    onPolicyInit();
}

void RMTQMonitorBase::handleMessage(cMessage *msg)
{
}

void RMTQMonitorBase::onPolicyInit()
{
}

void RMTQMonitorBase::prePDUInsertion(RMTQueue* queue)
{
}

void RMTQMonitorBase::postPDUInsertion(RMTQueue* queue)
{
}

void RMTQMonitorBase::prePDURelease(RMTQueue* queue)
{
}

void RMTQMonitorBase::postPDURelease(RMTQueue* queue)
{
}

void RMTQMonitorBase::onMessageDrop(RMTQueue* queue, const cPacket* pdu)
{
}

void RMTQMonitorBase::postQueueCreation(RMTQueue* queue)
{
}

void RMTQMonitorBase::preQueueRemoval(RMTQueue* queue)
{
}

