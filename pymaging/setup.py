#!/usr/bin/env python
#-*- coding: ISO-8859-1 -*-

#****h* /setup.py
#  NAME
#    setup -- 
#  COPYRIGHT
#
#  Copyright (C) 2007 Jérôme Laheurte <fraca7@free.fr>
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
#    23 Dec 2007
#***

__rcsid__ = '$Id: setup.py 154 2008-03-16 10:35:30Z fraca7 $'

from distutils.core import setup, Extension
import os

setup(name='pymaging',
      ext_modules=[Extension('pymaging',
                             ['main.cpp',
                              'color.cpp',
                              'filters.cpp',
                              'font.cpp',
                              'image.cpp',
                              'mask.cpp',
                              'transform.cpp'],
                             extra_compile_args=['-O3', '-Wall', '-march=i686', '-mmmx'],
                             define_macros=[('LINUX', 1), ('IMAGING_MODULE_NAME', '"pymaging"')],
                             include_dirs=[os.path.join('..', 'imaging')],
                             libraries=['imaging', 'png', 'jpeg', 'z'],
                             library_dirs=[os.path.join('..', 'imaging', 'build-linux')])])
