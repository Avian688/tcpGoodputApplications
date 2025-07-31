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

#include "TcpGoodputSinkAppThread.h"
#include <cmath>

Define_Module(TcpGoodputSinkAppThread);

void TcpGoodputSinkAppThread::initialize(int stage) {
    TcpSinkAppThread::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        goodputReceiverSignal = registerSignal("goodput");
        goodputTimer = new cMessage("goodputTimer");

        goodputInterval = par("goodputInterval");

        lastBytesReceived = 0;
        oldLastBytesReceived = 0;
    }

}

void TcpGoodputSinkAppThread::handleMessage(cMessage *message) {
    if (message == goodputTimer && message->isSelfMessage()) {
        EV_TRACE << "Message received at: " << simTime() << std::endl;
        double gpt = computeGoodput(false);

        emit(goodputReceiverSignal, gpt);

        oldLastBytesReceived = lastBytesReceived;
        lastBytesReceived = bytesRcvd;

        oldlastGoodputTime = lastGoodputTime;
        lastGoodputTime = simTime();

        scheduleAt(simTime() + goodputInterval, goodputTimer);
    }
}

void TcpGoodputSinkAppThread::established() {
    TcpSinkAppThread::established();

    lastGoodputTime = simTime();
    oldlastGoodputTime = simTime();

    EV_TRACE << "lastGoodputTime value set at: " << simTime() << std::endl;

    scheduleAt(std::ceil(simTime().dbl()), goodputTimer);

}

long TcpGoodputSinkAppThread::computeGoodput(bool peerClosed) {
    long thr;
    if (!peerClosed) {
        EV_TRACE << "Bytes received since last measurement: " << bytesRcvd - lastBytesReceived << "B. Time elapsed since last time measured: " << simTime() - lastGoodputTime << std::endl;
        thr = (bytesRcvd - lastBytesReceived) * 8 / (simTime().dbl() - lastGoodputTime.dbl());
        EV_TRACE << "Throughput computed from application: " << thr << std::endl;
    }
    else {
        EV_TRACE << "Bytes received since last measurement: " << bytesRcvd - oldLastBytesReceived << "B. Time elapsed since last time measured: " << simTime() - oldlastGoodputTime << std::endl;
        thr = (bytesRcvd - oldLastBytesReceived) * 8 / (simTime().dbl() - oldlastGoodputTime.dbl());
        EV_TRACE << "Throughput computed from application: " << thr << std::endl;
    }
    return thr;
}

void TcpGoodputSinkAppThread::peerClosed() {
    TcpSinkAppThread::peerClosed();
    EV_TRACE << "Peer closed" << std::endl;
    long gpt = computeGoodput(true);
    lastBytesReceived = bytesRcvd;
    lastGoodputTime = simTime();

    emit(goodputReceiverSignal, gpt);
    if (goodputTimer->isScheduled()) {
        cancelEvent(goodputTimer);
    }

}

TcpGoodputSinkAppThread::~TcpGoodputSinkAppThread() {
    cancelAndDelete(goodputTimer);
}

