#include "domain.h"

namespace Xenstat {

static Persistent<FunctionTemplate> constructor;

Handle<Value> Domain::NewInstance(int argc, Handle<Value> *argv) {
  Local<FunctionTemplate> constructorHandle = NanNew(constructor);
  return constructorHandle->GetFunction()->NewInstance(argc, argv);
}

NAN_METHOD(Domain::New) {
  NanScope();
  assert(args.IsConstructCall());

  Local<External> wrap = Local<External>::Cast(args[0]);
  xenstat_domain *xdomain = static_cast<xenstat_domain*>(wrap->Value());

  Local<Object> domain = NanNew<Object>();

  domain->Set(NanNew<String>("id"), NanNew<Number>(xenstat_domain_id(xdomain)));
  domain->Set(NanNew<String>("name"), NanNew<String>(xenstat_domain_name(xdomain)));
  domain->Set(NanNew<String>("cpu_ns"), NanNew<Number>(xenstat_domain_cpu_ns(xdomain)));

  // Vcpus (currently not implemented)
  domain->Set(NanNew<String>("num_vcpus"), NanNew<Number>(xenstat_domain_num_vcpus(xdomain)));
  domain->Set(NanNew<String>("vcpus"), NanUndefined());

  // Memory
  domain->Set(NanNew<String>("cur_mem"), NanNew<Number>(xenstat_domain_cur_mem(xdomain)));
  domain->Set(NanNew<String>("max_mem"), NanNew<Number>(xenstat_domain_max_mem(xdomain)));

  domain->Set(NanNew<String>("ssid"), NanNew<Number>(xenstat_domain_ssid(xdomain)));

  // States
  domain->Set(NanNew<String>("dying"), NanNew<Boolean>(xenstat_domain_dying(xdomain)));
  domain->Set(NanNew<String>("crashed"), NanNew<Boolean>(xenstat_domain_crashed(xdomain)));
  domain->Set(NanNew<String>("shutdown"), NanNew<Boolean>(xenstat_domain_shutdown(xdomain)));
  domain->Set(NanNew<String>("paused"), NanNew<Boolean>(xenstat_domain_paused(xdomain)));
  domain->Set(NanNew<String>("blocked"), NanNew<Boolean>(xenstat_domain_blocked(xdomain)));
  domain->Set(NanNew<String>("running"), NanNew<Boolean>(xenstat_domain_running(xdomain)));

  // Networks
  uint32_t num_networks = xenstat_domain_num_networks(xdomain);
  Local<Object> networks = NanNew<Array>(num_networks);

  for (uint32_t i = 0; i < num_networks; ++i) {
    xenstat_network *xnetwork = xenstat_domain_network(xdomain, i);
    Local<Object> network = NanNew<Object>();

    network->Set(NanNew<String>("id"), NanNew<Number>(xenstat_network_id(xnetwork)));
    network->Set(NanNew<String>("rbytes"), NanNew<Number>(xenstat_network_rbytes(xnetwork)));
    network->Set(NanNew<String>("rpackets"), NanNew<Number>(xenstat_network_rpackets(xnetwork)));
    network->Set(NanNew<String>("rerrs"), NanNew<Number>(xenstat_network_rerrs(xnetwork)));
    network->Set(NanNew<String>("rdrop"), NanNew<Number>(xenstat_network_rdrop(xnetwork)));
    network->Set(NanNew<String>("tbytes"), NanNew<Number>(xenstat_network_tbytes(xnetwork)));
    network->Set(NanNew<String>("tpackets"), NanNew<Number>(xenstat_network_tpackets(xnetwork)));
    network->Set(NanNew<String>("terrs"), NanNew<Number>(xenstat_network_terrs(xnetwork)));
    network->Set(NanNew<String>("tdrop"), NanNew<Number>(xenstat_network_tdrop(xnetwork)));

    networks->Set(i, network);
  }

  domain->Set(NanNew<String>("num_networks"), NanNew<Number>(num_networks));
  domain->Set(NanNew<String>("networks"), networks);

  // Vbds (virtual block devices)
  uint32_t num_vbds = xenstat_domain_num_vbds(xdomain);
  Local<Object> vbds = NanNew<Array>(num_vbds);

  for (uint32_t i = 0; i < num_vbds; ++i) {
    xenstat_vbd *xvbd = xenstat_domain_vbd(xdomain, i);
    Local<Object> vbd = NanNew<Object>();

    vbd->Set(NanNew<String>("type"), NanNew<Number>(xenstat_vbd_type(xvbd)));
    vbd->Set(NanNew<String>("dev"), NanNew<Number>(xenstat_vbd_dev(xvbd)));
    vbd->Set(NanNew<String>("oo_reqs"), NanNew<Number>(xenstat_vbd_oo_reqs(xvbd)));
    vbd->Set(NanNew<String>("rd_reqs"), NanNew<Number>(xenstat_vbd_rd_reqs(xvbd)));
    vbd->Set(NanNew<String>("rw_reqs"), NanNew<Number>(xenstat_vbd_wr_reqs(xvbd)));
    vbd->Set(NanNew<String>("rd_sects"), NanNew<Number>(xenstat_vbd_rd_sects(xvbd)));
    vbd->Set(NanNew<String>("wr_sects"), NanNew<Number>(xenstat_vbd_wr_sects(xvbd)));

    vbds->Set(i, vbd);
  }

  domain->Set(NanNew<String>("num_vbds"), NanNew<Number>(num_vbds));
  domain->Set(NanNew<String>("vbds"), vbds);

  NanReturnValue(domain);
}

void Domain::Init(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);

  NanAssignPersistent(constructor, tpl);
}

} // namespace
