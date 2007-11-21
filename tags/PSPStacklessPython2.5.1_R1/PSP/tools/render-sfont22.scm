;
; SFont Renderer
;
; Render a font to be used with the SFont library
;
; Written by Mike Oliphant (http://nostatic.org)
; Based on other logo examples that came with the gimp
;

; Modified  by  fraca7@free.fr  on   may  21  2006:  render  the  full
; ISO-8859-1 character set

(define (apply-create-sfont-effect img
                                   logo-layer
                                   top-color
                                   bottom-color
                                   do-bumpmap)
  (let* ((width (car (gimp-drawable-width logo-layer)))
         (height (car (gimp-drawable-height logo-layer)))
         (shadow-layer (car (gimp-layer-new img width height RGBA-IMAGE "Shadow" 100 MULTIPLY)))
         (old-fg (car (gimp-palette-get-foreground)))
         (old-bg (car (gimp-palette-get-background))))
    (gimp-selection-none img)
                                        ;    (script-fu-util-image-resize-from-layer img logo-layer)
    (gimp-image-resize img width height 0 0)
    (gimp-image-add-layer img shadow-layer 1)
    (gimp-palette-set-foreground '(0 0 0))
    (gimp-layer-set-preserve-trans logo-layer 100)
    (gimp-edit-fill logo-layer FOREGROUND-FILL)
    (gimp-edit-clear shadow-layer)
    (gimp-selection-layer-alpha logo-layer)
    (gimp-context-set-background '(0 0 0))
    (gimp-selection-grow img 1)
    (gimp-edit-fill shadow-layer BACKGROUND-FILL)
    (gimp-selection-none img)
    (gimp-context-set-foreground top-color)
    (gimp-context-set-background bottom-color)
    (gimp-edit-blend logo-layer FG-BG-RGB-MODE NORMAL-MODE GRADIENT-LINEAR 100 20 REPEAT-NONE FALSE FALSE 0 0 FALSE (/ width 2) 0 (/ width 2) height)

    (gimp-layer-set-preserve-trans logo-layer 0)

    (if (= do-bumpmap TRUE)
        (plug-in-bump-map 1 img logo-layer logo-layer 115 40 1 0 0 0 15
                          TRUE FALSE 0))

    (gimp-context-set-background old-bg)
    (gimp-context-set-foreground old-fg)))

(define (script-fu-render-sfont size
                                font
                                top-color
                                bottom-color
                                do-bumpmap)
  (let* ((text  "! \" # $ % & ' ( ) * + , - . / 0 1 2 3 4 5 6 7 8 9 : ; < = > ? @ A B C D E F G H I J K L M N O P Q R S T U V W X Y Z [ \\ ] ^ _ ` a b c d e f g h i j k l m n o p q r s t u v w x y z { | } ~ ¡ ¢ £ ¤ ¥ ¦ § ¨ © ª « ¬ ­ ® ¯ ° ± ² ³ ´ µ ¶ · ¸ ¹ º » ¼ ½ ¾ ¿ À Á Â Ã Ä Å Æ Ç È É Ê Ë Ì Í Î Ï Ð Ñ Ò Ó Ô Õ Ö × Ø Ù Ú Û Ü Ý Þ ß à á â ã ä å æ ç è é ê ë ì í î ï ð ñ ò ó ô õ ö ÷ ø ù ú û ü ý þ ÿ")
         (img (car (gimp-image-new 256 256 RGB)))
         (extents (gimp-text-get-extents-fontname text size PIXELS font))
         (descent (cadr (cddr extents)))
         (text-layer (car (gimp-text-fontname img -1 0 0 text 2 TRUE size PIXELS font)))
         (width (car (gimp-drawable-width text-layer)))
         (height (car (gimp-drawable-height text-layer))))
    (gimp-image-undo-disable img)
    (gimp-layer-set-name text-layer "font")
    (gimp-layer-resize text-layer width (+ height descent) 0 descent)
    (gimp-layer-set-offsets text-layer 0 0)
    (apply-create-sfont-effect img text-layer top-color bottom-color
                               do-bumpmap)
    (gimp-image-undo-enable img)
    (gimp-display-new img)))

(script-fu-register "script-fu-render-sfont"
                    _"<Toolbox>/Xtns/Script-Fu/SFont/Create SFont"
                    "Creates font for use with the SFont library"
                    "Mike Oliphant"
                    "Mike Oliphant"
                    "2004"
                    ""
                    SF-ADJUSTMENT _"Font Size (pixels)" '(28 2 1000 1 10 0 1)
                    SF-FONT       _"Font" "Sans"
                    SF-COLOR      _"Top Color" '(16 23 229)
                    SF-COLOR      _"Bottom Color" '(156 241 244)
                    SF-TOGGLE     _"Apply 3D effect" TRUE)
