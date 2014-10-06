#ifndef XENSTAT_VBD_H_
#define XENSTAT_VBD_H_

#include "node_xenstat.h"

namespace Xenstat {

class Vbd : public ObjectWrap {
  public:
    static void Init(Handle<Object> target);
    static Handle<Value> NewInstance(int argc, Handle<Value> *argv);

    Vbd(xenstat_vbd *xvbd) : xvbd_(xvbd) {};

    static NAN_METHOD(New);

    static NAN_GETTER(GetType);
    static NAN_GETTER(GetDev);
    static NAN_GETTER(GetOoReqs);
    static NAN_GETTER(GetReadReqs);
    static NAN_GETTER(GetWriteReqs);
    static NAN_GETTER(GetReadSects);
    static NAN_GETTER(GetWriteSects);

    xenstat_vbd *xvbd_;
};

} // namespace

#endif
