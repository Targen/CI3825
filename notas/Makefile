.PHONY: all

all: notas.pdf

clean:
	rm -f ./*.log ./*.aux ./*.pdf

%.pdf: %.tex
	xelatex $<
