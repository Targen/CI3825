pandoc = pandoc

pandocflags =               \
        --normalize         \
        --preserve-tabs     \
        --table-of-contents \
        --number-sections   \
        --section-divs      \
        --standalone

pandocrule = $(pandoc) $(pandocflags) -o $@ $<

files = $(formats:%=proyecto2.%)

formats = txt html pdf



.PHONY: all clean

all: $(files)

clean:
	rm -f $(files)

%.txt : %.md ; $(pandocrule)
%.html: %.md ; $(pandocrule)
%.pdf : %.md ; $(pandocrule) --latex-engine=xelatex -H tocname.tex
