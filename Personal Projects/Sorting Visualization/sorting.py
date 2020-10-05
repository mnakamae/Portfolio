import pygame
import random
import time
import sys

# CONSTANTS
HEIGHT = 800
WIDTH = 1280
NUM_ENTRIES = 85

# COLORS
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)


# create screen
pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
screen.fill(BLACK)
pygame.display.set_caption("Sorting Algorithms")
clock = pygame.time.Clock()

def check_exit():
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			pygame.quit()
			sys.exit()

def get_data():
	tmp = [0] * NUM_ENTRIES
	for i in range(NUM_ENTRIES):
		tmp[i] = random.randint(50, HEIGHT - 50)
	return tmp

def draw_bar(x, height, color):
	pygame.draw.rect(screen, color, (x, 0, 10, height))

def draw_data(data, color):
	for i in range(NUM_ENTRIES):
		draw_bar(5 + i * 15, data[i], color)

def bubble_sort(arr):
	n = len(arr)
	for i in range(n - 1):
		for j in range(0, n - i - 1):
			if arr[j] > arr[j + 1]:
				arr[j], arr[j + 1] = arr[j + 1], arr[j]

			# refresh data on screen
			pygame.draw.rect(screen, BLACK, (0, 0, WIDTH, HEIGHT))
			draw_data(arr, WHITE)
			draw_bar(5 + (j * 15), data[j], RED)
			draw_bar(5 + ((j + 1) * 15), data[j + 1], RED)
			pygame.display.update()

			check_exit()

def partition(arr, low, high):
	i = (low-1)         # index of smaller element
	pivot = arr[high]     # pivot

	for j in range(low, high):
		if arr[j] <= pivot:
			i = i+1
			arr[i], arr[j] = arr[j], arr[i]
		pygame.draw.rect(screen, BLACK, (0, 0, WIDTH, HEIGHT))
		draw_data(arr, WHITE)
		draw_bar(5 + (low * 15), data[low], (255,255,0))
		draw_bar(5 + (high * 15), data[high], (255,255,0))
		draw_bar(5 + (j * 15), data[j], RED)
		draw_bar(5 + (i * 15), data[i], BLUE)
		pygame.display.update()
		check_exit()

	arr[i+1], arr[high] = arr[high], arr[i+1]
	return (i+1)

def quick_sort(arr, low, high):
	if len(arr) == 1:
		return arr
	if low < high:
		pi = partition(arr, low, high)

		quick_sort(arr, low, pi-1)
		quick_sort(arr, pi+1, high)
 

def merge_sort(arr):
	print(arr)

# initialize first set of data
data = get_data()
print("Original Data: ", data)
draw_data(data, WHITE)


while True:
	pygame.display.update()

	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			pygame.quit()
			sys.exit()
		elif event.type == pygame.KEYDOWN:
			if event.key == pygame.K_0:
				pygame.display.set_caption("Sorting Algorithms: Bubble Sort")
				bubble_sort(data)
				print("Sorted Data: ", data)
				draw_data(data, GREEN)
			elif event.key == pygame.K_1:
				pygame.display.set_caption("Sorting Algorithms: Quick Sort")
				quick_sort(data, 0, NUM_ENTRIES - 1)
				print("Sorted Data: ", data)
				draw_data(data, GREEN)
			elif event.key == pygame.K_2:
				pygame.display.set_caption("Sorting Algorithms: Merge Sort")
				merge_sort("hello2")


# finished sorting


				
	