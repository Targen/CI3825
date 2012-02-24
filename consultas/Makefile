pandoc := pandoc

pandocflags :=            \
        --normalize       \
        --number-sections \
        --preserve-tabs   \
        --section-divs    \
        --standalone

formats       := txt html pdf
clean-formats := aux log out toc

pandoc-latex-args := --latex-engine=xelatex -H header.tex

sources := $(wildcard *.md)

# ¿Cómo se automatiza esto? ☹
files := $(sources:%.md=%.txt) $(sources:%.md=%.html) $(sources:%.md=%.pdf)

pandocrule =                                                       \
        sed < $<                                                   \
                -e 's/^@\(@[^@]\+\)*@$(suffix $@)\(@[^@]\+\)*@@//' \
                -e '/^@\(@[^@]\+\)\+@@/d'                          \
        | $(pandoc) $(pandocflags) -o $@



.PHONY: all clean

all: $(files)

clean:
	rm -f $(clean-formats:%=*.%) $(files)



%.txt : %.md            ; $(pandocrule)
%.html: %.md            ; $(pandocrule)
%.pdf : %.md header.tex ; $(pandocrule) $(pandoc-latex-args)
%.tex : %.md header.tex ; $(pandocrule) $(pandoc-latex-args)
