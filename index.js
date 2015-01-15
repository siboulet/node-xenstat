var xenstat = require('bindings')('xenstat');

// Flags for types of information to collect
xenstat.VCPU = 0x1;
xenstat.NETWORK = 0x2;
xenstat.XEN_VERSION = 0x4;
xenstat.VBD = 0x8;
xenstat.ALL = xenstat.VCPU|xenstat.NETWORK|xenstat.XEN_VERSION|xenstat.VBD;

exports = module.exports = xenstat;
