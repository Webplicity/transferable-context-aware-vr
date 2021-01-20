import pyglet
from pyglet.window import key
import ratcave as rc

import numpy as np


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
scene.bgColor = 1, 0, 0


# # Functions to Run in Event Loop
# def rotate_meshes(dt):
#     monkey.rotation.y += 15 * dt  # dt is the time between frames
#     torus.rotation.x += 80 * dt
# pyglet.clock.schedule(rotate_meshes)


def move_camera(dt):
    camera_speed = 3
    if keys[key.LEFT]:
        euler = monkey.rotation.from_matrix(quaternion_rotation_matrix([0.574611, 0.046511, 0.816911, 0.010811]))
        monkey.rotation.x = euler[0]
        monkey.rotation.y = euler[1]
        monkey.rotation.z = euler[2]
        # scene.camera.position.x -= camera_speed * dt
    if keys[key.RIGHT]:
        euler = monkey.rotation.from_matrix(quaternion_rotation_matrix([0.407511, 0.583511, 0.578411, 0.397811]))
        monkey.rotation.x = euler[0]
        monkey.rotation.y = euler[1]
        monkey.rotation.z = euler[2]
        # scene.camera.position.x += camera_speed * dt
pyglet.clock.schedule(move_camera)


@window.event
def on_draw():
    with rc.default_shader:
        scene.draw()


pyglet.app.run()