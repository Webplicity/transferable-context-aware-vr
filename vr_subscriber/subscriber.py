import zmq
import threading
import time
import pyglet
from pyglet.window import key
import ratcave as rc
import math
import numpy as np
from psychopy import visual, event
import paho.mqtt.client as mqtt
import time
from fastlogging import LogInit
import matplotlib.pyplot as plt
import numpy as np


from numpy import arange, sin, pi

import matplotlib

# uncomment the following to use wx rather than wxagg
#matplotlib.use('WX')
#from matplotlib.backends.backend_wx import FigureCanvasWx as FigureCanvas

# comment out the following to use wx rather than wxagg
matplotlib.use('WXAgg')
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas

from matplotlib.backends.backend_wx import NavigationToolbar2Wx

from matplotlib.figure import Figure

import wx
import wx.lib.mixins.inspection as WIT
TIME = []
QW = []
QX = []
QY = []
QZ = []


class CanvasFrame(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None, -1,
                          'CanvasFrame', size=(550, 350))

        self.figure = Figure()
        self.axes = self.figure.add_subplot(111)


        self.axes.plot(TIME, QW, '-o', color='orange', label='QW')
        self.axes.plot(TIME, QX, '-o', color='red', label='QX')
        self.axes.plot(TIME, QY, '-o', color='blue', label='QY')
        self.axes.plot(TIME, QZ, '-o', color='green', label='QZ')
        self.axes.legend()
        self.axes.grid(True)

        self.canvas = FigureCanvas(self, -1, self.figure)

        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.canvas, 1, wx.LEFT | wx.TOP | wx.EXPAND)
        self.SetSizer(self.sizer)
        self.Fit()

        self.add_toolbar()  # comment this out for no toolbar

    def add_toolbar(self):
        self.toolbar = NavigationToolbar2Wx(self.canvas)
        self.toolbar.Realize()
        # By adding toolbar in sizer, we are able to put it at the bottom
        # of the frame - so appearance is closer to GTK version.
        self.sizer.Add(self.toolbar, 0, wx.LEFT | wx.EXPAND)
        # update the axes menu on the toolbar
        self.toolbar.update()


# alternatively you could use
#class App(wx.App):
class App(WIT.InspectableApp):
    def OnInit(self):
        'Create the main window and insert the custom frame'
        self.Init()
        frame = CanvasFrame()
        frame.Show(True)

        return True

logger = LogInit(pathName="./test1.log", console=True, colors=True)


class Device:
    def __init__(self, device: str, coordinates: tuple, quaternion: tuple):
        self.name = device
        self.coordinates = coordinates
        self.x = float(coordinates[0])
        self.y = float(coordinates[1])
        self.z = float(coordinates[2])

        self.quaternion = quaternion
        self.qw = float(quaternion[0])
        self.qx = float(quaternion[1])
        self.qy = float(quaternion[2])
        self.qz = float(quaternion[3])

    def __str__(self):
        return self.name + ' ' + str(self.coordinates) + ' ' + str(self.quaternion)

    def update(self, coordinates: tuple, quaternion: tuple):
        self.coordinates = coordinates
        self.x = float(coordinates[0])
        self.y = float(coordinates[1])
        self.z = float(coordinates[2])

        self.quaternion = quaternion
        self.qw = float(quaternion[0])
        self.qx = float(quaternion[1])
        self.qy = float(quaternion[2])
        self.qz = float(quaternion[3])


class Subscriber:
    def __init__(self, mqtt_mode=False):
        self.devices = {}
        self.mqtt_mode = mqtt_mode
        self.count = 0
        self.prev_qw = 0.0
        self.prev_qx = 0.0
        self.prev_qy = 0.0
        self.prev_qz = 0.0
        self.time = []
        self.qw = []
        self.qx = []
        self.qy = []
        self.qz = []
        self.not_done = True
        if self.mqtt_mode:
            self.client = mqtt.Client()
            self.client.connect_async('127.0.0.1', 1883)
        else:
            #  Socket to talk to server
            self.context = zmq.Context()
            print("Connecting to hello world server…")
            self.socket = self.context.socket(zmq.PAIR)
            while True:
                try:
                    self.socket.connect("tcp://127.0.0.1:5555")
                    break
                except Exception:
                    time.sleep(5)
                    continue
            print("Connected! Ready to listen!")

    def on_connect(self, client, userdata, flags, rc):
        print("Connected to a broker!")
        client.subscribe("VR_CONTEXT")

    def on_message(self, client, userdata, message):
        msg = message.payload.decode()
        self.update_devices(msg)

    def run(self):
        print('Listening...')
        while self.not_done:
            # self.update_devices('HMD,1,1,1,0.1,0.1,0.1,0.1')
            # continue
            if self.mqtt_mode:
                self.client.on_connect = self.on_connect
                self.client.on_message = self.on_message
                self.client.loop_forever()
            else:
                # time.sleep(1)
                message = self.socket.recv()
                # print("Received info: [ %s ]" % message)
                msg = str(message, "ISO-8859-1")
                msg = msg.split('\x00', 1)[0]
                self.update_devices(msg)
                if self.count <= 10:
                    self.time = []
                    self.qw = []
                    self.qx = []
                    self.qy = []
                    self.qz = []

    def current_milli_time(self):
        return round(time.time() * 1000)

    def update_devices(self, msg):
        self.count += 1
        device, x, y, z, qw, qx, qy, qz = msg.split(',')
        x, y, z, qw, qx, qy, qz = float(x), float(y), float(z), float(qw), float(qx), float(qy), float(qz)
        try:
            time = self.current_milli_time()
            logger.info(msg + ' | ' + str(time))
            self.time.append(time)
            self.qw.append(abs(self.prev_qw-qw))
            self.qx.append(abs(self.prev_qx-qx))
            self.qy.append(abs(self.prev_qy-qy))
            self.qz.append(abs(self.prev_qz-qz))
            self.prev_qw = qw
            self.prev_qx = qx
            self.prev_qy = qy
            self.prev_qz = qz
            self.devices[device].update((x, y, z), (qw, qx, qy, qz))
        except KeyError:
            self.devices[device] = Device(device, (x, y, z), (qw, qx, qy, qz))

    def get_device_info(self, device: str):
        return self.devices[device]


def quaternion_rotation_matrix(Q):
    """
    Covert a quaternion into a full three-dimensional rotation matrix.

    Input
    :param Q: A 4 element array representing the quaternion (q0,q1,q2,q3)

    Output
    :return: A 3x3 element matrix representing the full 3D rotation matrix.
             This rotation matrix converts a point in the local reference
             frame to a point in the global reference frame.
    """
    # Extract the values from Q
    q0 = Q[0]
    q1 = Q[1]
    q2 = Q[2]
    q3 = Q[3]

    # First row of the rotation matrix
    r00 = 2 * (q0 * q0 + q1 * q1) - 1
    r01 = 2 * (q1 * q2 - q0 * q3)
    r02 = 2 * (q1 * q3 + q0 * q2)

    # Second row of the rotation matrix
    r10 = 2 * (q1 * q2 + q0 * q3)
    r11 = 2 * (q0 * q0 + q2 * q2) - 1
    r12 = 2 * (q2 * q3 - q0 * q1)

    # Third row of the rotation matrix
    r20 = 2 * (q1 * q3 - q0 * q2)
    r21 = 2 * (q2 * q3 + q0 * q1)
    r22 = 2 * (q0 * q0 + q3 * q3) - 1

    # 3x3 rotation matrix
    rot_matrix = np.array([[r00, r01, r02],
                           [r10, r11, r12],
                           [r20, r21, r22]])

    return rot_matrix


class Renderer:
    def __init__(self, sub: Subscriber):
        self.subscriber = sub

    def run(self):
        # Create Window and Add Keyboard State Handler to it's Event Loop
        window = pyglet.window.Window()
        keys = key.KeyStateHandler()
        window.push_handlers(keys)

        # Insert filename into WavefrontReader.
        obj_filename = rc.resources.obj_primitives
        obj_reader = rc.WavefrontReader(obj_filename)

        # Create Mesh
        monkey = obj_reader.get_mesh("Monkey", position=(0, 0, -1.5), scale=.6)

        # Create Scene
        scene = rc.Scene(meshes=[monkey])
        scene.bgColor = 0, 1, 0

        def set_quaternion(quaternion: tuple):
            euler = monkey.rotation.from_matrix(quaternion_rotation_matrix(list(quaternion)))
            monkey.rotation.x = -euler[0]
            monkey.rotation.y = euler[1]
            monkey.rotation.z = euler[2]

        def update(dt):
            if keys[key.LEFT]:
                set_quaternion((0.574611, 0.046511, 0.816911, 0.010811))
            if keys[key.RIGHT]:
                set_quaternion((0.407511, 0.583511, 0.578411, 0.397811))

            devices = self.subscriber.devices
            if len(devices) != 0:
                HMD = devices['HMD']
                set_quaternion(HMD.quaternion)

        pyglet.clock.schedule(update)

        @window.event
        def on_draw():
            with rc.default_shader:
                scene.draw()

        pyglet.app.run()


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    sub1 = Subscriber(mqtt_mode=True)
    renderer1 = Renderer(sub1)

    t1 = threading.Thread(target=sub1.run)  # api loop
    t2 = threading.Thread(target=renderer1.run)  # listen and send socket loop
    t1.start()
    # t2.start()
    time.sleep(20)
    sub1.not_done = False
    TIME = sub1.time
    QW = sub1.qw
    QX = sub1.qx
    QY = sub1.qy
    QZ = sub1.qz

    app = App(0)
    app.MainLoop()
    exit(1)

