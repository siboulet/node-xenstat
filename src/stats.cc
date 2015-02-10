#include "node_xenstat.h"
#include "stats.h"
#include "domain.h"

namespace Xenstat {

static Persistent<FunctionTemplate> constructor;

Stats::~Stats() {
  if (xnode_ != NULL)
    xenstat_free_node(xnode_);
}

Handle<Value> Stats::NewInstance(int argc, Handle<Value> *argv) {
  Local<FunctionTemplate> constructorHandle = NanNew(constructor);
  return constructorHandle->GetFunction()->NewInstance(argc, argv);
}

NAN_METHOD(Stats::New) {
  NanScope();
  assert(args.IsConstructCall());

  Local<External> wrap = Local<External>::Cast(args[0]);
  Stats *stats = new Stats(static_cast<xenstat_node*>(wrap->Value()));
  stats->Wrap(args.This());

  NanReturnValue(args.This());
}

NAN_GETTER(Stats::GetXenVersion) {
  NanScope();
  Stats* stats = ObjectWrap::Unwrap<Stats>(args.This());
  NanReturnValue(NanNew<String>(xenstat_node_xen_version(stats->xnode_)));
}

NAN_GETTER(Stats::GetTotMem) {
  NanScope();
  Stats* stats = ObjectWrap::Unwrap<Stats>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_node_tot_mem(stats->xnode_)));
}

NAN_GETTER(Stats::GetFreeMem) {
  NanScope();
  Stats* stats = ObjectWrap::Unwrap<Stats>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_node_free_mem(stats->xnode_)));
}

NAN_GETTER(Stats::GetFreeableMb) {
  NanScope();
  Stats* stats = ObjectWrap::Unwrap<Stats>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_node_freeable_mb(stats->xnode_)));
}

NAN_GETTER(Stats::GetNumDomains) {
  NanScope();
  Stats* stats = ObjectWrap::Unwrap<Stats>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_node_num_domains(stats->xnode_)));
}

NAN_GETTER(Stats::GetNumCpus) {
  NanScope();
  Stats* stats = ObjectWrap::Unwrap<Stats>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_node_num_cpus(stats->xnode_)));
}

NAN_GETTER(Stats::GetCpuHz) {
  NanScope();
  Stats* stats = ObjectWrap::Unwrap<Stats>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_node_cpu_hz(stats->xnode_)));
}

NAN_GETTER(Stats::GetDomains) {
  NanScope();
  Stats* stats = ObjectWrap::Unwrap<Stats>(args.This());

  uint32_t num_domains = xenstat_node_num_domains(stats->xnode_);

  Local<Array> array = NanNew<Array>(num_domains);

  for (uint32_t i = 0; i < num_domains; ++i) {
    Local<Value> argv[] = {
      NanNew<External>(xenstat_node_domain_by_index(stats->xnode_, i))
    };

    array->Set(i, Domain::NewInstance(1, argv));
  }

  NanReturnValue(array);
}

NAN_METHOD(Stats::GetDomainById) {
  NanScope();
  Stats* stats = ObjectWrap::Unwrap<Stats>(args.This());

  if (!args[0]->IsNumber()) {
    return NanThrowError("Domain ID must be a number");
  }

  uint32_t id = args[0]->Uint32Value();

  Local<Value> argv[] = {
    NanNew<External>(xenstat_node_domain(stats->xnode_, id))
  };

  NanReturnValue(Domain::NewInstance(1, argv));
}

NAN_METHOD(Stats::GetDomainByName) {
  NanScope();
  Stats* stats = ObjectWrap::Unwrap<Stats>(args.This());

  if (!args[0]->IsString()) {
    return NanThrowError("Domain name must be a string");
  }

  std::string name = *NanAsciiString(args[0]);

  // Libxenstat doesn't provide a method for retrieving domain by
  // name. We need to loop through each domain and compare name.

  uint32_t num_domains = xenstat_node_num_domains(stats->xnode_);

  for (uint32_t i = 0; i < num_domains; ++i) {
    xenstat_domain *xdomain = xenstat_node_domain_by_index(stats->xnode_, i);

    if (name == xenstat_domain_name(xdomain)) {
      // Found match
      Local<Value> argv[] = {
        NanNew<External>(xdomain)
      };

      NanReturnValue(Domain::NewInstance(1, argv));
    }
  }

  NanReturnUndefined();
}

void Stats::Init(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->InstanceTemplate()->SetAccessor(
    NanNew("xen_version"), GetXenVersion);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("tot_mem"), GetTotMem);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("free_mem"), GetFreeMem);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("freeable_mb"), GetFreeableMb);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("num_domains"), GetNumDomains);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("num_cpus"), GetNumCpus);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("cpu_hz"), GetCpuHz);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("domains"), GetDomains);

  tpl->PrototypeTemplate()->Set(NanNew<String>("getDomainById"),
    NanNew<FunctionTemplate>(GetDomainById)->GetFunction());
  tpl->PrototypeTemplate()->Set(NanNew<String>("getDomainByName"),
    NanNew<FunctionTemplate>(GetDomainByName)->GetFunction());

  NanAssignPersistent(constructor, tpl);
}

} // namespace
