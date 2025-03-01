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

#ifndef __TCPGOODPUTAPPLICATIONS_TCPGOODPUTSINKAPPTHREAD_H_
#define __TCPGOODPUTAPPLICATIONS_TCPGOODPUTSINKAPPTHREAD_H_

#include <inet/applications/tcpapp/TcpSinkApp.h>

using namespace omnetpp;
using namespace inet;


class TcpGoodputSinkAppThread : public TcpSinkAppThread
{
private:
    //Used to record throughput in result file
    simsignal_t goodputReceiverSignal;

    //Used to pace throughput recording
    cMessage *goodputTimer;

    //Last point in time we calculated throughput
    //Initialised for the first time in established() rather than initialise();
    simtime_t lastGoodputTime;
    simtime_t oldlastGoodputTime;

    //the amount of bytes received up to the last time we calculated throughput
    long lastBytesReceived;
    long oldLastBytesReceived;

    double goodputInterval;

protected:
    virtual void initialize(int stage) override;
    virtual long computeGoodput(bool peerClosed);
    virtual void established() override;

    //Althout the parent class provided a timerExpired API, there was an issue with ownership of the message when calling scheduleEvent() from hostmod.
    virtual void handleMessage(cMessage *message) override;
    virtual void peerClosed() override;

public:
    virtual ~TcpGoodputSinkAppThread();


};

#endif
