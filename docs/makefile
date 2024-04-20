all: plan.pdf
all: requirements.pdf
all: research.pdf

# include font.mk

# PDFDEPS += $(ALL_FONTS)
PDFDEPS += ./theme.yml

# uncomment to debug include errors
# ASCIIDOCTOR_ARGS += --trace

ASCIIDOCTOR_ARGS += --require asciidoctor-bibtex
ASCIIDOCTOR_ARGS += --require asciidoctor-pdf
# ASCIIDOCTOR_ARGS += --require ./plugin/helloworld
ASCIIDOCTOR_ARGS += --require asciidoctor-interdoc-reftext
ASCIIDOCTOR_ARGS += --backend pdf
ASCIIDOCTOR_ARGS += --attribute pdf-theme=./theme.yml
# ASCIIDOCTOR_ARGS += --attribute pdf-fontsdir=./res/font
ASCIIDOCTOR_ARGS += --attribute bibtex-file=./share/refs.bib
%.pdf: %.adoc $(PDFDEPS)
	bundle exec asciidoctor $(ASCIIDOCTOR_ARGS) $<

