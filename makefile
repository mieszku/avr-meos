#
# Copyright (c) 2016 Mieszko Mazurek
#

PROJ		= meos
PORT		= /dev/ttyACM0
PROG		= usbasp 
MCU		= atmega32
FCPU		= 14745600

############################

SRC		= ${shell find . -name '*.[cS]' -or -name '*.cxx'}
OBJ		= ${SRC:./%=obj/%.o}
DEP		= ${OBJ:%.o=%.d}

ELF		= ${PROJ}.elf
HEX		= ${PROJ}.hex

DEFS		= F_CPU=${FCPU}
INCLUDE		= . core drivers hardware cxxdrivers
CFLAGS		= -mmcu=${MCU} -Wall -Os ${addprefix -D, ${DEFS}}
CFLAGS		+= ${addprefix -I, ${INCLUDE}}
CFLAGS		+= -funsigned-char -fdata-sections
CFLAGS_C	= ${CFLAGS} -std=gnu11
CFLAGS_CXX	= ${CFLAGS} -std=gnu++11
ASFLAGS		= ${addprefix -D, ${DEFS}} -mmcu=${MCU} ${addprefix -I, ${INCLUDE}}
LDFLAGS		= -mmcu=${MCU} -Wl,--gc-sections -Tldscripts/avr5.xn 

CC		= avr-gcc -c
AS		= avr-gcc -c
CXX		= avr-g++ -c
LD		= avr-g++
CP		= avr-objcopy
SIZE		= avr-size
DUDE		= avrdude -F -V -P ${PORT}

############################

.PHONY: all clean hex elf flash size



all: elf

elf: ${ELF}

hex: ${HEX}

clean:
	@echo CLEAN
	@rm -rf ${ELF} ${HEX} obj/

flash: ${HEX}
	@echo FLASH $<
	@sudo ${DUDE} -c ${PROG} -p ${MCU} -U flash:w:$<

size: ${ELF}
	@echo SIZE $<
	@${SIZE} $< -B
	


${HEX}: ${ELF}
	@echo CP $@
	@${CP} -j .text -j .data -O ihex $< $@

${ELF}: ${OBJ}
	@echo LD $@
	@${LD} $^ -o $@ ${LDFLAGS}

obj/%.S.o: %.S
	@echo AS $@ 
	@mkdir -p ${dir $@}
	@${AS} $< -MMD -o $@ ${ASFLAGS}

obj/%.c.o: %.c
	@echo CC $@
	@mkdir -p ${dir $@}
	@${CC} $< -MMD -o $@ ${CFLAGS_C}

obj/%.cxx.o: %.cxx
	@echo CXX $@
	@mkdir -p ${dir $@}
	@${CXX} $< -MMD -o $@ ${CFLAGS_CXX}

-include ${DEP}
