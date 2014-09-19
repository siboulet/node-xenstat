var xenstat = require('../build/Release/xenstat.node');

var stats = xenstat.collectSync();

module.exports = {
  'Get dom0 by ID (with string)': function(test) {
    test.throws(function() {
      stats.getDomainById('0');
    });
    test.done();
  },

  'Get dom0 by name (with int)': function(test) {
    test.throws(function() {
      stats.getDomainByName(0);
    });
    test.done();
  },
};
