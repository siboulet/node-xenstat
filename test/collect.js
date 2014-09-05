var xenstat = require('../build/Release/xenstat.node');

module.exports = {
  'Collect asynchronous': function(test) {
    xenstat.collect(function(err, stats) {
      test.ifError(err);
      test.ok(stats && typeof stats === 'object');
      test.done();
    });
  },

  'Collect synchronous': function(test) {
    var stats = xenstat.collectSync();
    test.ok(stats && typeof stats === 'object');
    test.done();
  },
};
