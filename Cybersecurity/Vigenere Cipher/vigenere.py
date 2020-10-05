import sys
import itertools

#generate a list of strings built from the input encoded file. the groups are built
#based on each chars position in the file
def get_groups(num_groups):
	alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	lower = "abcdefghijklmnopqrstuvwxyz"
	groups = ["" for x in range(num_groups)]
	
	for group in range(num_groups):
		count = 0
		f = open(sys.argv[1], 'r')

		for line in f:
			for char in line:
				if (char in alpha or char in lower):
					if (count == group):
						groups[group] += char.upper()

					count += 1
				
				if (count == num_groups):
					count = 0

		f.close()

	return groups

#given a key, find the corresponding decoded character
def find_char(key_char, char):
	alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	tmp = letters[:alpha.find(key_char)]
	letters = letters[alpha.find(key_char):] + tmp

	return(alpha[letters.find(char)])

#given a key, print the decoded message from the input text file
def print_decoded(key):
	alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	decoded = ""
	f = open(sys.argv[1], 'r')

	i = 0
	for line in f:
		for char in line:
			if (char.upper() not in alpha):
				decoded += char

			else:
				decoded += find_char(key[i], char.upper())
				i += 1

			if (i >= len(key)):
				i = 0

	f.close()
	print(decoded)


def main():
	alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	expect = [8.12, 1.49, 2.71, 4.32, 12.02, 2.30, 2.03, 5.92, 7.31, 0.10, 0.69, 3.98, 2.61,\
				6.95, 7.68, 1.82, 0.11,  6.02, 6.28, 9.10, 2.88, 1.11, 2.09, 0.17, 2.11, 0.07]
	
	valid_keys = []
	guess = ""
	keylen = 0


	if("vigerene_easy_encrypted.txt" in sys.argv[1]):
		keylen = 5
	elif("vigerne_medium_encrypt.txt" in sys.argv[1]):
		keylen = 9
	elif("vigerene_hard_encrypt.txt" in sys.argv[1]):
		keylen = 13

	test = ""
	test_name = ""
	actual = [[[0.0 for x in range(26)] for y in range(26)] for z in range(keylen)]

	#separate input text into groups of letters based on their positions
	groups = get_groups(keylen)

	#generate an array containing the frequency of each character in the groups
	for group_num in range(keylen):
		letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		for shift_val in range(0, 26):
			for char in groups[group_num]:
				actual[group_num][shift_val][letters.find(char)] += 100.0 / len(groups[group_num])

			letters = letters[1:] + letters[0] 

	#find the most probable key value
	for group_num in range(keylen):
		largest = 0
		largest_idx = 0
		for shift_val in range(26):

			#key with largest probability value is most likely the correct key
			probability = 0
			for idx in range(26):
				probability += actual[group_num][shift_val][idx] * expect[idx]

			if(probability > largest):
				largest = probability
				largest_idx = shift_val

		#add letter to the test key
		test_name += alpha[largest_idx - 1]
		test += alpha[largest_idx]

	#test the key
	print("------------ " + test_name + " ------------")
	print_decoded(test)


if __name__ == '__main__':
	main()