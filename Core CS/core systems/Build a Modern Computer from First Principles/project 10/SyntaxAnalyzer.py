from JackTokenizer import JackTokenizer
from CompilationEngine import CompilationEngine

from sys import argv
import os

def analyzer(fname):
	jt = JackTokenizer(fname)
	with open('tokens'+fname[:-5]+'.txt', 'w') as f:
		f.write('\n'.join(jt.tokenize()))
	with open('my'+fname[:-5]+'.xml', 'w') as fp:
		ce = CompilationEngine(jt.tokenize(), fp)
		ce.compileClass()

if __name__ == '__main__':
	if argv[1][-5:] != '.jack':
		os.chdir(os.curdir+'\\'+argv[1])
		for fname in [x for x in os.listdir() if '.jack' in x]:
			analyzer(fname)
	else:
		fname = argv[1]
		analyzer(fname)
			
