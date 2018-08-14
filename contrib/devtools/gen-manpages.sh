#!/usr/bin/env bash

export LC_ALL=C
TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

STARWELSD=${STARWELSD:-$BINDIR/starwelsd}
STARWELSCLI=${STARWELSCLI:-$BINDIR/starwels-cli}
STARWELSTX=${STARWELSTX:-$BINDIR/starwels-tx}
STARWELSQT=${STARWELSQT:-$BINDIR/qt/starwels-qt}

[ ! -x $STARWELSD ] && echo "$STARWELSD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
MAIVER=($($STARWELSCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for starwelsd if --version-string is not set,
# but has different outcomes for starwels-qt and starwels-cli.
echo "[COPYRIGHT]" > footer.h2m
$STARWELSD --version | sed -n '1!p' >> footer.h2m

for cmd in $STARWELSD $STARWELSCLI $STARWELSTX $STARWELSQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${MAIVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${MAIVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
