#############################################
## Rights reserved for BlcArrayTCP4Server developed by Dr.Arnaud Blanchard ( ETIS LAB)
############################################################
import dbus
import dbus.mainloop.glib
from gi.repository import GLib
from gi.repository import GObject
import time
from optparse import OptionParser
import sys
import cv2
from blc_network import BlcArrayTCP4Server
import numpy
proxSensorsVal=[0,0,0,0,0]

def dbusReply():
    pass

def dbusError(e):
    print ('error %s')
    print (str(e))

#the communication part
## new communication part ################# IMAGE ####################
vid=cv2.VideoCapture(0)
#vid.set(cv2.CAP_PROP_FPS, 5)
height = int(vid.get(cv2.CAP_PROP_FRAME_HEIGHT))
width = int(vid.get(cv2.CAP_PROP_FRAME_WIDTH))
deep=1 # 1 if it is in gray
print(height,"x", width)
server_image=BlcArrayTCP4Server("31440", 'UIN8', 'RGB3', [height, width])
print("server_camera connected")
server_commands=BlcArrayTCP4Server("31441", 'UIN8', 'TEXT', [2])
print("server_commands connected")
server_image.wait_connection()
server_commands.wait_connection()


#server_image.wait_connection()
   #We create an numpy array (equivalent to cv:Mat in C++ ) associated with the data of server
server_array = numpy.ndarray([height, width], dtype=numpy.uint8, buffer=server_image.data, offset=0, strides=None, order='C')
######

def Braitenberg():
       t = time.time()
   ## new communication part #############################
       ret, img = vid.read()
       gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
   #we copy the pixels of the image to the array of the server (i.e. the buffer: server.data)
       numpy.copyto(server_array, gray)
       server_image.send_data()
       print("Image sent")
###################################################################

    #get the values of the sensors
       network.GetVariable("thymio-II", "prox.horizontal",reply_handler=get_variables_reply,error_handler=get_variables_error)

       server_commands.recv_data()
       print("command received")
       val_left=server_commands.data[0]
       val_right=server_commands.data[1]

    #send motor value to the robot
       network.SetVariable("thymio-II", "motor.left.target", [val_left])
       network.SetVariable("thymio-II", "motor.right.target", [val_right])

      # network.SetVariable("thymio-II","motor.left.target",[0])
       ##time.sleep(0.01)
       if (proxSensorsVal[2]>1000) or (proxSensorsVal[4]>1000) or (proxSensorsVal[3]>2000):
          network.SetVariable("thymio-II", "motor.left.target", [0])
          network.SetVariable("thymio-II", "motor.right.target", [0])


       Braitentime=time.time() - t
       print("run time=",Braitentime)
       return True

def get_variables_reply(r):
    global proxSensorsVal
    proxSensorsVal=r

def get_variables_error(e):
    print ('error:')
    print (str(e))
    loop.quit()

if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option("-s", "--system", action="store_true", dest="system", default=False,help="use the system bus instead of the session bus")

    (options, args) = parser.parse_args()

    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

    if options.system:
        bus = dbus.SystemBus()
    else:
        bus = dbus.SessionBus()

    #Create Aseba network
    network = dbus.Interface(bus.get_object('ch.epfl.mobots.Aseba', '/'), dbus_interface='ch.epfl.mobots.AsebaNetwork')
    #network.SetVariable("thymio-II", "motor.left.target", [0])
    #network.SetVariable("thymio-II", "motor.right.target", [0])

    #print in the terminal the name of each Aseba NOde
    print (network.GetNodesList())
    #GObject loop
    #print 'starting loop'
    #gobject.threads_init()
    mainloop = GObject.MainLoop()
    #call the callback of Braitenberg algorithm
    handle = GObject.timeout_add (10, Braitenberg) #every 0.1 sec
    mainloop.run()
