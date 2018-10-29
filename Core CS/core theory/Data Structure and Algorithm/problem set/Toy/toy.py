def up_down (V):
	V.reverse()
	print(V)
	return V

def logic_right (V):
	temp = V[3]
	V[3] = V[2]
	V[2] = V[1]
	V[1] = V[0]
	V[0] = temp
	temp = V[4]
	V[4] = V[5]
	V[5] = V[6]
	V[6] = V[7]
	V[7] = temp
	print(V)
	return V

def clockwise (V):
	temp = V[1]
	V[1] = V[6]
	V[6] = V[5]
	V[5] = V[2]
	V[2] = temp
	print(V)
	return V