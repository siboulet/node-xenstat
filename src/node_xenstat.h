#ifndef NODE_XENSTAT_H_
#define NODE_XENSTAT_H_

extern "C" {
#include <xenstat.h>
}

#include <nan.h>

using namespace node;
using namespace v8;

namespace Xenstat {
  static xenstat_handle *xhandle = xenstat_init();
}

#endif
