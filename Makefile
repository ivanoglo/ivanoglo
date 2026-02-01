# Makefile for building LaTeX documents to PDF

# Find all .tex files
TEX_FILES := $(shell find . -name "*.tex" -not -path "./cv/*" -not -path "./.git/*")

# Convert .tex to .pdf targets
PDF_FILES := $(TEX_FILES:.tex=.pdf)

# Default target: build all PDFs
all: $(PDF_FILES)

# Build a specific PDF from a .tex file
%.pdf: %.tex
	@echo "Building $<..."
	@cd $(dir $<) && latexmk -pdf -interaction=nonstopmode -file-line-error -synctex=1 $(notdir $<)
	@echo "Built $@"

# Clean auxiliary files
clean:
	@echo "Cleaning LaTeX auxiliary files..."
	@find . -name "*.aux" -delete
	@find . -name "*.log" -delete
	@find . -name "*.out" -delete
	@find . -name "*.synctex.gz" -delete
	@find . -name "*.fls" -delete
	@find . -name "*.fdb_latexmk" -delete
	@find . -name "*.bbl" -delete
	@find . -name "*.blg" -delete
	@echo "Clean complete"

# Clean all including PDFs
clean-all: clean
	@echo "Removing PDF files..."
	@find . -name "*.pdf" -not -path "./cv/*" -delete
	@echo "Clean-all complete"

.PHONY: all clean clean-all
