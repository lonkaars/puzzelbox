all: plan.pdf
all: requirements.pdf

# include font.mk

# PDFDEPS += $(ALL_FONTS)
PDFDEPS += ./theme.yml

ASCIIDOCTOR_ARGS += --require asciidoctor-bibtex
ASCIIDOCTOR_ARGS += --require asciidoctor-pdf
# ASCIIDOCTOR_ARGS += --require ./plugin/helloworld
ASCIIDOCTOR_ARGS += --backend pdf
ASCIIDOCTOR_ARGS += --attribute pdf-theme=./theme.yml
# ASCIIDOCTOR_ARGS += --attribute pdf-fontsdir=./res/font
ASCIIDOCTOR_ARGS += --attribute bibtex-file=./share/refs.bib
%.pdf: %.adoc $(PDFDEPS)
	asciidoctor $(ASCIIDOCTOR_ARGS) $<

