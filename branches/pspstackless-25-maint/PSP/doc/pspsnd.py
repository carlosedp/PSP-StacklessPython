#!/usr/bin/env python
#-*- coding: ISO-8859-1 -*-

#****h* /pspsnd.py
#  NAME
#    pspsnd -- Sound and music module for the PSP
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

Sound and music module for the PSP

"""

__rcsid__ = '$Id$'

def setMusicVolume(volume):
    """Sets the music volume.
    @param volume: New volume
    @type volume: integer, between 0 and 255"""

def setSndFxVolume(volume):
    """Sets the volume for sound effects.
    @param volume: New volume
    @type volume: integer, between 0 and 255"""

class Error(Exception):
    """Generic error for this module"""

class Music(object):
    """Music playback"""

    def __init__(self, filename, maxchan=128, loop=False):
        """Creates a new music object from a file format supported by
        mikmodlib.
        @param filename: The music file name
        @type filename: string
        @param maxchan: Maximum number of channels
        @type maxchan: integer
        @param loop: Whether to loop when the music is over
        @type loop: boolean"""

    def start(self):
        """Starts playing."""

    def stop(self):
        """Stops playing.
        @note: This frees all associated resources; you can't call
        L{start} after calling this; instantiate a new L{Music} object
        instead."""

class Sound(object):
    """Sound effects."""

    def __init__(self, filename):
        """Creates a new Sound object from a WAV file.
        @param filename: WAV file name
        @type filename: string"""

    def start(self):
        """Plays the sound effect. May be called several times."""
