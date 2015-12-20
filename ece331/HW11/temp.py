#import smbus
#import sqlite3
import subprocess
from datetime import datetime
from time import sleep
#conn = sqlite3.connect('tempdata.db')
#c = conn.cursor()

def ADCtoK(ADC):
	K = .172*ADC - 35.26
	return K

def KtoF(K):
	F = ((K*9)/5)-459.67
	return F

def getAvg():
	total = 0
	for i in range(0,40):
		p = subprocess.Popen(["i2cget", "-y", "1", "0x41", "0x06", "w"], stdout=subprocess.PIPE)
		output, err = p.communicate()
		total = total + int(output,16)
		sleep(0.1)
	avgVal = total/40
	print avgVal
	return avgVal

#bus=smbus.SMBus(1)
#address=0x41
#raw_temp_LSB = 0x06
#raw_temp_MSB = 0x07
now = datetime.now()
currentYear = now.year
currentMonth = now.month
currentDay = now.day
currentHour = now.hour
currentMin = now.minute

Ktemp = ADCtoK(getAvg())
Ftemp = KtoF(Ktemp)
Date = str(currentYear)+"-"+str(currentMonth)+"-"+str(currentDay)
Time = str(currentHour)+":"+str(currentMin)
print Date
print Time
frame = (Date,Time,Ftemp)
print frame

#c.execute('INSERT INTO data VALUES(?,?,?)',frame)

#c.execute('''INSERT INTO data('str(currentYear)+"-"+str(currentMonth)+"-"+str(currentDay)', 'str(currentHour)+":"+str(currentMin)', Ftemp)''')
#conn.commit()

#conn.close()
