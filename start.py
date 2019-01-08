import tkinter					
import tkinter.ttk as ttk       #to use advanced widget
from tkinter import messagebox  #to use messagebox
from tkinter import filedialog  #to use file dialog

#Definition of required function
def btn_Source_Function(arg=None):
	filename = filedialog.askopenfilename(filetypes=[("JPEG file","*.jpg")])
	lbl_Source_Setter.set(filename)

def btn_Dest_Function(arg=None):
	dirname=filedialog.askdirectory()
	lbl_Dest_Setter.set(dirname)

def btn_Scan_Function(arg=None):#when Enter pressed arg passed like -<KeyPress event state=Mod1 keysym=Return keycode=13 char='\r' x=69 y=24> (So we dont want to receive)
	sourceData=lbl_Source_Setter.get()
	destData=lbl_Dest_Setter.get()

	if(sourceData!="" and destData!=""):
		container.attributes("-disabled", 1)
		prgbar_Status=ttk.Progressbar(container,length=400,mode="indeterminate",orient="horizontal")
		prgbar_Status.place(x=100,y=200)
		prgbar_Status.start([80])
	
	else:
		if(sourceData=="" and destData==""):
			messagebox.showinfo("Information","Please select Source Image and Scan Folder")
		elif(sourceData==""):
			messagebox.showinfo("Information","Please select Source Image")
		else:
			messagebox.showinfo("Information","Please select Scan Folder")

def btn_Cancel_Function(arg=None):
	container.quit()


#Create Container to hold widget
container=tkinter.Tk()

w=container.winfo_screenwidth()//2  #take width of screen
h=container.winfo_screenheight()//2 #take height of screen

container.minsize(600,300)       				   #window size
container.geometry("+{}+{}".format(w-300,h-150))   #left and upper space
container.resizable(0,0)						   #Remove maximize button
container.title("Image Scanner")				   #Set Title
#container.withdraw()  for hiding purpose

#Configure Style
style=ttk.Style()
style.configure("TButton",width=25)

#Create Widget
btn_Source=ttk.Button(container,text="Browse Source Image",style="TButton",command=btn_Source_Function)
btn_Source.bind("<Return>",btn_Source_Function)
btn_Source.place(x=27,y=22)

lbl_Source_Setter=tkinter.StringVar()
lbl_Source=ttk.Label(container,textvariable=lbl_Source_Setter,width=60,background="white",anchor="center")
lbl_Source.place(x=197,y=22)

btn_Dest=ttk.Button(container,text="Browse Scan Folder",style="TButton",command=btn_Dest_Function)
btn_Dest.bind("<Return>",btn_Dest_Function)
btn_Dest.place(x=27,y=72)

lbl_Dest_Setter=tkinter.StringVar()
lbl_Dest=ttk.Label(container,textvariable=lbl_Dest_Setter,width=60,background="white",anchor="center")
lbl_Dest.place(x=197,y=72)

btn_Scan=ttk.Button(container,text="Scan",style="TButton",command=btn_Scan_Function)
btn_Scan.bind("<Return>",btn_Scan_Function)
btn_Scan.place(x=197,y=122)

btn_Cancel=ttk.Button(container,text="Cancel",style="TButton",command=btn_Cancel_Function)
btn_Cancel.bind("<Return>",btn_Cancel_Function)
btn_Cancel.place(x=403,y=122)

#Infinite loop to take action on event
container.mainloop()