import time
import bcrypt
import base64
from nltk.corpus import words

#generate a hash from given password and work factor
def try_hash(salt, password, work_factor):
	hashed = bcrypt.hashpw(password.encode("utf-8"), salt.encode("utf-8"))
	return hashed.decode("utf-8")

#brute force guess the password
def guess_pass(work_factor, salt, b_hashed):
	count = 0
	start_time = time.time()
	for word in words.words():
		#only words with length 6 <= len <= 10 are valid
		if(len(word) > 10 or len(word) < 6):
		 	continue

		#generate hashed password
		tmp = try_hash(salt, word, int(work_factor))

		#if password found
		if(tmp == b_hashed):
			print("Found Password: " + word)
			run_time = time.time() - start_time
			print("Time Elapsed: " + str(run_time) + " seconds")
			print("Attempts: " + str(count))
			break

		count += 1


def main():
	f = open("shadow.txt", 'r')
	print(str(words.words().index("purrone")))
	#for each password
	for line in f:

		#get fields from hashed password
		user = line[:line.find(':')]
		algo = line[(line.find('$') + 1):(line.find('$') + 3)]
		work_factor = line[(line.find("$") + 4):(line.find('$') + 6)]
		salt = line[(line.find('$')):(line.find('$') + 29)]
		hashed = line[(line.find('$') + 29):]
		b_hashed =  line[(line.find(':') + 1):-1]

		#print results
		print()
		print("====================================================")
		print("User: " + user)
		print("Hashed Password: " + b_hashed)
		guess_pass(work_factor, salt, b_hashed)
	f.close()

if __name__ == '__main__':
	main()