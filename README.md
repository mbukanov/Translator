Translator
==========

Simple translator using yandex api

install:
--------
$ ./install.sh

Usage:
------
translator [-i ru, en, ...] -o {ru,en, ...} -t TEXT | -f FILENAME  
-i        input language, else auto detect by 'text'  
-o                                   output language  
-t                input text in '-i <lang>' language  
-f                                text from filename  
Example: translator -o ru -t Hello		-> Привет  
Example: translator -i ru -o en -t Привет		-> Hello  
