all: plan.pdf
all: reqs.pdf
all: research.pdf
all: design.pdf

# include font.mk

# PDFDEPS += $(ALL_FONTS)
PDFDEPS += ./theme.yml

# uncomment to debug include errors
# ASCIIDOCTOR_ARGS += --trace

ASCIIDOCTOR_ARGS += --require asciidoctor-bibtex
ASCIIDOCTOR_ARGS += --require asciidoctor-pdf
# ASCIIDOCTOR_ARGS += --require ./plugin/helloworld
ASCIIDOCTOR_ARGS += --require asciidoctor-interdoc-reftext
ASCIIDOCTOR_ARGS += --require asciidoctor-lists
ASCIIDOCTOR_ARGS += --backend pdf
%.pdf: %.adoc $(PDFDEPS)
	bundle exec asciidoctor $(ASCIIDOCTOR_ARGS) $<

