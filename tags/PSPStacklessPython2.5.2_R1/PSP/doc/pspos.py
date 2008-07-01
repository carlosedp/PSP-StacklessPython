#!/usr/bin/env python
#-*- coding: ISO-8859-1 -*-

#****h* /pspos.py
#  NAME
#    pspos -- PSP-specific manipulation functions
#  COPYRIGHT
#
#  Copyright (C) 2008 Jérôme Laheurte <fraca7@free.fr>
#
# This library  is free software; you can  redistribute it and/or
# modify  it under  the terms  of the  GNU Lesser  General Public
# License as  published by  the Free Software  Foundation; either
# version  2.1 of  the License,  or  (at your  option) any  later
# version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY;  without even the implied warranty of
# MERCHANTABILITY or  FITNESS FOR A PARTICULAR  PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You  should have  received a  copy  of the  GNU Lesser  General
# Public License  along with this  library; if not, write  to the
# Free  Software Foundation,  Inc., 59  Temple Place,  Suite 330,
# Boston, MA 02111-1307 USA
#
#  CREATION DATE
#    29 Jun 2008
#***

"""

PSP-specific manipulation functions

"""

__rcsid__ = '$Id$'

def freemem():
    """@return: Total free memory, in bytes.
    @note: This doesn't seem to work very well, see
       L{realmem} instead."""

def realmem(size=4096):
    """@return: Actual free memory. This works by allocating blocks
          of size I{size} until malloc() returns NULL."""

def battery():
    """Returns a tuple (plugged, present, charging, lifep, lifet, temp, volt),
    where:

      - I{plugged} is 1 if the PSP is plugged in, 0 else.
      - I{present} is 1 if the battery is present, 0 else.
      - I{charging} is 1 if the battery is charging, 0 else.
      - I{lifep} is the life of the battery, in percentage.
      - I{lifet} is the life of the battery, in minutes (0 if it's plugged).
      - I{temp} is the battery temperature (celsius)
      - I{volt} is the battery voltage."""

def getclock():
    """@return: The CPU clock speed in MHz."""

def setclock(mhz):
    """Sets the CPU clock speed. Valid values are from 1 to 333."""

def getbus():
    """@return: The bus speed in MHz."""

def setbus(mhz):
    """Sets the bus speed in MHz. Valid values are from 1 to 167."""

def powertick():
    """Generates a power tick to prevent the PSP from going idle."""

def sendIR(data):
    """Sends IR codes using pspsircs.
    @type data: A list of 3-tuples (type, cmd, dev)."""
