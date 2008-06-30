#!/usr/bin/env python
#-*- coding: ISO-8859-1 -*-

#****h* /pspnet.py
#  NAME
#    pspnet -- Network-related classes and functions for the PSP
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

Network-related classes and functions for the PSP

@group Network information: getAPCTLState getIP enumConfigs
@group Connection: connectToAPCTL disconnectAPCTL
@group WLAN state: wlan*

"""

__rcsid__ = '$Id$'

def connectToAPCTL(config=1, callback=None, timeout=-1):
    """Connects to the network.
    @param config: The network configuration to use, starting at 1.
        See L{enumConfigs}.
    @type config: integer
    @param callback: An optional callback to be invoked at each
        state change in the connection process. It must take an
        integer state as argument.
    @type callback: callable
    @param timeout: Connection timeout
    @type timeout: integer"""

def getAPCTLState():
    """Returns the current connection state. This is the same value as
    the one passed to the callback in L{connectToAPCTL}."""

def disconnectAPCTL():
    """Disconnects from the network."""

def getIP():
    """@return: The current IP address.
    @returntype: string"""

def enumConfigs():
    """Lists all available network configurations.
    @return: A list of 3-tuples (index, name, IP address). I{index} is
        the value to pass as I{config} in L{connectToAPCTL}."""

def wlanIsPowered():
    """@return: True if WLAN is currently powered.
    @returntype: boolean"""

def wlanEtherAddr():
    """@return: The WLAN MAC address.
    @returntype: string"""

def wlanSwitchState():
    """@return: True if the WLAN switch is on 'enabled'.
    @returntype: boolean"""
