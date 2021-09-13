all: main.pdf

TEXS := $(wildcard *.tex) $(wildcard */*.tex) $(wildcard */*/*.tex)

main.pdf: $(TEXS)
	pdflatex -shell-escape main.tex main.pdf || exit
	pdflatex -shell-escape -interact=nonstopmode > /dev/null main.tex || exit
	rm -f *.aux *.log *.toc *.out
	rm -rf _minted*
	rm main.bcf
	rm main.ptc
	rm main.idx
	rm main.run.xml
