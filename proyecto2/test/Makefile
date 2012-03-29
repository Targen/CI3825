pandoc := pandoc

pandocflags :=            \
        --toc             \
        --normalize       \
        --number-sections \
        --preserve-tabs   \
        --section-divs    \
        --standalone

formats       := txt html pdf
clean-formats := aux log out toc

pandoc-latex-args := --latex-engine=xelatex -H header.tex

pandocrule = $(pandoc) $(pandocflags) -o $@ begin.md */*.md



.PHONY: all clean

all: $(formats:%=README.%)

clean:
	rm -f $(formats:%=*.%) $(clean-formats:%=*.%)



README.txt : begin.md */*.md            ; $(pandocrule)
README.html: begin.md */*.md            ; $(pandocrule)
README.pdf : begin.md */*.md header.tex ; $(pandocrule) $(pandoc-latex-args)
README.tex : begin.md */*.md header.tex ; $(pandocrule) $(pandoc-latex-args)
