
# CompilationEngine: gets its input from a JackTokenizer and emits its
#					 parsed structure into a output file.


from SymbolTable import SymbolTable
from VmWriter import VmWriter

class CompilationEngine(object):
	def __init__(self):
		self.classTable = SymbolTable()
		self.subroutineTable = SymbolTable()
		
	def run(self, tokens, fp):
		self.index = -1
		self.whileCount = 0
		self.ifCount = 0
		self.tokens = tokens
		self.methodList()
		self.writer = VmWriter(fp)
		self.compileClass()
	
	def methodList(self):
		self.methods = []
		for i in range(len(self.tokens)):
			if self.tokens[i][0] == 'method':
				self.methods.append(self.tokens[i+2][0])
	
	def compileClass(self):
		self.classTable.reset()
		self.className = self.tokens[1][0]
		self.index += 4
		# class className { @here
		while self.tokens[self.index][0] in ['static', 'field']:
			self.compileClassVarDec()
			# ... name , name ;(@here)
			self.index += 1
			# ... name , name ; @here
		while '}' not in self.tokens[self.index]:
			self.compileSubroutine()
			# subroutine @here
	
	def compileClassVarDec(self):
		# class className { @here
		kind = self.tokens[self.index][0]
		type = self.tokens[self.index + 1][0]
		name = self.tokens[self.index + 2][0]
		self.classTable.define(name, kind, type)
		self.index += 3
		# kind type name @here
		while ',' in self.tokens[self.index]:
			# ... name ,(@here) name
			name = self.tokens[self.index + 1][0]
			self.classTable.define(name, kind, type)
			self.index += 2
			# ... name , name @here
	
	def compileSubroutine(self):
		self.subroutineTable.reset()
		# subroutine(@here) type subName
		kind = self.tokens[self.index][0] 
		subroutineName = self.tokens[self.index + 2][0]
		if kind == 'method':
			self.subroutineTable.define('this', 'argument', '0')
		self.index += 4
		# subroutine type subName ( @here
		self.compileParameterList()
		
		self.index += 1
		# subroutine tyoe subName ( ... ) { @here
		while 'var' in self.tokens[self.index]:
			self.compileVarDec()
			# ... name, name ;(@here)
			self.index += 1
		# ... name, name ; @here	
		self.writer.writeFunction(self.className + '.' + subroutineName, self.subroutineTable.varCount('local'))
		if kind == 'constructor':
			self.writer.writePush('constant', str(len(self.classTable.table)))
			self.writer.writeCall('Memory.alloc', '1')
			self.writer.writePop('pointer', '0')
		elif kind == 'method':
			self.writer.writePush('argument', '0')
			self.writer.writePop('pointer', '0')
		self.compileStatements()
		# subroutine @here
		
	def compileParameterList(self):
		# subroutine type subName ( @here
		while ')' not in self.tokens[self.index]:
			if ',' in self.tokens[self.index]:
				# type argName ,(@here) type argName ...
				self.index += 1
				# type argName , type(@here) argName ...
			# type(@here) argName
			kind = 'argument'
			type = self.tokens[self.index][0]
			name = self.tokens[self.index + 1][0]
			self.subroutineTable.define(name, kind, type)
			self.index += 2
			# ( type argName @here
		self.index += 1
		# ... , type argName ) @here
		
	def compileVarDec(self):
		# var(@here) type name
		kind = 'local'
		type = self.tokens[self.index + 1][0]
		name = self.tokens[self.index + 2][0]
		self.subroutineTable.define(name, kind, type)
		self.index += 3
		# var type name @here
		while ',' in self.tokens[self.index]:
			# ... name ,(@here) name
			name = self.tokens[self.index + 1][0]
			self.subroutineTable.define(name, kind, type)
			self.index += 2
			# ... name , name @here
		# ... name, name ;(@here)
	
	def compileStatements(self):
		# let|do|while|if|return|}(@here)
		temp = self.tokens[self.index]
		while '}' not in temp:
			if 'do' in temp:
				self.compileDo()
			elif 'let' in temp:
				self.compileLet()
			elif 'while' in temp:
				self.compileWhile()
			elif 'if' in temp:
				self.compileIf()
			elif 'return' in temp:
				self.compileReturn()
			# statement ; @here
			temp = self.tokens[self.index]
		# statements }(@here)
		self.index += 1
		# statements } @here
	
	def compileDo(self):
		# do(@here) subName|className
		callName = self.tokens[self.index + 1][0]
		self.index += 2
		# do subName|className @here
		nArgs = 0
		if '.' in self.tokens[self.index]:
			className = callName
			if self.searchSymbolTable(className) != None:
				(segment, index, type) = self.searchSymbolTable(className)
				callName = type + '.'+self.tokens[self.index + 1][0]
				self.writer.writePush(segment, index)
				nArgs += 1
			else:
				callName = className + '.' + self.tokens[self.index + 1][0]
			# do className .(@here) subName
			self.index += 3
			# do className . subName ( @here
		else:
			subroutineName = callName
			callName = self.className + '.' + subroutineName
			# do subName ((@here)
			self.index += 1
			# do subName ( @here
			if subroutineName in self.methods:
				self.writer.writePush('pointer', '0')
				nArgs += 1
		nArgs = str(nArgs + int(self.compileExpressionList()))
		# doStatement ;(@here)
		self.writer.writeCall(callName, nArgs)
		self.writer.writePop('temp', '0')	# void type subroutine will return 0, discard this dummy value
		self.index += 1
		# doStatement ; @here
	
	def compileLet(self):
		# let(@here) arrayName|varName
		varName = self.tokens[self.index + 1][0]
		(segment, index, type) = self.searchSymbolTable(varName)
		
		self.index += 2
		# let arrayName|varName @here
		if '[' in self.tokens[self.index]:
			self.writer.writePush(segment, index)
			# let arrayName [(@here)
			self.index += 1
			# let arrayName [ @here
			self.compileExpression()
			self.writer.writeArithmetic('+')
			# let arrayName [ expression ](@here)
			self.index += 2
			# let arrayName [ expression ] = @here
			self.compileExpression()
			# let arrayName [ expression ] = expression ;(@here)
			self.writer.writePop('temp', '0')
			self.writer.writePop('pointer', '1')
			self.writer.writePush('temp', '0')
			segment = 'that'
			index = '0'
		else:
			# let varName ... =(@here)
			self.index += 1
			# let varName ... = @here
			self.compileExpression()
			# let varName [ expression ] = expression ;(@here)
		
		self.writer.writePop(segment, index)
		self.index += 1
		# letStatement ; @here
		
	def compileWhile(self):
		count = str(self.whileCount)
		self.whileCount += 1
		# while(@here) ( expression )
		self.index += 2
		# while ( expression(@here)
		self.writer.writeLabel('whileExp' + count)
		self.compileExpression()
		# while ( expression )(@here)
		self.writer.writeUnArithmetic('~')
		self.writer.writeIfGoto('whileEnd' + count)
		self.index += 2
		# while ( expression ) { @here
		self.compileStatements()
		# { statements } @here
		self.writer.writeGoto('whileExp' + count)
		self.writer.writeLabel('whileEnd' + count)
	
	def compileIf(self):
		count = str(self.ifCount)
		self.ifCount += 1
		# if(@here) ( expression )
		self.index += 2
		# if ( expression(@here)
		self.compileExpression()
		# if ( expression )(@here)
		self.writer.writeUnArithmetic('~')
		self.writer.writeIfGoto('ifFalse' + count)
		self.index += 2
		# if ( expression ) { @here
		self.compileStatements()
		# { statement } @here
		self.writer.writeGoto('ifEnd' + count)
		self.writer.writeLabel('ifFalse' + count)
		if 'else' in self.tokens[self.index]:
			# if ( expression ) { statements } else(@here)
			self.index += 2
			# else { @here
			self.compileStatements()
			# { statements } @here
		self.writer.writeLabel('ifEnd' + count)
		
	def compileReturn(self):
		# return(@here)
		self.index += 1
		# return @here
		if ';' not in self.tokens[self.index]:
			# return expression(@here)
			self.compileExpression()
			# return expression ;(@here)
		else:
			# return ;(@here)
			self.writer.writePush('constant', '0')	# void type subroutine, return 0
		self.index += 1
		# return ... ; @here
		self.writer.writeReturn()
	
	def compileExpression(self):
		# ... expression(@here)
		self.compileTerm()
		if self.tokens[self.index][0] in ['+', '-', '*', '/', '&', '<', '>', '|', '=']:
			# term op(@here) term
			op = self.tokens[self.index][0]
			self.index += 1
			# term op term(@here)
			self.compileTerm()
			self.writer.writeArithmetic(op)
		# ... term @here
	
	def compileTerm(self):
		# term(@here)
		temp = self.tokens[self.index]
		if 'identifier' in temp:
			# arrayName|subName|className|varName(@here)
			name = self.tokens[self.index][0]
			self.index += 1
			# arrayName|subName|className|varName @here
			temp = self.tokens[self.index]
			if '[' in temp:
				arrayName = name
				(segment, index, type) = self.searchSymbolTable(arrayName)
				self.writer.writePush(segment, index)
				# arrayName [(@here)
				self.index += 1
				# arrayName [ @here
				self.compileExpression()
				# arrayName [ expression ](@here)
				self.index += 1
				# arrayName [ expression ] @here
				self.writer.writeArithmetic('+')
				self.writer.writePop('pointer', '1')
				self.writer.writePush('that', '0')
				
			elif '(' in temp:
				subName = name
				# subName ((@here)
				self.index += 1
				# subName ( @here
				nArgs = self.compileExpressionList()
				# subName ( expressions ) @here
				self.writer.writeCall(subName, nArgs)
				
			elif '.' in temp:
				className = name
				nArgs = 0
				if self.searchSymbolTable(className) != None:
					(segment, index, type) = self.searchSymbolTable(className)
					name = type + '.' + self.tokens[self.index + 1][0]
					self.writer.writePush(segment, index)
					nArgs += 1
				else:
					name = className + '.' + self.tokens[self.index + 1][0]
				# className .(@here) subName
				self.index += 3
				# className . subName ( @here
				nArgs = str(nArgs + int(self.compileExpressionList()))
				# className . subName ( expressions ) @here
				self.writer.writeCall(name, nArgs)
				
			else:
				varName = name
				# varName @here
				(segment, index, type) = self.searchSymbolTable(varName)
				self.writer.writePush(segment, index)
				
		elif '(' in temp:
			# ((@here) expression
			self.index += 1
			# ( expression(@here)
			self.compileExpression()
			# ( term op term )(@here)
			self.index += 1
			# ( expression ) @here
			
		elif 'integerConstant' in temp:
			# intConst(@here)
			self.writer.writePush('constant', self.tokens[self.index][0])
			self.index += 1
			# intConst @here
			
		elif 'stringConstant' in temp:
			# strConst(@here)
			strConst = self.tokens[self.index][0]
			self.writer.writePush('constant', str(len(strConst)))
			self.writer.writeCall('String.new', '1')
			for  c in strConst:
				self.writer.writePush('constant', str(ord(c)))
				self.writer.writeCall('String.appendChar', '2')
			self.index += 1
			# strConst @here
			
		elif 'keyword' in temp:
			# keywordConst(@here)
			if temp[0] == 'false' or temp[0] == 'null':
				self.writer.writePush('constant', '0')
			elif temp[0] == 'true':
				self.writer.writePush('constant', '1')
				self.writer.writeUnArithmetic('-')
			elif temp[0] == 'this':
				self.writer.writePush('pointer', '0')
			self.index += 1
			# keywordConst @here
		
		elif temp[0] in ['-', '~']:
			# -|~(@here) term
			self.index += 1
			# -|~ term(@here)
			self.compileTerm()
			self.writer.writeUnArithmetic(temp[0])
	
	def compileExpressionList(self):
		count = 0
		# ... ( @here
		while ')' not in self.tokens[self.index]:
			# ... ( expression(@here) 
			if ',' in self.tokens[self.index]:
				# ( ... expression ,(@here)
				self.index += 1
				# ( ... expression , expression(@here)
			self.compileExpression()
			count += 1
		# ( expressions )(@here)
		self.index += 1
		# ( expressions ) @here
		return str(count)
		
	def searchSymbolTable(self, name):
		if name in self.subroutineTable.table:
			segment = self.subroutineTable.kindOf(name)
			index = self.subroutineTable.indexOf(name)
			type = self.subroutineTable.typeOf(name)
			return (segment, index, type)
		elif name in self.classTable.table:
			segment = self.classTable.kindOf(name)
			if segment == 'field':
				segment = 'this'
			index = self.classTable.indexOf(name)
			type = self.classTable.typeOf(name)
			return (segment, index, type)
		return None
		