all: plan.pdf

include font.mk

# PDFDEPS += $(ALL_FONTS)
PDFDEPS += ./theme.yml

ASCIIDOCTOR_ARGS += --backend pdf
ASCIIDOCTOR_ARGS += --attribute pdf-theme=./theme.yml
# ASCIIDOCTOR_ARGS += --attribute pdf-fontsdir=./res/font
%.pdf: %.adoc $(PDFDEPS)
	asciidoctor --require asciidoctor-pdf $(ASCIIDOCTOR_ARGS) $<

