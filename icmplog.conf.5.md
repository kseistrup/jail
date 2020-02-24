ICMPLOG.CONF(5) "Linux"

# NAME

icmplog.conf - configuration file for icmplog(8)

# SYNOPSIS

_/etc/icmplog.conf_

# DESCRIPTION

The *icmplog*(8) configuration file is used to specify the level at which
ICMP packets of a given type should be logged.

Log levels for packets are specified using the *type* [_level_] rule
described below.

A default log level can be specified with the *level* keyword, which is used
for packet types which have not been explicitly assigned a level in the
configuration.

The default syslog facility can be overriden with the *facility* keyword.

The *except* keyword can be used to specify a list of hosts from which
connections should be ignored.

Lines beginning with *#* (shell-style comments) and blank lines in the
configuration file are ignored.

An example configuration is included with the source distribution and is
installed by default into _/etc/icmplog.conf_.

# KEYWORDS

*except* [_host_], [_host_], [_host_]
	Specify a list of hosts to ignore connection requests from.  The
	list can be comma or whitespace separated, and may contain fragments
	like _192.168._ and _.foo.com_. If the fragment is numeric, it is
	compared with the IP of the source host, otherwise the comparison is
	made with the resolved hostname.

*level* [_level_]
	Specify the default logging level. The level can be any one of
	_IGNORE_, _ERR_, _WARNING_, _NOTICE_, _INFO_ and _DEBUG_.

*facility* [_facility_]
	Specify the default logging facility. The facility can be any one of
	_USER_, _DAEMON_ and _LOCAL0_-_LOCAL7_.

\[*type*\] [_level_]
	This is a generic rule which specifies the level at which packets of
	a certain type are logged. The type can either be numeric, or one of
	the keywords *echoreply*, *destunreach*, *quench*, *redirect*,
	*echo*, *timeout*, *paramprob*, *timereq*, *timerep*, *inforeq* or
	*inforep*.  The level, as above, is a _syslog.h_ symbolic constant.

# SEE ALSO

*icmplog*(8), *tcplog*(8), *tcplog.conf*(5), *syslogd*(8)

# DISTRIBUTION

Jail is distributed under the Artistic License.

# AUTHORS

- Abhijit Menon-Sen <ams@wiw.org> (original version)
- Klaus Alexander Seistrup <klaus@seistrup.dk>
