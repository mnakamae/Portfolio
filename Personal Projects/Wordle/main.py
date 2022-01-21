import pygame
from random import *

def draw_empty(dis, height):
    tile_length = 64
    split_length = 4
    tile_length = 64
    split_length = 4
    left_side = (get_width() - tile_length) // 2 - (2 * (tile_length + split_length))

    for i in range(5):
        outer_rect = pygame.Rect(left_side + i * (tile_length + split_length),
                           height, tile_length, tile_length)
        inner_rect = pygame.Rect(left_side + i * (tile_length + split_length) + 2,
                                 height + 2, tile_length - 4, tile_length - 4)
        pygame.draw.rect(dis, get_color("dark gray"), outer_rect)
        pygame.draw.rect(dis, get_color("black"), inner_rect)

def draw_word(dis, color_map, guess, height):
    tile_length = 64
    split_length = 4
    font = pygame.font.Font(None, 40)
    left_side = (get_width() - tile_length) // 2 - (2 * (tile_length + split_length))

    for i in range(len(guess)):
        text = font.render(guess[i], True, get_color("white"))
        rect = pygame.Rect(left_side + i * (tile_length + split_length),
                           height, tile_length, tile_length)
        pygame.draw.rect(dis, color_map[i], rect)
        dis.blit(text, (rect.left + (rect.width - text.get_rect().width) // 2,
                        rect.top + (rect.height - text.get_rect().height) // 2))

def get_color(color):
    my_colors = {
        "black": (18, 18, 19),
        "dark gray": (58, 58, 60),
        "gray": (129, 131, 132),
        "white": (215, 218, 220),
        "green": (83, 141, 78),
        "yellow": (181, 159, 59)
    }
    return my_colors[color]

def get_width():
    return 500

def get_height():
    return 700

def init_buttons():
    key_width = 40
    key_height = 56
    key_split = 4
    top_height = 500

    buttons = {}
    # first row
    row = ['Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P']
    for i in range(len(row)):
        buttons[row[i]] = pygame.Rect(i * (key_width + key_split), top_height, key_width, key_height)

    # second row
    row = ['A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L']
    for i in range(len(row)):
        buttons[row[i]] = pygame.Rect(i * (key_width + key_split),
                                      top_height + (key_height + key_split), key_width, key_height)

    # third row
    row = ["ENTER", 'Z', 'X', 'C', 'V', 'B', 'N', 'M', "<--"]
    buttons[row[0]] = pygame.Rect(0,
                                  top_height + 2 * (key_height + key_split), key_width * 1.5, key_height)
    for i in range(1, len(row) - 1):
        buttons[row[i]] = pygame.Rect((i + 0.5) * (key_width + key_split) - key_split//2,
                                      top_height + 2 * (key_height + key_split), key_width, key_height)
    buttons[row[-1]] = pygame.Rect((i + 1.5) * (key_width + key_split) - key_split//2,
                                  top_height + 2 * (key_height + key_split), key_width * 1.5, key_height)
    return buttons

def init_keyboard_colors():
    color_map = {}
    rows = [['Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'],
            ['A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'],
            ["ENTER", 'Z', 'X', 'C', 'V', 'B', 'N', 'M', "<--"]]
    for row in rows:
        for char in row:
            color_map[char] = get_color("gray")

    return color_map

def get_button_pressed(pos, buttons):
    for key, button in buttons.items():
        if button.collidepoint(pos):
            if key == "ENTER":
                return "return"
            elif key == "<--":
                return "backspace"

            return key

def get_keyboard_colors(color_map, answer, guesses):
    rows = [['Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'],
            ['A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'],
            ['Z', 'X', 'C', 'V', 'B', 'N', 'M']]

    # incorrect letter
    for guess in guesses:
        for char in guess:
            if char not in answer:
                color_map[char] = get_color("dark gray")

    # correct letter, incorrect location
    for guess in guesses:
        for i in range(len(guess)):
            if guess[i] in answer and guess[i] != answer[i]:
                color_map[guess[i]] = get_color("yellow")

    # correctly placed letters
    # NOTE: green overrides all other colors
    for guess in guesses:
        for i in range(len(guess)):
            if answer[i] == guess[i]:
                color_map[answer[i]] = get_color("green")

def get_guess_colors(color_map, answer, guesses):
    # incorrect letter
    for i in range(len(guesses)):
        for j in range(len(guesses[i])):
            if guesses[i][j] == ' ':
                break
            if guesses[i][j] not in answer:
                color_map[i][j] = get_color("dark gray")

    # correct letter, incorrect location
    for i in range(len(guesses)):
        for j in range(len(guesses[i])):
            if guesses[i][j] == ' ':
                break
            if guesses[i][j] in answer and guesses[i][j] != answer[j]:
                color_map[i][j] = get_color("yellow")

    # correctly placed letters
    # NOTE: green overrides all other colors
    for i in range(len(guesses)):
        for j in range(len(answer)):
            if guesses[i][j] == ' ':
                break
            if answer[j] == guesses[i][j]:
                color_map[i][j] = get_color("green")

def draw_all_empty(dis, heights):
    for h in heights:
        draw_empty(dis, h)

def update_kb(dis, color_map, buttons, answer, guesses):
    # get colors for each key on keyboard
    get_keyboard_colors(color_map, answer, guesses)
    for key, button in buttons.items():
        # create text to be written on top of button
        font = pygame.font.Font(None, 24)
        text = font.render(key.upper(), True, get_color("white"))

        # draw the button and text
        pygame.draw.rect(dis, color_map[key], button)
        dis.blit(text, (button.left + (button.width - text.get_rect().width) // 2,
                        button.top + (button.height - text.get_rect().height) // 2))

def update_tiles(dis, color_map, answer, guesses, heights):
    # draw all guesses
    get_guess_colors(color_map, answer, guesses)
    for i in range(len(guesses)):
        # guess used, draw colors accordingly
        if color_map[i] != [' ' for x in range(5)]:
            draw_word(dis, color_map[i], guesses[i], heights[i])
        # have not used this guess yet, draw nothing
        else:
            break

def update_cur_guess(dis, guess, height):
    tile_length = 64
    split_length = 4
    font = pygame.font.Font(None, 40)
    left_side = (get_width() - tile_length) // 2 - (2 * (tile_length + split_length))

    for i in range(len(guess)):
        text = font.render(guess[i], True, get_color("white"))
        outer_rect = pygame.Rect(left_side + i * (tile_length + split_length),
                                 height, tile_length, tile_length)
        inner_rect = pygame.Rect(left_side + i * (tile_length + split_length) + 2,
                                 height + 2, tile_length - 4, tile_length - 4)
        if i == len(guess) - 1 :
            pygame.draw.rect(dis, get_color("gray"), outer_rect)
        else:
            pygame.draw.rect(dis, get_color("dark gray"), outer_rect)

        pygame.draw.rect(dis, get_color("black"), inner_rect)
        dis.blit(text, (inner_rect.left + (inner_rect.width - text.get_rect().width) // 2,
                        inner_rect.top + (inner_rect.height - text.get_rect().height) // 2))

def create_restart():
    # draw the button and text
    return  pygame.Rect(get_width() // 2 + 2, 428, get_width() // 2 - 4, 68)

def draw_restart(dis, restart_rect):
    font = pygame.font.Font(None, 32)
    text = font.render("Replay?", True, get_color("white"))

    pygame.draw.rect(dis, get_color("green"), restart_rect)
    dis.blit(text, (restart_rect.left + (restart_rect.width - text.get_rect().width) // 2,
                    restart_rect.top + (restart_rect.height - text.get_rect().height) // 2))

def draw_answer(dis, answer):
    # draw current guess
    font = pygame.font.Font(None, 32)
    text = font.render("The word was: " + answer, True, get_color("white"))

    # draw the button and text
    answer_rect = pygame.Rect(0, 428, get_width() // 2 + 2, 68)
    pygame.draw.rect(dis, get_color("black"), answer_rect)
    dis.blit(text, (answer_rect.left,
                    answer_rect.top + (answer_rect.height - text.get_rect().height) // 2))

def main():
    # pygame functions
    pygame.init()
    dis = pygame.display.set_mode((get_width(), get_height()))
    pygame.display.set_caption('Walmart Wordle')
    game_over = False
    clock = pygame.time.Clock()
    buttons = init_buttons()
    kb_color_map = init_keyboard_colors()
    guess_color_map = [[' ' for x in range(5)] for y in range(6)]
    mouse_pressed = None

    # wordle stuff
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    guess, guesses = [], [[' ' for x in range(5)] for y in range(6)]
    num_guesses = 0

    # get word from dictionary
    words_fp = open("words.txt", 'r')
    words = words_fp.readlines()
    answer = words[randint(0, len(words) - 1)].strip('\n').upper()
    restart_button = create_restart()

    dis.fill(get_color("black"))
    while True:
        if num_guesses == 6:
            game_over = True
        if game_over:
            draw_restart(dis, restart_button)
            draw_answer(dis, answer)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                words_fp.close()
                pygame.quit()
                quit()

            # mouse clicks used to restart and interact with keyboard
            if event.type == pygame.MOUSEBUTTONDOWN:
                pos = pygame.mouse.get_pos()

                # game is finished and player wants to play again
                if game_over and restart_button.collidepoint(pos):
                    main()

                mouse_pressed = get_button_pressed(pos, buttons)
                if mouse_pressed is not None:
                    key_press_d = pygame.event.Event(pygame.KEYDOWN)
                    key_press_d.key = pygame.key.key_code(mouse_pressed)
                    key_press_d.unicode = mouse_pressed
                    pygame.event.post(key_press_d)

            if event.type == pygame.MOUSEBUTTONUP:
                if mouse_pressed is not None:
                    key_press_u = pygame.event.Event(pygame.KEYUP)
                    key_press_u.key = pygame.key.key_code(mouse_pressed)
                    key_press_u.unicode = mouse_pressed
                    pygame.event.post(key_press_u)
                    mouse_pressed = None

            # only allow functionality while less than 6 guesses
            if num_guesses < 6:
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_BACKSPACE:
                        if guess:
                            guess = guess[:-1]
                        kb_color_map["<--"] = get_color("dark gray")
                    elif event.key == pygame.K_RETURN:
                        kb_color_map["ENTER"] = get_color("dark gray")
                        if len(guess) == 5:
                            # correct answer given, exit out
                            if ''.join(guess) == answer:
                                game_over = True
                                guesses[num_guesses] = guess
                                continue
                            # show correct letters/positions
                            else:
                                guesses[num_guesses] = guess
                                num_guesses += 1
                                if num_guesses == 6:
                                    game_over = True
                                    continue
                                guess = []
                    elif (event.unicode).upper() in alphabet and (event.unicode).upper() is not '':
                        if len(guess) < 5:
                            guess.append((event.unicode).upper())

                # visual to change colors for RETURN and BACKSPACE keys
                if event.type == pygame.KEYUP:
                    if event.key == pygame.K_BACKSPACE:
                        kb_color_map["<--"] = get_color("gray")
                    elif event.key == pygame.K_RETURN:
                        kb_color_map["ENTER"] = get_color("gray")

        # heights used for guess tiles
        offset = 64 + 4  # tile_length + split_length
        start_height = 20
        heights = [start_height + i * offset for i in range(6)]

        # draw
        if not game_over:
            draw_all_empty(dis, heights)
        update_kb(dis, kb_color_map, buttons, answer, guesses)
        update_tiles(dis, guess_color_map, answer, guesses, heights)
        if not game_over:
            update_cur_guess(dis, guess, heights[num_guesses])

        # pygame functions
        pygame.display.update()
        clock.tick(30)

    words_fp.close()
    pygame.quit()
    quit()

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    main()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
