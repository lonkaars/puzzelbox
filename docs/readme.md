# docs

This folder contains the project documentation in [AsciiDoc][asciidoc] format.
To compile the documents, make sure you have [Ruby][rubydl] and the
dependencies installed by running the `bundle` command in this folder. A
makefile is provided for convenience.

## notes for writers

- "I<sup>2</sup>C" is written as `I^2^C`
- "C++" is written as `{cpp}`
- please give cross references, links, image files, figure ids, etc. meaningful
  names

## TODO

- transfer documents
    - [x] project plan
    - [x] requirements
    - [x] research
        - [ ] footnotes
    - [ ] software design
        - [ ] footnotes
- xrefs for--
    - [x] tables
    - [x] figures
    - [x] section numbers (headings)
    - [x] requirements
- [x] figures
- [x] citations
- [ ] table of tables
- [ ] table of figures
- [ ] glossary (w/ cross-references)
- [ ] PDF style
- [ ] give used requirements more meaningful id's

## for later reference

### get citation keys from .docx file

```
$ unar file.docx
$ grep -o 'CITATION[^<]\+' file/word/document.xml | awk '!x[$2]++ { i++; print "[" i "] " $2 }'
```

[rubydl]: https://www.ruby-lang.org/en/documentation/installation/
[asciidoc]: https://asciidoc.org/

