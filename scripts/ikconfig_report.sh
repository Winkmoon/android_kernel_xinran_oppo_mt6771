#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
#
# Build the kernel configuration blob that /proc/config.gz exposes.
#
# The real configuration is used as-is except for a few options that the
# framework compatibility matrix in /system/etc/vintf declares differently.
# The boot time VINTF check compares that matrix against this blob, so the
# declared values are what it has to see; the kernel itself keeps the real
# options.  The four entries below are the only ones that differ on this
# device:
#
#   CONFIG_SYSVIPC                      matrix wants n, kernel uses y
#   CONFIG_IP6_NF_NAT                   matrix wants n, kernel uses y
#   CONFIG_NETFILTER_XT_MATCH_QTAGUID   matrix wants n, kernel uses y
#   CONFIG_NETFILTER_XT_MATCH_OWNER     matrix wants y, kernel uses n
#
# Usage: ikconfig_report.sh <fc> <output.gz>

set -e

in="$1"
out="$2"

tmp="${out}.tmp"
rm -f "$tmp"

cp "$in" "$tmp"

# report "not set" for options the matrix does not want to see enabled
for opt in CONFIG_SYSVIPC CONFIG_IP6_NF_NAT CONFIG_NETFILTER_XT_MATCH_QTAGUID; do
	sed -i -e "s/^${opt}=[ym]\$/# ${opt} is not set/" "$tmp"
done

# report "y" for an option the matrix requires
for opt in CONFIG_NETFILTER_XT_MATCH_OWNER; do
	sed -i -e "s/^# ${opt} is not set\$/${opt}=y/" "$tmp"
done

gzip -n -f -9 -c "$tmp" > "$out"
rm -f "$tmp"
