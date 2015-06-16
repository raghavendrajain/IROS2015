import csv
with open('example_11.csv', 'rb') as f:
    reader = csv.reader(f)
    your_list = list(reader)

floatList = []
for i in range(len (your_list)):
	floatList.append(map(float, your_list[i]))


# print floatList

import transformations as T
newList=[]
anglesTargetFinal =[]

for i in range(len(floatList)):
	anglesTargetFinal = list(T.euler_from_quaternion(floatList[i][6:10]))
	newList.append([ int(floatList[i][0]), int(floatList[i][1]), T.euler_from_quaternion(floatList[i][2:6])[1], anglesTargetFinal[0], anglesTargetFinal[1], anglesTargetFinal[2], floatList[i][12], floatList[i][13], floatList[i][14],floatList[i][15], floatList[i][18], floatList[i][19], int(floatList[i][20]), floatList[i][21], floatList[i][22]
      ]) 

print newList


# print your_list

with open("output.csv", "wb") as f:
    writer = csv.writer(f)
    writer.writerows(newList)