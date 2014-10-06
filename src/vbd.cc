#include "vbd.h"

namespace Xenstat {

static Persistent<FunctionTemplate> constructor;

Handle<Value> Vbd::NewInstance(int argc, Handle<Value> *argv) {
  Local<FunctionTemplate> constructorHandle = NanNew(constructor);
  return constructorHandle->GetFunction()->NewInstance(argc, argv);
}

NAN_METHOD(Vbd::New) {
  NanScope();
  assert(args.IsConstructCall());

  Local<External> wrap = Local<External>::Cast(args[0]);
  xenstat_vbd *xvbd = static_cast<xenstat_vbd*>(wrap->Value());

  Vbd *vbd = new Vbd(xvbd);
  vbd->Wrap(args.This());

  NanReturnValue(args.This());
}

NAN_GETTER(Vbd::GetType) {
  NanScope();
  Vbd *vbd = ObjectWrap::Unwrap<Vbd>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_vbd_type(vbd->xvbd_)));
}

NAN_GETTER(Vbd::GetDev) {
  NanScope();
  Vbd *vbd = ObjectWrap::Unwrap<Vbd>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_vbd_dev(vbd->xvbd_)));
}

NAN_GETTER(Vbd::GetOoReqs) {
  NanScope();
  Vbd *vbd = ObjectWrap::Unwrap<Vbd>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_vbd_oo_reqs(vbd->xvbd_)));
}

NAN_GETTER(Vbd::GetReadReqs) {
  NanScope();
  Vbd *vbd = ObjectWrap::Unwrap<Vbd>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_vbd_rd_reqs(vbd->xvbd_)));
}

NAN_GETTER(Vbd::GetWriteReqs) {
  NanScope();
  Vbd *vbd = ObjectWrap::Unwrap<Vbd>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_vbd_wr_reqs(vbd->xvbd_)));
}

NAN_GETTER(Vbd::GetReadSects) {
  NanScope();
  Vbd *vbd = ObjectWrap::Unwrap<Vbd>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_vbd_rd_sects(vbd->xvbd_)));
}

NAN_GETTER(Vbd::GetWriteSects) {
  NanScope();
  Vbd *vbd = ObjectWrap::Unwrap<Vbd>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_vbd_wr_sects(vbd->xvbd_)));
}

void Vbd::Init(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("type"), GetType);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("dev"), GetDev);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("oo_reqs"), GetOoReqs);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("read_reqs"), GetReadReqs);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("write_reqs"), GetWriteReqs);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("read_sects"), GetReadSects);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("write_sects"), GetWriteSects);

  NanAssignPersistent(constructor, tpl);
}

} // namespace
