
class VmWriter(object):
	def __init__(self, fp):
		self.fp = fp
		
	def writePush(self, segment, index):
		self.fp.write('push ' + segment + ' ' + index + '\n')
	
	def writePop(self, segment, index):
		self.fp.write('pop ' + segment + ' ' + index + '\n')
		
	def writeArithmetic(self, op):
		if op == '+':
			self.fp.write('add\n')
		elif op == '-':
			self.fp.write('sub\n')
		elif op == '*':
			self.fp.write('call Math.multiply 2\n')
		elif op == '/':
			self.fp.write('call Math.divide 2\n')
		elif op == '<':
			self.fp.write('lt\n')
		elif op == '>':
			self.fp.write('gt\n')
		elif op == '=':
			self.fp.write('eq\n')
		elif op == '&':
			self.fp.write('and\n')
		elif op == '|':
			self.fp.write('or\n')
	
	def writeUnArithmetic(self, op):
		if op == '-':
			self.fp.write('neg\n')
		elif op == '~':
			self.fp.write('not\n')
			
	def writeLabel(self, label):
		self.fp.write('label ' + label + '\n')
	
	def writeGoto(self, label):
		self.fp.write('goto ' + label + '\n')
	
	def writeIfGoto(self, label):
		self.fp.write('if-goto ' + label + '\n')
	
	def writeCall(self, name, nArgs):
		self.fp.write('call ' + name + ' ' + nArgs + '\n')
	
	def writeFunction(self, name, nLocals):
		self.fp.write('function ' + name + ' ' + nLocals + '\n')
	
	def writeReturn(self):
		self.fp.write('return\n')
		