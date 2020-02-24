ICMPLOG(8) "Linux"

# NAME

icmplog - ICMP packet logger

# SYNOPSIS

*icmplog* [_OPTIONS_]

# DESCRIPTION

*icmplog*, an ICMP logger, is part of the *jail* (just another ip logger)
package.

*icmplog* is a small daemon process which logs all ICMP packets to the local
host. The syslog level at which a given packet is logged is configurable
depending on the ICMP type of the message. Packets from specific hosts can
be ignored, and a default log level can be specified.

*icmplog* can be useful in detecting attempted denials of service, and
monitoring network traffic, especially when used in conjunction with
*tcplog*(8).

The configuration is explained in the *icmplog.conf*(5) manual page.

# OPTIONS

*-f*, *--file* _FILENAME_
	Use _FILENAME_ as the configuration file instead of the default,
	_/etc/icmplog.conf_.

*-n*, *--no-resolve*
	Do not resolve IP addresses to hostnames.

*-v*, *--version*
	Display *icmplog*(8) version.

# SEE ALSO

*icmplog.conf*(5), *tcplog*(8), *tcplog.conf*(5), *syslogd*(8)

# DISTRIBUTION

Jail is distributed under the Artistic License.

# AUTHORS

- Abhijit Menon-Sen <ams@wiw.org> (original version)
- Klaus Alexander Seistrup <klaus@seistrup.dk>

Please see https://github.com/kseistrup/jail for full source and bug reports.
