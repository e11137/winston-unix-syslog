var util = require('util'),
    winston = require('winston');
var levels = Object.keys({
   debug: 0, 
   info: 1, 
   notice: 2, 
   warning: 3,
   error: 4, 
   crit: 5,
   alert: 6,
   emerg: 7
});

var UnixSyslog = exports.Syslog = function (options) {
   var SysLogger = require("./Logger.node").Logger;
   this. name = options.name || process.title;
   this.facility  = options.facility  || 'local0';
   this.logger = new SysLogger(this.name, this.facility);
}
util.inherits(UnixSyslog, winston.Transport);
winston.transports.Syslog = UnixSyslog;


UnixSyslog.prototype.log = function (level, msg) {
	this.logger.log(level, msg);
}

