import csv
with open('example_11.csv', 'rb') as f:
    reader = csv.reader(f)
    your_list = list(reader)

print your_list

with open("output.csv", "wb") as f:
    writer = csv.writer(f)
    writer.writerows(your_list)