import random

# print example board for user to select move
def print_example_board():
	print("1 | 2 | 3")
	print("---------")
	print("4 | 5 | 6")
	print("---------")
	print("7 | 8 | 9")
	print()

# check for a winner in the current board state
def check_win(board):
	# valid columns win, rows win, diagonals win respectively
	valid = [[0, 3, 6], [1, 4, 7], [2, 5, 8],\
			 [0, 1, 2], [3, 4, 5], [6, 7, 8],\
			 [0, 4, 8], [2, 4, 6]]
	
	# check for a winner in the current board state
	for win in valid:
		if(board[win[0]] == board[win[1]] and board[win[0]] == board[win[2]]\
			and board[win[0]] != ' '):
			return board[win[0]]

	# tie found
	if(' ' not in board):
		return "tie"

	# no winner found
	return None

# add a valid move to the board
def add_move(player, move, board):
	# invalid move
	if(move < 0 or move > 8):
		return False

	# valid move, board updated
	if(board[move] == ' '):
		board[move] = player
		return True

	#invalid move
	return False

# print the current board state
def print_board(board):
	print(board[0] + " | " + board[1] + " | " + board[2])
	print("---------")
	print(board[3] + " | " + board[4] + " | " + board[5])
	print("---------")
	print(board[6] + " | " + board[7] + " | " + board[8])
	print("\n=========\n")

# find best move for computer using minimax
def best_move(board):
	best_score = -10
	best_spot = None;

	for move in range(9):
		if(board[move] == ' '):
			board[move] = 'X'
			score = minimax(board, 0, False)
			board[move] = ' '
			if(score > best_score):
				best_score = score
				best_spot = move
	return best_spot

# recursively find the best move based on scorings
# a better score means the computer is more likely to win
def minimax(board, depth, isMax):
	res = check_win(board)
	if(res != None):
		if(res == "tie"):
			return 0
		elif(res == 'X'):
			return 1
		elif(res == 'O'):
			return -1

	# X's turn, find the best move
	if(isMax == True):
		best_score = -10
		for move in range(9):
			if(board[move] == ' '):
				board[move] = 'X'
				score = minimax(board, depth + 1, False)
				board[move] = ' '
				best_score = max(score, best_score)
		return best_score
	# O's turn, find the best move
	else:
		best_score = 10
		for move in range(9):
			if(board[move] == ' '):
				board[move] = 'O'
				score = minimax(board, depth + 1, True)
				board[move] = ' '
				best_score = min(score, best_score)
		return best_score

def main():
	# init
	board = [' '] * 9
	move_count = 0

	# determine who moves first
	if(random.randint(0, 1) == 0):
		turn = 'X'
	else:
		turn = 'O'

	# continue game while still available moves and no winner
	while(move_count < 9):
		# print board state
		print_example_board()
		print_board(board)

		# ask player for a move and check if it is valid on their turn
		if(turn == 'O'):
			move = input("Select a move: ")
			valid_move = add_move(turn, int(move) - 1, board)
		# otherwise use minimax algorithm to find computer's best next move
		else:
			move = best_move(board)
			add_move(turn, move, board)
			valid_move = True

		# check for a winner
		if(check_win(board) == 'X' or check_win(board) == '0'):
			print("\n\n")
			print_board(board)
			print(check_win(board) + " wins!")
			return

		# increment turn counter and change player
		if(valid_move == True):
			move_count += 1
			if(turn == 'X'):
				turn = 'O'
			else:
				turn = 'X'

	print("\n\n")
	print_board(board)
	print("It's a tie!")


if __name__ == '__main__':
	main()