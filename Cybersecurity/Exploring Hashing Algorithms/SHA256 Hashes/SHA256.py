from Crypto.Hash import SHA256
import time
import random
import string

import xlwt
from xlwt import Workbook

#compute hash for input string
def get_hash(string):
	h = SHA256.new()
	h.update(string.encode('utf-8'))
	return h.hexdigest()

#compute an n-bit hash for input string
def get_hash_truncated(string, num_bits):
	hashed = str(get_hash(string))
	trunc = "{0:256b}".format(int(hashed, 16))
	return trunc[:num_bits]

#test hash differences for hamming distance of 1
def test_hamming():
	alpha = "abcdefghijklmnopqrstuvwxyz"
	for ch in alpha:
		tmp = "abcd" + ch
		print("---- " + tmp + " ----")
		print("0x" + get_hash(tmp))
		print()

#produce random string to be used for test cases
def get_random_string():
	alpha = "abcdefghijklmnopqrstuvwxyz"
	return "".join(random.choice(alpha) for i in range(20))

#finds a collision for a specified input string hashed with n-bits
#and writes the outputs to an excel file
def collisions(m0, num_bits, sheet):
	hashes = {get_hash_truncated(m0, num_bits) : m0}
	start_time = time.time()

	while(True):
		m1 = get_random_string()

		#if collision is found and strings are not the same, exit loop
		if(get_hash_truncated(m1, num_bits) in hashes and 
			hashes[get_hash_truncated(m1, num_bits)] != m0):
			break

		#collision is not found, add hashed value to dictionary
		else:
			hashes[get_hash_truncated(m1, num_bits)] = m1

	#output to terminal
	print(" * * * * * * * * COLLISION FOUND * * * * * * * *")
	print("Hash was truncated to " + str(num_bits) + " bits")
	print("Original Message (m0): " + m0)
	print("Computed Message (m1): " + m1)
	print("Dictionary Entries: " + str(len(hashes)))
	print("Time Elapsed: " + str(time.time() - start_time) + " seconds")

	#output to excel sheet
	sheet.write(int(((num_bits - 8) / 2) + 1), 0, num_bits)
	sheet.write(int(((num_bits - 8) / 2) + 1), 1, time.time() - start_time)
	sheet.write(int(((num_bits - 8) / 2) + 1), 2, len(hashes))

#find collisions for 8, 10, 12, ..., 50 bit hashes
def test_collisions():
	wb = Workbook()
	sheet1 = wb.add_sheet("collisions.py")
	sheet1.write(0, 0, "Number of Bits")
	sheet1.write(0, 1, "Time Elapsed")
	sheet1.write(0, 2, "Dictionary Entries")
	for bits in range(8, 52, 2):
		collisions(get_random_string(), bits, sheet1)
		print()
	wb.save('asgn3.xls')

def main():
	test_hamming()
	test_collisions()
	

if __name__ == '__main__':
	main()