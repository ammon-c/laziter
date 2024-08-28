#
# FILENAME:     Makefile
# DESCRIPTION:  Build script for "Laziter" text mode arcade
#               game for IBM PC
# AUTHOR:       Ammon R. Campbell
#
# Tools Required:
#      Microsoft C Compiler with NMAKE
#

.SUFFIXES:
.SUFFIXES:   .c .obj

.c.obj:
   cl -nologo -c -f- -AS -Gs -Ox -O1 -Os -W3 -Gr $<

all:   laziter.exe

laziter.exe:   main.obj tmode.obj speaker.obj kbd_drv.obj
   link /NOLOGO /EXEPACK main.obj tmode.obj speaker.obj kbd_drv.obj, laziter.exe;

main.obj:   main.c tmode.h speaker.h kbd_drv.h

tmode.obj:   tmode.c tmode.h

speaker.obj:   speaker.c speaker.h

kbd_drv.obj:   kbd_drv.c kbd_drv.h

clean:
   if exist *.bak del *.bak
   if exist *.obj del *.obj
   if exist *.map del *.map
   if exist *.sym del *.sym
   if exist *.exe del *.exe
   if exist *.lst del *.lst

#
# End Makefile
#
