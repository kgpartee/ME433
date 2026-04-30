import pgzrun 

import pygame
import serial

ser = serial.Serial('COM6') # the name of your port here
print('Opening port: ' + str(ser.name))

import random

WIDTH = 400
HEIGHT = 600
n1_int = 0
n2_int = 0

# -----------------------------
# INPUT ABSTRACTION LAYER
# -----------------------------
# Replace these later with API calls.


def get_serial_data():
    n_bytes = ser.readline() # read all the letters available
    s = str(n_bytes) # turn them into a str
    result1 = s[s.find('(')+1:s.find(',')] # find everything beween ( and ,
    result2 = s[s.find(',')+1:s.find(')')] # find everything between , and )
    global n1_int
    n1_int = int(result1) # convert str to int
    global n2_int
    n2_int = int(result2)


def get_jump_input():
    """Return True when the player should jump."""

    return n1_int

def get_horizontal_input():
    """
    Return -1 for left, +1 for right, 0 for no movement.
    Replace this with API input later.
    """
    if n2_int < 2056:
        return -1
    if n2_int > 2056:
        return 1
    return 0


# -----------------------------
# PLAYER SETUP
# -----------------------------
player_size = 20

player = Rect((WIDTH//2, HEIGHT - 80), (player_size, player_size))

player.x = WIDTH // 2
player.y = HEIGHT - 80

velocity_y = 0
gravity = 0.4
jump_strength = -6
horizontal_speed = 4


# -----------------------------
# PLATFORM SETUP
# -----------------------------
platforms = []
platform_speed = 0.5
gap_width = 120

def create_platform(y):
    gap_x = random.randint(50, WIDTH - 50 - gap_width)
    return [0, y, gap_x]

for i in range(6):
    platforms.append(create_platform(HEIGHT - i * 120))

score = 0
game_over = False
game_started = False


def reset_game():
    global player, velocity_y, platforms, score, game_over

    player.x = WIDTH // 2
    player.y = HEIGHT - 80
    velocity_y = 0

    platforms = []
    for i in range(6):
        platforms.append(create_platform(HEIGHT - i * 120))

    score = 0
    game_over = False
    global game_started
    game_started = False



# -----------------------------
# GAME LOOP
# -----------------------------
def update():
    global velocity_y, score, game_over

    # Restart logic
    if game_over:
        if keyboard.r:
            reset_game()
        return

    global game_started

    get_serial_data()
    
# Start the game on first jump
    if not game_started:
        if get_jump_input():
            game_started = True
        else:
            return   # freeze everything until first jump

    
    # --- Horizontal movement (API-ready) ---
    move_dir = get_horizontal_input()
    player.x += move_dir * horizontal_speed
    player.x = max(0, min(WIDTH, player.x))

    # --- Vertical physics ---
    velocity_y += gravity
    player.y += velocity_y

    # Jump (API-ready)
    if get_jump_input():
        velocity_y = jump_strength

    # --- Move platforms ---
    for p in platforms:
        p[1] += platform_speed

    # --- Recycle platforms ---
    for p in platforms:
        if p[1] > HEIGHT:
            p[:] = create_platform(-100)
            score += 1

    # --- Collision detection ---
    for p in platforms:
        plat_y = p[1]
        gap_x = p[2]

        if player.y < plat_y + 10 and player.y > plat_y - 10:
            if not (gap_x < player.x < gap_x + gap_width):
                game_over = True

    # --- Out of bounds ---
    if player.y > HEIGHT or player.y < 0:
        game_over = True


# -----------------------------
# DRAW LOOP
# -----------------------------
def draw():
    screen.clear()
    screen.fill((0, 0, 0))

    # Platforms
    for p in platforms:
        y = p[1]
        gap_x = p[2]

        screen.draw.filled_rect(Rect((0, y), (gap_x, 20)), (200, 200, 200))
        screen.draw.filled_rect(Rect((gap_x + gap_width, y),
                                     (WIDTH - (gap_x + gap_width), 20)),
                                (200, 200, 200))

    # Player
    screen.draw.filled_rect(player, "white")


    # Score
    screen.draw.text(f"Score: {score}", (10, 10), fontsize=40, color="white")

    if game_over:
        screen.draw.text("GAME OVER", center=(WIDTH//2, HEIGHT//2),
                         fontsize=60, color="red")

pgzrun.go()
