import numpy as np
from cmotion import Scene
from cmotion.shape import Hexagon

scene = Scene(800, 800, "#232323")
hexagon = Hexagon(50, 50, radius=9, color="#FFFFFF")
scene.add(hexagon)

ratio = 3.141592653/180
hexagon = Hexagon(50 - 10*np.cos(30*ratio), 40 - 10*np.sin(30*ratio), radius=9,
                  color="#FFFFFF", stroke_width=.5)
scene.add(hexagon)

hexagon = Hexagon(50 + 10*np.cos(30*ratio), 40 - 10*np.sin(30*ratio), radius=9,
                  color="#FFFFFF", stroke_width=.5)
scene.add(hexagon)

hexagon = Hexagon(50 - 10*np.cos(30*ratio), 60 + 10*np.sin(30*ratio), radius=9,
                  color="#FFFFFF", stroke_width=.5)
scene.add(hexagon)

hexagon = Hexagon(50 + 10*np.cos(30*ratio), 60 + 10*np.sin(30*ratio), radius=9,
                  color="#FFFFFF", stroke_width=.5)
scene.add(hexagon)

hexagon = Hexagon(50 - 20*np.cos(30*ratio), 50, radius=9,
                  color="#FFFFFF", stroke_width=.5)
scene.add(hexagon)

hexagon = Hexagon(50 + 20*np.cos(30*ratio), 50, radius=9,
                  color="#FFFFFF", stroke_width=.5)
scene.add(hexagon)
scene.to_svg("test.svg")
