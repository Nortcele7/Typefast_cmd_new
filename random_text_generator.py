import random  

# Beginner Drills (Level 1)  
home_row = ['f', 'j', 'd', 'k', 's', 'l', 'a', ';']  
def generate_beginner_line():  
    drills = [  
        'fjfj jfjf ffjj jjff ffjj fjfj fffj jjjf ffjf jjff',  
        'dfjk jkdf djdj kfkf fkfk jdjd kjfd kjfd jkdf fdkj',  
        'sldk lskd slsl lsls klds lskd llks dksl lksd lkds',
        'a;a; asdf ;lkj a;s; ;;;a a;;; ;a;a fdsa lkja lkds',  
        ' '.join([''.join(random.choices(home_row, k=4)) for _ in range(10)]) 
    ]  
    return random.choice(drills)  

with open('Level1_Beginner_Drills.txt', 'w') as f:  
    for _ in range(1000):  
        f.write(generate_beginner_line() + '\n')  

# Repeat similar logic for Intermediate/Advanced levels  