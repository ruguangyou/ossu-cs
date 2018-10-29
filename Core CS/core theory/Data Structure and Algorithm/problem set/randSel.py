from random import randrange

r = 1000
count = 100000
unbalance = 0
for i in range(count):
	L = [randrange(r), randrange(r), randrange(r)]
	L.sort()
	mi = L[1]
	if mi < r/10 or mi > r*9/10:
		unbalance += 1

print(unbalance/count)
