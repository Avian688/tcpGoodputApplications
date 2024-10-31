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

package tcpgoodputapplications.applications.tcpapp;

import inet.applications.tcpapp.TcpSinkAppThread;
//
// Thread for TcpSinkApp that allows to calculate point goodput observed by the application. Every thrMeasurementInterval,
// TcpThroughputSinkAppThread calculated the achieved throughput in that interval and records it in ReceiverSideThroughput.
simple TcpGoodputSinkAppThread extends TcpSinkAppThread
{
    parameters:
    @class(TcpGoodputSinkAppThread);
    double goodputInterval @unit(s) = default(0.1s);
    
    @signal[goodput];
    @statistic[goodput](source=goodput;unit=bps; record=vector);
}
