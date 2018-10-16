
# JackTokenizer: Removes all the comments and white space from the input stream and 
#				 breaks it into Jack language tokens, as specified by the Jack grammar.

import re

class  JackTokenizer(object):
	def __init__(self, fname):
		self.fname = fname
		self.keywords = ['class','constructor','function','method','field',
					       'static','var','int','char','boolean','void','true', 
					       'false','null','this','let','do','if','else','while','return']
		self.symbols = ['{','}','(',')','[',']','.',',',';','+','-','*','/','&','|','<','>','=','~']
		self.longComment = False
		self.tokens = []
		
	def tokenize(self):
		with open(self.fname, 'r') as fp:
			lines = fp.readlines()
		for line in lines:
			if self.longComment:
				if '*/' in line:
				# the end of long comment
					self.longComment = False
				line = ''
			else:
				line = re.sub('/\*\*.*\*/', '', line)	# remove /** ... */ comment if any
				line = re.sub('/\*.*\*/', '', line)	# remove /* ... */ comment if any
				line = re.sub('\s*//.*', '', line)		# remove // ... comment if any
				line = re.sub('^\s*', '', line)		# remove white space in the beginning
				line = re.sub('\n', '', line)
				if '/**' in line:
				# is long comment that takes several lines
					self.longComment = True
					line = ''	
			if line == '':
			# origin line is comment or while line
				continue
			self.line = line
			while self.hasMoreTokens():
				self.advance()
		return self.tokens
		
	def hasMoreTokens(self):
		if self.line == '':
			return False
		return True
	
	def advance(self):
		self.token = ''
		self.line = re.sub('^\s*', '', self.line)	# remove white space at the beginning
		isString = False
		for i in range(len(self.line)):
			c = self.line[i]
			if c == '"':
				isString = not isString
			
			if not isString:
				if c == '"':
					self.line = self.line[i+1:]
					break
				if (c == ' ') or (c in self.symbols):
					self.line = self.line[i:]
					if self.token == '':
					# symbol token
						self.token = c
						self.line = self.line[i+1:]
					break
			self.token += c
		self.tokenType()
		
	def tokenType(self):
		if self.keyWord():
			type = 'keyword'
		elif self.symbol():
			type = 'symbol'
			
			if self.token == '<':
				self.token = '&lt;'
			elif self.token == '>':
				self.token = '&gt;'
			elif self.token == '&':
				self.token = '&amp;'
				
		elif self.intConst():
			type = 'integerConstant'
		elif self.stringConst():
			type = 'stringConstant'
			self.token = self.token[1:]
		else:
			type = 'identifier'
		self.tokens.append('<'+type+'> '+self.token+' </'+type+'>')
		
	def keyWord(self):
		return (self.token in self.keywords)
		
	def symbol(self):
		return (self.token in self.symbols)
		
	# def identifier(self):
		
	def intConst(self):
		return self.token.isnumeric()
		
	def stringConst(self):
		return '"' in self.token
		