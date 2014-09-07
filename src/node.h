#ifndef XENSTAT_NODE_H_
#define XENSTAT_NODE_H_

#include "node_xenstat.h"

namespace Xenstat {

class Node : public ObjectWrap {
  public:
    static void Init(Handle<Object> target);
    static Persistent<Function> constructor;

    Node(xenstat_node *xnode) : xnode_(xnode) {};
    ~Node();

    static NAN_METHOD(New);

    static NAN_GETTER(GetXenVersion);
    static NAN_GETTER(GetTotMem);
    static NAN_GETTER(GetFreeMem);
    static NAN_GETTER(GetFreeableMb);
    static NAN_GETTER(GetNumDomains);
    static NAN_GETTER(GetNumCpus);
    static NAN_GETTER(GetCpuHz);
    static NAN_GETTER(GetDomains);

    static NAN_METHOD(GetDomainById);
    static NAN_METHOD(GetDomainByName);

    xenstat_node *xnode_;
};

} // namespace

#endif
