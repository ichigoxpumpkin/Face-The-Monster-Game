import random

player = ["Name", 10, 1, 0, 0]
global game_state
game_state = True
fight_state = True


def create_enemy():
    dragon = ["Dragon", 10, 4, 100]
    orc = ["Orc", 5, 2, 25]
    slime = ["Slime", 1, 1, 10]
    enemy = [dragon, orc, slime]
    return random.choice(enemy)


def gamestate_check():
    global game_state
    if player[1] <= 0:
        game_state = False
        print(f'Game over! Better luck next time!')


def generate_potion():
    potion_type = random.choice(['health', 'strength', 'poison'])
    potion_effect = random.choice(['small', 'medium', 'large'])
    return potion_type, potion_effect


def drink_potion():
    potion = generate_potion()
    print(f"\nYou found a mystical potion. Do you want to drink it? (y/n)\n")
    choice = input()
    if choice == 'y':
        if potion[0] == 'health':
            print(f"You drink the {potion[1]} {potion[0]} and feel your wounds heal.\n")
            player[1] = player[1]+5
        elif potion[0] == 'strength':
            print(f"You drink the {potion[1]} {potion[0]} and feel your muscles bulge with strength.\n")
            player[2] = player[2]+1
        elif potion[0] == 'poison':
            print(f"You drink the {potion[1]} {potion[0]} and immediately feel ill.\n")
            player[1] = player[1]-1
        else:
            print("Invalid potion type!")


def fight(fight_enemy):
    global game_state
    while game_state:
        fight_choice = input('Do you wish to fight (f) or attempt to run (r)?')
        if fight_choice == 'r' or fight_choice == 'R':
            flee_chance = random.randint(0, 1)
            if flee_chance == 0:
                print('You have been successful in fleeing!')
                print(f'{player[0]}\nHealth: {player[1]}\nDamage: {player[1]}\nGold: {player[2]}')
                return
            else:
                print('You were not successful in fleeing!')
                player[1] = player[1] - fight_enemy[2]
                gamestate_check()
        elif fight_choice == 'f' or fight_choice == 'F':
            fight_enemy[1] = fight_enemy[1] - player[2]
            gamestate_check()
            if fight_enemy[1] > 0:
                player[1] = player[1] - fight_enemy[2]
            elif fight_enemy[1] <= 0:
                player[3] = fight_enemy[3] + player[3]
                player[4] = player[4] + 1
                player[2] = player[2] + 1
                drink_potion()
            gamestate_check()
        else:
            print('Invalid Input\n')
        print(f'{player[0]}\nHealth: {player[1]}\nDamage: {player[2]}\nGold: {player[3]}\nLevel: {player[4]}\n')
        print(f'{fight_enemy[0]}\nHealth: {fight_enemy[1]}\nDamage: {fight_enemy[1]}\nGold: {fight_enemy[2]}\n')
        if player[1] <= 0 or fight_enemy[1] <= 0:
            return


player[0] = input('Hello user, please enter your name\n')
print(f'Hello {player[0]}!\nHealth: {player[1]}\nDamage: {player[2]}\nGold: {player[3]}\nLevel: {player[4]}')
while game_state:
    current_enemy = create_enemy()
    print(
        f'\nYou have encountered a {current_enemy[0]}!\nHealth: {current_enemy[1]}\nDamage: {current_enemy[2]}\nGold: {current_enemy[3]}\n')
    fight(current_enemy)