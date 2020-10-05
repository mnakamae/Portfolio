import sys

#print the encoded file but shifted by a specified amount
def shift_chars(shift, line):
	alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	tmp = letters[:shift]
	letters = letters[shift:] + tmp
	
	for char in line:
		if (char not in alpha):
			print(char, end = "")
		else:
			print(letters[alpha.find(char)], end = "")
	print()

#test a specific shift value
def test_shift(shift):
	f = open(sys.argv[1], 'r')
	for line in f:
		shift_chars(shift, line.upper())
	f.close()

def main():
	shift = 0

	#test every possible shift for the 26 letters
	while(shift < 26):
		print(" --- SHIFTED " + str(26 - shift) + " ---")
		test_shift(shift)
		shift += 1
		print()


if __name__ == '__main__':
	main()