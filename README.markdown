Chris.Rohlf@gmail.com 2006-2011

## Introduction

QueMod is a fuzzing and protocol reverse engineering framework. It is the
rebirth of an earlier project: QueFuzz. QueMod lets you hijack packets inline
using the Linux kernel NetFilterQueue library. The framework hands the
packet off to your plugins for modification while it keeps statistics on
the whole thing. My plugins for it are currently private, but a template
has been provided so you can develop your own.

The entire framework, along with the plugin interface, is written in C. This
means plugins must be written in C. A plugin that embeds the Ruby interpreter
will be released shortly.

## Installation

For those Debian/Ubuntu users out there you can install the needed libraries
by running:

apt-get install libnetfilter-queue1 libnetfilter-queue-dev clang

(The Makefile uses clang, but can be changed to gcc)

Compile QueMod by typing 'make' in the directory. Then you want an IPTables
rule to hijack traffic for you, heres an example:

iptables -A INPUT -p tcp --dport 9191 -j QUEUE

This will queue all inbound TCP traffic with a destination port of 9191 and
make it available to QueMod, which will hand it off to your plugins via
the QueMod_get_packet hook.

## WARNING

QueMod runs as root for obvious reasons. It is a testing tool, so don't run
it in a production environment... ever.
