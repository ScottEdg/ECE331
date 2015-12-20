import matplotlib.pyplot as plt
import numpy
import math

def convert_to_dB(H):
	dB = 20*(numpy.log10(abs(H)))
	return dB

def calcul8(s):
	H = 8000*s/((s**2)+(5000*s)+(10**6))
	return H


plt.figure();
s = numpy.logspace(1, 4)
H = calcul8(s)
dB = convert_to_dB(H)
 
plt.semilogx(s,dB)
plt.title("Magnitude of H(s) in dB")
plt.ylabel("Magnitude (dB)")
plt.xlabel("j$\omega$ (rad/sec)")
plt.grid()
plt.savefig("plot.png")
