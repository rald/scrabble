SRC= CompDict.java Const.java WordList.java
CLASSES=$(SRC:%.java=%.class)
#JAVA=guavac -classpath .:/usr/local/java/lib/classes.zip
JAVA=javac -g

include ../Makefile.java

dict.trie:	dict CompDict.class
	java -mx15m CompDict dict dict.trie
