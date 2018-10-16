# VM code -> Hack Assembly language
# implementing arithmetic and memory access instructions
# todo: implementing branching and function instructions
# Ruguang 2018.2.26

class Parser(object):
	def __init__(self, lines):
		# store the lines in a list, initialize an index, command type lists
		self.lines = lines
		self.index = 0
		self.memoryAccess = ['push', 'pop']
		self.arithmetic = ['add', 'sub', 'neg', 'eq', 'gt', 'lt', 'and', 'or', 'not']
		self.branching = ['label', 'goto', 'if-goto']
		self.function = ['function', 'call', 'return']
		
	def hasMoreCommand(self):
		# compare the index with the length of the list, return ture if less than
		return self.index < len(self.lines)
	
	def nextCommand(self):
		# return next command
		line = self.lines[self.index]
		self.index += 1
		return line
		
	def commandParse(self, line):
		# ignore white space and comments
		# return the list of args, if there is no command just return an empty list
		args = []
		temp = ''
		for c in line:
			if c == '/':
				break
			if c not in [' ', '\n', '\t']:
				temp += c
			elif temp != '':
				args.append(temp)
				temp = ''
		return args
	
	def commandType(self, args):
		# 'arithmetic', 'push', 'pop', 'label', 'goto', 'if', 'function', 'return', 'call'
		if args == []:
			return 'white'
		if args[0] in self.memoryAccess:
			return 'pop_push'
		if args[0] in self.arithmetic:
			return 'arithmetic'
		if args[0] in self.branching:
			return 'branching'
		if args[0] in self.function:
			return 'function'
	
	def parseOneLine(self):
		args = self.commandParse(self.nextCommand())
		return (args, self.commandType(args))
			


class CodeWriter(object):
	def __init__(self, fileName):
		# set file pointer, fileName without extension, key words for some segments
		self.fp = open(fileName + '.asm', 'w')
		self.fileName = fileName
		self.segments = {'local':'LCL', 'argument':'ARG', 'this':'THIS', 'that':'THAT', 'temp':'5'}
		# used to set label name when implementing 'eq', 'lt', 'gt'
		self.cmpCount =  1 
		# used for writeArithmetic()
		self.getOne = '@SP\n'+'A=M-1\n'
		self.getTwo = '@SP\n'+'AM=M-1\n'+'D=M\n'+'A=A-1\n'
		# !!! if true set to 1 (not -1), false set to 0
		self.compareTwo = '@!\n'+'D;*\n'+'@0\n'+'D=A\n'+'@^\n'+'0;JMP\n'+'(!)\n'+'@1\n'+'D=A\n'+'(^)\n'+'@SP\n'+'A=M-1\n'+'M=D\n'
		# used for writePushPop()
		self.popOne = '@SP\n'+'AM=M-1\n'+'D=M\n'
		self.pushOne = '@SP\n'+'AM=M+1\n'+'A=A-1\n'+'M=D\n'
		# used to set the label of function return
		self.callCount = 1
		
	def writeArithmetic(self, args):
		if args[0] == 'add':
			self.writeAdd()
		elif args[0] == 'sub':
			self.writeSub()
		elif args[0] == 'neg':
			self.writeNeg()
		elif args[0] == 'eq':
			self.writeEq()
		elif args[0] == 'gt':
			self.writeGt()
		elif args[0] == 'lt':
			self.writeLt()
		elif args[0] == 'and':
			self.writeAnd()
		elif args[0] == 'or':
			self.writeOr() 
		elif args[0] == 'not':
			self.writeNot()
		
	def writeAdd(self):
		self.fp.write(self.getTwo+'M=M+D\n')
	def writeSub(self):
		self.fp.write(self.getTwo+'M=M-D\n')
	def writeNeg(self):
		self.fp.write(self.getOne+'M=-M\n')
	def writeEq(self):
		compareTwo = self.compareTwo.replace('!','X'*self.cmpCount).replace('^','Y'*self.cmpCount).replace('*','JEQ')
		self.fp.write(self.getTwo+'D=M-D\n'+compareTwo)
		self.cmpCount += 1
	def writeGt(self):
		compareTwo = self.compareTwo.replace('!','X'*self.cmpCount).replace('^','Y'*self.cmpCount).replace('*','JGT')
		self.fp.write(self.getTwo+'D=M-D\n'+compareTwo)
		self.cmpCount += 1
	def writeLt(self):
		compareTwo = self.compareTwo.replace('!','X'*self.cmpCount).replace('^','Y'*self.cmpCount).replace('*','JLT')
		self.fp.write(self.getTwo+'D=M-D\n'+compareTwo)
		self.cmpCount += 1
	def writeAnd(self):
		self.fp.write(self.getTwo+'M=M&D\n')
	def writeOr(self):
		self.fp.write(self.getTwo+'M=M|D\n')
	def writeNot(self):
		self.fp.write(self.getOne+'M=!M\n')
	
	def writePushPop(self, args):
		if args[0] == 'pop':
			if args[1] == 'static':
				assemblyCode = self.popOne+'@'+self.staticName+'.'+args[2]+'\n'+'M=D\n'
			elif args[1] == 'pointer':
				# 0: THIS, 1: THAT
				if args[2] == '0':
					seg = 'THIS'
				else:
					seg = 'THAT'
				assemblyCode = self.popOne+'@'+seg+'\n'+'M=D\n'
			else:
				# temp i: 5+i, local/argument/this/that i: RAM[seg]+i
				if args[1] == 'temp':
					reg = 'A'
				else:
					reg = 'M'
				seg = self.segments[args[1]]
				assemblyCode = '@'+args[2]+'\n'+'D=A\n'+'@'+seg+'\n'+'D=D+'+reg+'\n'+'@R13\n'+'M=D\n'+self.popOne+'@R13\n'+'A=M\n'+'M=D\n'
		else:
			if args[1] == 'constant':
				assemblyCode = '@'+args[2]+'\n'+'D=A\n'+self.pushOne
			elif args[1] == 'static':
				assemblyCode = '@'+self.staticName+'.'+args[2]+'\n'+'D=M\n'+self.pushOne
			elif args[1] == 'pointer':
				if args[2] == '0':
					seg = 'THIS'
				else:
					seg = 'THAT'
				assemblyCode = '@'+seg+'\n'+'D=M\n'+self.pushOne
			else:
				if args[1] == 'temp':
					reg = 'A'
				else:
					reg = 'M'
				seg = self.segments[args[1]]
				assemblyCode = '@'+args[2]+'\n'+'D=A\n'+'@'+seg+'\n'+'A=D+'+reg+'\n'+'D=M\n'+self.pushOne
		self.fp.write(assemblyCode)
	
	def writeBranch(self, args):
		if args[0] == 'label':
			self.writeLabel(args[1])
		elif args[0] == 'goto':
			self.writeGoto(args[1])
		elif args [0] == 'if-goto':
			self.writeIfgoto(args[1])
			
	def writeLabel(self, label):
		self.fp.write('('+label+')'+'\n')
	def writeGoto(self, label):
		self.fp.write('@'+label+'\n'+'0;JMP\n')
	def writeIfgoto(self, label):
		self.fp.write('@SP\n'+'AM=M-1\n'+'D=M\n'+'@'+label+'\n'+'D;JGT\n')
		
	def writeFunc(self, args):
		if args[0] == 'function':
			self.writeFunction(args[1], args[2])
		elif args[0] == 'call':
			self.writeCall(args[1], args[2])
		elif args[0] == 'return':
			self.writeReturn()
	
	def writeFunction(self, fName, numVars):
		self.writeLabel(fName)
		for i in range(int(numVars)):
			self.writePushPop(['push', 'constant', '0'])
	def writeCall(self, fName, numArgs):
		# store return address
		retAddr = fName+'$ret.'+str(self.callCount)
		self.fp.write('@'+retAddr+'\n'+'D=A\n'+self.pushOne)
		# store LCL
		self.pushReg('LCL')
		# store ARG
		self.pushReg('ARG')
		# store THIS
		self.pushReg('THIS')
		# store THAT
		self.pushReg('THAT')
		# set LCL
		self.fp.write('@SP\n'+'D=M\n'+'@LCL\n'+'M=D\n')
		# set ARG
		self.pushReg('SP')
		self.writePushPop(['push', 'constant', str(int(numArgs)+5)])
		self.writeSub()
		self.fp.write(self.popOne+'@ARG\n'+'M=D\n')
		# jump to callee
		self.writeGoto(fName)
		# set the return label
		self.writeLabel(retAddr)
		self.callCount += 1
	def pushReg(self, reg):
		self.fp.write('@'+reg+'\n'+'D=M\n'+self.pushOne)
	def writeReturn(self):
		# store SP in temperary variable
		self.fp.write('@ARG\n'+'D=M+1\n'+'@tempSP\n'+'M=D\n')
		# store return value in temperary variable
		# !!! if function call has 0 argument, 
		# !!!  ARG and retAddr has the same address,
		# !!!   it will overwrite the retAddr if write argument directly
		self.popReg('retValue')
		# restore THAT
		self.fp.write('@LCL\n'+'D=M\n'+'@SP\n'+'M=D\n')
		self.popReg('THAT')
		# restore THIS
		self.popReg('THIS')
		# restore ARG
		self.popReg('ARG')
		# restore LCL
		self.popReg('LCL')
		# restore return address
		self.popReg('retAddr')
		# restore SP
		self.fp.write('@tempSP\n'+'D=M\n'+'@SP\n'+'M=D\n')
		# store return value
		self.fp.write('@retValue\n'+'D=M\n'+'@SP\n'+'A=M-1\n'+'M=D\n')
		# goto retAddr
		self.fp.write('@retAddr\n'+'A=M\n'+'0;JMP\n')
	def popReg(self, reg):
		self.fp.write(self.popOne+'@'+reg+'\n'+'M=D\n')
		
	def writeCode(self, args, cType, vmFile):
		self.setStaticName(vmFile)
		if cType != 'white':
			self.fp.write('//' + ' '.join(args) + '\n')
		if cType == 'arithmetic':
			self.writeArithmetic(args)
		elif cType == 'pop_push':
			self.writePushPop(args)
		elif cType == 'branching':
			self.writeBranch(args)
		elif cType == 'function':
			self.writeFunc(args)
			
	def setStaticName(self, vmFile):
		self.staticName = self.fileName+'.'+vmFile[:-3]
			
	def closeFile(self):
		self.fp.close()
	

if __name__ == '__main__':
	from sys import argv
	import os	# deal with more than one .vm files
	if len(argv) != 2:
		print('Usage: python VMTranslator.py fileName/directoryName')
	else:
		if '.vm' in argv[1]:	# fileName
			with open(argv[1], 'r') as f_in:
				parserList = [Parser(f_in.readlines())]
			fileName = argv[1][:-3]
		else:				# directoryName
			os.chdir(os.curdir+'\\'+argv[1])
			fileList = os.listdir()
			# assume Sys.vm is in the directory
			with open('Sys.vm', 'r') as f_in:
				parserList = [Parser(f_in.readlines())]
				vmFiles = ['Sys.vm']
			for vm in [vm for vm in fileList if '.vm' in vm and vm != 'Sys.vm']:
				with open(vm, 'r') as f_in:
					parserList.append(Parser(f_in.readlines()))
					vmFiles.append(vm)
			fileName = argv[1]
			
		writer = CodeWriter(fileName)
		for i in range(len(parserList)):
			while parserList[i].hasMoreCommand():
				(args, cType) = parserList[i].parseOneLine()
				writer.writeCode(args, cType, vmFiles[i])
		writer.closeFile()		