from matplotlib import pyplot
import re
from numpy import *
#los archivos leidos vienen con lineas de la forma 'nframes,faults,readingtodisk,writingtodisk'

exp="(\d+),(\d+),(\d+),(\d+) *$"
fnames=["sort","scan","focus"]
for i in fnames:
	frames=[]
	faults=[]
	rtodisk=[]
	wtodisk=[]
	cframes=[]
	cfaults=[]
	crtodisk=[]
	cwtodisk=[]
	name1=i+"-fifo.txt"
	name2=i+"-custom.txt"
	fifo_txt=open(name1,'r')
	custom_txt=open(name2,'r')
	
	fifo_temp_lines=fifo_txt.readlines()
	custom_temp_lines=custom_txt.readlines()
	fifo_lines=[]
	custom_lines=[]
	len_lines=len(fifo_temp_lines)
	k=0
	while(k<len_lines):
		
		fnl=fifo_temp_lines[k].rstrip('\n')
		fifo_lines.append(fnl)
		cnl=custom_temp_lines[k].rstrip('\n')
		custom_lines.append(cnl)
		k+=1
	
	len_lines=len(fifo_lines)	

	for j in range(0,len(fifo_lines)):
		
		fifo_r=re.match(exp,fifo_lines[j])
		custom_r=re.match(exp,custom_lines[j])
		if((fifo_r is not None) and (custom_r is not None)):
			
			gs=fifo_r.groups()
			cgs=custom_r.groups()
			
			frames.append(gs[0])
			faults.append(gs[1])
			rtodisk.append(gs[2])
			wtodisk.append(gs[3])
	
			cframes.append(cgs[0])
			cfaults.append(cgs[1])
			crtodisk.append(cgs[2])
			cwtodisk.append(cgs[3])
	
	
	print faults
	print cfaults
	pyplot.plot(frames,faults,'r')
	
	pyplot.plot(frames,cfaults,'b')

	pyplot.title(i+": frames vs. page faults (red:FIFO, blue:CUSTOM)")
	pyplot.show()
	
	
	pyplot.plot(frames,rtodisk,'r')
	pyplot.plot(frames,crtodisk,'b')
	pyplot.title(i+": frames vs. readings to disk (red:FIFO, blue:CUSTOM)")
	pyplot.show()
	
	pyplot.plot(frames,wtodisk,'r')
	
	pyplot.plot(frames,cwtodisk,'b')
	pyplot.title(i+": frames vs. writings to disk (red:FIFO, blue:CUSTOM)")
	pyplot.show()
	
	pyplot.plot(frames,rtodisk,'r')
	
	pyplot.plot(frames,crtodisk,'b')
	pyplot.title(i+": frames vs. reading to disk (red:FIFO, blue:CUSTOM)")
	pyplot.show()
	print "------------------------------------------------"
	fifo_txt.close()
	custom_txt.close()

