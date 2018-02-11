
# This is a program that translate Hack Assembly Language to binary machine codes.
# To do this, several parts of work have to be done:
#   1. Read .asm file using the file name from command-line 
#   2. Recognize wihte space, including comments (begin with //) and white line, and ignore them
#   3. Translate instruction to corresponding binary code
#       A-instruction  @values
#       C-instruction  dest=comp;jump
#   4. Deal with symbols, store the symbol name and value in a table
#   5. write into .hack output file

# Ruguang 2018/1/31


class Assembler(object):
	def __init__(self, instructions):
		self.symbols = {'R0':'0', 'R1':'1', 'R2':'2', 'R3':'3', 'R4':'4', 'R5':'5', 'R6':'6', 
						'R7':'7', 'R8':'8', 'R9':'9', 'R10':'10', 'R11':'11', 'R12':'12', 
						'R13':'13', 'R14':'14', 'R15':'15', 'SP':'0', 'LCL':'1', 'ARG':'2', 
						'THIS':'3', 'THAT':'4', 'SCREEN':'16384', 'KBD':'24576'}     # symbol table
		self.pc = 0                 # program counter
		self.addr = 16           # from where to store variable in RAM
		self.instructions = instructions
		self.lib_comp = {'0':'0101010', '1':'0111111', '-1':'0111010', 'D':'0001100', 'A':'0110000',
						'!D':'0001101', '!A':'0110001', '-D':'0001111', '-A':'0110011', 'D+1':'0011111', 
						'A+1':'0110111', 'D-1':'0001110', 'A-1':'0110010', 'D+A':'0000010', 
						'D-A':'0010011', 'A-D':'0000111', 'D&A':'0000000', 'D|A':'0010101', 
						'M':'1110000', '!M':'1110001', '-M':'1110011', 'M+1':'1110111', 'M-1':'1110010', 
						'D+M':'1000010', 'D-M':'1010011', 'M-D':'1000111', 'D&M':'1000000', 'D|M':'1010101'}
		self.lib_dest = {'':'000', 'M':'001', 'D':'010', 'MD':'011', 'A':'100', 
						'AM':'101', 'AD':'110', 'AMD':'111'}
		self.lib_jump = {'':'000', 'JGT':'001', 'JEQ':'010', 'JGE':'011', 'JLT':'100',
						'JNE':'101', 'JLE':'110', 'JMP':'111'}
        
	def trans(self):
		binary = []
		self.first_pass()
		self.pc = 0
		for i in self.instructions:
			if len(i.split()) == 0:		# white line
				continue
			i = i.split()[0]
			if i[0] == '/' or i[0] == '(':	# comment or location symbol
				continue
			ins = self.parser(i)  		# parsed instruction
			binary.append(self.a2b(ins))
			self.pc += 1	
		return '\n'.join(binary)
	
	def first_pass(self):
		self.pc = 0
		for i in self.instructions:
			if len(i.split()) == 0:	# white line
				continue
			i = i.split()[0]
			if i[0:2] == '//':		# comment
				continue
			if i[0] == '(':
				sym = i.split(')')[0][1:]
				if sym not in self.symbols:
					self.symbols[sym] = str(self.pc)
					continue
			self.pc += 1
	
	def parser(self, i):
	# take a line as input, produce value if A instruction, [dest,comp,jump] if C instruction
		i = i.split('//')[0]
		if i[0] == '@':  # A-instruction
			if i[1:].isnumeric():
				return i[1:]
			else:
				sym = i[1:]
				if sym not in self.symbols:
					self.symbols[sym] = str(self.addr)
					self.addr += 1
				return self.symbols[sym]
		else:
			if '=' in i:
				dest = i.split('=')[0]
				i = ''.join(i.split('='))
			else:
				dest = ''
			if ';' in i:
				jump = i.split(';')[-1]
				i = ''.join(i.split(';'))
			else:
				jump = ''
			comp = i[len(dest):len(i)-len(jump)]
			return [comp, dest, jump]
	
	def a2b(self, ins):
		if type(ins) == type(''):  # A-instruction '0[value]'
			b = bin(int(ins))[2:]   # bin(n) -> '0bxxxx'
			return ('0' * (16 - len(b)) + b)
		else:   # C-instruction  '111[comp][dest][jump]'
			return ('111' + self.lib_comp[ins[0]] + self.lib_dest[ins[1]] + self.lib_jump[ins[2]])

if __name__ == '__main__':
	from sys import argv
	if len(argv) != 2:
		print('usage: python HackAssembler xxx.asm')
	else:
		with open(argv[1], 'r') as fin:
			instructions = fin.readlines()
		asm = Assembler(instructions)
		binary = asm.trans()
		with open((argv[1][:-4]+'.hack'), 'w') as fout:
			fout.write(binary)