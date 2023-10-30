import sys

OBJ_PATH = sys.argv[1]
OUT_PATH = sys.argv[2]

objFile = open(OBJ_PATH, "r")
fileText = objFile.read()
objFile.close()



lineVertexList = []
noSpacesVertexList = []
vertexList = []

lineNormalsList = []
noSpacesNormalsList = []
normalsList = []

lineUVList = []
noSpacesUVList = []
UVList = []

lineIndexList = []
noSpacesIndexList = []



outputText = ""



fileText = fileText[:-1]



for line in fileText.split("\n"):
	if list(line)[0] == "v":
		if list(line)[1] == "n":
			
			lineNormalsList.append(line)

		elif list(line)[1] == "t":

			lineUVList.append(line)

		else:

			lineVertexList.append(line)

	elif list(line)[0] == "f":

		lineIndexList.append(line)



def is_float(val):
	try:
		float(val)
		return True
	except:
		return False



for line in lineVertexList:
	for coord in line.split(" "):
		if is_float(coord):
			noSpacesVertexList.append(coord)

for line in lineNormalsList:
	for coord in line.split(" "):
		if is_float(coord):
			noSpacesNormalsList.append(coord)

for line in lineUVList:
	for coord in line.split(" "):
		if is_float(coord):
			noSpacesUVList.append(coord)

for line in lineIndexList:
	for coord in line.split(" "):
		if coord != "f":
			noSpacesIndexList.append(coord)



for vertex in range(int(len(noSpacesVertexList)/3)):
	vertexList.append(noSpacesVertexList[(vertex*3)] + "," + noSpacesVertexList[(vertex*3+1)] + "," + noSpacesVertexList[(vertex*3+2)])

for vertex in range(int(len(noSpacesNormalsList)/3)):
	normalsList.append(noSpacesNormalsList[(vertex*3)] + "," + noSpacesNormalsList[(vertex*3+1)] + "," + noSpacesNormalsList[(vertex*3+2)])

for vertex in range(int(len(noSpacesUVList)/2)):
	UVList.append(noSpacesUVList[(vertex*2)] + "," + noSpacesUVList[(vertex*2+1)])



for line in noSpacesIndexList:
	outputText += vertexList[int(line.split("/")[0])-1]
	outputText += ","
	outputText += normalsList[int(line.split("/")[2])-1]
	outputText += ","
	outputText += UVList[int(line.split("/")[1])-1]
	outputText += ","

outputText = outputText[:-1]
	


outputFile = open(OUT_PATH, "w+")
outputFile.write(outputText)