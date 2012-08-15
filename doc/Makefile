KERNELDOC=scripts/kernel-doc
export KERNELDOC

XML_BASENAMES = ortedoc

DSL_DIR = ../../../dsl

XML_SRC = ${XML_BASENAMES:%=%.xml}

#DSL_DEF = -d $(DSL_DIR)/ocera-print.dsl

ORTE_DOC_SOURCES = \
orte/typedefs_api.xml \
orte/protos_api.xml \
orte/defines_api.xml 

default: all

all: $(ORTE_DOC_SOURCES) orteman.pdf

XML_ENT_INCS = $(wildcard *.ent)
XML_ENTITIES = $(shell cat $(XML_ENT_INCS) | \
                 sed -n -e 's/^ *<!ENTITY *[^ ]* *SYSTEM *"\([^"]*\)" *> *$$/\1/p' | \
                 sed -e 's/^\([^.]*\)$$/\1.*/p' )

%.pdf: %.xml $(XML_ENTITIES) $(XML_ENT_INCS)
	#echo "$(XML_ENTITIES) $(XML_ENT_INCS)"
	docbook2pdf $(DSL_DEF) $<

%.xml: ../../orte/include/orte/%.h
	$(KERNELDOC) -docbook $< >$@


clean:
	@rm -f  ${XML_BASENAMES:%=%.html}
	@rm -f  ${XML_BASENAMES:%=%.pdf}
	@rm -f ./*.log
	@rm -f ./*.out
	@rm -f ./*~
