import sys
import os
sys.path.append("/Library/Frameworks/Python.framework/Versions/3.11/lib/python3.11/site-packages")

import numpy
from PIL import Image


f = open("salida.out", "r")
s = f.readlines()

height = int(s[0].split()[0])
width = int(s[1].split()[0])

print("\n\n\n\n")
print("Altura:", height)
print("Anchura:", width)



matrix = []
for i in range(height):
    vec = []
    elems = s[i+2].split()
    for j in range(0, 3*width, 3):
        aux = [elems[j], elems[j+1], elems[j+2]]
        aux = numpy.array(aux, dtype='uint8')
        vec.append(aux)
    matrix.append(numpy.array(vec, dtype='uint8'))

matrix = numpy.array(matrix, dtype='uint8')

f.close()
os.remove("salida.out")
im = Image.fromarray(matrix)
im.show(title='Imagen.png')

