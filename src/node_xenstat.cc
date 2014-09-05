#include "node_xenstat.h"
#include "domain.h"
#include "node.h"

namespace Xenstat {

class CollectWorker : public NanAsyncWorker {
  public:
    CollectWorker(NanCallback *callback, uint32_t flags)
      : NanAsyncWorker(callback), flags(flags) {}
    ~CollectWorker() {}

    void Execute() {
      // Asynchronously call xenstat
      xnode = xenstat_get_node(xhandle, XENSTAT_ALL);
      if (xnode == NULL)
        SetErrorMessage("Failed to retrieve statistics from libxenstat");
    }

    void HandleOKCallback() {
      NanScope();

      Local<Value> argv[2];

      // Create a new Node V8 Object with the xnode from the asynchronous
      // call to xenstat.
      argv[0] = NanNew<External>(xnode);
      Handle<Object> node = Node::constructor->NewInstance(1, argv);

      argv[0] = NanNull();
      argv[1] = NanNew(node);

      callback->Call(2, argv);
    }

  private:
    uint32_t flags;
    xenstat_node *xnode;
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

  Handle<Object> node = Node::constructor->NewInstance();

  NanReturnValue(node);
}

static void Init(Handle<Object> target) {
  NanScope();

  target->Set(NanNew<String>("collect"),
    NanNew<FunctionTemplate>(Collect)->GetFunction());
  target->Set(NanNew<String>("collectSync"),
    NanNew<FunctionTemplate>(CollectSync)->GetFunction());

  Domain::Init(target);
  Node::Init(target);
}

} // namespace

NODE_MODULE(xenstat, Xenstat::Init);
