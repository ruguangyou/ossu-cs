# VM code -> Hack Assembly language
# todo: implementing arithmetic and memory access instructions
# Ruguang 2018.2.21

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
			if c != ' ' and c != '\n':
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
		self.count =  1  #used to set symbol name
	
	def writeArithmetic(self, args):
		getOne = '@SP\n'+'A=M-1\n'
		getTwo = '@SP\n'+'AM=M-1\n'+'D=M\n'+'A=A-1\n'
		compareTwo = '@!\n'+'D;*\n'+'@0\n'+'D=A\n'+'@^\n'+'0;JMP\n'+'(!)\n'+'@1\n'+'D=-A\n'+'(^)\n'+'@SP\n'+'A=M-1\n'+'M=D\n'
		if args[0] == 'add':
			assemblyCode = getTwo+'M=M+D\n'
		elif args[0] == 'sub':
			assemblyCode = getTwo+'M=M-D\n'
		elif args[0] == 'neg':
			assemblyCode = getOne+'M=-M\n'
		elif args[0] == 'eq':
			assemblyCode = getTwo+'D=M-D\n'+compareTwo.replace('!','X'*self.count).replace('^','Y'*self.count).replace('*','JEQ')
			self.count += 1
		elif args[0] == 'gt':
			assemblyCode = getTwo+'D=M-D\n'+compareTwo.replace('!','X'*self.count).replace('^','Y'*self.count).replace('*','JGT')
			self.count += 1
		elif args[0] == 'lt':
			assemblyCode = getTwo+'D=M-D\n'+compareTwo.replace('!','X'*self.count).replace('^','Y'*self.count).replace('*','JLT')
			self.count += 1
		elif args[0] == 'and':
			assemblyCode = getTwo+'M=M&D\n'
		elif args[0] == 'or':
			assemblyCode = getTwo+'M=M|D\n'
		elif args[0] == 'not':
			assemblyCode = getOne+'M=!M\n'
		self.fp.write(assemblyCode)
		
	def writePushPop(self, args):
		if args[0] == 'pop':
			popOne = '@SP\n'+'AM=M-1\n'+'D=M\n'
			if args[1] == 'static':
				assemblyCode = popOne+'@'+self.fileName+'.'+args[2]+'\n'+'M=D\n'
			elif args[1] == 'pointer':
				# 0: THIS, 1: THAT
				if args[2] == '0':
					seg = 'THIS'
				else:
					seg = 'THAT'
				assemblyCode = popOne+'@'+seg+'\n'+'M=D\n'
			else:
				# temp i: 5+i, local/argument/this/that i: RAM[seg]+i
				if args[1] == 'temp':
					reg = 'A'
				else:
					reg = 'M'
				seg = self.segments[args[1]]
				assemblyCode = '@'+args[2]+'\n'+'D=A\n'+'@'+seg+'\n'+'D=D+'+reg+'\n'+'@R13\n'+'M=D\n'+popOne+'@R13\n'+'A=M\n'+'M=D\n'
		else:
			pushOne = '@SP\n'+'AM=M+1\n'+'A=A-1\n'+'M=D\n'
			if args[1] == 'constant':
				assemblyCode = '@'+args[2]+'\n'+'D=A\n'+pushOne
			elif args[1] == 'static':
				assemblyCode = '@'+self.fileName+'.'+args[2]+'\n'+'D=M\n'+pushOne
			elif args[1] == 'pointer':
				if args[2] == '0':
					seg = 'THIS'
				else:
					seg = 'THAT'
				assemblyCode = '@'+seg+'\n'+'D=M\n'+pushOne
			else:
				if args[1] == 'temp':
					reg = 'A'
				else:
					reg = 'M'
				seg = self.segments[args[1]]
				assemblyCode = '@'+args[2]+'\n'+'D=A\n'+'@'+seg+'\n'+'A=D+'+reg+'\n'+'D=M\n'+pushOne
		self.fp.write(assemblyCode)
		
	def writeCode(self, args, cType):
		if cType != 'white':
			self.fp.write('//' + ' '.join(args) + '\n')
		if cType == 'arithmetic':
			self.writeArithmetic(args)
		elif cType == 'pop_push':
			self.writePushPop(args)
			
	def closeFile(self):
		self.fp.close()
	

if __name__ == '__main__':
	from sys import argv
	if len(argv) != 2:
		print('Usage: python VMTranslator.py xxx.vm')
	else:
		with open(argv[1], 'r') as f_in:
			parser = Parser(f_in.readlines())
		
		writer = CodeWriter(argv[1][:-3])
		while parser.hasMoreCommand():
			(args, cType) = parser.parseOneLine()
			writer.writeCode(args, cType)
		writer.closeFile()		