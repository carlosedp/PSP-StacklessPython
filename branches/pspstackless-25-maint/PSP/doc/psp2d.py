#!/usr/bin/env python
#-*- coding: ISO-8859-1 -*-

#****h* /psp2d.py
#  NAME
#    psp2d -- 2D drawing module for the PSP
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

2D drawing module for the PSP

@group Drawing: Color Image Mask Font Transform
@group In-place image filters: *InPlaceFilter
@group Image filters: *Filter

"""

__rcsid__ = '$Id$'

IMG_PNG  = 0
IMG_JPEG = 1

TR_PLUS  = 0
TR_MULT  = 1
TR_G2A   = 2
TR_GRAY  = 3

def Screen():
    """This is a factory that returns a Screen instance. It has the
    same methods and properties as L{Image}."""

class Error(Exception):
    """Generic exception for this module"""

class Color(object):
    """An RGBA color"""

    def __init__(self, r, g, b, a=255):
        """Creates a new color from RGB[A] values.
        @param r: Red component
        @type r: integer
        @param g: Green component
        @type g: integer
        @param b: Blue component
        @type b: integer
        @param a: Alpha component
        @type a: integer"""

    def __get_red(self):
        pass
    def __set_red(self):
        pass
    red = property(__get_red, __set_red, doc='''Red component.
    @type: integer''')

    def __get_green(self):
        pass
    def __set_green(self):
        pass
    green = property(__get_green, __set_green, doc='''Green component.
    @type: integer''')

    def __get_blue(self):
        pass
    def __set_blue(self):
        pass
    blue = property(__get_blue, __set_blue, doc='''Blue component.
    @type: integer''')

    def __get_alpha(self):
        pass
    def __set_alpha(self):
        pass
    alpha = property(__get_alpha, __set_alpha, doc='''Alpha component.
    @type: integer''')

class Image(object):
    """An image class.

    @group Drawing methods: blit clear fillRect getPixel putPixel drawLine drawText
    @sort: blit
    @group File I/O: saveToFile"""

    def __init__(self, a, b=None):
        """Creates a new image. There are actually three ways to call
        this constructor:

          - Image(img): Copies another image
          - Image(filename): Loads from a PNG file
          - Image(w, h): Creates a new image with specified dimensions."""

    def __get_width(self):
        pass
    width = property(__get_width, doc='''The image width.
    @type: integer''')

    def __get_height(self):
        pass
    height = property(__get_height, doc='''The image height.
    @type: integer''')

    def __get_size(self):
        pass
    size = property(__get_size, doc='''The image size, as a 2-tuple (w, h).
    @type: tuple''')

    def clear(self, color):
        """Clears the image with the specified color.
        @param color: A color
        @type color: L{Color}"""

    def blit(self, src, sx=0, sy=0, w=-1, h=-1, dx=0, dy=0, blend=False, dw=-1, dh=-1):
        """Blits an image to this one. You can blit either:

          - An image to an image
          - An image to the screen

        @param src: Source image
        @type src: L{Image}
        @keyword sx: Source X position
        @type sx: integer
        @keyword sy: Source Y position
        @type sy: integer
        @keyword w: Source width (if -1, as much as possible)
        @type w: integer
        @keyword h: Source height (if -1, as much as possible)
        @type h: integer
        @keyword dx: Destination X position
        @type dx: integer
        @keyword dy: Destination Y position
        @type dy: integer
        @keyword blend: If True, blending will be performed based on the source's alpha channel.
        @type blend: boolean
        @keyword dw: Destination width; if -1, don't stretch
        @type dw: integer
        @keyword dh: Destination height; if -1, don't stretch
        @type dh: integer"""

    def fillRect(self, x, y, w, h, color):
        """Fills the specified rectangle with the specified color.

        @param x: Start X position of the rectangle
        @type x: integer
        @param y: Start Y position of the rectangle
        @type y: integer
        @param w: The rectangle width
        @type w: integer
        @param h: The rectangle height
        @type h: integer
        @param color: The color used to fill
        @type color: L{Color}"""

    def saveToFile(self, filename, type=IMG_PNG):
        """Saves this image to a file.

        @param filename: File name
        @type filename: string
        @param type: Type of image to save (PNG or JPEG)
        @type type: integer"""

    def putPixel(self, x, y, color=Color(255, 255, 255, 255)):
        """Sets the color for one pixel.
        @param x: X position
        @type x: integer
        @param y: Y position
        @type y: integer
        @param color: Pixel's color
        @type color: L{Color}"""

    def getPixel(self, x, y):
        """Gets the color of one pixel.
        @param x: X position
        @type x: integer
        @param y: Y position
        @type y: integer
        @return: The pixel's color
        @returntype: L{Color}"""

    def drawLine(self, x0, y0, x1, y1, color=Color(255, 255, 255, 255)):
        """Draws a line.
        @param x0: X source position
        @type x0: integer
        @param y0, Y source position
        @type y0: integer
        @param x1: X destination position
        @type x1: integer
        @param y1: Y destination position
        @type y1: integer
        @param color: Line color
        @type color: L{Color}"""

    def drawText(self, x, y, text, color=Color(255, 255, 255, 255)):
        """Draws some text with the PSP default system font.
        @param x: X position
        @type x: integer
        @param y: Y position
        @type y: integer
        @param text: Text to draw
        @type text: string
        @param color: Text color
        @type color: L{Color}"""

class Controller(object):
    """This class is used to read the PSP controller state (buttons,
    stick). The controller state is actually read upon instantiation."""

    def __init__(self):
        """Read the controller state and store it in this instance."""

    def __get_analogX(self):
        pass
    analogX = property(__get_analogX, doc='''The current value in the X direction for the analog stick.
    @type: integer, from -127 to 128.''')

    def __get_analogY(self):
        pass
    analogY = property(__get_analogX, doc='''The current value in the Y direction for the analog stick.
    @type: integer, from -127 to 128.''')

    def __get_square(self):
        pass
    square = property(__get_square, doc='''State of the Square button.
    @type: boolean''')

    def __get_triangle(self):
        pass
    triangle = property(__get_triangle, doc='''State of the Triangle button.
    @type: boolean''')

    def __get_circle(self):
        pass
    circle = property(__get_circle, doc='''State of the Circle button.
    @type: boolean''')

    def __get_cross(self):
        pass
    cross = property(__get_cross, doc='''State of the Cross button.
    @type: boolean''')

    def __get_up(self):
        pass
    up = property(__get_up, doc='''State of the up button on the digital stick.
    @type: boolean''')

    def __get_down(self):
        pass
    down = property(__get_down, doc='''State of the down button on the digital stick.
    @type: boolean''')

    def __get_left(self):
        pass
    left = property(__get_left, doc='''State of the left button on the digital stick.
    @type: boolean''')

    def __get_right(self):
        pass
    right = property(__get_right, doc='''State of the right button on the digital stick.
    @type: boolean''')

    def __get_start(self):
        pass
    start = property(__get_start, doc='''State of the Start button.
    @type: boolean''')

    def __get_select(self):
        pass
    select = property(__get_select, doc='''State of the Select button.
    @type: boolean''')

    def __get_l(self):
        pass
    l = property(__get_cross, doc='''State of the left trigger.
    @type: boolean''')

    def __get_r(self):
        pass
    r = property(__get_cross, doc='''State of the right trigger.
    @type: boolean''')

class Mask(object):
    """This is actually a two-dimensional boolean array, used for
    collision detection.

    @group Manipulation: union
    @group Collision: collide isIn"""

    def __init__(self, img, x, y, w, h, t=127):
        """Builds a new Mask from an L{Image} part.
        @param img: Source image.
        @type img: L{Image}
        @param x: X position to start from
        @type x: integer
        @param y: Y position to start from
        @type y: integer
        @param w: Width of the rectangle to consider.
        @type w: integer
        @param h: Height of the rectangle to consider.
        @type h: integer
        @param t: Gray threshold. For each pixel whose gray value is under this
            value, the corresponding mask bit will be 0, else 1.
        @type t: integer, from 0 to 255"""

    def collide(self, mask):
        """Check if two masks have pixels in common.
        @param mask: The other L{Mask}.
        @type mask: L{Mask}
        @return: Number of common pixels
        @returntype: integer"""

    def union(self, mask):
        """Adds all I{mask} pixels to this one.
        @param mask: The other L{Mask}.
        @type mask: L{Mask}"""

    def isIn(self, x, y):
        """Check if a pixel belongs to this mask.
        @param x: X position
        @type x: integer
        @param y: Y position
        @type y: integer
        @return: True if the pixel at (x, y) belongs to this mask.
        @returntype: boolean"""

class Font(object):
    """An SFont-like font. ISO-8859-1 character set supported."""

    def __init__(self, filename):
        """Loads a new font from an SFont-like pixmap. See the
        python-psp wiki, in the tools section, to get a Gimp plugin to
        generate such fonts.
        @param filename: SFont pixmap filename.
        @type filename: string"""

    def textWidth(self, text):
        """Computes the width of a specified text when it's drawn with
        this font.
        @param text: Some text in ISO-8859-1 encoding.
        @type text: string
        @return: Width in pixels.
        @returntype: integer"""

    def textHeight(self, text):
        """Computes the height of a specified text when it's drawn
        with this font. This actually always return the same value
        since SFont fonts are fixed-height.
        @param text: Some text in ISO-8859-1 encoding.
        @type text: string
        @return: Height in pixels.
        @returntype: integer"""

    def drawText(self, drw, x, y, text):
        """Draws some text with this font.
        @param drw: The drawable to draw to.
        @type drw: L{Image} or L{Screen}
        @param x: X position to draw to
        @type x: integer
        @param y: Y position to draw to
        @type y: integer
        @param text: Text to draw, in ISO-8859-1 encoding.
        @type text: string"""

class Transform(object):
    """Simple image transformation class."""

    def __init__(self, type_, param=0.0):
        """Creates a new Transform object. There are actually two constructors:

          - Transform(type) creates a new standard transformer
          - Transform(callable, param) creates a new user-defined transformer

        For standard transformers, I{type} may be one of:

          - TR_PLUS: Adds I{param} to each color component in the image
          - TR_MULT: Multiplies each color component by I{param}
          - TR_G2A: For each pixel, set the alpha level to the gray level
          - TR_GRAY: Grays the image
          - TR_BW: Converts the image to monochrome, using I{param} as a threshold.

        User transforms must provide a callable. It will be called for
        each pixel to transform, and given as arguments:

          - The pixel X position
          - The pixel Y position
          - The pixel value (a L{Color} instance)

        It must return a new L{Color} value for the pixel."""

    def apply(self, img):
        """Applies this transformation to an image.
        @param img: Image to transform
        @type img: L{Image}"""

class Timer(object):
    """TODO"""

class StructuringElement(object):
    """This is a structuring element for morphologic operations; see
    L{ErosionFilter} and L{DilationFilter}."""

    def __init__(self, obj):
        """There are actually two
        constructors:

          - StructuringElement(size): Creates a round SE with diameter I{size}
          - StructuringElement(list): Creates an SE from a list of boolean values"""

class Filter(object):
    """This class isn't actually available in the L{psp2d} module, but
    is a generic abstract parent for objects that take an L{Image} as
    input and return another L{Image}."""

    def apply(self, img):
        """Apply this filter to an image.
        @param img: Input Image.
        @type img: L{Image}
        @return: The filtered image.
        @returntype: L{Image}"""

class InPlaceFilter(object):
    """This class isn't actually available in the L{psp2d} module, but
    is a generic abstract parent for objects that modify an L{Image}
    in-place."""

    def apply(self, img):
        """Apply this filter to an image, in place.
        @param img: Image to filter.
        @type img: L{Image}"""

class MonochromeFilter(Filter):
    """Converts images to monochrome."""

    def __init__(self, t=128):
        """@param t: Threshold. If the gray level of a pixel is under
               this value, it will be changed to black.
           @type t: Integer"""

class MonochromeInPlaceFilter(InPlaceFilter):
    """In-place version of L{MonochromeFilter}."""

    def __init__(self, t=128):
        """@param t: Threshold. If the gray level of a pixel is under
               this value, it will be changed to black.
           @type t: Integer"""

class GrayFilter(Filter):
    """Converts images to gray level."""

class GrayInPlaceFilter(InPlaceFilter):
    """In-place version of L{GrayFilter}."""

class ErosionFilter(Filter):
    """Morphologic erosion filter."""

    def __init__(self, se):
        """@param se: The structuring element.
        @type se: L{StructuringElement}"""

class DilationFilter(Filter):
    """Morphologic dilation filter."""

    def __init__(self, se):
        """@param se: The structuring element.
        @type se: L{StructuringElement}"""

class ScaleFilter(Filter):
    """Image scaling filter, with linear interpolation."""

    def __init__(self, w, h):
        """@param w: Width of resulting image
        @type w: integer
        @param h: Height of resulting image
        @type h: integer"""

class SubFilter(Filter):
    """Image cropping filter."""

    def __init__(self, x, y, w, h):
        """@param x: X cropping position
        @type x: integer
        @param y: Y cropping position
        @type y: integer
        @param w: Cropping width
        @type w: integer
        @param h: Croping height
        @type h: integer"""

class SobelFilter(Filter):
    """Sobel gradient filter."""

class DifferenceFilter(Filter):
    """Image difference filter"""

    def __init__(self, img):
        """@param img: The image to substract.
        @type img: L{Image}"""
