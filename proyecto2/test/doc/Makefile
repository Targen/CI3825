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

cases := depth0 depth1 permsdepth0 permsdepth1 empty emptydepth1 indirect

case-sources := $(cases:%=../case/%/README.md)

pandoc-latex-args := --latex-engine=xelatex -H header.tex

pandocrule = $(pandoc) $(pandocflags) -o $@ begin.md $(case-sources)



.PHONY: all clean

all: $(formats:%=README.%)

clean:
	rm -f $(formats:%=*.%) $(clean-formats:%=*.%)



README.txt : begin.md $(case-sources)            ; $(pandocrule)
README.html: begin.md $(case-sources)            ; $(pandocrule)
README.pdf : begin.md $(case-sources) header.tex ; $(pandocrule) $(pandoc-latex-args)
README.tex : begin.md $(case-sources) header.tex ; $(pandocrule) $(pandoc-latex-args)
