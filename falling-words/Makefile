# Makefile for Assignment2
# david

JAVAC = javac
.SUFFIXES: .java .class
SRCDIR = src
BINDIR = bin
TESTDIR = testing
OUTPUTDIR = out

$(BINDIR)/%.class:$(SRCDIR)/%.java
	$(JAVAC) -d $(BINDIR)/ -cp $(BINDIR) $<

CLASSES = Score.class WordDictionary.class WordRecord.class HighScore.class WordApp.class WordManager.class WordPanel.class  
CLASS_FILES = $(CLASSES:%.class=$(BINDIR)/%.class)

default: $(CLASS_FILES)

clean:
	 rm $(BINDIR)/*.class

run: $(CLASS_FILES)
	java -cp $(BINDIR) WordApp $(totalWords) $(noWords) $(dict)

