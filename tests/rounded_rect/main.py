from cmotion import Scene
from cmotion.shape import Rectangle

scene = Scene(1000, 1000, "#232323")
rect = Rectangle(width=40, height=5, color="#FF0000",
                 border_radius=1)
scene.add(rect)
scene.save("test.png")
