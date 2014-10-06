#include "network.h"

namespace Xenstat {

static Persistent<FunctionTemplate> constructor;

Handle<Value> Network::NewInstance(int argc, Handle<Value> *argv) {
  Local<FunctionTemplate> constructorHandle = NanNew(constructor);
  return constructorHandle->GetFunction()->NewInstance(argc, argv);
}

NAN_METHOD(Network::New) {
  NanScope();
  assert(args.IsConstructCall());

  Local<External> wrap = Local<External>::Cast(args[0]);
  xenstat_network *xnetwork = static_cast<xenstat_network*>(wrap->Value());

  Network *network = new Network(xnetwork);
  network->Wrap(args.This());

  NanReturnValue(args.This());
}

NAN_GETTER(Network::GetId) {
  NanScope();
  Network *network = ObjectWrap::Unwrap<Network>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_network_id(network->xnetwork_)));
}

NAN_GETTER(Network::GetRxBytes) {
  NanScope();
  Network *network = ObjectWrap::Unwrap<Network>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_network_rbytes(network->xnetwork_)));
}

NAN_GETTER(Network::GetRxPackets) {
  NanScope();
  Network *network = ObjectWrap::Unwrap<Network>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_network_rpackets(network->xnetwork_)));
}

NAN_GETTER(Network::GetRxErrors) {
  NanScope();
  Network *network = ObjectWrap::Unwrap<Network>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_network_rerrs(network->xnetwork_)));
}

NAN_GETTER(Network::GetRxDrops) {
  NanScope();
  Network *network = ObjectWrap::Unwrap<Network>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_network_rdrop(network->xnetwork_)));
}

NAN_GETTER(Network::GetTxBytes) {
  NanScope();
  Network *network = ObjectWrap::Unwrap<Network>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_network_tbytes(network->xnetwork_)));
}

NAN_GETTER(Network::GetTxPackets) {
  NanScope();
  Network *network = ObjectWrap::Unwrap<Network>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_network_tpackets(network->xnetwork_)));
}

NAN_GETTER(Network::GetTxErrors) {
  NanScope();
  Network *network = ObjectWrap::Unwrap<Network>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_network_terrs(network->xnetwork_)));
}

NAN_GETTER(Network::GetTxDrops) {
  NanScope();
  Network *network = ObjectWrap::Unwrap<Network>(args.This());
  NanReturnValue(NanNew<Number>(xenstat_network_tdrop(network->xnetwork_)));
}

void Network::Init(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("id"), GetId);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("rx_bytes"), GetRxBytes);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("rx_packets"), GetRxPackets);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("rx_errors"), GetRxErrors);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("rx_drops"), GetRxDrops);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("tx_bytes"), GetTxBytes);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("tx_packets"), GetTxPackets);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("tx_errors"), GetTxErrors);
  tpl->InstanceTemplate()->SetAccessor(
    NanNew("tx_drops"), GetTxDrops);

  NanAssignPersistent(constructor, tpl);
}

} // namespace
