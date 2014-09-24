var xenstat = require('../build/Release/xenstat.node');

module.exports = {
  'Collect asynchronous': function(test) {
    xenstat.collect(function(err, stats) {
      test.ifError(err);
      test.ok(stats && typeof stats === 'object');
      test.ok(stats.num_domains >= 1);
      test.ok(stats.domains && stats.domains.length === stats.num_domains);
      test.strictEqual(Object.keys(stats).length, 8);
      test.done();
    });
  },

  'Collect synchronous': function(test) {
    var stats = xenstat.collectSync();
    test.ok(stats && typeof stats === 'object');
    test.ok(stats.num_domains >= 1);
    test.ok(stats.domains && stats.domains.length === stats.num_domains);
    test.strictEqual(Object.keys(stats).length, 8);
    test.done();
  },
};
