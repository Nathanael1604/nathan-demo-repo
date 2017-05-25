#Title: Foxes and Hounds
#Author: Nathan Park
#ID: 5404 9369

def play():
    """function to run game"""
    num_hounds = user_input("Number of hounds: ",1,9)
    length_field = user_input("Length of field: ",2,15)
    fox_start_pos_x = user_input("Fox starting position: ",1,num_hounds)
    fox_pos = [1,fox_start_pos_x]
    hounds_posi = hounds_pos(num_hounds,length_field)
    print_board(length_field,num_hounds,hounds_posi,fox_pos)
    while True:
        fox_pos = fox_move_check(length_field,num_hounds,fox_pos,hounds_posi)
        print_board(length_field,num_hounds,hounds_posi,fox_pos)
#============================================================================

def user_input(prompt,lower,upper):
    user_input = raw_input(prompt)
    while user_input.isdigit() == False or int(user_input) > upper or int(user_input) < lower:
        print "Please enter a number %d from %d" %(lower, upper)
        user_input = raw_input(prompt)
    return (int(user_input))


#========================================================================
def hounds_pos(num_hounds,length_field):
    """Creating the list for hounds positions"""
    hounds = []
    for hound in range(1,num_hounds+1):
            hounds.append([length_field,hound])
    return hounds


#========================================================================
def print_board(length_field,num_hounds,hounds_pos,fox_pos):
    for y in range(1,int(length_field)+1):
        if y%2 == 0:
            print '',
        for x in range(1,int(num_hounds)+1):
            if fox_pos == [y,x]:
                if x == int(num_hounds):
                    print 'f'
                    fox_pos = [y,x]
                else:
                    print 'f',
                    fox_pos = [y,x]
            elif [y,x] in hounds_pos:
                if x == int(num_hounds):
                    print (hounds_pos.index([y,x]) + 1)
                else:
                    print (hounds_pos.index([y,x]) + 1),
            elif x == int(num_hounds):
                print '.'
            else:
                print '.',

#==============================================================
def fox_move(fox_pos,input_direction):
    new_fox_pos = [0,0]
    """Must first check for odd or even row number on board"""
    if fox_pos[0]%2 != 0:
        if input_direction[0] == 'n' or input_direction[0] == 'N':
            new_fox_pos[0] = fox_pos[0] - 1
            if input_direction[1] == 'w' or input_direction[1] == 'W':
                new_fox_pos[1] = fox_pos[1] - 1
            else:
                new_fox_pos[1] = fox_pos[1]
        elif input_direction[0] == 's' or input_direction[0] == 'S':
            new_fox_pos[0] = fox_pos[0] + 1
            if input_direction[1] == 'w' or input_direction[1] == 'W':
                new_fox_pos[1] = fox_pos[1] - 1
            else:
                new_fox_pos[1] = fox_pos[1]
    elif fox_pos[0]%2 == 0:
        if input_direction[0] == 'n' or input_direction[0] == 'N':
            new_fox_pos[0] = fox_pos[0] - 1
            if input_direction[1] == 'e' or input_direction[1] == 'E':
                new_fox_pos[1] = fox_pos[1] + 1
            else:
                new_fox_pos[1] = fox_pos[1]
        elif input_direction[0] == 's' or input_direction[0] == 'S':
            new_fox_pos[0] = fox_pos[0] + 1
            if input_direction[1] == 'e' or input_direction[1] == 'E':
                new_fox_pos[1] = fox_pos[1] + 1
            else:
                new_fox_pos[1] = fox_pos[1]
    return new_fox_pos
#=======================================================
def fox_move_check(length_field,num_hounds,fox_pos,hounds_pos):
    """list of all possible valid directions"""
    valid_directions = ['nw','ne','sw','se','NW','NE','SW','SE','Nw','Ne','Sw','Se','nW','nE','sW','sE']       
    while True:
            input_direction = raw_input("Fox's move (NW, NE, SW, SE): ")
            new_fox_pos = fox_move(fox_pos,input_direction)
            if input_direction not in valid_directions:
                print "Invalid direction for fox move"
            elif new_fox_pos[0] not in range(1,length_field+1):
                print "Fox can't move in that direction"
            elif new_fox_pos[1] not in range(1,num_hounds+1):
                print "Fox can't move in that direction"
            elif [new_fox_pos[0],new_fox_pos[1]] in hounds_pos:
                print "Fox can't move in that direction"
            else:
                return new_fox_pos
#==========================================================
if __name__ == '__main__':
        play()