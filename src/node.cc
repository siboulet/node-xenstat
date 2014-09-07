#include "node_xenstat.h"
#include "node.h"
#include "domain.h"

namespace Xenstat {

Persistent<Function> Node::constructor;

Node::~Node() {
  if (xnode_ != NULL)
    xenstat_free_node(xnode_);
}

NAN_METHOD(Node::New) {
  NanScope();
  assert(args.IsConstructCall());

  xenstat_node *xnode = NULL;

  // This function can receive an optional xenstat_node object as
  // argument. If it's present, it will be used to initialize the
  // underlying xnode object. Otherwise a new xenstat_node object
  // will be instantiated.

  if (args[0]->IsExternal()) {
    Local<External> wrap = Local<External>::Cast(args[0]);
    xnode = static_cast<xenstat_node*>(wrap->Value());
  } else {
    xnode = xenstat_get_node(xhandle, XENSTAT_ALL);
  }

  Node *node = new Node(xnode);
  node->Wrap(args.This());

  NanReturnValue(args.This());
}

NAN_GETTER(Node::GetXenVersion) {
  NanScope();
  Node* node = ObjectWrap::Unwrap<Node>(args.This());
  NanReturnValue(NanNew<String>(xenstat_node_xen_version(node->xnode_)));
}

NAN_GETTER(Node::GetTotMem) {
  NanScope();
  Node* node = ObjectWrap::Unwrap<Node>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_node_tot_mem(node->xnode_)));
}

NAN_GETTER(Node::GetFreeMem) {
  NanScope();
  Node* node = ObjectWrap::Unwrap<Node>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_node_free_mem(node->xnode_)));
}

NAN_GETTER(Node::GetFreeableMb) {
  NanScope();
  Node* node = ObjectWrap::Unwrap<Node>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_node_freeable_mb(node->xnode_)));
}

NAN_GETTER(Node::GetNumDomains) {
  NanScope();
  Node* node = ObjectWrap::Unwrap<Node>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_node_num_domains(node->xnode_)));
}

NAN_GETTER(Node::GetNumCpus) {
  NanScope();
  Node* node = ObjectWrap::Unwrap<Node>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_node_num_cpus(node->xnode_)));
}

NAN_GETTER(Node::GetCpuHz) {
  NanScope();
  Node* node = ObjectWrap::Unwrap<Node>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_node_cpu_hz(node->xnode_)));
}

NAN_GETTER(Node::GetDomains) {
  NanScope();
  Node* node = ObjectWrap::Unwrap<Node>(args.This());

  uint32_t num_domains = xenstat_node_num_domains(node->xnode_);

  Local<Array> array = Array::New(num_domains);

  for (uint32_t i = 0; i < num_domains; ++i) {
    Local<Value> argv[] = {
      NanNew<External>(xenstat_node_domain_by_index(node->xnode_, i))
    };

    Handle<Object> domain = Domain::constructor->NewInstance(1, argv);
    array->Set(i, domain);
  }

  NanReturnValue(array);
}

NAN_METHOD(Node::GetDomainById) {
  NanScope();
  Node* node = ObjectWrap::Unwrap<Node>(args.This());

  if (!args[0]->IsNumber()) {
    NanReturnUndefined();
  }

  uint32_t id = args[0]->Uint32Value();

  Local<Value> argv[] = {
    NanNew<External>(xenstat_node_domain(node->xnode_, id))
  };

  Handle<Object> domain = Domain::constructor->NewInstance(1, argv);

  NanReturnValue(domain);
}

NAN_METHOD(Node::GetDomainByName) {
  NanScope();
  Node* node = ObjectWrap::Unwrap<Node>(args.This());

  if (!args[0]->IsString()) {
    NanReturnUndefined();
  }

  std::string name = *NanAsciiString(args[0]);

  // Libxenstat doesn't provide a method for retrieving domain by
  // name. We need to loop through each domain and compare name.

  uint32_t num_domains = xenstat_node_num_domains(node->xnode_);

  for (uint32_t i = 0; i < num_domains; ++i) {
    xenstat_domain *xdomain = xenstat_node_domain_by_index(node->xnode_, i);

    if (name == xenstat_domain_name(xdomain)) {
      // Found match
      Local<Value> argv[] = {
        NanNew<External>(xdomain)
      };

      Handle<Object> domain = Domain::constructor->NewInstance(1, argv);
      NanReturnValue(domain);
    }
  }

  NanReturnUndefined();
}

void Node::Init(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->InstanceTemplate()->SetAccessor(
    NanNew("xen_version"), Node::GetXenVersion);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("tot_mem"), Node::GetTotMem);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("free_mem"), Node::GetFreeMem);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("freeable_mb"), Node::GetFreeableMb);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("num_domains"), Node::GetNumDomains);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("num_cpus"), Node::GetNumCpus);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("cpu_hz"), Node::GetCpuHz);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("domains"), Node::GetDomains);

  tpl->PrototypeTemplate()->Set(String::NewSymbol("getDomainById"),
    FunctionTemplate::New(GetDomainById)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getDomainByName"),
    FunctionTemplate::New(GetDomainByName)->GetFunction());

  Node::constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("Node"), constructor);
}

} // namespace
