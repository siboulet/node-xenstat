#ifndef XENSTAT_DOMAIN_H_
#define XENSTAT_DOMAIN_H_

#include "node_xenstat.h"

namespace Xenstat {

class Domain : public ObjectWrap {
  public:
    static void Init(Handle<Object> target);
    static Handle<Value> NewInstance(int argc, Handle<Value> *argv);

    Domain(xenstat_domain *xdomain) {};

    static NAN_METHOD(New);
};

} // namespace

#endif
