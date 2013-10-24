#!/bin/bash

# Converte todos as imagens .png na pasta anterior e as armazena na pasta
# results.

for f in ../*.png
do
	perl bin2c.pl $f

	s1=${f%.*}
	s2=$(basename "$s1")

	mv $s1.h ./result/$s2-png.h
done
