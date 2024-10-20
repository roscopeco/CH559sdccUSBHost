PROJECT_NAME=CH559USB
XRAM_SIZE=0x0800
XRAM_LOC=0x0600
CODE_SIZE=0xEFFF
DFREQ_SYS=48000000

SDCC?=sdcc
PACKIHX?=packihx
HEX2BIN=makebin

OBJECTS=main.rel util.rel USBHost.rel uart.rel

all: $(PROJECT_NAME).bin

clean:
	rm -f *.asm *.lst *.rel *.sym *.rst $(PROJECT_NAME).ihx $(PROJECT_NAME).hex $(PROJECT_NAME).bin $(PROJECT_NAME).lk $(PROJECT_NAME).map $(PROJECT_NAME).mem

%.rel: %.c
	$(SDCC) -c -V -mmcs51 --model-large --xram-size $(XRAM_SIZE) --xram-loc $(XRAM_LOC) --code-size $(CODE_SIZE) -I/ -DFREQ_SYS=$(DFREQ_SYS) $<

$(PROJECT_NAME).ihx: $(OBJECTS)
	$(SDCC) $^ -V -mmcs51 --model-large --xram-size $(XRAM_SIZE) --xram-loc $(XRAM_LOC) --code-size $(CODE_SIZE) -I/ -DFREQ_SYS=$(DFREQ_SYS) -o $@

$(PROJECT_NAME).hex: $(PROJECT_NAME).ihx
	$(PACKIHX) $< > $@

$(PROJECT_NAME).bin: $(PROJECT_NAME).hex
	$(HEX2BIN) -p $< $@