#---------------------------------------------------------------------------------------
# GBACC makefile
#---------------------------------------------------------------------------------------

#
# You have to re-write for your target.
#
NAME = ReversiAdv
.CFILES	= ReversiMain.c ReversiOpening.c ReversiColorSelect.c ReversiLevelSelect.c ReversiGame.c ReversiEngine.c
.SFILES	= crt0.S
#
#
#

.OFILES = $(.CFILES:.c=.o)

MAPFILE = $(NAME).map
TARGET_ELF = $(NAME).elf
TARGET_BIN = $(NAME).mb.gba

.PHONY: all
all: $(TARGET_BIN)

CFLAGS = -O3 -Wall -mthumb -mthumb-interwork
ASFLAGS = -mthumb-interwork
LDFLAGS = -mthumb-interwork
LD = gcc

$(TARGET_BIN) : $(TARGET_ELF)
	objcopy -O binary $< $@

$(TARGET_ELF): crt0.o $(.OFILES)
	$(LD) $(LDFLAGS) $(.OFILES) -o $@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

%.o: %.S
	as $(ASFLAGS) $< -o $@

#---------------------------------------------------------------------------------------
# end
#---------------------------------------------------------------------------------------
