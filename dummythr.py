from threading import Thread
import cv2
class Mythread(Thread):
	def __init__(self,capx,lastFrame):
		Thread.__init__(self)
		self.capx=capx
		self.lastFrame=lastFrame
	def run(self):
		while (cap.isOpened()):
			while self.lastFrame!=capx.get(1):
				ret,frame=capx.read()
				gray=cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
				cv2.imshow('frame',frame)
				if cv2.waitKey(25)&0xFF ==ord('q'):
					break
		
cap=cv2.VideoCapture('sample.avi')
fps=cap.get(5)
nof=cap.get(7)

cp=[]
threads=[]
n=cap.get(7)
for i in range(4):
	c=cap.set(1,i*(n/4))
	cp.append(c)
	t=Mythread(c,i*(n/4)+(n/4))
	threads.append(t)
	t.start()
for t in threads:
 t.join()
cap.release()
print('main program ends')
