# Makefile
# $Id: Makefile,v 1.1.1.1 2008/05/19 20:20:45 davidlw Exp $
#
# 10-11-05: Borrowed from Constantine
# 04-07-08: Borrowed from Corey 
#

FILENAME  = thesis

TEXDIR    = ./srcs
PICDIR    = ./img
TEXSRCS   = $(shell find $(TEXDIR) -maxdepth 1 -iname "*.tex") 
EPSPICS   = $(shell find $(PICDIR) -maxdepth 2 -iname "*.eps")
ARXPICS   = $(shell find $(PICDIR) -maxdepth 2 -iname "*.eps" -or -iname "*.ps")
PICSRCS   = $(shell find $(PICDIR) -maxdepth 2 -iname "*.eps" -or -iname "*.ps" \
              -or -iname "*.gif" -or -iname "*.jpg" -or -iname "*.png")
PDFPICS   = $(patsubst %.eps,%.pdf,$(EPSPICS))


ARCH      = $(shell uname -s)
PDFTEX    = ./sh/pdflatex.sh
LATEX	  = ./sh/latex.sh
BIBTEX	  = ./sh/bibtex.sh
MKINDEX   = ./sh/makeindex.sh
FEYNMF    = ./sh/feynman.sh
GZIPEPS   = ./sh/gzipeps.sh
PSVIEW    = ggv
PDFVIEW   = acroread
DVIVIEW   = xdvi
DVIPS	  = dvips
PS2PDF    = ps2pdf
PDF2PS    = pdf2ps
EPS2EPS   = eps2eps
EPSTOPDF  = epstopdf
THUMBPDF  = thumbpdf
EPSOPTION = 

ADDFILES  = $(FILENAME).tex $(FILENAME).bib Makefile ./sh/*.sh 

BACKFILES = $(PICSRCS) $(TEXSRCS) $(ADDFILES) 


all:	$(FILENAME).pdf

chaps:	touch
	cp chapters.tex .tmpchap
	ln -sf .tmpchap chaps.tex

chap1:	touch
	echo "\input{srcs/chapter1}" > .tmpchap
	ln -sf .tmpchap chaps.tex

chap2:	touch
	echo "\input{srcs/chapter2}" > .tmpchap
	ln -sf .tmpchap chaps.tex

chap3:	touch
	echo "\input{srcs/chapter3}" > .tmpchap
	ln -sf .tmpchap chaps.tex

chap4:	touch
	echo "\input{srcs/chapter4}" > .tmpchap
	ln -sf .tmpchap chaps.tex

chap5:	touch
	echo "\input{srcs/chapter5}" > .tmpchap
	ln -sf .tmpchap chaps.tex

chap6:	touch
	echo "\input{srcs/chapter6}" > .tmpchap
	ln -sf .tmpchap chaps.tex

chap7:	touch
	echo "\input{srcs/chapter7}" > .tmpchap
	ln -sf .tmpchap chaps.tex

recon:	touch
	echo "\input{srcs/reconChapter.tex}" > .tmpchap
	ln -sf .tmpchap chaps.tex

touch:
	touch $(FILENAME).tex


feyn:
	$(FEYNMF) $(FILENAME)
	touch $(FILENAME).tex

$(FILENAME):		$(FILENAME).tex $(TEXSRCS) $(EPSPICS) $(ADDFILES) .tmpchap
	$(LATEX) 	$(FILENAME)

$(FILENAME).pdf:	$(FILENAME).tex $(TEXSRCS) $(PDFPICS) $(ADDFILES) .tmpchap
	$(PDFTEX) 	$(FILENAME)
#	$(FEYNMF) 	$(FILENAME)
	$(BIBTEX) 	$(FILENAME)
	$(MKINDEX)	$(FILENAME)
	$(PDFTEX) 	$(FILENAME)
#	$(THUMBPDF)     $(FILENAME)
	$(PDFTEX) 	$(FILENAME)

$(FILENAME).dvi:	$(FILENAME).tex $(TEXSRCS) $(EPSPICS) $(ADDFILES) .tmpchap
	$(LATEX) 	$(FILENAME)
	$(BIBTEX) 	$(FILENAME)
	$(MKINDEX) 	$(FILENAME)
	$(LATEX) 	$(FILENAME)
	$(LATEX) 	$(FILENAME)

$(FILENAME).ps:		$(FILENAME).dvi
	$(DVIPS) 	$(FILENAME).dvi -o $(FILENAME).ps

ps: 	$(FILENAME).ps

gzip:	$(FILENAME).ps
	gzip $(FILENAME).ps

pdf: 	$(FILENAME).pdf

fpdf: 	
	$(PDFTEX) 	$(FILENAME)

ps2pdf:	$(FILENAME).dvi
	$(DVIPS) -Ppdf $(FILENAME).dvi -o $(FILENAME).ps
	$(PS2PDF) $(FILENAME).ps

pdf2ps: $(FILENAME).pdf
	$(PDF2PS) -ddLanguageLevel=2 $(FILENAME).pdf

vdvi:	$(FILENAME).dvi
	$(DVIVIEW) $(FILENAME).dvi

vps:	$(FILENAME).ps
	$(PSVIEW) $(FILENAME).ps

vpdf:	$(FILENAME).pdf
	$(PDFVIEW) $(FILENAME).pdf

print:  $(FILENAME).ps
	lpr $(FILENAME).ps

$(FILENAME).tar.gz:	$(BACKFILES)
	tar -cvzf $(FILENAME).tar.gz $(BACKFILES)

back:	$(FILENAME).tar.gz

arxiv:	
	tar -czf arxiv.tar.gz $(FILENAME).tex $(FILENAME).bbl  $(ARXPICS) $(TEXSRCS) 
	mv arxiv.tar.gz /tmp
	rm -rf /tmp/a.cl && mkdir -p /tmp/a.cl
	(cd /tmp/a.cl; tar -xzf ../arxiv.tar.gz)
	(cd /tmp/a.cl; mv arbeit.tex thesis-loizides.tex; mv arbeit.bbl thesis-loizides.bbl)
	(cd /tmp/a.cl; rm ./pics/chap3/InOutPlane.eps ./pics/chap3/cXYandL-AuAu.eps \
                           ./pics/chap3/cOmegaDist.eps ./pics/chap4/ALICE_layout.eps )
	$(GZIPEPS) /tmp/a.cl
	cp $(HOME)/daten/localtexmf/hyperref/hypernat.sty /tmp/a.cl
	cp $(HOME)/daten/localtexmf/acronym/acronym.sty /tmp/a.cl
	(cd /tmp/a.cl; tar -czf ../arxiv.tar.gz *)      
	(cd /tmp/; rm -rf a.cl)      

sbackup:	tar -cvzf $(FILENAME).tar.gz $(BACKFILES)
	scp -p $(FILENAME).tar.gz pcikf27.ikf.physik.uni-frankfurt.de:~/backup

dbackup: 
	tar -cvzf ./backup/$(FILENAME)-backup`./sh/dbackup.sh $(FILENAME) backup 5`.tar.gz $(BACKFILES)

sdbackup: dbackup
	scp -p ./backup/$(FILENAME)-backup`cat ./backup/.arbeit-count`.tar.gz pcikf27.ikf.physik.uni-frankfurt.de:~/backup

clean:
	rm -f $(FILENAME).dvi \
	$(FILENAME).toc $(FILENAME).aux $(FILENAME).out \
	$(FILENAME).bbl $(FILENAME).blg $(FILENAME).log \
	$(FILENAME).lof $(FILENAME).lot $(FILENAME).idx \
	$(FILENAME).ind $(FILENAME).ilg $(FILENAME).glo \
	$(FILENAME).blg $(FILENAME).brf \
        texput.log core .tmp.eps 

cleangen:
	rm -f $(FILENAME).toc \
        $(FILENAME).bbl $(FILENAME).blg $(FILENAME).lof \
        $(FILENAME).lot $(FILENAME).idx $(FILENAME).ind \
        $(FILENAME).ilg $(FILENAME).glo $(FILENAME).blg \
        $(FILENAME).brf $(FILENAME).aux $(FILENAME).log

realclean: clean
	rm -f $(FILENAME).ps $(FILENAME).pdf $(FILENAME).tar.gz \
	$(PDFPICS) fmffig* .tmpchap chaps.tex
	find . -type f -name "*~" -exec rm -f "{}" \;
	find . -type d -exec chmod 755 "{}" \;
	find . -type f -exec chmod 644 "{}" \;
	find . -name "*.sh" -type f -exec chmod 755 "{}" \;

.phony:	clean print backup sbackup dbackup sdbackup rpdf

$(PDFPICS): %.pdf: %.eps
ifeq ($(ARCH),Darwin)
	perl -i -pe 's/\015/\012/g' $<
endif
	$(EPSTOPDF) $(EPSOPTION) $< --outfile=$@
#	$(EPS2EPS) $< .tmp.eps && $(EPSTOPDF) $(EPSOPTION) .tmp.eps --outfile=$@ && rm -f .tmp.eps

