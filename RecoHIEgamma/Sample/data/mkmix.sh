head -7 mix.cfi >mymix.cfi
ls $1* |replace '/pnfs' '          "dcache:///pnfs'|replace '.root' '.root",' >>mymix.cfi
tail -15 mix.cfi >>mymix.cfi
