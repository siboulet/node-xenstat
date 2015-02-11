var xenstat = require('../build/Release/xenstat.node');

var stats = xenstat.collectSync();

module.exports = {
  'Get dom0 by ID': function(test) {
    var dom0 = stats.getDomainById(0);
    test.ok(dom0 && typeof dom0 === 'object');
    test.strictEqual(dom0.id, 0);
    test.strictEqual(dom0.max_mem, Math.pow(2,64));
    test.strictEqual(Object.keys(dom0).length, 19);
    test.done();
  },

  'Get dom0 by name': function(test) {
    var dom0 = stats.getDomainByName('Domain-0');
    test.ok(dom0 && typeof dom0 === 'object');
    test.strictEqual(dom0.name, 'Domain-0');
    test.strictEqual(dom0.max_mem, Math.pow(2,64));
    test.strictEqual(Object.keys(dom0).length, 19);
    test.done();
  },
};
