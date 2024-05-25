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

## todo (low priority)

- cross-references to glossary

## for later reference

### get citation keys from .docx file

```
$ unar file.docx
$ grep -o 'CITATION[^<]\+' file/word/document.xml | awk '!x[$2]++ { i++; print "[" i "] " $2 }'
```

[rubydl]: https://www.ruby-lang.org/en/documentation/installation/
[asciidoc]: https://asciidoc.org/

