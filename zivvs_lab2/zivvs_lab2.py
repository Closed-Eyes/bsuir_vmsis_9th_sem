import math
import random

def isPrime(num):
	if num == 2:
		return True
	for i in range(2, num):
		if num % i == 0:
			return False
	return True

def calculateKeys(p):
	keys = []

	for i in range(3, p):
		if isPrime(i) and ((p-1) % i) != 0:
			keys.append(findKey(i, p))

	return keys

def findKey(num, p):
	for i in range(1, p-1):
		if num * i > (p-1) and (num * i)%(p-1) == 1:
			return i

def findFromMu2(mu2, a, b, p):
	mu1 = 0;
	for i in range(1,p):
		if pow(i, b, p) == mu2:
			mu1 = i
			break

	for i in range(1, p):
		if mu1 == pow(i, a, p):
			return i

	return -1

def findFromMu3(mu3, a, b, p, alpha):
	mu2 = 0

	for i in range(1, p):
		if pow(i, alpha, p) == mu3:
			mu2 = i
			break

	return findFromMu2(mu2, a, b, p)

def checkMu(mu, a, b, alpha, beta, p):
	mu1 = pow(mu, a, p)
	mu2 = pow(mu1, b, p)
	mu3 = pow(mu2, alpha, p)
	mu4 = pow(mu3, beta, p)

	if mu == mu4:
		return True
	else:
		return False




p = 0
a = 0
b = 0
alpha = 0
beta = 0

messageSource = "OSEN"
message = []

for char in messageSource:
	message.append(ord(char))

while True:
	p = random.randrange(200,500)
	if isPrime(p) == True:
		break

keys = calculateKeys(p)

a = keys[random.randrange(0, len(keys))]
b = keys[random.randrange(0, len(keys))]

alpha = findKey(a, p)
beta = findKey(b, p)

print "P = ", p
print "A:"
print "	a = ", a, "; alpha = ", alpha
print "B:"
print "	b = ", b, "; beta = ", beta

print "\nMessage:\n",

for char in message:
	print char, chr(char)

print "\n"

print "\nTransmiting message..."

encodedMessage = []
mu2s = []
mu3s = []

for char in message:
	mu1 = pow(char, a, p)
	mu2 = pow(mu1, b, p)
	mu3 = pow(mu2, alpha, p)
	mu4 = pow(mu3, beta, p)
	encodedMessage.append(mu4)
	
	mu2s.append(mu2)
	mu3s.append(mu3)

	print "Char =", char, "; mu1 =", mu1, "; mu2 =", mu2, "; mu3 =",mu3, "; mu4 =", mu4

print "\n"

print "\nTransmitted message:"
for char in encodedMessage:
	print char, chr(char)

print "\n"

print "\nDecoding message, knowing only mu2's"

for a in keys:
	for b in keys:

		rightEcondedCount = 0

		alpha = findKey(a, p)
		beta = findKey(b, p)

		for char in mu2s:
			encodedChar = findFromMu2(char, a, b, p)
			if checkMu(encodedChar, a, b, alpha, beta, p) == True:
				if encodedChar >= ord('A') and encodedChar <= ord('Z'):
					rightEcondedCount += 1

		if rightEcondedCount == len(mu2s):
			print "a =", a, "; b =", b, "; alpha =", alpha, "; beta =", beta

print "\nDecoding message, knowing only mu3's"

for a in keys:
	for b in keys:

		rightEcondedCount = 0

		alpha = findKey(a, p)
		beta = findKey(b, p)

		for char in mu3s:
			encodedChar = findFromMu3(char, a, b, p, alpha)
			if checkMu(encodedChar, a, b, alpha, beta, p) == True:
				if encodedChar >= ord('A') and encodedChar <= ord('Z'):
					rightEcondedCount += 1

		if rightEcondedCount == len(mu2s):
			print "a =", a, "; b =", b, "; alpha =", alpha, "; beta =", beta