#ifndef XENSTAT_NETWORK_H_
#define XENSTAT_NETWORK_H_

#include "node_xenstat.h"

namespace Xenstat {

class Network : public ObjectWrap {
  public:
    static void Init(Handle<Object> target);
    static Handle<Value> NewInstance(int argc, Handle<Value> *argv);

    Network(xenstat_network *xnetwork) : xnetwork_(xnetwork) {};

    static NAN_METHOD(New);

    static NAN_GETTER(GetId);
    static NAN_GETTER(GetRxBytes);
    static NAN_GETTER(GetRxPackets);
    static NAN_GETTER(GetRxErrors);
    static NAN_GETTER(GetRxDrops);
    static NAN_GETTER(GetTxBytes);
    static NAN_GETTER(GetTxPackets);
    static NAN_GETTER(GetTxErrors);
    static NAN_GETTER(GetTxDrops);

    xenstat_network *xnetwork_;
};

} // namespace

#endif
