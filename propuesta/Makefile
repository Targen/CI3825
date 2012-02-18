pandoc      = pandoc
pandocflags =               \
        --normalize         \
        --preserve-tabs     \
        --table-of-contents \
        --number-sections   \
        --section-divs      \
        --standalone

all: proyecto2.pdf proyecto2.html proyecto2.txt

%.pdf : %.md ; $(pandoc) $(pandocflags) --latex-engine=xelatex -H tocname.tex -o $@ $<
%.txt : %.md ; $(pandoc) $(pandocflags) -o $@ $<
%.html: %.md ; $(pandoc) $(pandocflags) -o $@ $<
