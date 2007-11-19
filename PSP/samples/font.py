#-*- coding: ISO-8859-1 -*-

import psp2d, time

def main():
    scr = psp2d.Screen()

    img = psp2d.Image('background.png')
    scr.blit(img)

    font = psp2d.Font('font.png')
    msg = 'Hello, world! éçà'
    font.drawText(scr, (480 - font.textWidth(msg))/2, 20, msg)

    scr.swap()

    while True:
        pad = psp2d.Controller()
        if pad.circle:
            break

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
    except:
        import traceback
        traceback.print_exc(file = file('trace.txt', 'w'))
