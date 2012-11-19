var util = require('util'),
    winston = require('winston');
var levels = Object.keys({
   emerg:0,
   alert:1,
   crit:2,
   error:3,
   warning:4,
   notice:5,
   info:6,
   debug:7
});

var UnixSyslog = exports.Syslog = function (options) {
   var SysLogger = require("./build/Release/Logger.node").Logger;
   this. name = options.name || process.title;
   this.facility  = options.facility  || 'local0';
   this.logger = new SysLogger(this.name, this.facility);
}
util.inherits(UnixSyslog, winston.Transport);
winston.transports.Syslog = UnixSyslog;


UnixSyslog.prototype.log = function (level, msg) {
	this.logger.log(level, msg);
}

