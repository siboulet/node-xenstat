#ifndef XENSTAT_DOMAIN_H_
#define XENSTAT_DOMAIN_H_

#include "node_xenstat.h"

namespace Xenstat {

class Domain : public ObjectWrap {
  public:
    static void Init(Handle<Object> target);
    static Handle<Value> NewInstance(int argc, Handle<Value> *argv);

    Domain(xenstat_domain *xdomain) : xdomain_(xdomain) {};

    static NAN_METHOD(New);

    static NAN_GETTER(GetId);
    static NAN_GETTER(GetName);
    static NAN_GETTER(GetCpuNs);
    static NAN_GETTER(GetNumVcpus);
    static NAN_GETTER(GetVcpus);
    static NAN_GETTER(GetCurMem);
    static NAN_GETTER(GetMaxMem);
    static NAN_GETTER(GetSsid);
    static NAN_GETTER(GetDying);
    static NAN_GETTER(GetCrashed);
    static NAN_GETTER(GetShutdown);
    static NAN_GETTER(GetPaused);
    static NAN_GETTER(GetBlocked);
    static NAN_GETTER(GetRunning);
    static NAN_GETTER(GetNumNetworks);
    static NAN_GETTER(GetNetworks);
    static NAN_GETTER(GetNumVbds);
    static NAN_GETTER(GetVbds);

    xenstat_domain *xdomain_;
};

} // namespace

#endif
