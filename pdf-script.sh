#!/usr/bin/zsh

echo "creating pdf for file: $1";
vim -e -c "set printoptions=number:y" -c "let &printheader=\"Name: Milo Griffiths | Candidate: 5057 | Centre: 63261 | %f\"" -c "hardcopy > %.ps" -c q $1 > /dev/null
ps2pdf $1.ps
rm $1.ps
