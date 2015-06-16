import csv
with open('cubeToolAfford.csv', 'rb') as f:
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
	anglesTargetFinal = list(T.euler_from_quaternion(floatList[i][11:15]))
	newList.append([ int(floatList[i][0]), int(floatList[i][1]), int(floatList[i][2]),  T.euler_from_quaternion(floatList[i][7:11])[1], anglesTargetFinal[0], anglesTargetFinal[1], anglesTargetFinal[2], floatList[i][21], floatList[i][22], floatList[i][23], floatList[i][24], floatList[i][25], floatList[i][26], floatList[i][33],floatList[i][34], floatList[i][39], floatList[i][40]]) 

print newList


# print your_list

with open("outputCubeTool.csv", "wb") as f:
    writer = csv.writer(f)
    writer.writerows(newList)