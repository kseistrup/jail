TCPLOG(8) "Linux"

# NAME

tcplog - TCP packet logger

# SYNOPSIS

*tcplog* [_OPTIONS_]

# DESCRIPTION

*tcplog*, a TCP packet logger, is part of the *jail* (just another ip logger)
package.

*tcplog* is a small daemon process which logs all TCP connection requests to
the local host. The syslog level at which a given packet is logged is
configurable depending on the destination port. Packets from specific
hosts can be ignored, and a default log level can be specified.

*tcplog* can be useful in detecting attempted denials of service, and
monitoring network traffic, especially when used in conjunction with
*icmplog*(8).

The configuration is explained in the *tcplog.conf*(5) manual page.

# OPTIONS

*-f*, *--file* _FILENAME_
	Use _FILENAME_ as the configuration file instead of the default,
	_/etc/tcplog.conf_.

*-n*, *--no-resolve*
	Do not resolve IP addresses to hostnames.

*-v*, *--version*
	Display *tcplog*(8) version.

# SEE ALSO

*tcplog.conf*(5), *icmplog*(8), *icmplog.conf*(5), *syslogd*(8)

# DISTRIBUTION

Jail is distributed under the Artistic License.

# AUTHORS

- Abhijit Menon-Sen <ams@wiw.org> (original version)
- Klaus Alexander Seistrup <klaus@seistrup.dk>

Please see https://github.com/kseistrup/jail for full source and bug reports.
