
# This is a program that translate Hack Assembly Language to binary machine codes.
# Rewrite to achieve better construction.

# Ruguang 2018/2/3

class HackAssembler(object):
	def __init__(self, lines):
		self.lines = lines
		self.dest_table = {'':'000', 'M':'001', 'D':'010', 'MD':'011', 
						'A':'100', 'AM':'101', 'AD':'110', 'AMD':'111'}
		self.comp_table = {'0':'0101010', '1':'0111111', '-1':'0111010', 'D':'0001100', 'A':'0110000',
						    '!D':'0001101', '!A':'0110001', '-D':'0001111', '-A':'0110011', 'D+1':'0011111', 
						    'A+1':'0110111', 'D-1':'0001110', 'A-1':'0110010', 'D+A':'0000010', 
						    'D-A':'0010011', 'A-D':'0000111', 'D&A':'0000000', 'D|A':'0010101', 
						    'M':'1110000', '!M':'1110001', '-M':'1110011', 'M+1':'1110111', 'M-1':'1110010', 
						    'D+M':'1000010', 'D-M':'1010011', 'M-D':'1000111', 'D&M':'1000000', 'D|M':'1010101'}
		self.jump_table = {'':'000', 'JGT':'001', 'JEQ':'010', 'JGE':'011',
						    'JLT':'100', 'JNE':'101', 'JLE':'110', 'JMP':'111'}
		self.pc = 0	# program counter
		self.symbol_table = {'R0':'0', 'R1':'1', 'R2':'2', 'R3':'3', 'R4':'4', 'R5':'5', 'R6':'6', 
							'R7':'7', 'R8':'8', 'R9':'9', 'R10':'10', 'R11':'11', 'R12':'12', 
							'R13':'13', 'R14':'14', 'R15':'15', 'SP':'0', 'LCL':'1', 'ARG':'2', 
							'THIS':'3', 'THAT':'4', 'SCREEN':'16384', 'KBD':'24576'} 
		self.address = 16	# starting address in RAM that could be used to store variable
		
	def clear_ws(self):
	# change self.lines
	# remove white space, including comments, white lines and white space
		new_lines = []
		for line in self.lines:
			new_line = ''
			for c in line:
				if c == ' ' or c == '\t' or c == '\n':
				# neglect white space
					c = ''
					continue
				elif c == '/':
				# skip comment, which starts with //
					break
				else:
					new_line += c
			if new_line != '':
			# if "break" happened in the for loop, use this to check 
			#		whether the comment is behind the instruction
				new_lines.append(new_line)
		self.lines = new_lines
		return None
	
	def first_pass(self):
	# change self.lines
	# deal with the symbolic instruction locations (stored in symbol_table), then remove them from lines
	# e.g.	(LOOP)	(END)
		self.pc = 0
		new_lines = []
		for line in self.lines:
			if line[0] == '(':
				goto = line[1:-1]
				if goto not in self.symbol_table:
					self.symbol_table[goto] = str(self.pc)
				continue
			new_lines.append(line)
			self.pc += 1
		self.lines = new_lines
		return None
	
	def parse(self):
	# change self.lines 
	# A-instruction -- string (value); C-instruction -- list ([comp, dest, jump])
		new_lines = []
		for line in self.lines:
			if line[0] == '@':
			# A-instruction, @value or @symbol
				if line[1:].isnumeric():
					 new_line = line[1:]
				else:
					if line[1:] not in self.symbol_table:
					# variable, should be stored in RAM, starting from address 16
						self.symbol_table[line[1:]] = str(self.address)
						self.address += 1
					new_line = self.symbol_table[line[1:]]
			else:
			# C-instruction, dest=comp;jump
				if '=' in line:
					dest = line[:line.index('=')]
					line = line[line.index('=')+1:]
				else:
					dest = ''
				if ';' in line:
					jump = line[line.index(';')+1:]
					line = line[:line.index(';')]
				else:
					jump = ''
				comp = line
				new_line =  [comp, dest, jump]
			new_lines.append(new_line)
		self.lines = new_lines
		return None
	
	def assemble(self):
		new_lines = []
		for line in self.lines:
			if type(line) == type(''):
			# A-instruction
				new_line = '0' + self.to_binary(line)
			else:
				new_line = '111' + self.comp_table[line[0]] + self.dest_table[line[1]] + self.jump_table[line[2]]
			new_lines.append(new_line)
		self.lines = new_lines
		return None
		
	def to_binary(self, value):
	# produce 15-bit binary value	
		value = int(value)
		result = ''
		while value != 0:
			result = str(value%2) + result
			value = value // 2
		return ('0'*(15-len(result)) + result)
	
	def get_lines(self):
		return self.lines
		
		
if __name__ == '__main__':
	from sys import argv
	if len(argv) != 2:
		print('Usage: python HackAssembler_v1.1 xxx.asm')
	else:
		with open(argv[1], 'r') as f_in:
			lines = f_in.readlines()
		Hack = HackAssembler(lines)
		Hack.clear_ws()
		Hack.first_pass()
		Hack.parse()
		Hack.assemble()
		new_lines = '\n'.join(Hack.get_lines())
		with open(argv[1][:-4]+'.hack', 'w') as f_out:
			f_out.write(new_lines)
			