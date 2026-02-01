#!/bin/bash
# File watcher script for LaTeX files
# This script watches for .tex file changes and rebuilds PDFs automatically

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}Watching for .tex file changes...${NC}"
echo "Press Ctrl+C to stop"

# Watch for changes in .tex files and rebuild
fswatch -o --event=Updated --include='\.tex$' . | while read f; do
    # Find the changed .tex file
    CHANGED_FILE=$(fswatch -1 --include='\.tex$' . 2>/dev/null | head -1)
    
    if [ -n "$CHANGED_FILE" ] && [ -f "$CHANGED_FILE" ]; then
        echo -e "${GREEN}Building $CHANGED_FILE...${NC}"
        cd "$(dirname "$CHANGED_FILE")"
        latexmk -pdf -interaction=nonstopmode -file-line-error -synctex=1 "$(basename "$CHANGED_FILE")"
        echo -e "${GREEN}Done!${NC}\n"
    fi
done
