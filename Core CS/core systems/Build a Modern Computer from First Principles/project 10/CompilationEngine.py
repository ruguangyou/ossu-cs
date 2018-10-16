
# CompilationEngine: gets its input from a JackTokenizer and emits its
#					 parsed structure into a output file.

class CompilationEngine(object):
	def __init__(self, tokens, fp):
		self.tokens = tokens
		self.fp = fp
		self.index = -1
		self.indent = 0
		
	def nextToken(self):
		self.index += 1
		return self.tokens[self.index]
	
	def writeNtime(self, n):
		for i in range(n):
			self.fp.write('  '*self.indent+self.nextToken()+'\n')
	
	def compileClass(self):
		self.fp.write('  '*self.indent+'<class>\n')
		self.indent += 1
		
		# <keyword> class </keyword>
		# <identifier> className </identifier>
		# <symbol> { </symbol>
		self.writeNtime(3)
		
		temp = self.tokens[self.index+1]
		while ('static' in temp) or ('field' in temp):
			self.compileClassVarDec()
			temp = self.tokens[self.index+1]
		
		while '}' not in temp:
			self.compileSubroutine()
			temp = self.tokens[self.index+1]
		
		# <symbol> } </symbol>
		self.writeNtime(1)
		
		self.indent -= 1
		self.fp.write('  '*self.indent+'</class>\n')
	
	def compileClassVarDec(self):
		self.fp.write('  '*self.indent+'<classVarDec>\n')
		self.indent += 1
		
		# <keyword> static | field </keyword>
		# <keyword> type </keyword>
		# <identifier> varName </identifier>
		self.writeNtime(3)
		
		temp = self.tokens[self.index+1]
		while ',' in temp:
		# <symbol> , </symbol>
		# <identifier> varName </identifier>
			self.writeNtime(2)
			temp = self.tokens[self.index+1]
		
		# <symbol> ; </symbol>
		self.writeNtime(1)
		
		self.indent -= 1
		self.fp.write('  '*self.indent+'</classVarDec>\n')
	
	def compileSubroutine(self):
		self.fp.write('  '*self.indent+'<subroutineDec>\n')
		self.indent += 1
		
		# <keyword> constructor | function | method </keyword>
		# <keyword> void | type </keyword>
		# <identifier> subroutineName </identifier>
		# <symbol> ( </symbol>
		self.writeNtime(4)
		
		self.compileParameterList()
		
		# <symbol> ) </symbol>
		self.writeNtime(1)
		
		self.fp.write('  '*self.indent+'<subroutineBody>\n')
		self.indent += 1
		# <symbol> { </symbol>
		self.writeNtime(1)
		
		temp = self.tokens[self.index+1]
		while 'var' in temp:
			self.compileVarDec()
			temp = self.tokens[self.index+1]
			
		self.compileStatements()
		
		# <symbol> } </symbol>
		self.writeNtime(1)
		
		self.indent -= 1
		self.fp.write('  '*self.indent+'</subroutineBody>\n')
		
		self.indent -= 1
		self.fp.write('  '*self.indent+'</subroutineDec>\n')
	
	def compileParameterList(self):
		self.fp.write('  '*self.indent+'<parameterList>\n')
		self.indent += 1
		
		temp = self.tokens[self.index+1]
		while ')' not in temp:
			self.writeNtime(1)
			temp = self.tokens[self.index+1]
		
		self.indent -= 1
		self.fp.write('  '*self.indent+'</parameterList>\n')
	
	def compileVarDec(self):
		self.fp.write('  '*self.indent+'<varDec>\n')
		self.indent += 1
		
		# <keyword> var </keyword>
		# <keyword> type </keyword>
		# <identifier> varName </identifier>
		self.writeNtime(3)
		
		temp = self.tokens[self.index+1]
		while ',' in temp:
		# <symbol> , </symbol>
		# <identifier> varName </identifier>
			self.writeNtime(2)
			temp = self.tokens[self.index+1]
		
		# <symbol> ; </symbol>
		self.writeNtime(1)
		
		self.indent -= 1
		self.fp.write('  '*self.indent+'</varDec>\n')
	
	def compileStatements(self):
		self.fp.write('  '*self.indent+'<statements>\n')
		self.indent += 1
		
		temp = self.tokens[self.index+1]
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
			temp = self.tokens[self.index+1]
			
		self.indent -= 1
		self.fp.write('  '*self.indent+'</statements>\n')
	
	def compileDo(self):
		self.fp.write('  '*self.indent+'<doStatement>\n')
		self.indent += 1
		
		# <keyword> do </keyword>
		# <identifier> className | varName | subroutineName </identifier>
		self.writeNtime(2)
		
		temp = self.tokens[self.index+1]
		if '.' in temp:
		# <symbol> . </symbol>
		# <identifier> subroutineName </identifier>
			self.writeNtime(2)
		
		# <symbol> ( </symbol>
		self.writeNtime(1)
		
		self.compileExpressionList()
		
		# <symbol> ) </symbol>
		# <symbol> ; </symbol>
		self.writeNtime(2)
		
		self.indent -= 1
		self.fp.write('  '*self.indent+'</doStatement>\n')
	
	def compileLet(self):
		self.fp.write('  '*self.indent+'<letStatement>\n')
		self.indent += 1
		
		# <keyword> let </keyword>
		# <identifier> varName </identifier>
		self.writeNtime(2)
		
		temp = self.tokens[self.index+1]
		if '[' in temp:
			# <symbol> [ </symbol>
			self.writeNtime(1)
			self.compileExpression()
			# <symbol> ] </symbol>
			self.writeNtime(1)
		
		# <symbol> = </symbol>
		self.writeNtime(1)
		
		self.compileExpression()
		
		# <symbol> ; </symbol>
		self.writeNtime(1)
		
		self.indent -= 1
		self.fp.write('  '*self.indent+'</letStatement>\n')
	
	def compileWhile(self):
		self.fp.write('  '*self.indent+'<whileStatement>\n')
		self.indent += 1
		
		# <keyword> while </keyword>
		# <symbol> ( </symbol>
		self.writeNtime(2)
		
		self.compileExpression()
		
		# <symbol> ) </symbol>
		# <symbol> { </symbol>
		self.writeNtime(2)
		
		self.compileStatements()
		
		# <symbol> } </symbol>
		self.writeNtime(1)
		
		self.indent -= 1
		self.fp.write('  '*self.indent+'</whileStatement>\n')
	
	def compileIf(self):
		self.fp.write('  '*self.indent+'<ifStatement>\n')
		self.indent += 1
		
		# <keyword> if </keyword>
		# <symbol> ( </symbol>
		self.writeNtime(2)
		
		self.compileExpression()
		
		# <symbol> ) </symbol>
		# <symbol> { </symbol>
		self.writeNtime(2)
		
		self.compileStatements()
		
		# <symbol> } </symbol>
		self.writeNtime(1)
		
		temp = self.tokens[self.index+1]
		if 'else' in temp:
			# <keyword> else </keyword>
			# <symbol> { </symbol>
			self.writeNtime(2)
			
			self.compileStatements()
			
			# <symbol> { </symbol>
			self.writeNtime(1)
		
		self.indent -= 1
		self.fp.write('  '*self.indent+'</ifStatement>\n')
		
	def compileReturn(self):
		self.fp.write('  '*self.indent+'<returnStatement>\n')
		self.indent += 1
		
		# <keyword> return </keyword>
		self.writeNtime(1)
		
		temp = self.tokens[self.index+1]
		if ';' not in temp:
			self.compileExpression()
		
		# <symbol> ; </symbol>
		self.writeNtime(1)
		
		self.indent -= 1
		self.fp.write('  '*self.indent+'</returnStatement>\n')
	
	def compileExpression(self):
		self.fp.write('  '*self.indent+'<expression>\n')
		self.indent += 1
		
		self.compileTerm()
		
		temp = self.tokens[self.index+1]
		if 'symbol' in temp:
			if len(temp) == len('<symbol> + </symbol>'):
				symbol = temp[9]
			elif len(temp) == len('<symbol> &lt; </symbol>'):
				symbol = temp[9:13]
			elif len(temp) == len('<symbol> &amp; </symbol>'):
				symbol = temp[9:14]
			if symbol in ['+','-','*','/','&amp;','|','&lt;','&gt;','=']:
				# <symbol> op </symbol>
				self.writeNtime(1)
				self.compileTerm()
		
		self.indent -= 1
		self.fp.write('  '*self.indent+'</expression>\n')
	
	def compileTerm(self):
		self.fp.write('  '*self.indent+'<term>\n')
		self.indent += 1
		
		temp = self.tokens[self.index+1]
		if 'identifier' in temp:
			# <identifier> varName | subroutineName | className </identifier>
			self.writeNtime(1)
			
			temp = self.tokens[self.index+1]
			if '[' in temp:
				# <symbol> [ </symbol>
				self.writeNtime(1)
				
				self.compileExpression()
				
				# <symbol> ] </symbol>
				self.writeNtime(1)
			elif '(' in temp:
				# <symbol> ( </symbol>
				self.writeNtime(1)
				
				self.compileExpressionList()
				
				# <symbol> ) </symbol>
				self.writeNtime(1)
			elif '.' in temp:
				# <symbol> . </symbol>
				# <identifier> subroutineName </identifier>
				# <symbol> ( </symbol>
				self.writeNtime(3)
				
				self.compileExpressionList()
				
				# <symbol> ) </symbol>
				self.writeNtime(1)		
		elif '(' in temp:
			# <symbol> ( </symbol>
			self.writeNtime(1)
			
			self.compileExpression()
			
			# <symbol> ) </symbol>
			self.writeNtime(1)
		elif 'integerConstant' in temp:
			# <integerConstant> intConst </integerConstant>
			self.writeNtime(1)
		elif 'stringConstant' in temp:
			# <stringConstant> stringConst </stringConstant>
			self.writeNtime(1)
		elif 'keyword' in temp:
			# <keyword> keywordConst </keyword>
			if ('true' in temp) or ('false' in temp) or ('null' in temp) or ('this' in temp):
				self.writeNtime(1)
		elif ('-' in temp )or ('~' in temp):
			# <symbol> - | ~ </symbol>
			self.writeNtime(1)
			self.compileTerm()
		
		self.indent -= 1
		self.fp.write('  '*self.indent+'</term>\n')
	
	def compileExpressionList(self):
		self.fp.write('  '*self.indent+'<expressionList>\n')
		self.indent += 1
		
		temp = self.tokens[self.index+1]
		while ')' not in temp:
			if ',' in temp:
				# <symbol> , </symbol>
				self.writeNtime(1)
				
			self.compileExpression()
			
			temp = self.tokens[self.index+1]
			
		self.indent -= 1
		self.fp.write('  '*self.indent+'</expressionList>\n')