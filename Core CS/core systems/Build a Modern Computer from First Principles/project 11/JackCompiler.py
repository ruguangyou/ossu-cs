
from JackTokenizer import JackTokenizer
from CompilationEngine import CompilationEngine

from sys import argv
import os

from JackTokenizer import JackTokenizer

if __name__ == '__main__':
	jt = JackTokenizer()
	ce = CompilationEngine()
	if argv[1][-5:] != '.jack':
		os.chdir(os.curdir+'\\'+argv[1])
		for fname in [x for x in os.listdir() if '.jack' in x]:
			with open(fname[:-4]+'vm', 'w') as fp:
				tokens = jt.tokenize(fname)
				ce.run(tokens, fp)
	else:
		fname = argv[1]
		with open(fname[:-4]+'vm', 'w') as fp:
			ce.run(jt.tokenize(fname), fp)
			
