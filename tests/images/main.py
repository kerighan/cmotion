import math

from cmotion import Scene
from cmotion.media import Image
from cmotion.shape import Circle

scene = Scene(1400, 800, "#ffffff")
ratio = 1400 / 800
N = 5

for i in range(N):

    x = 50 + 20 * math.cos(2 * 3.141592653 * i / N)
    y = 50 + 20 * math.sin(2 * 3.141592653 * i / N) * ratio

    img = Image("ukraine.jpg", width=25, height=25 *
                ratio, fit="cover", z_index=i)
    # img = Circle(radius=.2, color="#FF0000")
    img.set_position(x, y)

    scene.add(img)
# scene.to_svg("test.svg")
scene.save("test.png")
