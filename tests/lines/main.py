from cmotion import Scene
from cmotion.shape import Line


scene = Scene(1000, 1000)

x = [0, 25, 50, 75, 100]
y = [25, 90, 25, 33, 25]
line = Line(x, y, color="#FFFFFF")
scene.add(line)

scene.to_svg("test.svg")
