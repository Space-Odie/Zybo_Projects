#################################################################################################################
#   Created by Ian O'Donnell
#   Date: 4/10/21
#################################################################################################################
#   Purpose: 
#   Convert arrays to be useable in a different scripting language
#   The inputs are taken from the terminal output and place in the Files.py 
#################################################################################################################

#################################################################################################################
#   Input: Python Format Array
#   Output: Matlab format Array
#################################################################################################################
def c2m(c, title):
    print(f'Converting:  \n{title} Image')
    temp = []
    for i, num in enumerate(c):
        length = 25
        if (i+1) % length == 0 and i != 0:
            temp.append(str(c[i]) + ';')
            print(str(temp).replace("'","").replace(";,",";").replace("[","").replace("]",""))
            temp = []
        else: 
            temp.append(str(c[i]))
    
    print("Convert Complete \n")

#Used for the Compressed Image 2 Matlab
def c2m2(c, title):
    print(f'Converting:  \n{title} Image')
    temp = []
    for i, num in enumerate(c):
        length = 25
        if (i+1) % 1 == 0:
            temp.append(str(c[i]) + ';')
            print(str(temp).replace("'","").replace(";,",";").replace("[","").replace("]",""))
            temp = []
        else: 
            temp.append(str(c[i]))
    
    print("Convert Complete \n")

    
#################################################################################################################
#   Convert 625x2 matrix to a form I can use in C
#   I need this so I can decompress the output of the compressed image
#################################################################################################################
#   Input: Python Format Array
#   Output: C format Array
#################################################################################################################
def c2c(c, title):
    print(f'Converting:  \n{title} Array')
    temp = []
    for i, num in enumerate(c):
        if (i+1) % 2 == 0 and i != 0:
            temp.append(str(c[i]) + '},')
        else: 
            temp.append('{' + str(c[i]))

    print(str(temp).replace("'","").replace(",,",","))#.replace("[","{").replace("]","}"))
    print("Convert Complete \n")

