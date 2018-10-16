
class SymbolTable(object):
	def __init__(self):
		self.table = {}
	
	def reset(self):
		self.table = {}
		
	def define(self, name, kind, type):
		index = self.varCount(kind)
		self.table[name] = [kind, type, index]
		
	def varCount(self, kind):
		count = 0
		for key in self.table:
			if self.kindOf(key) == kind:
				count += 1
		return str(count)
		
	def kindOf(self, name):
	# static, field, local, argument
		return self.table[name][0]
	
	def typeOf(self, name):
	# int, char, boolean, Class
		return self.table[name][1]
	
	def indexOf(self, name):
		return self.table[name][2]