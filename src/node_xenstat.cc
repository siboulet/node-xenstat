#include <iostream>

#include "node_xenstat.h"
#include "domain.h"
#include "stats.h"

namespace Xenstat {

static xenstat_handle *xhandle = xenstat_init();

class CollectWorker : public NanAsyncWorker {
  public:
    CollectWorker(NanCallback *callback, uint32_t flags)
      : NanAsyncWorker(callback), flags_(flags) {}
    ~CollectWorker() {}

    void Execute() {
      // Asynchronously call xenstat
      xnode_ = xenstat_get_node(xhandle, flags_);
      if (xnode_ == NULL)
        SetErrorMessage("Failed to retrieve statistics from libxenstat");
    }

    void HandleOKCallback() {
      NanScope();

      Local<Value> argv[2];

      // Create a new Node V8 Object with the xnode from the asynchronous
      // call to xenstat.
      argv[0] = NanNew<External>(xnode_);
      Local<Value> node = NanNew<Value>(Stats::NewInstance(1, argv));

      argv[0] = NanNull();
      argv[1] = node;

      callback->Call(2, argv);
    }

  private:
    uint32_t flags_;
    xenstat_node *xnode_;
};

NAN_METHOD(Collect) {
  NanScope();

  // Flags is optional and defaults to XENSTAT_ALL
  uint32_t flags = (args[0]->IsNumber()) ? args[0]->Uint32Value() : XENSTAT_ALL;

  // Check last argument is callback and is function
  if (args.Length() == 0 || !args[args.Length()-1]->IsFunction())
    NanReturnUndefined();
  NanCallback *callback = new NanCallback(args[args.Length()-1].As<Function>());

  NanAsyncQueueWorker(new CollectWorker(callback, flags));
  NanReturnUndefined();
}

NAN_METHOD(CollectSync) {
  NanScope();

  // Flags is optional and defaults to XENSTAT_ALL
  uint32_t flags = (args[0]->IsNumber()) ? args[0]->Uint32Value() : XENSTAT_ALL;

  Local<Value> argv[1];
  argv[0] = NanNew<External>(xenstat_get_node(xhandle, flags));

  NanReturnValue(Stats::NewInstance(1, argv));
}

static void Init(Handle<Object> target) {
  NanScope();

  if (xhandle == NULL) {
    std::cerr << "Failed to initialize libxenstat. Is Xen running?" << std::endl << std::endl;
    return;
  }

  target->Set(NanNew<String>("collect"),
    NanNew<FunctionTemplate>(Collect)->GetFunction());
  target->Set(NanNew<String>("collectSync"),
    NanNew<FunctionTemplate>(CollectSync)->GetFunction());

  Domain::Init(target);
  Stats::Init(target);
}

} // namespace

NODE_MODULE(xenstat, Xenstat::Init);
