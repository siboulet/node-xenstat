#include "domain.h"

namespace Xenstat {

Persistent<Function> Domain::constructor;

NAN_METHOD(Domain::New) {
  NanScope();
  assert(args.IsConstructCall());

  Local<External> wrap = Local<External>::Cast(args[0]);
  xenstat_domain *xdomain = static_cast<xenstat_domain*>(wrap->Value());

  Domain *domain = new Domain(xdomain);
  domain->Wrap(args.This());

  NanReturnValue(args.This());
}

NAN_GETTER(Domain::GetId) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_domain_id(domain->xdomain_)));
}

NAN_GETTER(Domain::GetName) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<String>(xenstat_domain_name(domain->xdomain_)));
}

NAN_GETTER(Domain::GetCpuNs) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_domain_cpu_ns(domain->xdomain_)));
}

NAN_GETTER(Domain::GetNumVcpus) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_domain_num_vcpus(domain->xdomain_)));
}

NAN_GETTER(Domain::GetVcpus) {
  NanScope();
  NanReturnUndefined();
}

NAN_GETTER(Domain::GetCurMem) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_domain_cur_mem(domain->xdomain_)));
}

NAN_GETTER(Domain::GetMaxMem) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_domain_max_mem(domain->xdomain_)));
}

NAN_GETTER(Domain::GetSsid) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_domain_ssid(domain->xdomain_)));
}

NAN_GETTER(Domain::GetDying) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<Boolean>(xenstat_domain_dying(domain->xdomain_)));
}

NAN_GETTER(Domain::GetCrashed) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<Boolean>(xenstat_domain_crashed(domain->xdomain_)));
}

NAN_GETTER(Domain::GetShutdown) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<Boolean>(xenstat_domain_shutdown(domain->xdomain_)));
}

NAN_GETTER(Domain::GetPaused) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<Boolean>(xenstat_domain_paused(domain->xdomain_)));
}

NAN_GETTER(Domain::GetBlocked) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<Boolean>(xenstat_domain_blocked(domain->xdomain_)));
}

NAN_GETTER(Domain::GetRunning) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<Boolean>(xenstat_domain_running(domain->xdomain_)));
}

NAN_GETTER(Domain::GetNumNetworks) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_domain_num_networks(domain->xdomain_)));
}

NAN_GETTER(Domain::GetNetworks) {
  NanScope();
  NanReturnUndefined();
}

NAN_GETTER(Domain::GetNumVbds) {
  NanScope();
  Domain *domain = ObjectWrap::Unwrap<Domain>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_domain_num_vbds(domain->xdomain_)));
}

NAN_GETTER(Domain::GetVbds) {
  NanScope();
  NanReturnUndefined();
}

void Domain::Init(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("id"), GetId);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("name"), GetName);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("cpu_ns"), GetCpuNs);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("num_vcpus"), GetNumVcpus);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("vcpus"), GetVcpus);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("cur_mem"), GetCurMem);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("max_mem"), GetMaxMem);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("ssid"), GetSsid);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("dying"), GetDying);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("crashed"), GetCrashed);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("shutdown"), GetShutdown);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("paused"), GetPaused);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("blocked"), GetBlocked);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("running"), GetRunning);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("num_networks"), GetNumNetworks);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("networks"), GetNetworks);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("num_vbds"), GetNumVbds);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("vbds"), GetVbds);

  constructor = Persistent<Function>::New(tpl->GetFunction());
}

} // namespace
